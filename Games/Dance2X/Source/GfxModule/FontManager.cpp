#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include "./SurfaceWrapper.h"
#include "./FontWrapper.h"
#include "./FontManager.h"
#include "../Logger/Logger.h"

using namespace GfxModule;
using namespace LogModule;
using namespace std;

FontManager::FontManager(const unsigned int NumberOfFonts, const SDL_Surface * const ScreenSurface)
{
	ErrLog << "<< Creating a Font Manager of " << NumberOfFonts << " Fonts >>\n";
	// Resize the vector to NumberOfFonts
	_VectorOfFontPointers.resize(NumberOfFonts);
	// For every Font, in the vector, create a new Font Wrapper
	vector<FontWrapper *>::iterator Itr = _VectorOfFontPointers.begin();
	while(Itr != _VectorOfFontPointers.end() )
	{
		*Itr = new FontWrapper();
		// Add the pointer to the queue
		_QueueOfFreeFontPointers.push(*Itr);
		++Itr;
	}
	ErrLog << "Creating the Text Surface\n";
	// Create the Text surface
	_TextSurface = new SurfaceWrapper();
	_TextSurface->_Surface = 	SDL_CreateRGBSurface
								(
                                SDL_HWSURFACE | SDL_HWPALETTE,                  // Format of the surface
                                ScreenSurface->w,                               // Width
                                ScreenSurface->h,                               // Height
                                ScreenSurface->format->BitsPerPixel,            // Colour depth
                                ScreenSurface->format->Rmask,                   // Red mask
                                ScreenSurface->format->Gmask,                   // Green mask
                                ScreenSurface->format->Bmask,                   // Blue mask
                                ScreenSurface->format->Amask                    // Alpha mask
								);
	// Convert the surface to be 100% sure
	_TextSurface->_Surface = SDL_DisplayFormat( _TextSurface->_Surface );

	SDL_SetColorKey(
            _TextSurface->_Surface, SDL_SRCCOLORKEY | SDL_RLEACCEL,
            SDL_MapRGB( _TextSurface->_Surface->format, 255, 0, 255)
        );

	ErrLog << "<< Created a Font Manager >>\n";
}

FontManager::~FontManager(void)
{
	ErrLog << "<< Destroying a Font Manager >>\n";
	// Clear the queue
	while(!_QueueOfFreeFontPointers.empty() )
	{
		_QueueOfFreeFontPointers.pop();
	}
	// For every font in the vector, free the memory used
	vector<FontWrapper *>::iterator Itr = _VectorOfFontPointers.begin();
	while(Itr != _VectorOfFontPointers.end() )
	{
		delete *Itr;
		++Itr;
	}
	// Clear the vector
	_VectorOfFontPointers.clear();
	// Free the text layer
	delete _TextSurface;
	ErrLog << "<< Destroyed a Font Manager >>\n";
}

FontWrapper * FontManager::CreateFont(const string &Filename, const int PtSize)
{
	ErrLog << "Creating a Font from filename: " << Filename << " of pt size " << PtSize <<"\n";
	// Check if there are any free font wrappers left
	if(_QueueOfFreeFontPointers.empty() )
	{
		// If not, return 0
		ErrLog << "No free fonts left\n";
		return 0;
	}
	// Create a temp pointer pointing to the top of the queue
	FontWrapper * AFontWrapper = _QueueOfFreeFontPointers.front();
	// Load the font to the pointer
	AFontWrapper->_Font = TTF_OpenFont(Filename.c_str(), PtSize);
	// Check for errors
	if(!AFontWrapper->_Font)
	{
		// If there is an errpr, return 0
		ErrLog << "Error loading Font\n";
		return 0;
	}

	// Pop the queue
	_QueueOfFreeFontPointers.pop();
	ErrLog << "Created a Font\n";

	// Return the temp pointer
	return AFontWrapper;
}

void FontManager::LoadFont(FontWrapper * AFontWrapper, const string &Filename, const int PtSize)
{
	ErrLog << "Loading a new Font from filename: " << Filename << " of pt size " << PtSize <<"\n";
	// Check the Font Wrapper is valid
	if(!AFontWrapper)
	{
		// If not, return
		ErrLog << "Font Wrapper not valid\n";
		return;
	}

	// Close the existing font
	if(AFontWrapper->_Font)
		TTF_CloseFont(AFontWrapper->_Font);
	// Load the font to the pointer
	AFontWrapper->_Font = TTF_OpenFont(Filename.c_str(), PtSize);
	// Check for errors
	if(!AFontWrapper->_Font)
	{
		// If there is an errpr, return 0
		ErrLog << "Error loading Font\n";
		return;
	}

	ErrLog << "Loaded a new Font from filename: " << Filename << "\n";
}

void FontManager::ClearFont(FontWrapper * AFontWrapper)
{
	ErrLog << "Clearing a Font\n";
	// Check that the Font Wrapper is valid
	if(!AFontWrapper)
	{
		// If not, return
		ErrLog << "Font Wrapper not valid\n";
		return;
	}
	// Close the existing font
	if(AFontWrapper->_Font)
		TTF_CloseFont(AFontWrapper->_Font);
	ErrLog << "Cleared a Font\n";
}

void FontManager::ClearAllFonts(void)
{
	ErrLog << "Clearing all Fonts\n";
	// For every font, clear the existing TTF_Font
	vector<FontWrapper *>::iterator Itr = _VectorOfFontPointers.begin();
	while(Itr != _VectorOfFontPointers.end() )
	{
		TTF_CloseFont( (*Itr)->_Font );
		++Itr;
	}
	ErrLog << "Cleared all Fonts\n";
}

void FontManager::WriteText(
	const FontWrapper * const AFontWrapper,
	const int XPositionOnScreen,
	const int YPositionOnScreen,
	const std::string &Text,
	const char Red,
	const char Green,
	const char Blue,
	const Alignment TextAlign
)
{
	// Check that the FontWrapper is valid
	if(!AFontWrapper)
	{
		// If not, return
		ErrLog << Text << " : cannot be written due to invalid Font\n";
		return;
	}
	// Create an SDL color
	SDL_Color TextColor = {Red, Green, Blue};
	// Get the Surface with the rendered text
	SDL_Surface * RenderedTextSurface = TTF_RenderText_Solid(AFontWrapper->_Font, Text.c_str(), TextColor);
	// Move the surface to left if aligned right
	int NewXPosition = XPositionOnScreen - (RenderedTextSurface->w * TextAlign);
	// Create the SD_Rect for the destination
	SDL_Rect DstRect = {NewXPosition, YPositionOnScreen, 0, 0};
	// Blit to the Text Surface
	SDL_BlitSurface( RenderedTextSurface, 0, _TextSurface->_Surface, &DstRect);
	// Free the render text surface
	SDL_FreeSurface(RenderedTextSurface);
}

void FontManager::WriteText(
	const FontWrapper * const AFontWrapper,
	const int XPositionOnScreen,
	const int YPositionOnScreen,
	const std::string &Text,
	const ColourRGBA &Colour,
	const Alignment TextAlign
)
{
	this->WriteText(AFontWrapper,
					XPositionOnScreen,
					YPositionOnScreen,
					Text,
					Colour._Red,
					Colour._Green,
					Colour._Blue,
					TextAlign );
}

void FontManager::FreeFont(FontWrapper * AFontWrapper)
{
	ErrLog << "Freeing a Font\n";
	// Check that the Font Wrapper is valid
	if(!AFontWrapper)
	{
		// If not, return
		ErrLog << "Font Wrapper not valid\n";
		return;
	}
	// Clear the TTF_Font
	if(AFontWrapper->_Font)
		TTF_CloseFont(AFontWrapper->_Font);

	AFontWrapper->_Font = 0;
	// Add pointer to a queue
	_QueueOfFreeFontPointers.push(AFontWrapper);
	ErrLog << "Freeed a Font\n";
}

void FontManager::FreeAllFonts(void)
{
	ErrLog << "Freeing all Fonts\n";
	// Clear the queue
	while(!_QueueOfFreeFontPointers.empty() )
	{
		_QueueOfFreeFontPointers.pop();
	}
	// For all Font Wrappers
	vector<FontWrapper *>::iterator Itr = _VectorOfFontPointers.begin();
	while(Itr != _VectorOfFontPointers.end() )
	{
		// Close the TTF_Font
		if( (*Itr)->_Font)
			TTF_CloseFont( (*Itr)->_Font );
		(*Itr)->_Font = 0;
		// Add to the queue
		_QueueOfFreeFontPointers.push(*Itr);
		++Itr;
	}
	ErrLog << "Freeed all Fonts\n";
}

void FontManager::ClearTextSurface(void)
{
	// Clear the text surface to bright pink
	SDL_FillRect(
            _TextSurface->_Surface,
            0,
            SDL_MapRGB( _TextSurface->_Surface->format, 255, 0, 255)
        );
}

/*
History
=======
2006-07-22: Added support for aligned text
2006-07-14: Added support for using the colour class
2006-07-02: Created file and base functionality
*/
