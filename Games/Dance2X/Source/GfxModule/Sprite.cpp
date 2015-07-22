#include "./Sprite.h"
#include "./SurfaceWrapper.h"
#include "../ToolKit/SDL_rotozoom.h"

using namespace GfxModule;

void Sprite::ScaleSprite(const float ScaleFactor)
{
    // Check if there is a a Wrapper
    if(_SurfaceWrapper)
    {
        // Change the width and height of the clipping rectange
        _ClippingRectangle.w = static_cast<unsigned int> (static_cast<float>(_ClippingRectangle.w) * ScaleFactor);
        _ClippingRectangle.h = static_cast<unsigned int> (static_cast<float>(_ClippingRectangle.h) * ScaleFactor);

        // If so, resize the surface
        SDL_Surface * Temp = _SurfaceWrapper->_Surface;
        _SurfaceWrapper->_Surface = zoomSurface( _SurfaceWrapper->_Surface, ScaleFactor, ScaleFactor, 0);
        SDL_FreeSurface(Temp);

        // Convert the surface to Video
        _SurfaceWrapper->_Surface = SDL_DisplayFormat(_SurfaceWrapper->_Surface);
    }
}

void Sprite::Update(void)
{
    // Decrement the timer
    --_FrameDelayCountdown;
    // Check if the timer has reached last then 0
    if(_FrameDelayCountdown == 0)
    {
        // If it has, reset timer
        _FrameDelayCountdown = _FrameDelayInTicks;
        // Increment the current Frame
        _CurrentFrame = _CurrentFrame + (!_Pause);
        _AnimationFinished = false;
        // Check if the current Frame is more then the total number of frames
        if(_CurrentFrame >=_TotalNumberOfFrames)
        {
            // If so, check if Animation Loop is on
            if(_AnimationLoop)
            {
                // If so, Reset the CurrentFrame to 0
                _CurrentFrame = 0;
            }
            else
            {
               // Else leave at the last frame
               _CurrentFrame = _TotalNumberOfFrames - 1;
               // Animation has now finished
               _AnimationFinished = true;
            }
        }
    }
}

/*
History
=======
2006-07-22: Made sure that when a surface is scaled, it is in the same format as the screen
2006-07-17: Changed from SDL_Resize to RotoZoom
2006-07-07: Added pause and resume functionality
2006-06-25: Added support for scaling
2006-06-20: Created the file
*/
