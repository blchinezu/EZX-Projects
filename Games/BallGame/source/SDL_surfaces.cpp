/* 
Code		->	RomanH, 2004/2005
License		->	GPL License (see http://www.gnu.org)
*/

#include "variables.h"

void setSurfaces()
{

	//*********************************
	// create windowed / fullscreen environment
	if(intFullscreen == 0){DisplaySurface = SDL_SetVideoMode(SCREEN_WIDTH,SCREEN_HEIGHT,16, SDL_ANYFORMAT);}
	if(intFullscreen == 1){DisplaySurface = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT,16, SDL_SWSURFACE|SDL_FULLSCREEN);}
	if (DisplaySurface == NULL)
	{fprintf(stderr,"Could not set up display surface! non es bueno.\n");exit(1);}
	//*********************************


	//*********************************
	// hide cursor if in fullscreen mode
	int SDL_ShowCursor(int toggle);
 //   SDL_ShowCursor(SDL_DISABLE);
	//*********************************


	//*********************************
	// set transparent colour
	Uint32 CKColor = SDL_MapRGB(DisplaySurface->format,255,0,255);
	//*********************************

	//*********************************
	// generate sprites surface
	SpritesSurface = SDL_DisplayFormat (SDL_LoadBMP("sprites.bmp"));
	SDL_SetColorKey(SpritesSurface, SDL_SRCCOLORKEY, CKColor);
	if(SpritesSurface==NULL){fprintf(stderr,"Could not load sprites.bmp!\n");exit(1);}
	else{fprintf(stdout,"Succesfully loaded sprites.bmp...\n");}
	//*********************************

}

