////////////////////////////////////////////////////////////////////
/*

File:			fontttf.cpp

Class impl:		FontTTF

Description:	A class to wrap all font handling, using the SDL_TTF dll

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
#include "fontttf.h"

#include <iostream>


FontTTF::FontTTF() : _font(0), _init(false)
{
	_shadow = BLACK_COLOUR;
}

FontTTF::FontTTF(std::string fileName, int size) : _font(0), _init(false)
{
	load(fileName, size);
	memset(&_shadow, 0, sizeof(SDL_Color));	//black {0x00,0x00,0x00,0},
}

FontTTF::~FontTTF()
{
	cleanUp();
}

void FontTTF::cleanUp()
{
	if (_font) TTF_CloseFont(_font);
	_font = 0;
	_init = false;
}

bool FontTTF::load(std::string fileName, int size)
{
	cleanUp();
	_font = TTF_OpenFont( fileName.c_str(), size );
	if (NULL == _font)
		std::cerr << "Failed to load font " << fileName << ". Cannot start." << std::endl;
	_init = (_font != NULL);
	return _init;
}

//output a text string to the destination surface
//If shadow selected, then draw in the shadow colour first offset by 1, then the text in the actual position
int FontTTF::put_text(Surface *s, int x, int y, const char *textstr, const SDL_Color &textColour, bool bShadow /*= false*/)
{
	if (bShadow)
	{
		SDL_Surface *text = TTF_RenderText_Blended( _font, textstr, _shadow ); 
		if (text != NULL)
		{
			if (-999 == x) x = (s->surface()->w - text->w ) / 2;
			s->blit_surface(text, 0, x+1, y+1);
			SDL_FreeSurface(text);
		}
	}

	SDL_Surface *text = TTF_RenderText_Blended( _font, textstr, textColour ); 
	if (text != NULL)
	{
		if (-999 == x) x = (s->surface()->w - text->w ) / 2;
		s->blit_surface(text, 0, x, y);
		SDL_FreeSurface(text);
	}

	return x;	//in case y only (-999) option used - give caller position of text
}

//centered text
int FontTTF::put_text(Surface *s, int y, const char *textstr, const SDL_Color &textColour, bool bShadow /*= false*/)
{
	return put_text(s, -999, y, textstr, textColour, bShadow);
}

int FontTTF::put_text_right(Surface *s, int y, const char *textstr, const SDL_Color &textColour, bool bShadow /*= false*/)
{
	SDL_Surface *text = TTF_RenderText_Blended( _font, textstr, textColour ); 
	return put_text(s, s->surface()->w - text->w, y, textstr, textColour, bShadow);
}

//output a integer number string to the destination surface
int FontTTF::put_number(Surface *s, int x, int y, unsigned int number, const char *format, const SDL_Color &textColor, bool bShadow /*= false*/)
{
	sprintf(_buffer, format, number);	//if it exceeds buffer[100], we're screwed!
	return put_text(s, x, y, _buffer, textColor, bShadow);
}

//center a integer number string to the destination surface
int FontTTF::put_number(Surface *s, int y, unsigned int number, const char *format, const SDL_Color &textColor, bool bShadow /*= false*/)
{
	sprintf(_buffer, format, number);	//if it exceeds buffer[100], we're screwed!
	return put_text(s, -999, y, _buffer, textColor, bShadow);
}

//set the font shadow (to something other than the default black) 
void FontTTF::setShadowColour(SDL_Color &c)
{
	_shadow = c;
}

