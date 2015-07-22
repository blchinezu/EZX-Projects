/* 
Code		->	RomanH, 2004/2005
License		->	GPL License (see http://www.gnu.org)
*/

#include "variables.h"

int JoyButtonIndex;
Uint8 *keys;

int Key_UP;
int Key_DOWN;
int Key_LEFT;
int Key_RIGHT;
int Button_0;
int Button_1;
int Button_2;
int Button_3;
int Key_ESCAPE;    int Key_ESCAPE_pressed_value;    bool Key_ESCAPE_pressed;
int Key_ENTER;     int Key_ENTER_pressed_value;     bool Key_ENTER_pressed;
int Key_1;         int Key_1_pressed_value;         bool Key_1_pressed;
int Key_2;         int Key_2_pressed_value;         bool Key_2_pressed;
int Key_3;         int Key_3_pressed_value;         bool Key_3_pressed;
int Key_4;         int Key_4_pressed_value;         bool Key_4_pressed;
int Key_5;         int Key_5_pressed_value;         bool Key_5_pressed;
int Key_6;         int Key_6_pressed_value;         bool Key_6_pressed;
int Key_7;         int Key_7_pressed_value;         bool Key_7_pressed;
int Key_8;         int Key_8_pressed_value;         bool Key_8_pressed;
int Key_9;         int Key_9_pressed_value;         bool Key_9_pressed;
int Key_0;         int Key_0_pressed_value;         bool Key_0_pressed;

void getInput()
{
	void SDL_PumpEvents(void);
	if(SDL_PollEvent(&sdlEvent)==0) //IS NEDDED! (WHY?)
	{
		keys = SDL_GetKeyState(NULL);
		Key_ESCAPE = 0;
		Key_ENTER = 0;
		Key_UP = 0;
		Key_DOWN = 0;
		Key_LEFT = 0;
		Key_RIGHT = 0;
		Button_0 = 0;
		Button_1 = 0;
		Button_2 = 0;
		Button_3 = 0;
		Key_1 = 0;
		Key_2 = 0;
		Key_3 = 0;
		Key_4 = 0;
		Key_5 = 0;
		Key_6 = 0;
		Key_7 = 0;
		Key_8 = 0;
		Key_9 = 0;
		Key_0 = 0;    	

/*		if (keys[SDLK_ESCAPE] == 1)		{Key_ESCAPE	=	1;}
		if (keys[SDLK_RETURN] == 1)		{Key_ENTER	=	1;}
		if (keys[SDLK_LEFT] == 1)		{Key_LEFT	=	1;}
		if (keys[SDLK_RIGHT] == 1)		{Key_RIGHT	=	1;}
		if (keys[SDLK_DOWN] == 1)		{Key_DOWN	=	1;}
		if (keys[SDLK_UP] == 1)			{Key_UP		=	1;}
		if (keys[SDLK_RSHIFT] == 1)		{Button_1	=	1;}
		if (keys[SDLK_1] == 1)		    {Key_1	    =	1;}
		if (keys[SDLK_2] == 1)		    {Key_2	    =	1;}
		if (keys[SDLK_3] == 1)		    {Key_3	    =	1;}        
		if (keys[SDLK_4] == 1)		    {Key_4	    =	1;}
		if (keys[SDLK_5] == 1)		    {Key_5	    =	1;}
		if (keys[SDLK_6] == 1)		    {Key_6	    =	1;}
		if (keys[SDLK_7] == 1)		    {Key_7	    =	1;}
		if (keys[SDLK_8] == 1)		    {Key_8	    =	1;}
		if (keys[SDLK_9] == 1)		    {Key_9	    =	1;}                    	
		if (keys[SDLK_0] == 1)		    {Key_0	    =	1;}
		*/

		if (keys[SDLK_PLUS] == 1)		{Key_ESCAPE	=	1;}
		if (keys[SDLK_RETURN] == 1)		{Key_ENTER	=	1;}
		if (keys[SDLK_LEFT] == 1)		{Key_LEFT	=	1;}
		if (keys[SDLK_RIGHT] == 1)		{Key_RIGHT	=	1;}
		if (keys[SDLK_DOWN] == 1)		{Key_DOWN	=	1;}
		if (keys[SDLK_UP] == 1)			{Key_UP		=	1;}
		if (keys[SDLK_MINUS] == 1)		{Button_1	=	1;}
		if (keys[SDLK_KP1] == 1)		    {Key_1	    =	1;}
		if (keys[SDLK_KP2] == 1)		    {Key_2	    =	1;}
		if (keys[SDLK_KP3] == 1)		    {Key_3	    =	1;}        
		if (keys[SDLK_KP4] == 1)		    {Key_4	    =	1;}
		if (keys[SDLK_KP5] == 1)		    {Key_5	    =	1;}
		if (keys[SDLK_KP6] == 1)		    {Key_6	    =	1;}
		if (keys[SDLK_KP7] == 1)		    {Key_7	    =	1;}
		if (keys[SDLK_KP8] == 1)		    {Key_8	    =	1;}
		if (keys[SDLK_KP9] == 1)		    {Key_9	    =	1;}                    	
		if (keys[SDLK_KP0] == 1)		    {Key_0	    =	1;}

/* DISABLE joy
 		if(SDL_JoystickGetButton(SDL_JoystickOpen(0),8)) {Key_ENTER = true;}	// = start button
		if(SDL_JoystickGetButton(SDL_JoystickOpen(0),9)) {Key_ESCAPE = true;}	// = select button
		if(SDL_JoystickGetButton(SDL_JoystickOpen(0),12)) {Key_DOWN = true;}	// = A button
		if(SDL_JoystickGetButton(SDL_JoystickOpen(0),3)) {Key_DOWN = true;}	// = DOWN LEFT
		if(SDL_JoystickGetButton(SDL_JoystickOpen(0),4)) {Key_DOWN = true;}	// = DOWN
		if(SDL_JoystickGetButton(SDL_JoystickOpen(0),5)) {Key_DOWN = true;}	// = DOWN RIGHT
*/

/*
	if(SDL_JoystickGetAxis(SDL_JoystickOpen(0),1) < -100) {Key_UP		=	1;}
	if(SDL_JoystickGetAxis(SDL_JoystickOpen(0),0) < -100) {Key_LEFT		=	1;}
	if(SDL_JoystickGetAxis(SDL_JoystickOpen(0),0) > 100)  {Key_RIGHT	=	1;}
	if(SDL_JoystickGetAxis(SDL_JoystickOpen(0),1) > 100)  {Key_DOWN		=	1;}

	if(SDL_JoystickGetButton(SDL_JoystickOpen(0),0)) {Button_0 = 1;}	// BUTTON 0 IS PRESSED
	if(SDL_JoystickGetButton(SDL_JoystickOpen(0),1)) {Button_1 = 1;}	// BUTTON 1 IS PRESSED
	if(SDL_JoystickGetButton(SDL_JoystickOpen(0),2)) {Button_2 = 1;}	// BUTTON 2 IS PRESSED
	if(SDL_JoystickGetButton(SDL_JoystickOpen(0),3)) {Button_3 = 1;}	// BUTTON 3 IS PRESSED
*/

    if (Key_ENTER == 0)	    {Key_ENTER_pressed_value =	0; Key_ESCAPE_pressed = false;} else
                            {if (Key_ENTER_pressed_value == 2){Key_ENTER_pressed_value = 1; Key_ENTER_pressed = false;}
                             if (Key_ENTER_pressed_value == 0){Key_ENTER_pressed_value = 2; Key_ENTER_pressed = true;}}                             
    if (Key_ESCAPE == 0)	{Key_ESCAPE_pressed_value =	0; Key_ESCAPE_pressed = false;} else
                            {if (Key_ESCAPE_pressed_value == 2){Key_ESCAPE_pressed_value = 1; Key_ESCAPE_pressed = false;}
                             if (Key_ESCAPE_pressed_value == 0){Key_ESCAPE_pressed_value = 2; Key_ESCAPE_pressed = true;}}
    if (Key_1 == 0)	{Key_1_pressed_value =	0; Key_1_pressed = false;} else
                    {if (Key_1_pressed_value == 2){Key_1_pressed_value = 1; Key_1_pressed = false;}
                     if (Key_1_pressed_value == 0){Key_1_pressed_value = 2; Key_1_pressed = true;}}
    if (Key_2 == 0)	{Key_2_pressed_value =	0; Key_2_pressed = false;} else
                    {if (Key_2_pressed_value == 2){Key_2_pressed_value = 1; Key_2_pressed = false;}
                     if (Key_2_pressed_value == 0){Key_2_pressed_value = 2; Key_2_pressed = true;}}
    if (Key_3 == 0)	{Key_3_pressed_value =	0; Key_3_pressed = false;} else
                    {if (Key_3_pressed_value == 2){Key_3_pressed_value = 1; Key_3_pressed = false;}
                     if (Key_3_pressed_value == 0){Key_3_pressed_value = 2; Key_3_pressed = true;}}
    if (Key_4 == 0)	{Key_4_pressed_value =	0; Key_4_pressed = false;} else
                    {if (Key_4_pressed_value == 2){Key_4_pressed_value = 1; Key_4_pressed = false;}
                     if (Key_4_pressed_value == 0){Key_4_pressed_value = 2; Key_4_pressed = true;}}
    if (Key_5 == 0)	{Key_5_pressed_value =	0; Key_5_pressed = false;} else
                    {if (Key_5_pressed_value == 2){Key_5_pressed_value = 1; Key_5_pressed = false;}
                     if (Key_5_pressed_value == 0){Key_5_pressed_value = 2; Key_5_pressed = true;}}
    if (Key_6 == 0)	{Key_6_pressed_value =	0; Key_6_pressed = false;} else
                    {if (Key_6_pressed_value == 2){Key_6_pressed_value = 1; Key_6_pressed = false;}
                     if (Key_6_pressed_value == 0){Key_6_pressed_value = 2; Key_6_pressed = true;}}
    if (Key_7 == 0)	{Key_7_pressed_value =	0; Key_7_pressed = false;} else
                    {if (Key_7_pressed_value == 2){Key_7_pressed_value = 1; Key_7_pressed = false;}
                     if (Key_7_pressed_value == 0){Key_7_pressed_value = 2; Key_7_pressed = true;}}
    if (Key_8 == 0)	{Key_8_pressed_value =	0; Key_8_pressed = false;} else
                    {if (Key_8_pressed_value == 2){Key_8_pressed_value = 1; Key_8_pressed = false;}
                     if (Key_8_pressed_value == 0){Key_8_pressed_value = 2; Key_8_pressed = true;}}
    if (Key_9 == 0)	{Key_9_pressed_value =	0; Key_9_pressed = false;} else
                    {if (Key_9_pressed_value == 2){Key_9_pressed_value = 1; Key_9_pressed = false;}
                     if (Key_9_pressed_value == 0){Key_9_pressed_value = 2; Key_9_pressed = true;}}
    if (Key_0 == 0)	{Key_0_pressed_value =	0; Key_0_pressed = false;} else
                    {if (Key_0_pressed_value == 2){Key_0_pressed_value = 1; Key_0_pressed = false;}
                     if (Key_0_pressed_value == 0){Key_0_pressed_value = 2; Key_0_pressed = true;}}
	}
}

