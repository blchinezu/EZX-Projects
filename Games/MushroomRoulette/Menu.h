/*
	Code written by James Fletcher (LiX). 
	Copyright 2009 - 2010. 
	ethocybin@googlemail.com
*/

#ifndef MENU_H
#define MENU_H

#include "sdl_extra.h"

//Menu Directions
#define MENU_UP 0
#define MENU_DOWN 1

//Level Struct
struct level
{
	char name[32];
};

//Menu Class
class Menu
{
public:
	Menu();
	~Menu();

	//Functions
	void Render();
	void MenuMove(bool direction);
	void MenuSelect();

	//Global Variables
	bool m_enabled;
	int m_selected;
	bool m_credits;
	bool m_settings;
	bool m_system;

	bool m_sound;
	bool m_colourblind;

	bool m_levelpacks;
	int m_maxpacks;

private:
	SDL_Surface* m_surf_background;
	SDL_Surface* m_surf_c[4];
	SDL_Surface* m_surf_levelpacks;
	level levelpacks[32];
	int m_cp;
};

#endif






