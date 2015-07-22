/* 
Code		->	RomanH, 2004/2005
License		->	GPL License (see http://www.gnu.org)
*/

#include "variables.h"

SDL_Surface* DisplaySurface = NULL;	//display surface
SDL_Surface* FontSurface = NULL;	//font surface
SDL_Surface* SpritesSurface = NULL;	//sprites surface
SDL_Event sdlEvent;					//event structure
SDL_Rect SrcRect,DstRect;			//source and destination rectangles


SDL_Joystick* pStick;	// define the Joystick
int nStickButtons;
int StickAxis[2];


void initSDL()
{
	//*********************************
	// initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO)==-1)
	{fprintf(stderr,"SDL_VIDEO fucked up seriously!\n");exit(1);}
	else
	{fprintf(stdout,"SDL_VIDEO initialized properly! Well done.\n");/*atexit(SDL_Quit);*/}
	//*********************************

	//*********************************
	// initialize joystick
	SDL_Init(SDL_INIT_JOYSTICK);
	pStick = SDL_JoystickOpen(0);
	nStickButtons = SDL_JoystickNumButtons(pStick);
	SDL_JoystickEventState(SDL_ENABLE);
	//*********************************

}
