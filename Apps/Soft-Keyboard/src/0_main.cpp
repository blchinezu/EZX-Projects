#include "main.h"

/*
#include "app/0_Init.cpp"
#include "app/1_Event.cpp"
#include "app/2_Loop.cpp"
#include "app/3_Render.cpp"
#include "app/4_Cleanup.cpp"
#include "app/Functions.cpp"
*/
int OnExecute() {
	Timer maxfps; Timer fps;
	if(OnInit() == false) { return -1; }

	#ifdef DEBUG
		std::cout<<"> Entering main loop.\n";
	#endif
	SDL_Event Event;
	fps.start();
	while(Engine::Running) {
		maxfps.start();
		
	    while(SDL_PollEvent(&Event)) {
	        OnEvent(&Event);
	    }
	    OnLoop();
	    OnRender();
	    
		Timer::frame++;
		if((Config::LimitFPS)&&(maxfps.get_ticks()<1000/Config::ScreenMaxFPS)) { SDL_Delay((1000/Config::ScreenMaxFPS)-maxfps.get_ticks()); }		// Assure Maximum FPS
		if((fps.get_ticks()>1000) && (Config::ShowFPS)) { Timer::FPS=Timer::frame; Graphics::NeedScreenUpdate=true; fps.start(); Timer::frame=0; }	// Current Frame Rate
	}
	#ifdef DEBUG
		std::cout<<"> Main loop ended.\n";
	#endif
	OnCleanup();

	return 0;
}

int main(int argc, char* argv[]) {
	std::cout<<" Starting "<<Config::AppName<<" "<<Config::AppVersion
	#ifdef DEBUG
		<<" - DEBUGGING Version"
	#endif
	<<"\n\n";
	Engine::Running = true;
	return OnExecute();
}

