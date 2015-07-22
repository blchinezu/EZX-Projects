#include <cstdlib>
#include <vector>
#include <list>
#include <sstream>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>

//#ifdef GP2X
//#include <SDL/SDL_gp2x.h>
//#endif // GP2X

#include "./Kernel.h"
#include "../Logger/Logger.h"
#include "../SoundModule/SoundModuleWrapper.h"
#include "../GfxModule/GfxModuleWrapper.h"
#include "../GfxModule/BGManager.h"
#include "../GfxModule/BGObject.h"
#include "../GfxModule/SpriteManager.h"
#include "../GfxModule/SurfaceWrapper.h"
#include "../GfxModule/SurfaceManager.h"
#include "../GfxModule/FontManager.h"
#include "../GfxModule/Sprite.h"
#include "../InputModule/Input.h"
#include "../Timer/SdlTimer.h"
#include "../Timer/FpsCounter.h"

using namespace LogModule;
using namespace KernelModule;
using namespace GfxModule;
using namespace SoundModule;
using namespace InputModule;
using namespace TimerModule;

using namespace std;

const unsigned int DEFAULT_VIDEO_MODE = (SDL_HWSURFACE | SDL_HWPALETTE | SDL_DOUBLEBUF);

Kernel::Kernel( const unsigned int Width,
				const unsigned int Height,
				const unsigned int ColourDepth,
				const unsigned int SdlFlags,
				const unsigned int NumberOfSurfaces,
				const unsigned int NumberOfLayers,
				const unsigned int NumberOfSprites,
				const unsigned int FramesPerSecond,
				const unsigned int NumberOfSfx,
				const unsigned int NumberOfChannels,
				const unsigned int NumberOfFonts
				)
	: _FrameDelay(1000/FramesPerSecond)       // Calculate the time for each frame
{
//	ErrLog << "<< Creating the Kernel >>\n";

	// Initialise the variables
	_Status 			= true;
	_InputManager 		= 0;
	_GfxModuleWrapper 	= 0;
	_SoundModuleWrapper = 0;
	_Screen 			= 0;
	_FpsCapTimer 		= 0;
	_FpsCounter			= 0;
	_DebugFont			= 0;

	// Check the number of layers is valid
	if(NumberOfLayers == 0)
	{
	//	ErrLog << "Error: number of Layers is 0 - Changing it to 1\n";
		_Status = false;
	}

	// Initalise the SDL Video and check if it fails
	if (SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
	//	ErrLog << "Failed to initialise SDL Video: " << SDL_GetError() << "\n";
		_Status = false;
		return;
	}

	// Makes sure that SDL cleans up before exit
	atexit(SDL_Quit);

	// Create the screen surface
	_Screen = SDL_SetVideoMode(Width, Height, ColourDepth, SdlFlags);

	// Check if the screen exists
	if (!_Screen)
	{
	//	ErrLog << "Failed to initialise the screen surface: " << SDL_GetError() << "\n";
		_Status = false;
		return;
	}

	// Initialise the SDL_TTF
	if(TTF_Init() < 0)
	{
		// If error, flag and crash out
		_Status = false;
		return;
	}

	// Make sure that SDL cleans up after itself
	atexit(TTF_Quit);

	// Create the InputModule
	_InputManager 		= new Input();

	// Create the GfxModules
	_GfxModuleWrapper 	= new GfxModuleWrapper(NumberOfLayers, NumberOfSprites, NumberOfFonts, _Screen);

	// Create the timer class
	_FpsCapTimer 		= new SdlTimer();

	// Create the FPS counter
	_FpsCounter			= new FpsCounter();

//	ErrLog << "Framerate capped at " << FramesPerSecond << "fps (" << _FrameDelay << " ms per frame)\n";

	// Initialise the sound subsystem
	SDL_InitSubSystem(SDL_INIT_AUDIO);

	#ifdef PC
	if( Mix_OpenAudio(22050, AUDIO_S16SYS, 1, 512) < 0)
	{
		ErrLog << "Failed to set audio mixer: " << Mix_GetError() << "\n";
		_Status = false;
		return;
	}
	#endif // PC

	#ifdef GP2X
	if( Mix_OpenAudio(22050, AUDIO_S16SYS, 1, 128) < 0)
	{
	//	ErrLog << "Failed to set audio mixer: " << Mix_GetError() << "\n";
		_Status = false;
		return;
	}
	#endif // GP2X

	// Make sure that SDL cleans up the audio system
	atexit(Mix_CloseAudio);

	// Create the SoundModule
	_SoundModuleWrapper = new SoundModuleWrapper(NumberOfSfx, NumberOfChannels);

	// Load the debug font
	_DebugFont = _GfxModuleWrapper->GetFontManager()->CreateFont("./Fonts/adlibn.ttf", 12);

//	ErrLog << "<< Created the Kernel >>\n";
}

Kernel::~Kernel(void)
{
//	ErrLog << "<< Destroying Kernel >>\n";

	// Free the debug font
	_GfxModuleWrapper->GetFontManager()->FreeFont(_DebugFont);

	// Freeing the screen surface
	if(_Screen)
		SDL_FreeSurface(_Screen);

	// Check if the managers exist and freeing the memory
	if(_InputManager)
		delete _InputManager;
	if(_GfxModuleWrapper)
		delete _GfxModuleWrapper;
    if(_SoundModuleWrapper)
		delete _SoundModuleWrapper;
    if(_FpsCapTimer)
		delete _FpsCapTimer;
	if(_FpsCounter)
		delete _FpsCounter;

    // Cleaning up SDL
    Mix_CloseAudio();
    TTF_Quit();
    SDL_Quit();

//	ErrLog << "<< Destroyed Kernel >>\n";
}

void Kernel::RenderScreen(void)
{
	// Destination Rectangle
	SDL_Rect DstRect = {0, 0, 0, 0}, SrcRect = {0, 0, 0, 0};

	// Get a vector pointer to the BGObjects
	const vector<BGObject *> * const BGObjectsPointer = _GfxModuleWrapper->GetBGManager()->GetVectorOfBGObjects();
	// Sort the sprites by priority
    _GfxModuleWrapper->GetSpriteManager()->SortSprites();
	// Get a vector pointer to the SpriteSurfaces
	const vector< list<Sprite *> > * const SpriteLayersPointer = _GfxModuleWrapper->GetSpriteManager()->GetVectorOfSpriteLayers();

    // Loop through all the background objects render/blit then to the screen. Layer 0 is on the back
    vector<BGObject *>::const_iterator BGItr = BGObjectsPointer->begin();
    const vector<BGObject *>::const_iterator BGItrEnd = BGObjectsPointer->end();
    // Loop round the layers filled with sprites. Layer 0 is at the back
    vector< list<Sprite *> >::const_iterator SpriteLayersItr = SpriteLayersPointer->begin();
    while(BGItr != BGItrEnd )
    {
		DstRect.x = (*BGItr)->GetXPosition();
		DstRect.y = (*BGItr)->GetYPosition();
		// Blit to the screen
		SDL_BlitSurface( (*BGItr)->GetSurfaceWrapper()->_Surface, 0, _Screen, &DstRect);
        ++BGItr;

        // Blit the sprites to screen
        list<Sprite *>::const_iterator SpriteItr = (*SpriteLayersItr).begin();
        const list<Sprite *>::const_iterator SpriteItrEnd = (*SpriteLayersItr).end();
        while(SpriteItr != SpriteItrEnd )
        {
			SrcRect = (*SpriteItr)->GetClippingRectangle();
			DstRect.x = (*SpriteItr)->GetXPosition();
			DstRect.y = (*SpriteItr)->GetYPosition();
			SDL_BlitSurface( (*SpriteItr)->GetSurfaceWrapper()->_Surface, &SrcRect, _Screen, &DstRect);
			++SpriteItr;
        }
        ++SpriteLayersItr;
    }

    // Display the FPS
//    std::ostringstream DebugText;
//    DebugText << "FPS: " << _FpsCounter->GetFpsCount();
//    _GfxModuleWrapper->GetFontManager()->WriteText(_DebugFont, 1, 1, DebugText.str(), 255, 255, 255);
	

    // Blit the Text Surface to the Screen at 0,0
    DstRect.x = 0;
    DstRect.y = 0;
	SDL_BlitSurface( _GfxModuleWrapper->GetFontManager()->GetTextSurface()->_Surface,
						0, _Screen,	&DstRect);

//	#ifdef GP2X
	// For the GP2X, wait for the blitter to finish
//	SDL_GP2X_WaitForBlitter();
//	#endif // GP2X

	// Flip the buffers
	SDL_Flip(_Screen);

	// Clear screen to black
	SDL_FillRect(_Screen, 0, SDL_MapRGB(_Screen->format, 0, 0, 0));

	// Clear the text buffer
	_GfxModuleWrapper->GetFontManager()->ClearTextSurface();

	_FpsCounter->Update();
}

void Kernel::RegulateFramerate(void)
{
    // Limit the framerate
	// Update the timer and store the time taken to render the frame
	unsigned int Deltatime = _FpsCapTimer->GetDeltatime();
	// Sleep for the remaining time needed to limit the FPS only if Deltatime is less then the FrameDelay
	if(Deltatime < _FrameDelay)
	{
        _FpsCapTimer->Delay(_FrameDelay - Deltatime);
    }
    // Reset deltatime in the clock
    _FpsCapTimer->GetDeltatime();
}

/*
History
=======
2006-07-24: Modified code to use the new Sprite engine's optimised code
2006-07-21: Added an FPS counter and font to display debug text. For the GP2X release, added a Wait for Blitter
2006-07-02: Added support for the Text module
2006-06-30: Added support for the Sound Module
2006-06-23: Seperated the frame delay from the render screen
2006-06-21: Adding the Sprite Manager support. Removed the variable _NumberOfLayers
2006-06-19: Attempted fix on FPS cap code
2006-06-17: Changed the way BGManager handles its data so RenderScreen is affected.
2006-06-16: Added frame limit functionality
2006-06-15: Changed the Gfx Managers to use the GfxModuleWrapper
2006-06-12: Changing the const unsigned int to a #define. Added pointers to the Surface Manager and
Input Manager.
2006-06-10: Added constness to functions.
2006-06-02: Vreated the Kernel file
*/
