/*-----------------------------------------------------------------------------
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
#ifndef NJAM_UTILS_H
#define NJAM_UTILS_H
#include "SDL.h"
#include "njamfont.h"
//-----------------------------------------------------------------------------

void NjamSetRect(SDL_Rect& rect, int x, int y, int w=0, int h=0);
void NjamSetRectx(SDL_Rect& rect, int x, int y, int w=0, int h=0);
void NjamCopyRect(SDL_Rect& from, SDL_Rect& to);
SDLKey NjamGetch(bool Wait);
Uint8 GetClick(int& x, int &y, bool Wait = true);
int NjamRandom(int MaxValue);

typedef enum { fxBlackWhite, fxDarken } tEffect;
Uint32 GetPixel(SDL_Surface *surface, int x, int y);
void PutPixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
void PatternFill(SDL_Surface *pattern, SDL_Surface *surface);
bool NjamLoad(SDL_Surface **surface, const char *filename);
bool SurfaceEffect(SDL_Surface *surface, SDL_Rect& r, tEffect Effect = fxBlackWhite);
void CopySurfaceStreched(SDL_Surface *from, SDL_Surface *to);
SDL_Surface *CreateSurface(int w, int h);
//-----------------------------------------------------------------------------
class LogFile	// writes string to a file, used for logging
{
public:
	LogFile(const char *Text, bool Create = false);
};
//-----------------------------------------------------------------------------
class CommonException
{
private:
	int m_flag;
public:
	CommonException(int flag) { m_flag = flag; };
};
//-----------------------------------------------------------------------------
#endif
