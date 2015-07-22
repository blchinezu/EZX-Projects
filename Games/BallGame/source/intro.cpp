/* 
Code		->	RomanH, 2004/2005
License		->	GPL License (see http://www.gnu.org)
*/

#include "variables.h"

Uint32 timecounter_intro;
int speedhor[11];
int speedvert[11];
int positionx[11];
int positiony[11];

void intro()
{
    bool StartGame = false;
    bool QuitGame = false;
    int StartLevel = 1;  
    int q;
	for(q = 1;q<6;q++)
	{
		speedhor[q] = 2;
		speedvert[q] = 2;
		speedhor[5+q] = -2;
		speedvert[5+q] = -2;
	}

	positionx[1] = 16;	positiony[1] = 64;
	positionx[2] = 106;	positiony[2] = 78;
	positionx[3] = 240;	positiony[3] = 120;
	positionx[4] = 18;	positiony[4] = 210;
	positionx[5] = 300;	positiony[5] = 84;
	positionx[6] = 50;	positiony[6] = 92;
	positionx[7] = 83;	positiony[7] = 100;
	positionx[8] = 16;	positiony[8] = 108;
	positionx[9] = 106;	positiony[9] = 116;
	positionx[10] = 240;positiony[10] = 120;

	DrawBackground();
	timecounter_intro = SDL_GetTicks();
   
	while(QuitGame==false && StartGame==false)
	{
		if(SDL_GetTicks() - timecounter_intro > 10)
			{
			timecounter_intro = SDL_GetTicks(); 	// reset counter
			DrawBackground();
			
			for(q = 1; q < 11; q++)
			{
				positionx[q] = positionx[q] + speedhor[q];
				positiony[q] = positiony[q] + speedvert[q];
				if (positionx[q] > 320-16-1)	{speedhor[q] = - 2;}
				if (positiony[q] > 240-16-1)	{speedvert[q] = - 2;}
				if (positionx[q] < 0)		    {speedhor[q] = 2;}
				if (positiony[q] < 80)		    {speedvert[q] = 2;}
			}

			for(q = 1; q<4;q++)	{DrawBall(positionx[q], positiony[q], 1);}
			for(q = 4; q<8;q++)	{DrawBall(positionx[q], positiony[q], 2);}
			for(q = 8; q<11;q++){DrawBall(positionx[q], positiony[q], 3);}

			Print (8*24	,0,  "Port by BruceLee");
			Print (8*13	,24, "OK   - Start");
			Print (8*13	,40, "PLUS - Quit");
   			Print (0	,72, "________________________________________");

			SDL_UpdateRect(DisplaySurface,0,0,0,0);
			}	
		getInput();
		if(Key_ENTER_pressed == true) {StartGame = true;}
		if(Key_ESCAPE_pressed == true) {QuitGame = true;}
		if(Key_9_pressed == true & StartLevel>1) {StartLevel--;}
		if(Key_0_pressed == true & StartLevel<12) {StartLevel++;}		
	}
	
    if(StartGame == true){LevelNumber = StartLevel; LoadLevel(LevelNumber); game_singleplayer();}

}
