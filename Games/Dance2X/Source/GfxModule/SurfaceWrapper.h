//! A wrapper for the SDL surface to insure that it is cleaned up correctly

#ifndef SURFACEWRAPPER_H
#define SURFACEWRAPPER_H

#include <SDL/SDL.h>

namespace GfxModule
{
	class SurfaceWrapper
	{
	public:
		//! Pointer to SDL_Surface
		SDL_Surface *_Surface;

		//! Standard Constructor. Nulls the pointer
		SurfaceWrapper(void) : _Surface(0)
		{
			// Intentionally left blank
		}

		//! Standard Destructor. Checks if the pointer is Null, if not, it frees the surface
		~SurfaceWrapper(void)
		{
			if(_Surface)
				SDL_FreeSurface(_Surface);
		}
	};
}

#endif // SURFACEWRAPPER_H

/*
History
=======
2006-06-30: Moved code from the cpp file to here
2006-06-15: Class removed from SurfaceManager and pasted here. Added new functionality.
*/
