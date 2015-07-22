#include <SDL/SDL.h>
#include "./SdlTimer.h"
#include "../Logger/Logger.h"

using namespace TimerModule;
using namespace LogModule;

SdlTimer::SdlTimer(void)
{
    ErrLog << "<< Creating a Timer >>\n";

    // Initialise the member variables getting the number of ticks from SDL
    _PreviousTickCount = 0;
    _PreviousTickCount = SDL_GetTicks();

    ErrLog << "<< Created a Timer with start tick count: " << _PreviousTickCount << " >>\n";
}

SdlTimer::~SdlTimer(void)
{
    ErrLog << "<< Destroying a Timer >>\n";
    // Intentionally left blank
    ErrLog << "<< Desttroyed a Timer >>\n";
}

const unsigned int SdlTimer::GetDeltatime(void)
{
    // Get the current tick count
    unsigned int CurrentTickCount = SDL_GetTicks();

    // Calculate the deltatime
    unsigned int Deltatime = CurrentTickCount - _PreviousTickCount;

    // Save the current tick count
    _PreviousTickCount = CurrentTickCount;

    //Return deltatime
    return Deltatime;
}

void SdlTimer::Delay(const unsigned int Milliseconds)
{
    SDL_Delay(Milliseconds);
}

/*
History
=======
2006-06-16: Created the Timer files and implemented the main feature
*/
