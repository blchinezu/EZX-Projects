#ifndef _TIMER_H_
#define _TIMER_H_

#include "engine.h"

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
		static bool		NeedFPSUpdate;
		

		void start();
		void stop();
		void pause();
		void unpause();

		int get_ticks();

		bool is_started();
		bool is_paused();
		
		static void FPSRender();
};

#endif /* _TIMER_H_ */
