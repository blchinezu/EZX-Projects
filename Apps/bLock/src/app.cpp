#include "app.h"


/***************************[ CONSTRUCTOR ]*******************************/

APP::APP() {
	printf("\n Application:\t%s\n Version:\t%s\n Author:\t%s\n Build:\t\t%s\n\n", AppName, AppVersion, AppAuthor, AppDate);
	Running	= false;
	Focused = true;
	go_active_flag = false;
	status = ACTIVE;
	gfx.Screen	= NULL;
	wallpaper = NULL;
	last_relX = last_relY = 0;
	ezx.backlight(true);
	ezx.set_brightness(ezx.current_br);
}

/*****************************[ START ]***********************************/

bool APP::Run() {
	#ifdef DEBUG
	Timer fps;
	fps.start();
	#endif
	Timer maxfps; Timer iclocktimer; Timer light_time;
	if(Init() == false) { return -true; }

	iclocktimer.start();
	light_time.start();
	Running = true;
	
	if(UNLOCK_VISIBLE) unLock.moveAuto(-1);
	if(INFO_VISIBLE) infop.moveAuto(1);
	
	while(Running) {
		maxfps.start();
		
		HandleEvent();
		Loop();
		Render();
	   
		Timer::frame++;
		// Limit FPS
		if((LIMITFPS) && (maxfps.get_ticks()<1000/SCREENMAXFPS)) {
			SDL_Delay((1000/SCREENMAXFPS)-maxfps.get_ticks());
		}

		#ifdef DEBUG
		// Get current frame rate
		if(fps.get_ticks()>1000) {
			//Timer::FPS=Timer::frame;
			printf("FPS: %d\n", Timer::frame);
			fps.start();
			Timer::frame=0;
		}
		#endif
		if(iclocktimer.is_started() && iclocktimer.get_ticks()>2000) {
			iclock.updateTime();
			infop.updateData();
			iclocktimer.start();
		}
		
		if(go_active_flag) {
			go_active_flag=false;
			if (!light_time.is_started()) {
				#ifdef DEBUG
				printf("going active\n");
				#endif
				iclocktimer.start();
				iclock.updateTime();
				infop.updateData();
				go_active();
			}
			light_time.start();
		}
		else
		if(light_time.is_started() &&
			light_time.get_ticks()>
			LIGHTTIME*1000
			)
		{
			#ifdef DEBUG
			printf("going idle\n");
			#endif
			light_time.stop();
			iclocktimer.stop();
			go_idle();
		}
	}
	Quit();
	return false;
}

/*****************************[ LOOP ]************************************/

void APP::Loop() {
	if (unLock.returningArrow) unLock.returnArrowStep();
}

/*****************************[ QUIT ]************************************/

void APP::Quit() {
	Running = false;
	ezx.vibrate(VIB_TIME);
	ezx.backlight(true);
	ezx.set_LCDOffSeconds(ezx.LCDOffSeconds);
	ezx.set_brightness(ezx.normal_br);
	if(TTF_WasInit()!=0) TTF_Quit();
	SDL_Quit();
	
	// Write data to configuration
	SET_CLOCKX(iclock.relX);
	SET_CLOCKY(iclock.relY);
	
	exit(1);
}

APP::~APP() {
	gfx.ReleaseSurface(wallpaper);
}

/******************************* HANDLE STATUS ********************************/

void APP::go_active() {
	ezx.power_saving(false);
	SDL_Rect clip = gfx.SetClip(0, 0, ScreenWidth, ScreenHeight);
	gfx.AddSurfaceTopLeft(wallpaper, gfx.Screen, &clip);
	iclock.draw(gfx.Screen, false);
	unLock.draw(gfx.Screen, false);
	infop.draw(gfx.Screen, false);
	gfx.FullUpdate(gfx.Screen);
	
	ezx.fadein_screen(FADE_TIME);
	ezx.set_LCDOffSeconds(ezx.LCDOffSeconds);
	status = ACTIVE;
}

void APP::go_idle() {
	ezx.fadeout_screen(FADE_TIME);
	ezx.set_LCDOffSeconds(0);
	
	gfx.Blank(gfx.Screen);
	gfx.FullUpdate(gfx.Screen);
	
	if(ENABLE_FADE) usleep(500000);
	ezx.backlight(false);
	
	ezx.power_saving(true);
	status = IDLE;
}

