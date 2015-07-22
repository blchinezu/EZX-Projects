/*
  C-Dogs SDL
  A port of the legendary (and fun) action/arcade cdogs.
  Copyright (C) 1995 Ronny Wester
  Copyright (C) 2003 Jeremy Chin 
  Copyright (C) 2003 Lucas Martin-King 

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

  -------------------------------------------------------------------------------

  keyboard.c - keyboard stuff... just think what was once here... (DOS interrupt
  handlers... *shudder*

*/

#include <string.h>
#include "SDL.h"
#include "keyboard.h"
#include "sounds.h"
#include "gp2x.h"

extern SDL_Joystick *joy;

void _dos_setvect(int intr_num, void *isr)
{
	return;
}

void *_dos_getvect(int x)
{
	return NULL;
}

char *keyNames[256] =
{ /* $00 */ NULL, "Esc", "1", "2", "3", "4", "5", "6",
  /* $08 */ "7", "8", "9", "0", "+", "Apostrophe", "Backspace",
  "Tab",
  /* $10 */ "Q", "W", "E", "R", "T", "Y", "U", "I",
  /* $18 */ "O", "P", "è", "^", "Enter", "Left Ctrl", "A", "S",
  /* $20 */ "D", "F", "G", "H", "J", "K", "L", "ô",
  /* $28 */ "é", "Paragraph", "Left shift", "'", "Z", "X", "C", "V",
  /* $30 */ "B", "N", "M", ",", ".", "-", "Right shift", "* (pad)",
  /* $38 */ "Alt", "Space", "Caps Lock", "F1", "F2", "F3", "F4",
  "F5",
  /* $40 */ "F6", "F7", "F8", "F9", "F10", "Num Lock", "Scroll Lock",
  "7 (pad)",
  /* $48 */ "8 (pad)", "9 (pad)", "- (pad)", "4 (pad)", "5 (pad)",
  "6 (pad)", "+ (pad)", "1 (pad)",
  /* $50 */ "2 (pad)", "3 (pad)", "0 (pad)", ", (pad)", "SysRq",
  NULL, "<", "F11", "F12",
  /* $59 */ NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  /* $60 */ NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  /* $70 */ NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  /* $80 */ NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  /* $90 */ NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  NULL, NULL, NULL, "Enter (pad)", "Right Ctrl", NULL, NULL,
  /* $A0 */ NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  /* $B0 */ NULL, NULL, NULL, NULL, NULL, "/ (pad)", NULL, "PrtScr",
  "Alt Gr", NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  /* $C0 */ NULL, NULL, NULL, NULL, NULL, NULL, NULL, "Home",
  /* $C8 */ "Up arrow", "Page Up", NULL, "Left arrow", NULL,
  "Right arrow", NULL, "End",
  /* $D0 */ "Down arrow", "Page Down", "Insert", "Delete", NULL,
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  NULL,
  /* $E0 */ NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  /* $F0 */ NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  NULL, NULL, NULL, NULL, NULL, NULL, NULL
};


static volatile unsigned char uKeys[256];

void InitKeyboard(void)
{

}

// Check if clicked on rectangle surface on screen
char ClickedOver(int cx, int cy, int x, int y, int w, int h)
{
	if(cx>=x && cy>=y && cx<x+w && cy<y+h) return 1;
	return 0;
}

// Adding touchscreen buttons support for EZX
char TouchState(int key)
{
	int cx,cy;
	Uint8 ms = SDL_GetMouseState(&cx, &cy);
	
	if (ms & SDL_BUTTON(SDL_BUTTON_LEFT))
		switch(key)
		{
			case SDLK_b:		if(ClickedOver(cx, cy, 160,   0, 160, 120)) return 1; break; // Top Right corner
			case SDLK_a:		if(ClickedOver(cx, cy, 160, 120, 160, 120)) return 1; break; // Bottom Right corner
			case SDLK_PLUS:		if(ClickedOver(cx, cy,   0,   0, 160, 120)) return 1; break; // Top Left corner
			case SDLK_MINUS:	if(ClickedOver(cx, cy,   0, 120, 160, 120)) return 1; break; // Bottom Left corner
		}
	return 0;
}

char KeyDown(int key)
{
	SDL_PumpEvents();
	Uint8 *keystate = SDL_GetKeyState(NULL);

	switch(key)
	{
	case SDLK_LEFT:		return keystate[SDLK_LEFT];									break;
	case SDLK_RIGHT:	return keystate[SDLK_RIGHT];								break;
	case SDLK_UP:		return keystate[SDLK_UP];									break;
	case SDLK_DOWN:		return keystate[SDLK_DOWN];									break;
	case SDLK_a:		return ( keystate[SDLK_PLUS]  || TouchState(SDLK_a) );		break;
	case SDLK_b:		return ( keystate[SDLK_MINUS] || TouchState(SDLK_b) );		break;
	case SDLK_y:		return keystate[SDLK_r];									break;
	case SDLK_RETURN:	return keystate[SDLK_RETURN];								break;
	case SDLK_ESCAPE:	return keystate[SDLK_PAUSE];								break;
	
	case SDLK_PLUS:		return TouchState(SDLK_PLUS);								break;
	case SDLK_MINUS:	return TouchState(SDLK_MINUS);								break;
	
	case SDLK_l:		return keystate[SDLK_q];									break;
	case SDLK_r:		return keystate[SDLK_p];									break;
	case SDLK_x:		return keystate[SDLK_o];									break;
	};
	return 0;
}

int GetKeyDown(void)
{
	SDL_PumpEvents(); 
	Uint8 *keystate = SDL_GetKeyState(NULL);

	if(keystate[SDLK_LEFT])								return SDLK_LEFT;
	if(keystate[SDLK_RIGHT])							return SDLK_RIGHT;
	if(keystate[SDLK_UP])								return SDLK_UP;
	if(keystate[SDLK_DOWN])								return SDLK_DOWN;
	if(keystate[SDLK_PLUS] || TouchState(SDLK_a))		return SDLK_a;
	if(keystate[SDLK_MINUS] || TouchState(SDLK_b))		return SDLK_b;
	if(keystate[SDLK_r])								return SDLK_y;
	if(keystate[SDLK_RETURN])							return SDLK_RETURN;
	if(keystate[SDLK_PAUSE])							return SDLK_ESCAPE;

	if(TouchState(SDLK_PLUS))							return SDLK_PLUS;
	if(TouchState(SDLK_MINUS))							return SDLK_MINUS;

	if(keystate[SDLK_q])								return SDLK_l;
	if(keystate[SDLK_p])								return SDLK_r;
	if(keystate[SDLK_o])								return SDLK_x;

	return 0;
}

int AnyKeyDown(void)
{
	return (GetKeyDown() > 0);
}

void ClearKeys(void)
{
}
