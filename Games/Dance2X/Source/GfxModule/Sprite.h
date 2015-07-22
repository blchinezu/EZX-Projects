//! A class to represent a sprite and its attributes

#ifndef SPRITE_H
#define SPRITE_H

#include <SDL/SDL.h>

namespace GfxModule
{
    // Forward declare classes
	class SurfaceWrapper;

    class Sprite
    {
    private:
        //! X position of the sprite relative to the screen
        int _XPosition;

        //! Y position of the sprite relative to the screen
        int _YPosition;

        //! Priority of the sprite. Blits the sprite to layer[Priority]
        unsigned int _Priority;

        //! Current Frame
        unsigned int _CurrentFrame;

        //! The number of frames in the current surface
        unsigned int _TotalNumberOfFrames;

        //! Number of Ticks between each frame
        unsigned int _FrameDelayInTicks;

        //! Countdown timer for the framedelay
        unsigned int _FrameDelayCountdown;

        //! Toggle to declare a looping animation
        bool _AnimationLoop;

        //! Toogle to pause and resume the animation
        bool _Pause;

        //! Flag to hold whether the animation has finished
        bool _AnimationFinished;

        //! Clipping Rectangle for the frame
        SDL_Rect _ClippingRectangle;

        //! Pointer to the Surface Wrapper
        SurfaceWrapper * _SurfaceWrapper;

    public:
        //! Standard Constructor. Initialises all the variables
        Sprite(void) :
            _XPosition(0),
            _YPosition(0),
            _Priority(0),
            _CurrentFrame(0),
            _TotalNumberOfFrames(0),
            _FrameDelayInTicks(0),
            _FrameDelayCountdown(0),
            _AnimationLoop(0),
            _Pause(false),
            _AnimationFinished(false),
            _SurfaceWrapper(0)
        {
            // SDL_Rect is a struct so we have to do this the old fashioned way
            _ClippingRectangle.x = 0;
            _ClippingRectangle.y = 0;
            _ClippingRectangle.w = 0;
            _ClippingRectangle.h = 0;
        }

        //! Standard Destructor. Does nothing
        ~Sprite(void)
        {
            // Intentionally left blank
        }

        //! Gets a passed a SurfaceWrapper and assigns it to the Sprite's Wrapper
        void SetSurfaceWrapper(SurfaceWrapper * ASurfaceWrapper)
        {
            _SurfaceWrapper = ASurfaceWrapper;
        }

        //! Returns the a pointer in the SurfaceWrpper
        const SurfaceWrapper * const GetSurfaceWrapper(void) const
        {
            return _SurfaceWrapper;
        }

        //! Set the X and Y Position
        void SetXYPosition(const int NewXPosition, const int NewYPosition)
        {
			_XPosition = NewXPosition;
			_YPosition = NewYPosition;
		}

        //! Set the X Position of the Sprite
        void SetXPosition(const int NewXPosition)
        {
            _XPosition = NewXPosition;
        }

        //! Set the Y Position of the Sprite
        void SetYPosition(const int NewYPosition)
        {
            _YPosition = NewYPosition;
        }

        //! Move the sprite by X and Y
        void MoveSprite(const int X, const int Y)
        {
            _XPosition += X;
            _YPosition += Y;
        }

        //! Set the priority of the sprite
        void SetPriority(const unsigned int Priority)
        {
            _Priority = Priority;
        }

        //! Return the X Position
        const int GetXPosition(void) const
        {
            return _XPosition;
        }

        //! Return the Y Position
        const int GetYPosition(void) const
        {
            return _YPosition;
        }

        //! Returns the priority of the sprite
        const unsigned int GetPriority(void) const
        {
            return _Priority;
        }

        //! Set the various sprite dimensions
        void SetSpriteDimensions(const unsigned int FrameWidth, const unsigned int FrameHeight)
        {
            _ClippingRectangle.w = FrameWidth;
            _ClippingRectangle.h = FrameHeight;
        }

        //! Set the various sprite animation
        void SetSpriteAnimation(
			const unsigned int NumberOfFrames,
            unsigned int FrameDelayInTicks,
            const bool Loop)
        {
            _AnimationFinished = false;
            _TotalNumberOfFrames = NumberOfFrames;
            _CurrentFrame = 0;
            _FrameDelayCountdown = _FrameDelayInTicks = FrameDelayInTicks;
            _AnimationLoop = Loop;
        }

        //! Return the clipping Rectangle for the frame
        const SDL_Rect & GetClippingRectangle(void)
        {
            // Update the Clipping rectangle position
            _ClippingRectangle.x = _ClippingRectangle.w * _CurrentFrame;
            return _ClippingRectangle;
        }

        //! Scale the sprite by a factor with a filter
        void ScaleSprite(const float ScaleFactor);

        //! Update function called once per frame to regularlate the animation
        void Update(void);

        //! Check if the animation has finished
        bool IsAnimationFinished(void) const
        {
			return _AnimationFinished;
		}

        //! Pause the animation
        void PauseAnimation(void)
        {
            _Pause = true;
        }

        //! Resume the animation
        void ResumeAnimation(void)
        {
            _Pause = false;
        }

        //! Reset the animation
        void ResetAnimation(void)
        {
            _CurrentFrame = 0;
        }
    };
}

#endif // SPRITE_H

/*
History
=======
2006-07-16: SetProperties are now split between animation and dimensions. Added a SetXYPosition function
2006-07-07: Added pause and resume functionality
2006-06-25: Added support for scaling
2006-06-22: Add MoveSprite Function
2006-06-20: Created the file
*/
