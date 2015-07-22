#include "./FpsCounter.h"
#include "../Logger/Logger.h"

using namespace TimerModule;
using namespace LogModule;

#define ONE_SECOND 1000

FpsCounter::FpsCounter(void) : _CurrentFps(0), _FrameCount(0), _TimeElapsedInMs(0)
{
    ErrLog << "<< Creating a FPS Counter >>\n";
    // Reset the deltatime counter
    _SdlTimer.GetDeltatime();
    ErrLog << "<< Created a FPS Counter >>\n";
}

FpsCounter::~FpsCounter(void)
{
    ErrLog << "<< Destroying a FPS Counter >>\n";
    // Intentionally left blank
    ErrLog << "<< Destroyed a FPS Counter >>\n";
}

void FpsCounter::Update(void)
{
    // Get the deltatime and add to TimeElapsedInMs
    _TimeElapsedInMs += _SdlTimer.GetDeltatime();
    // Increment FrameCount
    ++_FrameCount;
    // Check if 1 second (1000 ms) has passed
    if(_TimeElapsedInMs >= ONE_SECOND)
    {
        // If so, CurrentFps is equal to the FrameCount
        _CurrentFps = _FrameCount;

        // Reset all the counters taking into account of previous overflow
        _FrameCount = 0;
        _TimeElapsedInMs -= ONE_SECOND;
    }
}

/*
History
=======
2006-06-16: Created file and base functions
*/
