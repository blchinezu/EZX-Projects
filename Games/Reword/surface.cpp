////////////////////////////////////////////////////////////////////
/*

File:			surface.cpp

Class impl:		Surface

Description:	A class to wrap a SDL surface. Screen and Image classes are 
				derived from this.

Author:			Al McLuckie (al-at-purplepup-dot-org)

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
#include "surface.h"
#include "utils.h"

Surface::Surface() : _surface(0)
{
}

Surface::~Surface()
{
	cleanUp();
}

//create a specific size surface, used mainly by Image class
bool Surface::create(unsigned int w, unsigned int h, int iAlpha /*=-1*/)
{
	cleanUp();	//destroy any existing surface
	SDL_Surface *s;

#if defined GP2X
	s = SDL_CreateRGBSurface(SDL_SWSURFACE,w,h,16,0,0,0,0);
#else
	s = SDL_CreateRGBSurface(SDL_HWSURFACE|SDL_DOUBLEBUF,w,h,32,0,0,0,0);
#endif

	bool b = initSurface(s, iAlpha);

	return (b && _surface != NULL);
}

bool Surface::initSurface(SDL_Surface *newSurface, int iAlpha /* = -1 */)
{
	if( newSurface != NULL ) 
	{
		//Create an optimized image 
		_surface = SDL_DisplayFormat( newSurface ); 
		//Free the old image 
		SDL_FreeSurface( newSurface ); 

		//set the transparent pixel
		if (iAlpha >= 0)											//-1 = no transparency
		{
			Uint32 key = SDL_MapRGB(_surface->format, 255,0,255);	//putrid purple
			SDL_SetColorKey(_surface, SDL_SRCCOLORKEY, key);
			SDL_SetAlpha(_surface, SDL_SRCALPHA, iAlpha); //0=transparent, 255 = full opacity
		}	
		return true;
	}
	return false;
}

void Surface::cleanUp()
{
	if (_surface) SDL_FreeSurface(_surface);
	_surface = 0;
	_r.x = _r.y = _r.w = _r.h = 0;
}

void Surface::setSurface(SDL_Surface *s)
{
	if (s == NULL) return;
	cleanUp();
	_surface = s;
}

// Get format
SDL_PixelFormat* Surface::format(void) const
{
	if (!_surface) return 0;
    return _surface->format;
}

//get actual surface
SDL_Surface * Surface::surface(void) const
{
	if (!_surface) return 0;
	return _surface;
}

// Get width
int Surface::width(void) const
{
	if (!_surface) return 0;
    return _surface->w;
}

// Get height
int Surface::height(void) const 
{
	if (!_surface) return 0;
    return _surface->h;
}

//public draw functions
void Surface::drawSolidRect (int x, int y, int w, int h, const SDL_Color& c)
{
	// Draw a rectangle
    _r.x = x;
    _r.y = y;
    _r.w = w;
    _r.h = h;
    Uint32 colour = SDL_MapRGB(_surface->format, c.r, c.g, c.b);
    SDL_FillRect (_surface, &_r, colour);   
}


//void Surface::putPixel (int x, int y, const Colour& c)
void Surface::PutPixel(int x, int y, Uint32 colour)
{
	// Put pixel of colour c at x,y
	// If colour is NONE - no pixel is ploted
    if (x >=0 && x < _surface->w && y >=0 && y < _surface->h) // && c != Colour::NONE)
    {
		unsigned short* dst = static_cast<unsigned short*>(_surface->pixels);
		dst[y * _surface->pitch/sizeof(unsigned short) + x] = (unsigned short)colour;	//##fudge!!
    }    
}

//[static] function to blit, but not just to _screen - both source and destination surface needed
void Surface::blit_surface(SDL_Surface* source, SDL_Rect* srcRect, SDL_Surface* dest, int destX, int destY) 
{
	SDL_Rect destRect;	//needed local as this is static fn
	destRect.x = destX; 
	destRect.y = destY; 
	SDL_BlitSurface( source, srcRect, dest, &destRect ); 
}

//leave srcRect clip param 0 (or NULL) to blit whole surface or specify srcRect clip to blit 
//only part of the source to the destination
void Surface::blit_surface(SDL_Surface* source, SDL_Rect* srcRect, int destX, int destY) 
{ 
	//set up destination rectangle
	_r.x = destX; 
	_r.y = destY; 
    _r.w = _r.h = 0;	//height/width ignored by SDL_BlitSurface
	//do it
	SDL_BlitSurface( source, srcRect, _surface, &_r ); 
}



