#ifndef _OnEvent_
#define _OnEvent_

#include "../main.h"

void OnEvent(SDL_Event* Event) {
	switch(Event->type) {
		case SDL_QUIT:		{ Engine::Running = false; break; }
		case SDL_KEYDOWN:	{ OnKeyDown(Event->key.keysym.sym); break; }
		case SDL_KEYUP:		{ OnKeyUp(Event->key.keysym.sym); break; }
		case SDL_MOUSEMOTION: {
			if(!SK::Show)	// TODO: Allow the mouse events actions to be processed only if the keyboard is not shown
			{				//		(or the mouse event action will be executed both for the game/app and SK)
							//		USED FOR THE SEAMLESS MODE. The SEPARATED mode doesn't need this
				OnMouseMove(Event->motion.x,Event->motion.y,Event->motion.xrel,Event->motion.yrel,(Event->motion.state&SDL_BUTTON(SDL_BUTTON_LEFT))!=0,(Event->motion.state&SDL_BUTTON(SDL_BUTTON_RIGHT))!=0,(Event->motion.state&SDL_BUTTON(SDL_BUTTON_MIDDLE))!=0); break;
			}
		}
	}
	SK::ProcEvent(Event); // TODO: Allow SK to process the mouse events and add new key events to the queue
}

// MOUSE MOVEMENT
void OnMouseMove(int mX, int mY, int relX, int relY, bool Left, bool Right, bool Middle) {
	#ifdef DEBUG
		std::cout<<"OnMouseMove("<<mX<<", "<<mY<<", "<<relX<<", "<<relY<<", "<<Left<<", "<<Right<<", "<<Middle<<")\n";
	#endif
	Mouse::NeedUpdate=true; if(Config::ShowCursor) { Graphics::NeedScreenUpdate=true; }
}
char x[50];
// KEY PRESS
void OnKeyDown(SDLKey sym) {
//	#ifdef DEBUG
		std::cout<<" > Pressed key:\t"<<sym<<"\t( "<<SDL_GetKeyName(sym)<<" )\n";
//	#endif
	// Motorola ROKR E6 Keys
	if(sym==SDLK_SPACE)		{ SK::SetTitle("Seamless Mode - Host App is still running"); SK::ToggleShowSeamless();		} else	// Call // TODO: Use this to run SK along with the game/app
	if(sym==SDLK_ESCAPE)	{ Engine::Running=false;											} else	// HangUp
//	if(sym==SDLK_PAUSE)		{																	} else	// Camera
	if(sym==SDLK_PLUS)		{ Mouse::SwitchShowCursor();										} else	// Vol+
	if(sym==SDLK_MINUS)		{ Engine::ToggleUpdateAll();										} else	// Vol-
//	if(sym==SDLK_RETURN)	{																	} else	// OK
//	if(sym==SDLK_RIGHT)		{																	} else	// Up
//	if(sym==SDLK_LEFT)		{																	} else	// Down
//	if(sym==SDLK_DOWN)		{																	} else	// Right
//	if(sym==SDLK_UP)		{																	} else	// Left
//	if(sym==SDLK_o)			{ 																	} else	// Back
//	if(sym==SDLK_p)			{																	} else	// Pause
//	if(sym==SDLK_q)			{																	} else	// Forward
	if(sym==SDLK_r)			{ SK::SetTitle("Separated Mode - Host App is paused"); SK::GetString(x);	} else	// Under.Call // TODO: Use this to pause the game/app while using SK
//	if(sym==SDLK_f)			{																	} else	// Mod*Call
//	if(sym==SDLK_a)			{																	} else	// Mod*HangUp
//	if(sym==SDLK_e)			{																	} else	// Mod*Camera
//	if(sym==SDLK_c)			{																	} else	// Mod*Vol+
//	if(sym==SDLK_d)			{																	} else	// Mod*Vol-
//	if(sym==SDLK_b)			{																	} else	// Mod*OK
//	if(sym==SDLK_j)			{																	} else	// Mod*Up
//	if(sym==SDLK_i)			{																	} else	// Mod*Down
//	if(sym==SDLK_h)			{																	} else	// Mod*Right
//	if(sym==SDLK_g)			{																	} else	// Mod*Left
//	if(sym==SDLK_k)			{																	} else	// Mod*Back
//	if(sym==SDLK_l)			{																	} else	// Mod*Pause
//	if(sym==SDLK_m)			{																	} else	// Mod*Forward
//	if(sym==SDLK_n)			{ 																	} else	// Mod*Under.Call
	// If it's an unknown value:
	if(sym==SDLK_UNKNOWN)	{																	};
}

// KEY RELEASE
void OnKeyUp(SDLKey sym) {
	#ifdef DEBUG
		std::cout<<" > Released key: "<<sym<<"\n";
	#endif
	// Motorola ROKR E6 Keys
//	if(sym==SDLK_SPACE)		{																	} else	// Call
//	if(sym==SDLK_ESCAPE)	{ 																	} else	// HangUp
//	if(sym==SDLK_PAUSE)		{																	} else	// Camera
//	if(sym==SDLK_PLUS)		{																	} else	// Vol+
//	if(sym==SDLK_MINUS)		{																	} else	// Vol-
//	if(sym==SDLK_RETURN)	{																	} else	// OK
//	if(sym==SDLK_RIGHT)		{																	} else	// Up
//	if(sym==SDLK_LEFT)		{																	} else	// Down
//	if(sym==SDLK_DOWN)		{																	} else	// Right
//	if(sym==SDLK_UP)		{																	} else	// Left
//	if(sym==SDLK_o)			{																	} else	// Back
//	if(sym==SDLK_p)			{																	} else	// Pause
//	if(sym==SDLK_q)			{																	} else	// Forward
//	if(sym==SDLK_r)			{ 																	} else	// Under.Call
//	if(sym==SDLK_f)			{																	} else	// Mod*Call
//	if(sym==SDLK_a)			{																	} else	// Mod*HangUp
//	if(sym==SDLK_e)			{																	} else	// Mod*Camera
//	if(sym==SDLK_c)			{																	} else	// Mod*Vol+
//	if(sym==SDLK_d)			{																	} else	// Mod*Vol-
//	if(sym==SDLK_b)			{																	} else	// Mod*OK
//	if(sym==SDLK_j)			{																	} else	// Mod*Up
//	if(sym==SDLK_i)			{																	} else	// Mod*Down
//	if(sym==SDLK_h)			{																	} else	// Mod*Right
//	if(sym==SDLK_g)			{																	} else	// Mod*Left
//	if(sym==SDLK_k)			{																	} else	// Mod*Back
//	if(sym==SDLK_l)			{																	} else	// Mod*Pause
//	if(sym==SDLK_m)			{																	} else	// Mod*Forward
//	if(sym==SDLK_n)			{ 																	} else	// Mod*Under.Call
	// If it's an unknown value:
	if(sym==SDLK_UNKNOWN)	{																	};
}

#endif /* _OnEvent_ */

