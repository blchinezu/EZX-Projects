#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "../Logger/Logger.h"
#include "./SurfaceManager.h"
#include "./SurfaceWrapper.h"

using namespace LogModule;
using namespace GfxModule;

SurfaceWrapper * BLANK_SURFACEWRAPPER = 0;

SurfaceManager::SurfaceManager(const unsigned int NumberOfSurfaces)
{
	ErrLog << "<< Creating the SDL_Surface Manager with " << NumberOfSurfaces << " Surfaces >>\n";
	// Create the vector of size NumberOfSurfaces
	_VectorOfSurfacePointers.resize(NumberOfSurfaces);
	// Create an iterator and point to the beginning of the list
	std::vector<SurfaceWrapper *>::iterator Itr = _VectorOfSurfacePointers.begin();
	while(Itr != _VectorOfSurfacePointers.end() )
	{
		// Create a new object for each SurfaceWrapper
        (*Itr) = new SurfaceWrapper();
        // Add the pointer to the queue since they are all free
        _QueueOfFreeSurfacePointers.push(*Itr);
        ++Itr;
	}

	// Create a blank surface for BLANK_SURFACEWRAPPER
	SDL_Surface * ScreenSurface = SDL_GetVideoSurface();
	BLANK_SURFACEWRAPPER = new SurfaceWrapper();
	BLANK_SURFACEWRAPPER->_Surface = 	SDL_CreateRGBSurface(
										SDL_HWSURFACE | SDL_HWPALETTE,                  // Format of the surface
										1,        				                        // Width
										1,                      			            // Height
										ScreenSurface->format->BitsPerPixel,            // Colour depth
										ScreenSurface->format->Rmask,                   // Red mask
										ScreenSurface->format->Gmask,                   // Green mask
										ScreenSurface->format->Bmask,                   // Blue mask
										ScreenSurface->format->Amask                    // Alpha mask
										);

	// Set Transparency
	SDL_SetColorKey(
		BLANK_SURFACEWRAPPER->_Surface, SDL_SRCCOLORKEY | SDL_RLEACCEL,
		SDL_MapRGB( BLANK_SURFACEWRAPPER->_Surface->format, 255, 0, 255)
	);

	// Convert the surface to be 100% sure
	BLANK_SURFACEWRAPPER->_Surface = SDL_DisplayFormat( BLANK_SURFACEWRAPPER->_Surface );

	ErrLog << "<< Created the SDL_Surface Manager >>\n";
}

SurfaceManager::~SurfaceManager(void)
{
	ErrLog << "<< Destroying SDL_Surface Manager >>\n";
    ErrLog << "Freeing all SDL_Surfaces\n";

    // Delete the global surface
    delete BLANK_SURFACEWRAPPER;
    BLANK_SURFACEWRAPPER = 0;

	// Create an iterator and point to the beginning of the list
	std::vector<SurfaceWrapper *>::iterator Itr = _VectorOfSurfacePointers.begin();

	while(Itr != _VectorOfSurfacePointers.end() )
	{
		// Free the memory
		delete (*Itr);
		// Increment the iterator
		++Itr;
		//ErrLog << "Freeed a SDL_Surface\n";
	}
    ErrLog << "Freeed all SDL_Surfaces\n";
	// Clear the list
	_VectorOfSurfacePointers.clear();
	ErrLog << "<< Destroyed SDL_Surface Manager >>\n";
}

SurfaceWrapper * SurfaceManager::CreateSurface(const std::string &Filename)
{
	ErrLog << "Creating a new SDL_Surface with Filename: " << Filename << "\n";

	// Check if queue has any surface wrappers free
	if(_QueueOfFreeSurfacePointers.empty() )
	{
        // If not return NULL
        ErrLog << "No Surface Wrappers are Free\n";
        return 0;
    }
    // Use a temp pointer to the free surface wrapper
    SurfaceWrapper * ASurfaceWrapper = _QueueOfFreeSurfacePointers.front();

	// Load the surface into the Wrapper
	ASurfaceWrapper->_Surface = IMG_Load(Filename.c_str() );

	// Check for errors in loading the surface
	if(!ASurfaceWrapper->_Surface)
	{
        // If there is an error, log it and return NULL
        ErrLog << "File failed to load. Error: " << SDL_GetError() << "\n";
        return 0;
	}

	// Create a Temp Surface
    SDL_Surface * TempSurface = ASurfaceWrapper->_Surface;

    // Set Transparency
	SDL_SetColorKey(
        ASurfaceWrapper->_Surface,
        SDL_SRCCOLORKEY | SDL_RLEACCEL,
        SDL_MapRGB( ASurfaceWrapper->_Surface->format, 255, 0, 255)
    );

	// Convert the image to HW
    ASurfaceWrapper->_Surface = SDL_DisplayFormat(ASurfaceWrapper->_Surface);

    // Check for errors from the conversion
    if(ASurfaceWrapper->_Surface)
    {
        // Pop the index off the queue
        _QueueOfFreeSurfacePointers.pop();
    }
    else
    {
        ErrLog << "Error occured on SDL_Surface conversion to HW\n";
    }

    // Free the Temp surface
    SDL_FreeSurface(TempSurface);

	// Return the pointer to the free surface
	return ASurfaceWrapper;
}

SurfaceWrapper * SurfaceManager::GetFreeSurface(void)
{
    // Check if there are any free surfaces available in the queue
    ErrLog << "Getting a free Surface Wrapper object\n";

	// Check if queue has any surface wrappers free
	if(_QueueOfFreeSurfacePointers.empty() )
	{
        // If not return NULL
        ErrLog << "No Surface Wrappers are Free\n";
        return 0;
    }

    // Store the pointer at the front of the queue
    SurfaceWrapper * ASurfaceWrapper = _QueueOfFreeSurfacePointers.front();

    // Dequeue the the pointer from the queue
    _QueueOfFreeSurfacePointers.pop();

    // Return the Surface Wrapper
    return ASurfaceWrapper;
}

SurfaceWrapper * SurfaceManager::LoadSurface(SurfaceWrapper * ASurfaceWrapper, const std::string &Filename)
{
    ErrLog << "Loading a new Surface to a Wrapper of Filename: " << Filename << "\n";
    // Check if the pointer is valid
    if(!ASurfaceWrapper)
    {
        ErrLog << "Wrapper is NULL\n";
        return 0;
    }

    // Check if the pointer has a surface already
    if(ASurfaceWrapper->_Surface)
    {
        // If so free the surface
        ErrLog << "Freeing the Surface\n";
        SDL_FreeSurface(ASurfaceWrapper->_Surface);
    }
    // Load a new surface with image "Filename"
    ASurfaceWrapper->_Surface = IMG_Load(Filename.c_str() );
    // Check for errors
    // Check for errors in loading the surface
	if(!ASurfaceWrapper->_Surface)
	{
        // If there is an error, log it and return NULL
        ErrLog << "File failed to load. Error: " << SDL_GetError() << "\n";
        return 0;
	}

    // Create a Temp Surface
    SDL_Surface * TempSurface = ASurfaceWrapper->_Surface;

    // Set Transparency
	SDL_SetColorKey(
        ASurfaceWrapper->_Surface,
        SDL_SRCCOLORKEY | SDL_RLEACCEL,
        SDL_MapRGB( ASurfaceWrapper->_Surface->format, 255, 0, 255)
    );

	// Convert the image to HW
    ASurfaceWrapper->_Surface = SDL_DisplayFormat(ASurfaceWrapper->_Surface);

    // Check for errors from the conversion
    if(!ASurfaceWrapper->_Surface)
    {
        ErrLog << "Error occured on SDL_Surface conversion to HW\n";
    }

    // Free the Temp surface
    SDL_FreeSurface(TempSurface);

    // Return the pointer
    return ASurfaceWrapper;
}

void SurfaceManager::ClearSurface(SurfaceWrapper *ASurfaceWrapper)
{
    // Check if the Wrapper is valid
    ErrLog << "Clearing the Surface of a Wrapper\n";
    // Check if the pointer is valid
    if(!ASurfaceWrapper)
    {
        ErrLog << "Wrapper is NULL\n";
    }
    else
    {
		// Check if there is a surface there
		if(ASurfaceWrapper->_Surface)
		{
			// Free the surface
			ErrLog << "Freeing the Surface\n";
			SDL_FreeSurface(ASurfaceWrapper->_Surface);
			ASurfaceWrapper->_Surface = 0;
		}
    }
    ErrLog << "Cleared Surface of a Wrapper\n";
}

void SurfaceManager::ClearAllSurfaces(void)
{
    ErrLog << "Clearing all the Surfaces of the Wrappers\n";
    // For all pointers in the vector
    std::vector<SurfaceWrapper *>::iterator Itr = _VectorOfSurfacePointers.begin();
    while(Itr != _VectorOfSurfacePointers.end() )
    {
        // Check if there is a SDL_Surface used
        if( (*Itr)->_Surface )
        {
            // Free the Surface
            SDL_FreeSurface( (*Itr)->_Surface );
        }
    }
    ErrLog << "Cleared all the Surfaces of the Wrappers\n";
}

void SurfaceManager::FreeSurface(SurfaceWrapper * ASurfaceWrapper)
{
    ErrLog << "Freeing a Surface Wrapper\n";
    // Check ASurfaceWrapper is valid
    if(!ASurfaceWrapper)
    {
        ErrLog << "Wrapper is NULL\n";
    }
    else
    {
		// Check if the Surface is valid
		if(ASurfaceWrapper->_Surface)
		{
			ErrLog << "Freeing the Surface\n";
			// If so, free the surface
			SDL_FreeSurface(ASurfaceWrapper->_Surface);
			// NULL the surface
			ASurfaceWrapper->_Surface = 0;
		}
	}

    // Add to the queue of free SurfaceWrappers
    _QueueOfFreeSurfacePointers.push(ASurfaceWrapper);
    ErrLog << "Freed the Surface\n";
}

void SurfaceManager::FreeAllSurfaces(void)
{
    ErrLog << "Freeing all Surface Wrappers\n";
    // Empty the queue
    ErrLog << "Clearing the queue\n";
    while(!_QueueOfFreeSurfacePointers.empty() )
    {
        _QueueOfFreeSurfacePointers.pop();
    }

    ErrLog << "Clearing all the Surfaces of the Wrappers\n";
    // For all pointers in the vector
    std::vector<SurfaceWrapper *>::iterator Itr = _VectorOfSurfacePointers.begin();
    while(Itr != _VectorOfSurfacePointers.end() )
    {
        // Check if there is a SDL_Surface used
        if( (*Itr)->_Surface )
        {
            // Free the Surface
            SDL_FreeSurface( (*Itr)->_Surface );
            (*Itr)->_Surface = 0;
        }

        // Add the SurfaceWrapper pointer to the queue
        _QueueOfFreeSurfacePointers.push( *Itr );
        ++Itr;
    }
    ErrLog << "Cleared all the Surfaces of the Wrappers\n";
    ErrLog << "Freeed all Surface Wrappers\n";
}

/*
History
=======
2006-07-22: Attempt at speeding up the gfx Renderer by getting rid of as many if statements as possible
2006-07-17: Fixed crash error in free surface.
2006-07-14: Added a \n on the end of an error message.
2006-07-06: Forget to increment an iterator in FreeAllSurfaces
2006-06-30: Used vector.resize() rather then the push back method to populate the vector
2006-06-23: Fixed a memory leak in SDL_DisplayFormat. It creates a NEW copy of the surface.
2006-06-22: LoadSurface was missing support for transparent colours/images. This has now been rectified
The transparent colour is now deemed as bright pink (RGB (255, 0, 255)
2006-06-17: Major redeisgn of the graphics module. Surface Manager is heavily affected
2006-06-15: Changed the parameters of CreateNewSurface
2006-06-09: Created file and class
*/
