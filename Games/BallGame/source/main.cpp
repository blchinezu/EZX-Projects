/* 
Code		->	RomanH, 2004/2005
License		->	GPL License (see http://www.gnu.org)
*/

#include "variables.h"			// include shared variables, definitions, etc.
#include <time.h>

const int SCREEN_WIDTH = 320;	//screen dimensions
const int SCREEN_HEIGHT = 240;	//screen dimensions
bool GameOver = false;
int LevelNumber = 1;
int LevelHeight = 1;
int intFullscreen = 1;			//0 = windowed, 1 = fullscreen

int main(int argc, char* argv[])
{
    srand(time(NULL));
	initSDL();
	setSurfaces();
	setPositions();
	defineSprites();
    SDL_ShowCursor(SDL_DISABLE);
	intro();

	//*********************************
	// terminate program
	SDL_JoystickClose(pStick);
	fprintf(stdout,"Programm terminated properly!\n"); 	//normal termination
//	chdir("/usr/gp2x");
//	execl("/usr/gp2x/gp2xmenu", "/usr/gp2x/gp2xmenu", NULL);
	return(0);
	//*********************************
}




