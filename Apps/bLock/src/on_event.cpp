#include "main.h"

void APP::HandleEvent() {
	switch (status) {
		case ACTIVE: {
			while(SDL_PollEvent(&Event))
				switch(Event.type) {
	
					case SDL_QUIT:
						Running = false;
						break;
					/*
					case SDL_KEYDOWN:
						KeyDown(Event.key.keysym.sym);
						break;
					*/
					case SDL_KEYUP:
						KeyUp(Event.key.keysym.sym);
						break;

					case SDL_MOUSEBUTTONDOWN:
						MouseDown(Event.motion.x,Event.motion.y,
								Event.motion.xrel,Event.motion.yrel);
						break;

					case SDL_MOUSEBUTTONUP:
						MouseUp(Event.motion.x,Event.motion.y,
								Event.motion.xrel,Event.motion.yrel);
						break;

					case SDL_MOUSEMOTION:
						MouseMove(Event.motion.x,Event.motion.y,
								Event.motion.xrel,Event.motion.yrel);
						break;

					case SDL_ACTIVEEVENT:
						HandleActiveEvent();
						break;
				};
		};	break;
		case IDLE: {
			bool loop = true;
			while(loop) {
				SDL_WaitEvent(&Event);
				switch (Event.type) {
			
					case SDL_QUIT:
						Quit();
						loop=false;
						break;
					
					case SDL_ACTIVEEVENT: {
						HandleActiveEvent();
						loop=false;
						}; break;
					
/*					case SDL_MOUSEBUTTONDOWN:
						ezx.backlight(false);
						ezx.keyslight(false);
						break;
*/					
					case SDL_KEYDOWN:
						if (Event.key.keysym.sym == LIGHTUPKEY) {
							go_active_flag = true;
							loop=false;
						} else {
							ezx.keyslight(false);
							ezx.backlight(false);
						}
						break;
				}
			}
		};	break;
	}
}

void APP::HandleActiveEvent() {
	// If the window loses focus exit imediately
	if (Event.active.state == SDL_APPINPUTFOCUS && !Event.active.gain) {
		ezx.backlight(true);
		ezx.set_brightness(ezx.normal_br);
		exit(1);
	}
}


// MOUSE MOVEMENT
void APP::MouseMove(int mX, int mY, int relX, int relY) {
	#ifdef DEBUG
	printf("MOUSE MOVE: %dx%d (%dx%d)\n", mX, mY, relX, relY);
	#endif
	last_relX = relX;
	last_relY = relY;
	if (iclock.dragging) iclock.moveRel(relX, relY);
	if (unLock.dragging) unLock.moveRel(relX, relY);
	if (unLock.draggingArrow) unLock.moveRelArrow(relX);
	if (infop.dragging) infop.moveRel(relX, relY);
	go_active_flag = true;
}

// MOUSE PRESS
void APP::MouseDown(int mX, int mY, int relX, int relY) {
	#ifdef DEBUG
	printf("MOUSE DOWN: %dx%d (%dx%d)\n", mX, mY, relX, relY);
	#endif
	
	if(iclock.hover(mX, mY)) iclock.dragging = true;
	
	if(unLock.hoverArrow(mX, mY)) {
		unLock.draggingArrow = true;
		unLock.returningArrow = false;
		ezx.vibrate(VIB_TIME);
	} else
	if(unLock.hover(mX, mY)) unLock.dragging = true;
	
	if(infop.hover_button(mX, mY)) {
		infop.button_clicked = true;
		infop.draw_button(gfx.Screen, true, false);
		gfx.FullUpdate(gfx.Screen);
	} else
	if(infop.hover(mX, mY)) infop.dragging = true;
	
	go_active_flag = true;
}

// MOUSE RELEASE
void APP::MouseUp(int mX, int mY, int relX, int relY) {
	#ifdef DEBUG
	printf("MOUSE UP: %dx%d (%dx%d)\n", mX, mY, relX, relY);
	#endif
	if(iclock.dragging)	iclock.dragging = false;
	if(infop.dragging) {
		infop.dragging = false;
		infop.moveAuto(last_relY);
	}
	if(unLock.dragging) {
		unLock.dragging = false;
		unLock.moveAuto(last_relY);
	}
	if(unLock.draggingArrow) {
		unLock.draggingArrow = false;
		unLock.returningArrow = true;
	}
	if(infop.button_clicked) {
		infop.button_clicked = false;
		infop.draw_button(gfx.Screen, false, false);
		gfx.FullUpdate(gfx.Screen);
		ezx.vibrate(VIB_TIME);
		SETTINGS set(wallpaper);
	}
	go_active_flag = true;
}

// KEY PRESS
void APP::KeyDown(SDLKey sym) {
	// Motorola ROKR E6 Keys
//	if(sym==SDLK_SPACE)		{									} else	// Call
//	if(sym==SDLK_ESCAPE)	{									} else	// HangUp
//	if(sym==SDLK_PAUSE)		{									} else	// Camera
//	if(sym==SDLK_PLUS)		{									} else	// Vol+
//	if(sym==SDLK_MINUS)		{									} else	// Vol-
//	if(sym==SDLK_RETURN)	{									} else	// OK
//	if(sym==SDLK_RIGHT)		{									} else	// Up
//	if(sym==SDLK_LEFT)		{									} else	// Down
//	if(sym==SDLK_DOWN)		{									} else	// Right
//	if(sym==SDLK_UP)		{									} else	// Left
//	if(sym==SDLK_o)			{									} else	// Back
//	if(sym==SDLK_p)			{									} else	// Pause
//	if(sym==SDLK_q)			{									} else	// Forward
//	if(sym==SDLK_r)			{									} else	// Under.Call
//	if(sym==SDLK_f)			{									} else	// Mod*Call
//	if(sym==SDLK_a)			{									} else	// Mod*HangUp
//	if(sym==SDLK_e)			{									} else	// Mod*Camera
//	if(sym==SDLK_c)			{									} else	// Mod*Vol+
//	if(sym==SDLK_d)			{									} else	// Mod*Vol-
//	if(sym==SDLK_b)			{									} else	// Mod*OK
//	if(sym==SDLK_j)			{									} else	// Mod*Up
//	if(sym==SDLK_i)			{									} else	// Mod*Down
//	if(sym==SDLK_h)			{									} else	// Mod*Right
//	if(sym==SDLK_g)			{									} else	// Mod*Left
//	if(sym==SDLK_k)			{									} else	// Mod*Back
//	if(sym==SDLK_l)			{									} else	// Mod*Pause
//	if(sym==SDLK_m)			{									} else	// Mod*Forward
//	if(sym==SDLK_n)			{ 			} else	// Mod*Under.Call
	// If it's an unknown value:
	if(sym==SDLK_UNKNOWN)	{									};
}

// KEY RELEASE
void APP::KeyUp(SDLKey sym) {
	// Motorola ROKR E6 Keys
	if(sym==LIGHTUPKEY)		{ go_active_flag = true;			} else
//	if(sym==SDLK_SPACE)		{									} else	// Call
//	if(sym==SDLK_ESCAPE)	{									} else	// HangUp
//	if(sym==SDLK_PAUSE)		{									} else	// Camera
//	if(sym==SDLK_PLUS)		{									} else	// Vol+
//	if(sym==SDLK_MINUS)		{									} else	// Vol-
//	if(sym==SDLK_RETURN)	{									} else	// OK
//	if(sym==SDLK_RIGHT)		{									} else	// Up
//	if(sym==SDLK_LEFT)		{									} else	// Down
//	if(sym==SDLK_DOWN)		{									} else	// Right
//	if(sym==SDLK_UP)		{									} else	// Left
//	if(sym==SDLK_o)			{									} else	// Back
//	if(sym==SDLK_p)			{									} else	// Pause
//	if(sym==SDLK_q)			{									} else	// Forward
//	if(sym==SDLK_r)			{									} else	// Under.Call
//	if(sym==SDLK_f)			{									} else	// Mod*Call
//	if(sym==SDLK_a)			{									} else	// Mod*HangUp
//	if(sym==SDLK_e)			{									} else	// Mod*Camera
//	if(sym==SDLK_c)			{									} else	// Mod*Vol+
//	if(sym==SDLK_d)			{									} else	// Mod*Vol-
//	if(sym==SDLK_b)			{									} else	// Mod*OK
//	if(sym==SDLK_j)			{									} else	// Mod*Up
//	if(sym==SDLK_i)			{									} else	// Mod*Down
//	if(sym==SDLK_h)			{									} else	// Mod*Right
//	if(sym==SDLK_g)			{									} else	// Mod*Left
//	if(sym==SDLK_k)			{									} else	// Mod*Back
//	if(sym==SDLK_l)			{									} else	// Mod*Pause
//	if(sym==SDLK_m)			{									} else	// Mod*Forward
//	if(sym==SDLK_n)			{									} else	// Mod*Under.Call
	// If it's an unknown value:
	if(sym==SDLK_UNKNOWN)	{									};
}

