#include "timer.h"

Timer::Timer() {
	startTicks = 0;
	pausedTicks = 0;
	paused = false;
	started = false;
}

	int		Timer::frame			= 0;
	float	Timer::FPS				= 0;
	bool	Timer::NeedFPSUpdate	= true;


void Timer::start() {
	started = true;
	paused = false;
	startTicks = SDL_GetTicks();
}
void Timer::stop() {
	started = false;
	paused = false;
}
void Timer::pause() {
    if( ( started == true ) && ( paused == false ) ) {
        paused = true;
        pausedTicks = SDL_GetTicks() - startTicks;
    }
}
void Timer::unpause() {
    if( paused == true ) {
        paused = false;
        startTicks = SDL_GetTicks() - pausedTicks;
        pausedTicks = 0;
    }
}
int Timer::get_ticks() {
    if( started == true ) {
        if( paused == true ) {
            return pausedTicks;
        } else {
            return SDL_GetTicks() - startTicks;
        }
    }
    return 0;
}
bool Timer::is_started() {
    return started;
}
bool Timer::is_paused() {
    return paused;
}
void Timer::FPSRender() {
	if(Config::ShowFPS) {
		std::ostringstream text; text<<"FPS: "<<FPS;
		SDL_Surface* text_surface = Graphics::Text(Graphics::font_normal, text.str(), 255, 255, 255);
		Graphics::AddSurface(0, 25, text_surface, Graphics::Screen, NULL);
		Graphics::ReleaseSurface(text_surface);
		#ifdef DEBUG
			std::cout<<" Top-Left Screen: "<<text.str()<<"\n";
		#endif
		NeedFPSUpdate=false;
		Graphics::NeedScreenUpdate=true;
	}
}

