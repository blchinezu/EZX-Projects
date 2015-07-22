#ifndef _MAIN_H_
#define _MAIN_H_

#include "engine/engine.h"
#include "soft-keyboard.h"	// TODO: include SK in your source
	
// Main
int		OnExecute	();
bool	OnInit		();
bool	LoadData	();
void	OnEvent		(SDL_Event* Event);
void	OnLoop		();
void	OnRender	();
void	OnCleanup	();

// Events
void	OnMouseMove	(int mX, int mY, int relX, int relY, bool Left,bool Right,bool Middle);
void	OnKeyDown	(SDLKey sym);
void	OnKeyUp		(SDLKey sym);
    

#endif /* _MAIN_H_ */
