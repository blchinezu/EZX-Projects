#ifndef _TIMER_H_
#define _TIMER_H_

#include "main.h"

class Timer
{
    private:
		int startTicks;
		int pausedTicks;

		bool paused;
		bool started;

    public:
		Timer();
    	static int		frame;
    	static float	FPS;

		void start();
		void stop();
		void pause();
		void unpause();

		int get_ticks();

		bool is_started();
		bool is_paused();
};

#endif /* _TIMER_H_ */
