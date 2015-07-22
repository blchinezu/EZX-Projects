#include "wiz.h"
#include "global.h"

#include "SDL.h"

#include <cstdio>
#include <sys/ioctl.h>
#include <sys/soundcard.h>
#include <fcntl.h>
#include <unistd.h>

int EZX_volume;
int volume_direction;

 
void EZX_set_vol(int vol) {
	EZX_volume=vol;
	Mix_VolumeMusic(vol);
	Mix_Volume(-1, vol);
}

void EZX_init_vol() {
	EZX_set_vol(40);
}

void EZX_quit_vol() {
	EZX_set_vol(128);
}

 
// Check if clicked on rectangle surface on screen
char EZX_ClickedOver(int cx, int cy, int x, int y, int w, int h)
{
	if(cx>=x && cy>=y && cx<x+w && cy<y+h) return 1;
	return 0;
}

// Adding touchscreen buttons support for EZX
char EZX_TouchState(int place, int cx, int cy)
{
	switch(place)
	{
		case TopLeft:		if(EZX_ClickedOver(cx, cy,   0,   0, 160, 120)) return 1; break;
		case BottomLeft:	if(EZX_ClickedOver(cx, cy,   0, 120, 160, 120)) return 1; break;
		case TopRight:		if(EZX_ClickedOver(cx, cy, 160,   0, 160, 120)) return 1; break;
		case BottomRight:	if(EZX_ClickedOver(cx, cy, 160, 120, 160, 120)) return 1; break;
	}
	return 0;
}

void EZX_ProcessTouch(int cx, int cy, int value)
{
	if(EZX_TouchState(TopLeft, cx, cy))		WIZ_KeyboardEmulation(EZX_TopLeft, value);		else
	if(EZX_TouchState(BottomLeft, cx, cy))	WIZ_KeyboardEmulation(EZX_BottomLeft, value);	else
	if(EZX_TouchState(TopRight, cx, cy))	WIZ_KeyboardEmulation(EZX_TopRight, value);		else
	if(EZX_TouchState(BottomRight, cx, cy)) WIZ_KeyboardEmulation(EZX_BottomRight, value);
}

void WIZ_AdjustVolume( int direction )
{
	if( direction != VOLUME_NOCHG )
	{
		if( EZX_volume <= 2*VOLUME_CHANGE_RATE )
		{
			if( direction == VOLUME_UP )   EZX_volume += VOLUME_CHANGE_RATE/2;
			if( direction == VOLUME_DOWN ) EZX_volume -= VOLUME_CHANGE_RATE/2;
		}
		else
		{
			if( direction == VOLUME_UP )   EZX_volume += VOLUME_CHANGE_RATE;
			if( direction == VOLUME_DOWN ) EZX_volume -= VOLUME_CHANGE_RATE;
		}

		if( EZX_volume < VOLUME_MIN ) EZX_volume = VOLUME_MIN;
		if( EZX_volume > VOLUME_MAX ) EZX_volume = VOLUME_MAX;
		
		Mix_VolumeMusic(EZX_volume);
		Mix_Volume(-1, EZX_volume);
		
		printf( "Volume Change: %i\n", EZX_volume );
	}
}

void WIZ_KeyboardEmulation( int button, int value )
{
	SDL_Event fakeevent1;
	SDL_Event fakeevent2;

	//printf( "Button %d Value %d\n", button, value );

	if( value == 1 ) {
	      fakeevent1.type			= SDL_KEYDOWN;
	      fakeevent1.key.state		= SDL_PRESSED;
	      fakeevent1.key.type		= SDL_KEYDOWN;
	      fakeevent1.key.keysym.mod		= KMOD_NONE;

	      fakeevent2.type			= SDL_KEYDOWN;
	      fakeevent2.key.state		= SDL_PRESSED;
	      fakeevent2.key.type		= SDL_KEYDOWN;
	      fakeevent2.key.keysym.mod		= KMOD_NONE;
	}
	else {
	      fakeevent1.type			= SDL_KEYUP;
	      fakeevent1.key.state		= SDL_RELEASED;
	      fakeevent1.key.type		= SDL_KEYUP;
	      fakeevent1.key.keysym.mod		= KMOD_NONE;

	      fakeevent2.type			= SDL_KEYUP;
	      fakeevent2.key.state		= SDL_RELEASED;
	      fakeevent2.key.type		= SDL_KEYUP;
	      fakeevent2.key.keysym.mod		= KMOD_NONE;
	}

	fakeevent1.key.keysym.sym = SDLK_UNKNOWN;
	fakeevent2.key.keysym.sym = SDLK_UNKNOWN;
	switch(button)
	{
		case EZX_TopRight:		fakeevent1.key.keysym.sym = SDLK_MINUS;		break;
		case EZX_BottomRight:	fakeevent1.key.keysym.sym = SDLK_PLUS;		break;
		case EZX_TopLeft:		if( value == 1) volume_direction = VOLUME_UP;
								else			volume_direction = VOLUME_NOCHG;
								break;
		case EZX_BottomLeft:
								if( value == 1)		volume_direction = VOLUME_DOWN;
								else				volume_direction = VOLUME_NOCHG;
								break;
	}

	if( fakeevent1.key.keysym.sym != SDLK_UNKNOWN )
	{
		SDL_PushEvent (&fakeevent1);
	}
	if( fakeevent2.key.keysym.sym != SDLK_UNKNOWN )
	{
		SDL_PushEvent (&fakeevent2);
	}

	WIZ_AdjustVolume(volume_direction);
}
