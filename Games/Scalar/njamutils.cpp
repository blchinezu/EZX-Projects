/*-----------------------------------------------------------------------------
Njamutils.cpp

Various utility functions to simplify programming.
See njamutils.h for function list.

Copyright 2003 Milan Babuskov

This file is part of Scalar (http://scalar.sourceforge.net).

Scalar is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

Scalar is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Scalar in file COPYING; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include "SDL.h"
#include "SDL_image.h"
#include "njamutils.h"
#include "njamfont.h"

//-----------------------------------------------------------------------------
void NjamCopyRect(SDL_Rect& from, SDL_Rect& to)
{
	to.x = from.x;
	to.y = from.y;
	to.w = from.w;
	to.h = from.h;
}
//-----------------------------------------------------------------------------
void CopySurfaceStreched(SDL_Surface *from, SDL_Surface *to)
{
	if ( SDL_MUSTLOCK(from) && SDL_LockSurface(from) < 0 )
	{
		fprintf(stderr, "Can't lock surface: %s\n", SDL_GetError());
		return;
	}
	if ( SDL_MUSTLOCK(to) && SDL_LockSurface(to) < 0 )
	{
		fprintf(stderr, "Can't lock surface: %s\n", SDL_GetError());
		return;
	}

	double kx = (double)(from->w-1)/(double)(to->w-1);
	double ky = (double)(from->h-1)/(double)(to->h-1);

	Uint8 r, g, b;
	for (double x=0; x < to->w; x++)
	{
		for (double y=0; y < to->h; y++)
		{
			// get pixel that matches in source
			SDL_GetRGB(GetPixel(from, int(x * kx), int(y * ky)), from->format, &r, &g, &b);
			PutPixel(to, int(x), int(y), SDL_MapRGB(to->format, r, g, b));
		}
	}

    if ( SDL_MUSTLOCK(from) )
        SDL_UnlockSurface(from);
    if ( SDL_MUSTLOCK(to) )
        SDL_UnlockSurface(to);
}
//-----------------------------------------------------------------------------
bool NjamLoad(SDL_Surface **surface, const char *filename)
{
 	SDL_Surface *temp;
 	printf("Loading image: %s ... ", filename);
 	temp = IMG_Load(filename);
 	if (!temp)
 	{
 		printf("FAILED.\nError: %s\n", SDL_GetError());
 		return false;
 	}
 	printf("OK.\n");

	// Convert image to video format
	*surface = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);
	if ( *surface == NULL )
	{
		printf("Couldn't convert image: %s\n",	SDL_GetError());
		return false;
	}

	return true;
}
//-----------------------------------------------------------------------------
void NjamSetRect(SDL_Rect& rect, int x, int y, int w, int h)
{
 	rect.x = x/2.5;
 	rect.y = y/2.5;
 	rect.w = w/2.5;
 	rect.h = h/2.5;
}
//-----------------------------------------------------------------------------
void NjamSetRectx(SDL_Rect& rect, int x, int y, int w, int h)
{
 	rect.x = x;
 	rect.y = y;
 	rect.w = w;
 	rect.h = h;
}
//-----------------------------------------------------------------------------
void PatternFill(SDL_Surface *pattern, SDL_Surface *surface)
{
	int w1, w2, h1, h2;

	w2 = surface->w*2.5;
	h2 = surface->h*2.5;
	w1 = pattern->w*2.5;
	h1 = pattern->h*2.5;

	SDL_Rect to;
	for (int x=0; x < w2/w1 + 1; x++)
	{
		for (int y=0; y < h2/h1 + 1; y++)
		{
			NjamSetRect(to, x * w1, y * h1);
			SDL_BlitSurface(pattern, NULL, surface, &to);
		}
	}
}
//-----------------------------------------------------------------------------
SDL_Surface *CreateSurface(int w, int h)
{
    SDL_Surface *surface;
    Uint32 rmask, gmask, bmask, amask;

    /* SDL interprets each pixel as a 32-bit number, so our masks must depend
       on the endianness (byte order) of the machine */
	#if SDL_BYTEORDER == SDL_BIG_ENDIAN
		rmask = 0xff000000;
		gmask = 0x00ff0000;
		bmask = 0x0000ff00;
		amask = 0x000000ff;
	#else
		rmask = 0x000000ff;
		gmask = 0x0000ff00;
		bmask = 0x00ff0000;
		amask = 0xff000000;
	#endif

    surface = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 32, rmask, gmask, bmask, amask);
    if (surface == NULL)
        fprintf(stderr, "CreateRGBSurface failed: %s\n", SDL_GetError());

	return surface;
}

//-----------------------------------------------------------------------------
Uint8 GetClick(int& x, int &y, bool Wait)
{
	// init random nr. generator
	srand( SDL_GetTicks() );

	SDL_Event event;
	do
	{
		SDL_GetMouseState(&x, &y);	// in case we never enter the while loop below

		while(SDL_PollEvent(&event))	 // Loop until there are no events left on the queue
		{
			if (event.type == SDL_QUIT)
				throw CommonException(1);

			if (event.type == SDL_MOUSEBUTTONDOWN)
			{
				Uint8 state = SDL_GetMouseState(&x, &y);	// has to be checked again, since it seems that
				if (state & SDL_BUTTON(1))					// Polling the event, resets the state. Ohterwise I was
					return 1;								// unable to "catch" a click.
				if (state & SDL_BUTTON(2))
					return 2;
				if (state & SDL_BUTTON(3))
					return 3;
			} else
			if (event.type == SDL_KEYDOWN)
			{
				if(event.key.keysym.sym==SDLK_ESCAPE)	throw CommonException(1);			else
				if(event.key.keysym.sym==SDLK_SPACE)	return 4;							else
				if(event.key.keysym.sym==SDLK_MINUS)	return 5;							else
				if(event.key.keysym.sym==SDLK_PLUS)		return 6;
			}
		}
	}
	while (Wait);

	return 0;
}
//-----------------------------------------------------------------------------
SDLKey NjamGetch(bool Wait)
{
    do
    {
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				throw CommonException(2);
			if (event.type == SDL_KEYDOWN)
				return event.key.keysym.sym;
		}
    }
    while (Wait);

    return SDLK_LAST;	// this is probably never used by SDL, so it denotes that key isn't pressed
}
//-----------------------------------------------------------------------------
int NjamRandom(int MaxValue)
{
 	return ((int)((double)MaxValue*rand()/(RAND_MAX+1.0)));	// according to rand man page
}
//-----------------------------------------------------------------------------
// general logging function
LogFile::LogFile(const char *Text, bool Create)
{
 /*   FILE *fptr;
    if (Create)
        fptr = fopen("log.txt", "w+");
    else
        fptr = fopen("log.txt", "a+");

    if (fptr)
    {
        fprintf(fptr, "%s", Text);
        fclose(fptr);
    }*/
    printf("%s", Text);
}
//-----------------------------------------------------------------------------
// Return the pixel value at (x, y)
// NOTE: The surface must be locked before calling this!
Uint32 GetPixel(SDL_Surface *surface, int x, int y)
{
	if (x >= surface->w || y >= surface->h)
		return 0;

    int bpp = surface->format->BytesPerPixel;
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp)
	{
		case 1:	        return *p;
		case 2:	        return *(Uint16 *)p;
		case 3:
			#pragma warn -ccc
			#pragma warn -rch
			if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
				return p[0] << 16 | p[1] << 8 | p[2];
			else
				return p[0] | p[1] << 8 | p[2] << 16;
			#pragma warn +ccc
			#pragma warn +rch

		case 4:	        return *(Uint32 *)p;
		default:        return 0;       /* shouldn't happen, but avoids warnings */
    }
}
//-----------------------------------------------------------------------------
// Set the pixel at (x, y) to the given value
// NOTE: The surface must be locked before calling this!
void PutPixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
//	x/=2.5;
//	y/=2.5;

    int bpp = surface->format->BytesPerPixel;
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp)
	{
		case 1:        *p = pixel;        			break;
		case 2:        *(Uint16 *)p = pixel;        break;
		case 3:
			#pragma warn -ccc
			#pragma warn -rch
			if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
				p[0] = (pixel >> 16) & 0xff;
				p[1] = (pixel >> 8) & 0xff;
				p[2] = pixel & 0xff;
			} else {
				p[0] = pixel & 0xff;
				p[1] = (pixel >> 8) & 0xff;
				p[2] = (pixel >> 16) & 0xff;
			}
			break;
			#pragma warn +ccc
			#pragma warn +rch

		case 4:			*(Uint32 *)p = pixel;		break;
    }
}
//-----------------------------------------------------------------------------
bool SurfaceEffect(SDL_Surface *surface, SDL_Rect& r, tEffect Effect)
{
    int x, y;
	if ( SDL_MUSTLOCK(surface) && SDL_LockSurface(surface) < 0 )
	{
		fprintf(stderr, "njamutils.cpp: Can't lock surface: %s\n", SDL_GetError());
		return false;
	}

	Uint8 r1, g1, b1;
	for (int x = r.x; x < r.w+r.x; x++)
	{
		for (int y = r.y; y < r.h+r.y; y++)
		{
			SDL_GetRGB(GetPixel(surface, x, y), surface->format, &r1, &g1, &b1);
			int val = (r1 + g1 + b1) / 3;
			Uint32 color;

			if (Effect == fxBlackWhite)
				color = SDL_MapRGB(surface->format, (unsigned char)(val/1.1), (unsigned char)val, (unsigned char)(val/1.1));
			else if (Effect == fxDarken)
				color = SDL_MapRGB(surface->format, r1/2, g1/2, b1/2);
			PutPixel(surface, x, y, color);
		}
	}

    if ( SDL_MUSTLOCK(surface) )
        SDL_UnlockSurface(surface);

	return true;
}
//-----------------------------------------------------------------------------
