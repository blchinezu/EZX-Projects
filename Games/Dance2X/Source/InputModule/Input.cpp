#include <SDL/SDL.h>
#include "../Logger/Logger.h"
#include "./Input.h"

using namespace LogModule;
using namespace InputModule;

// Keys FOR EZX
#define EZXK_UP         SDLK_UP
#define EZXK_DOWN       SDLK_DOWN
#define EZXK_LEFT       SDLK_LEFT
#define EZXK_RIGHT      SDLK_RIGHT
#define EZXK_OK     	SDLK_RETURN
#define EZXK_PLUS		SDLK_PLUS
#define EZXK_MINUS		SDLK_MINUS
#define EZXK_CAMERA     SDLK_PAUSE

// ROKR E6
#define EZXK_NEXT		SDLK_q
#define EZXK_PAUSE		SDLK_p

Input::Input(void) : _CurrentInputState(0), _PreviousInputState(0), _KillFlag(false)
{
	SDL_ShowCursor(SDL_DISABLE);
	ErrLog << "<< Hid the mouse cursor >>\n";
}

Input::~Input(void)
{
}

// Check if clicked on rectangle surface on screen
char ClickedOver(int cx, int cy, int x, int y, int w, int h)
{
	if(cx>=x && cy>=y && cx<x+w && cy<y+h) return 1;
	return 0;
}

void Input::UpdateInputState(void)
{
	// Store the current state to the previous buffer
	_PreviousInputState = _CurrentInputState;

	// Clear current state
	//_CurrentInputState = 0;

	// message processing loop
	SDL_Event Event;
	while (SDL_PollEvent(&Event))
	{
		// check for messages
		switch (Event.type)
		{
			// Check if the app has been forced to quit
			case SDL_QUIT:
			{
				_KillFlag = true;
			} break;

			// check for touchscreen press
			case SDL_MOUSEBUTTONDOWN:
			{
				if(ClickedOver(Event.button.x, Event.button.y,   0,   0, 160, 120)) _CurrentInputState |= BUTTON_VOLUP; 	else	// Top Left corner
				if(ClickedOver(Event.button.x, Event.button.y,   0, 120, 160, 120)) _CurrentInputState |= BUTTON_VOLDOWN;	else	// Bottom Left corner
				if(ClickedOver(Event.button.x, Event.button.y, 160,   0, 160, 120)) _CurrentInputState |= BUTTON_A;			else	// Top Right corner
				if(ClickedOver(Event.button.x, Event.button.y, 160, 120, 160, 120)) _CurrentInputState |= BUTTON_B; 				// Bottom Right corner
			} break;

			// check for touchscreen release
			case SDL_MOUSEBUTTONUP:
			{
				if(ClickedOver(Event.button.x, Event.button.y,   0,   0, 160, 120)) _CurrentInputState &= ~BUTTON_VOLUP; 	else	// Top Left corner
				if(ClickedOver(Event.button.x, Event.button.y,   0, 120, 160, 120)) _CurrentInputState &= ~BUTTON_VOLDOWN;	else	// Bottom Left corner
				if(ClickedOver(Event.button.x, Event.button.y, 160,   0, 160, 120)) _CurrentInputState &= ~BUTTON_A;		else	// Top Right corner
				if(ClickedOver(Event.button.x, Event.button.y, 160, 120, 160, 120)) _CurrentInputState &= ~BUTTON_B; 				// Bottom Right corner
			} break;
			
			// check for keypresses
			case SDL_KEYDOWN:
			{
				switch( Event.key.keysym.sym )
				{
					case EZXK_NEXT:
					case EZXK_LEFT: 		_CurrentInputState |= LEFT;					break;
					case EZXK_PAUSE:
					case EZXK_DOWN: 		_CurrentInputState |= DOWN;					break;
					case EZXK_MINUS:
					case EZXK_UP: 			_CurrentInputState |= UP;					break;
					case EZXK_PLUS:
					case EZXK_RIGHT: 		_CurrentInputState |= RIGHT;				break;
					case EZXK_OK:			_CurrentInputState |= BUTTON_B;				break;
					case EZXK_CAMERA:		_KillFlag = true;							break;
					default: break;
				}
			} break;

			// check for key release
			case SDL_KEYUP:
			{
				switch( Event.key.keysym.sym )
				{
					case EZXK_NEXT:
					case EZXK_LEFT: 		_CurrentInputState &= ~LEFT;				break;
					case EZXK_PAUSE:
					case EZXK_DOWN: 		_CurrentInputState &= ~DOWN;				break;
					case EZXK_MINUS:
					case EZXK_UP: 			_CurrentInputState &= ~UP;					break;
					case EZXK_PLUS:
					case EZXK_RIGHT: 		_CurrentInputState &= ~RIGHT;				break;
					case EZXK_OK:			_CurrentInputState &= ~BUTTON_B;			break;
					case EZXK_CAMERA:		_KillFlag = true;							break;
					default: break;
				}
			} break;
			default: break;
		} // end switch
	} // end of message processing
}
