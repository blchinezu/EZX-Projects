/*
	Code written by James Fletcher (LiX). 
	Copyright 2009 - 2010. 
	ethocybin@googlemail.com
*/

#pragma comment(lib, "sdl.lib")


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL.h>
#include <cmath>
#include "PathFinder.h"
#include "Rand.h"
#include "GameWorld.h"

int screen_regulator=0;
int update_regulator=0;
int pathupdate_regulator=0;

int updatetime=33;
int screentime=40;


/* EZX button mapping */
#define VK_UP SDLK_UP
#define VK_DOWN SDLK_DOWN
#define VK_LEFT SDLK_LEFT
#define VK_RIGHT SDLK_RIGHT
#define VK_START SDLK_ESCAPE
#define VK_SELECT SDLK_RETURN
#define VK_VOL_UP SDLK_PLUS
#define VK_VOL_DOWN SDLK_MINUS

#define VK_QUIT SDLK_PAUSE

#define VK_FL 1
#define VK_FR 1
#define VK_FA 1
#define VK_FB 1
#define VK_FX SDLK_r
#define VK_FY SDLK_SPACE
#define VK_TAT 1

#define VK_UP_RIGHT 1
#define VK_UP_LEFT 1
#define VK_DOWN_LEFT 1
#define VK_DOWN_RIGHT 1

//Touch Screen (Joyrider)
//FILE* fd;

typedef struct {
    uint16_t              pressure;
    uint16_t              x;
    uint16_t              y;
    uint16_t              pad;
    struct timeval stamp;
} TS_EVENT;

TS_EVENT ts_event;


/* The screen surface */
SDL_Rect renderarea;
SDL_Surface *screen = NULL;

//Terminate Application
void Terminate(void)
{
//	SDL_Quit();
}


//Main Function
int main(int argc, char* argv[])
{
	int done;

	/* Initialize SDL */
	if (SDL_Init (SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
		fprintf (stderr, "Couldn't initialize SDL: %s\n", SDL_GetError ());
		exit (1);
	}
	atexit (Terminate);

	//Disable Cursor
	SDL_ShowCursor(SDL_DISABLE);

	/* Set 320x240 16-bits video mode */
	screen = SDL_SetVideoMode (320, 240, 16, SDL_HWSURFACE);
	if (screen == NULL) {
		fprintf (stderr, "Couldn't set 320x240x16 video mode: %s\n", SDL_GetError ());
		exit (2);
	}

	//Sets render area
	renderarea.x = 0;
    renderarea.y = 0;
    renderarea.w = screen->w;
    renderarea.h = screen->h;

	//Set Caption
	SDL_WM_SetCaption("Mushroom Roulette", 0);

	//Buttons
	bool bpress[4];
	for(int i = 0; i < 4; i++){bpress[i]=0;}
	bool md=0;
	bool autocalib = true;
	int calibX = 0, calibY = 0;

	done = 0;
	while (!done)
	{
		SDL_Event event;

		/* Check for events */
		while (SDL_PollEvent (&event))
		{
			switch (event.type)
			{   
				case SDL_KEYDOWN:
				{
					if(event.key.keysym.sym == VK_QUIT) done=1; break;
					if(event.key.keysym.sym == VK_FX)
					{
						if(gGameWorld.gScores.m_enabled == 1){break;}
						gGameWorld.gMenu.m_enabled =! gGameWorld.gMenu.m_enabled;
						gGameWorld.gMenu.m_settings = 0;
					}
					if(gGameWorld.gMenu.m_enabled == 1)
					{
						if(event.key.keysym.sym == VK_UP){gGameWorld.gMenu.MenuMove(0);}
						if(event.key.keysym.sym == VK_DOWN){gGameWorld.gMenu.MenuMove(1);}
						if(event.key.keysym.sym == VK_FB)
						{
							if(gGameWorld.gMenu.m_selected == 4){done=1;}
							gGameWorld.gMenu.MenuSelect();
						}
						break;
					}

					if(event.key.keysym.sym == VK_UP){bpress[0]=1;}
					if(event.key.keysym.sym == VK_DOWN){bpress[1]=1;}
					if(event.key.keysym.sym == VK_LEFT){bpress[2]=1;}
					if(event.key.keysym.sym == VK_RIGHT){bpress[3]=1;}
				}
				break;

				case SDL_KEYUP:
				{
					if(event.key.keysym.sym == VK_UP){bpress[0]=0;}
					if(event.key.keysym.sym == VK_DOWN){bpress[1]=0;}
					if(event.key.keysym.sym == VK_LEFT){bpress[2]=0;}
					if(event.key.keysym.sym == VK_RIGHT){bpress[3]=0;}
				}
				break;
                
                case SDL_MOUSEBUTTONUP:
				{
					if(event.button.button == SDL_BUTTON_LEFT)
					{
						//Toggle Menu
						if(event.button.x > 264 && event.button.x < 313 &&
							event.button.y > 222 && event.button.y < 237)
						{
							if(gGameWorld.gScores.m_enabled == 1){break;}
							gGameWorld.gMenu.m_enabled = !gGameWorld.gMenu.m_enabled;
							break;
						}
						
						//Menu Selections
						if(gGameWorld.gMenu.m_enabled == 1)
						{
							if(gGameWorld.gMenu.m_levelpacks == 0)
							{
								if(gGameWorld.gMenu.m_selected == 4){done=1;}
							}
							gGameWorld.gMenu.MenuSelect();
							break;
						}
					}
				}
				break;

				case SDL_MOUSEMOTION:
				{
					if(gGameWorld.gMenu.m_enabled == 0 || gGameWorld.gScores.m_enabled == 1 || gGameWorld.gMenu.m_credits == 1){break;}

					if(gGameWorld.gMenu.m_levelpacks == 1)
					{
						for(int i = 0; i < gGameWorld.gMenu.m_maxpacks; i++)
						{
							if(event.button.y > 40+(15*i) && event.button.y < (40+(15*i))+12)
							{
								gGameWorld.gMenu.m_selected = i;
							}
						}

						break;
					}

					if(gGameWorld.gMenu.m_settings == 1)
					{
						if(event.button.y > 106 && event.button.y < 118)
						{
							gGameWorld.gMenu.m_selected = 0;
						}

						if(event.button.y > 120 && event.button.y < 132)
						{
							gGameWorld.gMenu.m_selected = 1;
						}

						if(event.button.y > 134 && event.button.y < 147)
						{
							gGameWorld.gMenu.m_selected = 2;
						}
						
						if(event.button.y > 149 && event.button.y < 161)
						{
							gGameWorld.gMenu.m_selected = 3;
						}

						break;
					}

					//If mouse over New Game
					if(event.button.y > 90 && event.button.y < 102)
					{
						gGameWorld.gMenu.m_selected = 0;
					}

					//If mouse over Settings
					if(event.button.y > 104 && event.button.y < 116)
					{
						gGameWorld.gMenu.m_selected = 1;
					}

					//If mouse over Highscores
					if(event.button.y > 118 && event.button.y < 130)
					{
						gGameWorld.gMenu.m_selected = 2;
					}

					//If mouse over Credits
					if(event.button.y > 132 && event.button.y < 144)
					{
						gGameWorld.gMenu.m_selected = 3;
					}

					//If mouse over Quit Game
					if(event.button.y > 146 && event.button.y < 158)
					{
						gGameWorld.gMenu.m_selected = 4;
					}

					//If mouse over Restart Level
					if(event.button.y > 175 && event.button.y < 185)
					{
						gGameWorld.gMenu.m_selected = 5;
					}
				}
				break;
                
                
				
                case SDL_QUIT:
					done = 1;
					break;
				default:
					break;
			}
		}
        
        //keep random generator sweet
        rnd_AddList();

		//Button Presses
		if(bpress[0] == 1){gGameWorld.GetPlayer(0)->Move(0);}
		if(bpress[1] == 1){gGameWorld.GetPlayer(0)->Move(1);}
		if(bpress[2] == 1){gGameWorld.GetPlayer(0)->Move(2);}
		if(bpress[3] == 1){gGameWorld.GetPlayer(0)->Move(3);}
		
		//Toggle path finding
		if((SDL_GetMouseState(0, 0)&SDL_BUTTON(1) || ts_event.pressure>0) && gGameWorld.gMenu.m_enabled == 0)
		{
			md=1;
		}
		else
		{
			md=0;
		}

		//Update Game World
		if(SDL_GetTicks() >= update_regulator+updatetime)
		{
			gGameWorld.Update();
			update_regulator = SDL_GetTicks();
		}

		//Render Game World
		if(SDL_GetTicks() >= screen_regulator+screentime)
		{
			gGameWorld.Render();
			screen_regulator = SDL_GetTicks();
		}
		
		//Path Finder
		if(md == 1)
		{
			if(GetTicks() >= pathupdate_regulator+10)
			{
				vector<Vector2D> path;

				Vector2D p1, p2;
				p1.Set(gGameWorld.GetPlayer(0)->GetPos().x, gGameWorld.GetPlayer(0)->GetPos().y);
				p2 = gGameWorld.GetNearestGridSquarePositionToMouse(event.button.x, event.button.y);
				if(p2.x == -1 && p2.y == -1){continue;} //Oops I don't know what you clicked!!
				if(p2.x == gGameWorld.GetPlayer(0)->GetPos().x && p2.y == gGameWorld.GetPlayer(0)->GetPos().y){continue;} //Can't click on yourself!

				//If no path found
				if(gPathFinder.GetPathTo(&path, p1, p2) == 0)
				{
					//Then move in general direction
					int dir=0;

					if(fabs((double)p2.x - p1.x) > fabs((double)p2.y - p1.y))
					{
						if(p2.x > p1.x){dir=3;}
						if(p2.x < p1.x){dir=2;}
					}else{
						if(p2.y > p1.y){dir=1;}
						if(p2.y < p1.y){dir=0;}
					}


					//If there's something in the general direction we're about to move, DONT GO!
					if(dir == DIRECTION_UP){p1.y--;}
					if(dir == DIRECTION_DOWN){p1.y++;}
					if(dir == DIRECTION_LEFT){p1.x--;}
					if(dir == DIRECTION_RIGHT){p1.x++;}

					if(gGameWorld.GetGridCell(p1.x, p1.y)->ent != NULL
						&& gGameWorld.GetGridCell(p1.x, p1.y)->ent->IsVisible() == 1)
					{
						if(p1.x == p2.x && p1.y == p2.y){} //If the users selected destination is this square, DO GO.
						else{continue;}
					}

					//Otherwise let's move in general direction!
					gGameWorld.GetPlayer(0)->Move(dir);
					continue;
				}

				//If path was found...
				gGameWorld.GetPlayer(0)->MoveAllongPath(path);

				//Update Regulator
				pathupdate_regulator = GetTicks();
			}
		}

	}

	return 0;
}
