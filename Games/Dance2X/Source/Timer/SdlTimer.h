//! A class for timer functionality using SDL. Everything is in millseconds with 0.01 accuracy.
//! Any instances of this class must be made AFTER the Kernel/SDL initialisation.

#ifndef SDLTIMER_H
#define SDLTIMER_H

namespace TimerModule
{
    class SdlTimer
    {
    private:
        //! Variable to hold the last tick count
        unsigned int _PreviousTickCount;

    public:
        //! Standard Constructor. Intialises the tick count
        SdlTimer(void);

        //! Standard Destructor.
        ~SdlTimer(void);

        //! Get the time elapsed since last call
        const unsigned int GetDeltatime(void);

        //! Delay game by X amount of milliseconds
        static void Delay(const unsigned int DelayTimeInMs);
    };
}

#endif // SDLTIMER_H

/*
History
=======
2006-07-15: Changed the Delay function to be static.
2006-06-16: Created the Timer files and implemented the main feature
*/
