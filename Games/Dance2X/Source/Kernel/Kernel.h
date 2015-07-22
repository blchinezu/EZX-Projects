//! A class that creates an SDL Window instance

#ifndef KERNAL_H
#define KERNAL_H


// Forward declare some classes
namespace GfxModule
{
	class GfxModuleWrapper;
	class FontWrapper;
};

namespace SoundModule
{
	class SoundModuleWrapper;
};

namespace InputModule
{
	class Input;
}

namespace TimerModule
{
	class SdlTimer;
	class FpsCounter;
}

struct SDL_Surface;

// Default video mode for (SDL_HWSURFACE | SDL_HWPALETTE | SDL_DOUBLEBUF)
extern const unsigned int  DEFAULT_VIDEO_MODE;

namespace KernelModule
{
	class Kernel
	{
	private:
		//! Pointer to the SDL Screen
		SDL_Surface *_Screen;

		//! Status of the Kernel
		bool _Status;

		//! Pointer to the Input Manager
		InputModule::Input *_InputManager;

		//! Pointer to the Gfx Module Wrapper
		GfxModule::GfxModuleWrapper *_GfxModuleWrapper;

		//! Pointer to the Sound Module Wrapper
		SoundModule::SoundModuleWrapper *_SoundModuleWrapper;

		//! Const for the minimum amount of time to process a frame in milliseconds
		const unsigned int _FrameDelay;

		//! Internal timer to control the FPS
		TimerModule::SdlTimer *_FpsCapTimer;

		//! Fps counter
		TimerModule::FpsCounter *_FpsCounter;

		//! Font wrapper for debug text in the Kernal
		GfxModule::FontWrapper *_DebugFont;

	public:
		//! Standard Constructor. Creates an instance of an SDL window and an instance of the managers
		//! and defaults values
		Kernel(	const unsigned int Width 				= 320,
				const unsigned int Height 				= 240,
				const unsigned int ColourDepth 			= 16,
				const unsigned int SdlFlags 			= DEFAULT_VIDEO_MODE,
				const unsigned int NumberOfSurfaces 	= 128,
				const unsigned int NumberOfLayers 		= 4,
				const unsigned int NumberOfSprites 		= 128,
				const unsigned int FramesPerSecond 		= 60,
				const unsigned int NumberOfSfx 			= 16,
				const unsigned int NumberOfChannels 	= 4,
				const unsigned int NummberOfFonts 		= 4
				);

		//! Standard Destructor. Cleans up SDL and the managers
		~Kernel(void);

		//! The main display loop. Blit's all the layers to the main screen. Works with the GfxModule
		void RenderScreen(void);

		//! Regulates the framerate
		void RegulateFramerate(void);

		//! Return the Status of the Kernel
		bool GetStatus(void) const
		{
			return _Status;
		}

		//! Return the pointer to the InputManager
		InputModule::Input *GetInputManager(void)
		{
			return _InputManager;
		}

		//! Return the pointer to the GfxModuleWrapper
		GfxModule::GfxModuleWrapper *GetGfxModuleWrapper(void)
		{
			return _GfxModuleWrapper;
		}

		//! Return the pointer to the SoundModuleWrapper
		SoundModule::SoundModuleWrapper *GetSoundModuleWrapper(void)
		{
			return _SoundModuleWrapper;
		}
	};
};

#endif // KERNAL_H

/*
History
=======
2006-07-21: Added an FPS counter and font to display debug text
2006-07-02: Added support for the Text module
2006-06-30: Added support for the Sound Module
2006-06-23: Seperated the frame delay from the render screen
2006-06-21: Adding the Sprite Manager support. Removed the variable _NumberOfLayers
2006-06-19: Attempted fix on FPS cap code
2006-06-16: Added frame limit functionality
2006-06-15: Changed the Gfx Managers to use the GfxModuleWrapper
2006-06-12: Changing the const unsigned int to a #define. Added pointers to the Surface Manager and
Input Manager.
2006-06-10: Added Constness to all the functions. Added support for layers
2006-06-02: Created the Kernel file
*/
