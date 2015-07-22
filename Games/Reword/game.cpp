////////////////////////////////////////////////////////////////////
/*

File:			game.cpp

Class impl:		Game

Description:	A control class for the whole game. Some initialisation, then 
				each screen is created and called from here, passing input and 
				events as they happen.

Author:			Al McLuckie (al-at-purplepup-dot-org)
				Based on framework by Dave Parker drparker@freenet.co.uk

Date:			06 April 2007

Licence:		This program is free software; you can redistribute it and/or modify
				it under the terms of the GNU General Public License as published by
				the Free Software Foundation; either version 2 of the License, or
				(at your option) any later version.

				This software is distributed in the hope that it will be useful,
				but WITHOUT ANY WARRANTY; without even the implied warranty of
				MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
				GNU General Public License for more details.

				You should have received a copy of the GNU General Public License
				along with this program; if not, write to the Free Software
				Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

*/
////////////////////////////////////////////////////////////////////

#include "global.h"

#include <SDL.h>
#include <SDL_image.h>	//for IMG_ functions
#include <SDL_ttf.h>	//for TTF_ functions
//#include <SDL_mixer.h>

#include "screen.h" 
#include "game.h"
#include "gamedata.h"	//for colour in fps etc

#include "audio.h"
#include "framerate.h"

#include "playmenu.h"
#include "playmainmenu.h"
#include "playmodemenu.h"
#include "playdiff.h"
#include "playhigh.h"
#include "playinst.h"
#include "playgame.h"

#include <cassert>


#define MAX_FRAME_RATE 60


using namespace std;



Game::Game() :
	_init(false), _screen(0), _input(0), _audio(0), _gd(0)
{
  
}

Game::~Game()
{
	delete _gd;

	//now unload the SDL stuff
	delete _screen;
	delete _input;
	delete _audio;

	if (_init)
	{
		TTF_Quit();
	}
}

void Game::splash()
{
	std::string base = "data/";	//for linux builds, reword bin is peer level to data/ dir
#if defined(WIN32) || defined (_WIN32)
	base = "../" + base;	//up to source dir as data/ is off that, not the debug or release dir
#endif
	Image img(base+"images/splash16.png");	//solid background
	img.blitTo(_screen);
	_screen->update();	//flip
}

bool Game::init()
{
	//sanity check
	assert(false==_init);	//shouldn't be called once initialised successfully
//	if (_init) return false;

	_screen  = new Screen(320, 240);
	if (!_screen->initDone())
	{
		setLastError(_screen->lastError());
		return false;
	}

	SDL_ShowCursor (0);		//hide - place here after video init else stays on screen on GP2X
	splash();				//show splash png not text

	//Initialize SDL_ttf 
	if( TTF_Init() == -1 )
	{
		setLastError("Unable to init fonts");
		return false;
	}

	_input = new Input();
#if defined GP2X
	//SDL_Init(SDL_INIT_JOYSTICK | SDL_INIT_VIDEO);
//	joystick = SDL_JoystickOpen(0);
#else
    SDL_WM_SetCaption("REWORD", 0);		//windowed caption
#endif


//	Audio *a = Singleton<Audio>::instance();	//not currently using it as some compilers bork (VC6)
//	a->init();
	_audio = new Audio();
	_audio->init();

	//load all game data (images, fonts, etc, etc)
	_gd = new GameData();
	_gd->_current_w = _screen->width();
	_gd->_current_h = _screen->height();

	return (_init = (_gd && _gd->isLoaded()));
}

//factory fn to run the game, 
//and manage the state transitions between screens etc
bool Game::run(void)
{
	assert(_init);
	
	//call different play classes 

	IPlay *p = 0;
	bool b = true;
	while (b &&_gd->_state != ST_EXIT)
	{
		switch (_gd->_state)
		{
		case ST_MENU:		p = new PlayMainMenu(*_gd);break;
		
		case ST_MODE:		p = new PlayModeMenu(*_gd);break;

		case ST_GAME:		p = new PlayGame(*_gd);break;

		case ST_DIFF:		p = new PlayDiff(*_gd);break;

		case ST_HIGH:
		case ST_HIGHEDIT:	p = new PlayHigh(*_gd);break;

		case ST_INST:		p = new PlayInst(*_gd);break;

		default: return false;
		}

		b = play(p);	//do the biz!
		delete p;
	}

	return b;
}


#define MAXIMUM_FRAME_RATE 60
//#define MINIMUM_FRAME_RATE 15
//#define UPDATE_INTERVAL (1.0 / MAXIMUM_FRAME_RATE)	//0.0166666
//#define MAX_CYCLES_PER_FRAME (MAXIMUM_FRAME_RATE / MINIMUM_FRAME_RATE)	//4

// Check if clicked on rectangle surface on screen
char ClickedOver(int cx, int cy, int x, int y, int w, int h)
{
	if(cx>=x && cy>=y && cx<x+w && cy<y+h) return 1;
	return 0;
}

// Adding touchscreen buttons support for EZX
char TouchState(int key, int cx, int cy)
{
	switch(key)
	{
		case SDLK_a:		if(ClickedOver(cx, cy, 160,   0, 160, 120)) return 1; break; // Top Right corner
		case SDLK_b:		if(ClickedOver(cx, cy, 160, 120, 160, 120)) return 1; break; // Bottom Right corner
		case SDLK_PLUS:		if(ClickedOver(cx, cy,   0,   0, 160, 120)) return 1; break; // Top Left corner
		case SDLK_MINUS:	if(ClickedOver(cx, cy,   0, 120, 160, 120)) return 1; break; // Bottom Left corner
	}
	return 0;
}


bool Game::play(IPlay *p)
{
    if (NULL == p) return false;

	bool bCap = true;

//	Audio *audio = Singleton<Audio>::getPtr();
	
/*	//tinkering... Fixed interval time-based animation variables
	double lastFrameTime = SDL_GetTicks();//0.0;
	double cyclesLeftOver = 0.0;
	double currentTime;
	double updateIterations;
*/
	//vars used here, mainly for events and fps
	SDL_Event event;

	// Initialise play/level specific stuff
	p->init(_input);

	Framerate fr;
	fr.init(MAXIMUM_FRAME_RATE);

    // Main loop
    while(p->running())
    {
		fr.setSpeedFactor();

/*
		//tinkering... 
		currentTime = SDL_GetTicks();
		updateIterations = ((currentTime - lastFrameTime) + cyclesLeftOver);
		if (updateIterations > (MAX_CYCLES_PER_FRAME * UPDATE_INTERVAL)) {	//4*0.01666 = 0.0666666666
			updateIterations = (MAX_CYCLES_PER_FRAME * UPDATE_INTERVAL);
		}
		while (updateIterations > UPDATE_INTERVAL) 
		{
			updateIterations -= UPDATE_INTERVAL;
			// Update game state a variable number of times 
*/
			// Do work/think stuff
			//_gd->_fact = fr.speedFactor(); 
			p->work(_input, fr.speedFactor());

			// Handle SDL events
			while (SDL_PollEvent(&event))
			{
				switch (event.type)
				{
					// Handle keys 
					// a - a
					// b - b
					// x - x
					// y - y
					// l - l
					// r - r
					// + - vol+
					// - - vol-
					// Return - select
					// Start  - space
					// Cursor keys - joystick

					case SDL_KEYDOWN:
					{
						Input::ButtonType b = _input->translate(event.key.keysym.sym);
						if (b != Input::NUMBER_OF_BUTTONS)
						{
							_input->down(b);
							p->button(_input,b);
						}
					}
					break;
					case SDL_MOUSEBUTTONDOWN:
					{
						if(TouchState(SDLK_a, event.button.x, event.button.y)) {
							Input::ButtonType b = _input->translate(SDLK_a);
							if (b != Input::NUMBER_OF_BUTTONS) { _input->down(b); p->button(_input,b); }
						} else
						if(TouchState(SDLK_b, event.button.x, event.button.y)) {
							Input::ButtonType b = _input->translate(SDLK_b);
							if (b != Input::NUMBER_OF_BUTTONS) { _input->down(b); p->button(_input,b); }
						} else
						if(TouchState(SDLK_PLUS, event.button.x, event.button.y)) {
							_audio->volumeUp();
						} else
						if(TouchState(SDLK_MINUS, event.button.x, event.button.y)) {
							_audio->volumeDown();
						}
					}
					break;
					case SDL_KEYUP:
					{
						// If escape is pressed...
						if (event.key.keysym.sym == SDLK_ESCAPE)
						{
							if (_gd->_state == ST_MENU) return false; //exit menu & game
							_gd->_state = ST_MENU;	//back to menu screen
							p->quit();				//sets _running false
							break;
						}

#ifdef _DEBUG			//if Q key pressed, toggle capped framerate 
						if (event.key.keysym.sym == SDLK_q)	bCap = !bCap;
#endif

						Input::ButtonType b = _input->translate(event.key.keysym.sym);
						if (b != Input::NUMBER_OF_BUTTONS)
						{
							_input->up(b);
							p->button(_input, b);
						}

						if (Input::VOLUP == b) 
							//audio->volumeUp();
							_audio->volumeUp();
						else
							if (Input::VOLDOWN == b) 
								//audio->volumeDown();
								_audio->volumeDown();

					}
					break;
					
					// Handle GP2X button events
/*
					case SDL_JOYBUTTONUP:
					{
						Input::ButtonType b = static_cast<Input::ButtonType>(event.jbutton.button);
						_input->up(b);
						p->button(_input, b);

						if (Input::VOLUP == b) 
							//audio->volumeUp();
							_audio->volumeUp();
						else
							if (Input::VOLDOWN == b) 
								//audio->volumeDown();
								_audio->volumeDown();
					}
					break;
					
					case SDL_JOYBUTTONDOWN:
					{
						Input::ButtonType b = static_cast<Input::ButtonType>(event.jbutton.button);
						_input->down(b);
						p->button(_input, b);
					}
					break;
*/
					case SDL_QUIT:	
						return false;	//valid exit

					default://let the play implementation handle other events in its own way
						p->handleEvent(event);
						break;
				}
			}
/*
		//tinkering... 
		}	//while (updateIterations > UPDATE_INTERVAL) 
		cyclesLeftOver = updateIterations;
		lastFrameTime = currentTime;
*/
		_screen->lock();
		p->render(_screen);	//screen render
#ifdef _DEBUG	//overlay the framerate and any other debug info required
		_gd->_fntSmall.put_number(_screen,0,30,fr.fps(),"%d", BLACK_COLOUR);
#endif
		_screen->unlock();
		_screen->update();

		if (bCap) fr.capFrames();

    }	//while p->running()

	//on exiting the screen, reset all keys repeat rates to none.
	//Its up to the next screen (play class) to set as required on entry.
	_input->clearRepeat();

	return true;
}







