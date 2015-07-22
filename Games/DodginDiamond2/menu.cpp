/*

    Dodgin' Diamond 2, a shot'em up arcade
    Copyright (C) 2003,2004 Juan J. Martinez <jjm@usebox.net>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License Version 2 as
    published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

*/
#include<SDL.h>
#include<SDL_mixer.h>
#include"soft-keyboard.h"
#include"menu.h"
#include"engine.h"
#include"cfg.h"
#include"control.h"
#include"SDL_plus.h"

extern SDL_Surface *screen, *gfx;
extern SDL_Joystick *joy[2];
extern Mix_Chunk *efx[8];
extern Mix_Music *bgm;
extern int sound;

extern pDesc player[2];

extern score hiscore[10];
extern cfg conf;

extern float scroll,scroll2;

void soundLoad(void);

void
drawGetName(char *name, int place, int playern)
{
	char buffer[64];

	/* erase the screen */
	SDL_FillRect(screen,NULL,SDL_MapRGB(screen->format,0,0,0));

	writeCString(gfx, screen, 90, 20, "congratulations", 0);

	sprintf(buffer,"player %i with score %.6li",playern,player[playern-1].score);
	writeCString(gfx, screen, 10, 50, buffer, 1);

	switch(place) {
		case 1:			sprintf(buffer,"you got %ist place",place);		break;
		case 2:			sprintf(buffer,"you got %ind place",place);		break;
		case 3:			sprintf(buffer,"you got %ird place",place);		break;
		default:		sprintf(buffer,"you got %ith place",place);		break;
	}
	writeCString(gfx, screen, 10, 67, buffer, 1);

	SDL_Flip(screen);
}

int
getName(char *name, int place, int playern)
{
	bool ok;
	int i;

	SK::RemoveTitle();

	SDL_Event event; while(SDL_PollEvent(&event)) { /* Make sure that no clicks are transmitted to the keyboard before showing it */ };

	do {
		ok=true;
		
		drawGetName(name,place,playern);
		SK::GetString(name);
		if(strlen(name)<1) { ok=false; SK::SetTitle("Please enter your name:"); }
		else
			if(strlen(name)>8) { ok=false; SK::SetTitle("Maximum 8 characters!"); }
			else
				for(i=0; i<strlen(name); i++)
					if(!isalnum(name[i]) && name[i]!=' ') { ok=false; SK::SetTitle("Only alphanumeric characters!"); break; }
		if(ok)
			for(i=0; i<strlen(name); i++)
				name[i]=tolower(name[i]);
	} while(!ok);

	drawGetName(name,place,playern);
	SDL_Delay(250);

	return 0;
}

void
drawHiscores(int max)
{
	int i;
	SDL_Rect a,b;

	/* erase the screen */
	SDL_FillRect(screen,NULL,SDL_MapRGB(screen->format,0,0,0));

	/* DD2 characters */
	a.x=60;
	a.y=25;
	b.x=450;
	b.y=43;
	b.w=211;
	b.h=190;
	SDL_BlitSurface(gfx, &b, screen, &a);

	/* header */
	writeCString(gfx, screen, 80, 10, "the hall of fame", 1);

	for(i=0;i<max;i++) {
		writeNumber(gfx, screen, 10, 43+i*17, i+1, 2);
		writeCString(gfx, screen, 30, 40+i*17, hiscore[i].name, 0);
		writeCString(gfx, screen, 180, 40+i*17, "st",0);
		writeNumber(gfx, screen, 200, 43+i*17, hiscore[i].stage,2);
		writeCString(gfx, screen, 236, 40+i*17, "sc",0);
		writeNumber(gfx, screen, 260, 43+i*17, hiscore[i].score,6);
	}

	SDL_Flip(screen);
}

int
hiscores()
{
	Uint32 tick;
	SDL_Event mevent;
	int i;

	for(i=0;i<10;i++) {
		drawHiscores(i+1);
		SDL_Delay(300);
	}

	tick=SDL_GetTicks();
	while(1) {
		while(SDL_PollEvent(&mevent)) {
    		if (mevent.type==SDL_QUIT)
    			return 0;
			if(mevent.type==SDL_KEYDOWN) {
				return 1;
			}
		}
		/* wait some time and return */
		if(SDL_GetTicks()-tick>10000) {
			/* pirutupiiii */
			if(sound && efx[7])
				Mix_PlayChannel(-1,efx[7],0);
			return 1;
		}
	}

	return 0;
}

void
drawConfigure(int option)
{
	/* erase the screen */
	SDL_FillRect(screen,NULL,SDL_MapRGB(screen->format,0,0,0));

	/* options */
	writeCString(gfx, screen, 80, 40,  "player 1", 0);
	if(conf.control[0]==KEYBOARD)
		writeCString(gfx, screen, 80, 57,  "   joypad", (option==1));
	else
		writeCString(gfx, screen, 80, 57,  "   joystick 1", (option==1));
	writeCString(gfx, screen, 80, 74,  "player 2", 0);
	if(conf.control[1]==KEYBOARD)
		writeCString(gfx, screen, 80, 91,  "   joypad", (option==2));
	else
		writeCString(gfx, screen, 80, 91,  "   joystick 2", (option==2));
	writeCString(gfx, screen, 80, 125,  "sound", 0);
	switch(conf.sound) {
		default:
		case SOUND_HI:
			writeCString(gfx, screen, 80, 142,  "   high quality", (option==3));
		break;
		case SOUND_MED:
			writeCString(gfx, screen, 80, 142,  "   medium quality", (option==3));
		break;
		case SOUND_LOW:
			writeCString(gfx, screen, 80, 142,  "   low quality", (option==3));
		break;
		case NO_SOUND:
			writeCString(gfx, screen, 80, 142,  "   no sound", (option==3));
		break;
	}
	writeCString(gfx, screen, 80, 159,  "graphic mode", 0);
	if(conf.fullscreen)
		writeCString(gfx, screen, 80, 176,  "   fullscreen", (option==4));
	else
		writeCString(gfx, screen, 80, 176,  "   windowed", (option==4));

	SDL_Flip(screen);
}

int
configure()
{
	SDL_Event mevent;
	int option=1,i;

	drawConfigure(option);

	while(1) {
		while(SDL_PollEvent(&mevent)) {
    			if (mevent.type==SDL_QUIT)
    				return 0;

			/* joystick control for the menu */
/*			if(joy[0])
			{
				SDL_JoystickUpdate();

				i=SDL_JoystickGetAxis(joy[0],1);
				if(i>4200)
				{
					mevent.type=SDL_KEYDOWN;
					mevent.key.keysym.sym=SDLK_DOWN;
				}
				if(i<-4200)
				{
					mevent.type=SDL_KEYDOWN;
					mevent.key.keysym.sym=SDLK_UP;
				}

				if(SDL_JoystickGetButton(joy[0], 0))
				{
					mevent.type=SDL_KEYDOWN;
					mevent.key.keysym.sym=SDLK_LCTRL;
				}

				if(SDL_JoystickGetButton(joy[0], 1))
				{
					mevent.type=SDL_KEYDOWN;
					mevent.key.keysym.sym=(SDLK_TAB|SDLK_ESCAPE);
				}
			}
*/
			if(mevent.type==SDL_KEYDOWN) {
				if(mevent.key.keysym.sym==SDLK_TAB||mevent.key.keysym.sym==SDLK_ESCAPE)
					return 1;
				if(mevent.key.keysym.sym==SDLK_DOWN ||
					mevent.key.keysym.sym==SDLK_s) {
					option++;
					if(option>4)
						option=1;
					drawConfigure(option);
				}
				if(mevent.key.keysym.sym==SDLK_UP ||
					mevent.key.keysym.sym==SDLK_w) {
					option--;
					if(option<1)
						option=4;
					drawConfigure(option);
				}
				if(mevent.key.keysym.sym==SDLK_RETURN) {
					switch(option) {
						default:
						break;
						case 1:
							if(joy[0]) {
								conf.control[0]=conf.control[0] ? 0 : 1;
								drawConfigure(option);
							}
						break;
						case 2:
							if(joy[1]) {
								conf.control[1]=conf.control[1] ? 0 : 1;
								drawConfigure(option);
							}
						break;
						case 3:
							conf.sound--;
							if(conf.sound<0)
								conf.sound=3;

							if(sound) {
								if(bgm) {
									Mix_FreeMusic(bgm);
									bgm=NULL;
								}

								for(i=0;i<NUM_EFX;i++)
									if(efx[i]) {
										Mix_FreeChunk(efx[i]);
										efx[i]=NULL;
									}
								Mix_CloseAudio();
							}

							if(conf.sound!=NO_SOUND) {
								switch(conf.sound) {
										default:
										case SOUND_HI:
											i=44100;
										break;
										case SOUND_MED:
											i=22050;
										break;
										case SOUND_LOW:
											i=16000;
										break;
								}
								if(Mix_OpenAudio(i, MIX_DEFAULT_FORMAT, 2, 2048)<0) {
									fprintf(stderr, "Unable to set audio: %s\n", SDL_GetError());
									sound=0;
								} else {
									soundLoad();
									if(efx[7])
										Mix_PlayChannel(-1,efx[7],0);
									sound=1;
								}
							}
							drawConfigure(option);
						break;
						case 4:
							conf.fullscreen=conf.fullscreen ? 0 : 1;
							drawConfigure(option);
						break;
					}
				}
			}
		}
	}

	return 0;
}

void
drawMenu(int option)
{
	SDL_Rect a,b;

	/* erase the screen */
	SDL_FillRect(screen,NULL,SDL_MapRGB(screen->format,0,0,0));

	/* BETA */
	a.x=77;
	a.y=40;
	b.x=100;
	b.y=46;
	b.w=166;
	b.h=15;
	SDL_BlitSurface(gfx, &b, screen, &a);

	/* options */
	writeCString(gfx, screen, 105, 70,  "one player", (option==1));
	writeCString(gfx, screen, 105, 87,  "two players", (option==2));
	writeCString(gfx, screen, 105, 114, "hall of fame", (option==3));
	writeCString(gfx, screen, 105, 131, "configure", (option==4));
	writeCString(gfx, screen, 105, 158, "about", (option==5));
	writeCString(gfx, screen, 105, 175, "exit game", (option==6));

	/* some credit */
	a.x=150;
	a.y=204;
	b.x=268;
	b.y=57;
	b.w=166;
	b.h=16;
	SDL_BlitSurface(gfx, &b, screen, &a);

	SDL_Flip(screen);
}

int
menu()
{
	SDL_Event mevent;
	int option=1, i;

	/* pirutupiiii */
	if(efx[7])
		Mix_PlayChannel(-1,efx[7],0);

	drawMenu(option);

	/* some dirty init */
	scroll=scroll2=0;

	while(1) {
	
		while(SDL_PollEvent(&mevent)) {
			if (mevent.type==SDL_QUIT)
				return 0;

			/* joystick control for the menu */
/*			if(joy[0])
			{
				SDL_JoystickUpdate();

				i=SDL_JoystickGetAxis(joy[0],1);
				if(i>4200)
				{
					mevent.type=SDL_KEYDOWN;
					mevent.key.keysym.sym=SDLK_DOWN;
				}
				if(i<-4200)
				{
					mevent.type=SDL_KEYDOWN;
					mevent.key.keysym.sym=SDLK_UP;
				}

				if(SDL_JoystickGetButton(joy[0], 0))
				{
					mevent.type=SDL_KEYDOWN;
					mevent.key.keysym.sym=SDLK_LCTRL;
				}

				if(SDL_JoystickGetButton(joy[0], 1))
				{
					mevent.type=SDL_KEYDOWN;
					mevent.key.keysym.sym=(SDLK_TAB|SDLK_ESCAPE);
				}
			}
*/			
			if(mevent.type==SDL_KEYDOWN) {
				if(mevent.key.keysym.sym==SDLK_TAB||mevent.key.keysym.sym==SDLK_ESCAPE)
					return 0;
				if(mevent.key.keysym.sym==SDLK_DOWN ||
					mevent.key.keysym.sym==SDLK_s) {
					option++;
					if(option>6)
						option=1;
					drawMenu(option);
				}
				if(mevent.key.keysym.sym==SDLK_UP ||
					mevent.key.keysym.sym==SDLK_w) {
					option--;
					if(option<1)
						option=6;
					drawMenu(option);
				}
				if(mevent.key.keysym.sym==SDLK_RETURN) {
					switch(option) {
						default:
						break;
						case 1:
							player[0].shield=10;
							player[1].shield=0;
							player[0].score=player[1].score=0;
							player[0].stage=player[1].stage=0;
							return 1;
						case 2:
							player[0].shield=10;
							player[1].shield=10;
							player[0].score=player[1].score=0;
							player[0].stage=player[1].stage=0;
							return 1;
						case 3:
							if(!hiscores())
								return 0;
							drawMenu(option);
						break;
						case 4:
							#ifndef GP32
							if(!configure())
								return 0;
							drawMenu(option);
							#endif
						break;
						case 5:
							if(!credits())
								return 0;
							drawMenu(option);
						break;
						case 6:
							return 0;
						break;
					}
				}
			}
		}
	}

	return 0;
}

void
drawCredits()
{
	SDL_Rect a,b;
	
	/* erase the screen */
	SDL_FillRect(screen,NULL,SDL_MapRGB(screen->format,0,0,0));

	/* BETA */
	a.x=77;
	a.y=40;
	b.x=100;
	b.y=46;
	b.w=166;
	b.h=15;
	SDL_BlitSurface(gfx, &b, screen, &a);

	writeCString(gfx, screen, 20, 70, "dd2 v0.2.1-ezx", 0);
	writeCString(gfx, screen, 20, 100, "author", 1);
	writeCString(gfx, screen, 40, 120, "juan j. martinez", 0);
	writeCString(gfx, screen, 20, 150, "porter", 1);
	writeCString(gfx, screen, 40, 170, "brucelee", 0);
	
	SDL_Flip(screen);
}

int
credits()
{
	Uint32 tick;
	SDL_Event mevent;

	drawCredits();

	tick=SDL_GetTicks();
	while(1) {
		while(SDL_PollEvent(&mevent)) {
    		if (mevent.type==SDL_QUIT)
    			return 0;
			if(mevent.type==SDL_KEYDOWN) {
				return 1;
			}
		}
		/* wait some time and return */
		if(SDL_GetTicks()-tick>10000) {
			/* pirutupiiii */
			if(sound && efx[7])
				Mix_PlayChannel(-1,efx[7],0);
			return 1;
		}
	}

	return 0;
}

