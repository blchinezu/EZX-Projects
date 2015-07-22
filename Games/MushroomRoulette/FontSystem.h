/*
	Code written by James Fletcher (LiX). 
	Copyright 2009 - 2010. 
	ethocybin@googlemail.com
*/

#ifndef FONTSYSTEM_H
#define FONTSYSTEM_H

#include "sdl_extra.h"
#include <string>
using namespace std;

#define MAX_CHARS 69

//Colours
#define FONT_NORMAL 0
#define FONT_RED 1
#define FONT_GREEN 2
#define FONT_BLUE 3
#define FONT_WHITE 4

//Font System Class
class FontSystem
{
public:
	FontSystem();
	~FontSystem();

	void DrawString(SDL_Surface* dest, int x, int y, string text, int colour);
	void DrawStringO(SDL_Surface* dest, int x, int y, string text, int colour, float opacity);

private:
	SDL_Surface* m_surf[MAX_CHARS];
	SDL_Surface* m_surf_red[MAX_CHARS];
	SDL_Surface* m_surf_green[MAX_CHARS];
	SDL_Surface* m_surf_blue[MAX_CHARS];
	SDL_Surface* m_surf_white[MAX_CHARS];
};

#endif


