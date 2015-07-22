#include "./BGObject.h"

#include <SDL/SDL.h>
#include "./SurfaceWrapper.h"
#include "../ToolKit/SDL_rotozoom.h"

using namespace GfxModule;

void BGObject::ScaleBG(const unsigned int Width, const unsigned int Height)
{
    // Check if a surface exists
    if(_SurfaceWrapper->_Surface)
    {
        // If so call the scale function
        SDL_Surface * Temp = _SurfaceWrapper->_Surface;
        _SurfaceWrapper->_Surface = zoomSurface(_SurfaceWrapper->_Surface,
			(float)Width / (float)_SurfaceWrapper->_Surface->w , (float)Height / (float)_SurfaceWrapper->_Surface->h, 0);

		SDL_FreeSurface(Temp);

        // Set Transparency
        SDL_SetColorKey(
            _SurfaceWrapper->_Surface,
            SDL_SRCCOLORKEY | SDL_RLEACCEL,
            SDL_MapRGB( _SurfaceWrapper->_Surface->format, 255, 0, 255)
        );

        // Convert the surface to Video
        _SurfaceWrapper->_Surface = SDL_DisplayFormat(_SurfaceWrapper->_Surface);
    }
}

void BGObject::ScaleBG(const float ScaleFactor)
{
    // Check if a surface exists
    if(_SurfaceWrapper->_Surface)
    {
        // If so call the scale function
        SDL_Surface * Temp = _SurfaceWrapper->_Surface;
        _SurfaceWrapper->_Surface = zoomSurface(_SurfaceWrapper->_Surface,
			ScaleFactor, ScaleFactor, 0);

		SDL_FreeSurface(Temp);

        // Set Transparency
        SDL_SetColorKey(
            _SurfaceWrapper->_Surface,
            SDL_SRCCOLORKEY | SDL_RLEACCEL,
            SDL_MapRGB( _SurfaceWrapper->_Surface->format, 255, 0, 255)
        );

        // Convert the surface to Video
        _SurfaceWrapper->_Surface = SDL_DisplayFormat(_SurfaceWrapper->_Surface);
    }
}

/*
History
=======
2006-07-22: Made sure that when a surface is scaled, it is in the same format as the screen
2006-07-17: Changed from SDL_Resize to RotoZoom
2006-06-25: Created file
*/
