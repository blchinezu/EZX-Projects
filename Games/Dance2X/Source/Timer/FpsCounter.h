//! A class to calculate the FPS as long as the update function is called once per frame

#ifndef FPSCOUNTER_H
#define FPSCOUNTER_H

#include "./SdlTimer.h"

namespace TimerModule
{
    // Forward declare classes
    class FpsCounter
    {
    private:
        //! Stores the current FPS
        unsigned int _CurrentFps;

        //! Keeps count the number of frames processed in the last second
        unsigned int _FrameCount;

        //! Keeps track of the number of milliseconds passed
        unsigned int _TimeElapsedInMs;

        //! Internal timer
        SdlTimer _SdlTimer;

    public:
        //! Standard Constructor. Intialises the timer and member variables
        FpsCounter(void);

        //! Standard Destructor. Does nothing
        ~FpsCounter(void);

        //! Update the status on the FPS. Must be called once per frame
        void Update(void);

        //! Get the FPS count
        const unsigned int GetFpsCount(void) const
        {
            return _CurrentFps;
        }
    };
}

#endif // FPSCOUNTER_H

/*
History
=======
2006-06-16: Created file and base functions
*/
