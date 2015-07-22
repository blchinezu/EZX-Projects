/*
	Code written by James Fletcher (LiX). 
	Copyright 2009 - 2010. 
	ethocybin@googlemail.com
*/

#include "Menu.h"
#include "GameWorld.h"
#include <stdio.h>
#include "MushRlt.h"
bool loadimages=0;



char ToLower(char c)
{
     if(c == 'A'){return 'a';}
     if(c == 'B'){return 'b';}
     if(c == 'C'){return 'c';}
     if(c == 'D'){return 'd';}
     if(c == 'E'){return 'e';}
     if(c == 'F'){return 'f';}
     if(c == 'G'){return 'g';}
     if(c == 'H'){return 'h';}
     if(c == 'I'){return 'i';}
     if(c == 'J'){return 'j';}
     if(c == 'K'){return 'k';}
     if(c == 'L'){return 'l';}
     if(c == 'M'){return 'm';}
     if(c == 'N'){return 'n';}
     if(c == 'O'){return 'o';}
     if(c == 'P'){return 'p';}
     if(c == 'Q'){return 'q';}
     if(c == 'R'){return 'r';}
     if(c == 'S'){return 's';}
     if(c == 'T'){return 't';}
     if(c == 'U'){return 'u';}
     if(c == 'V'){return 'v';}
     if(c == 'W'){return 'w';}
     if(c == 'X'){return 'x';}
     if(c == 'Y'){return 'y';}
     if(c == 'Z'){return 'z';}
     
     return 0;
}

//Constructor
Menu::Menu()
{
	m_enabled = 1;
	m_selected = 0;
	m_cp=0;
	m_credits=0;
	m_settings=0;
	m_sound = 1;
	m_colourblind = 0;
	m_levelpacks = 0;
	m_maxpacks=0;

	//Load avalible levelpack names
	FILE* o = fopen("levelpacks.txt", "r");
	if(o != NULL)
	{
		for(int i = 0; i < 32; i++)
		{
			fscanf(o, "%s", levelpacks[i].name);
			if(levelpacks[i].name[0] == 0x00){m_maxpacks = i+1; break;}
		}

		fclose(o);
	}
}

//Destructor
Menu::~Menu()
{
	//
}


//Called on each frame
void Menu::Render()
{
	if(loadimages == 0)
	{
		m_surf_background = gGameWorld.gConMgr.GetSurface("sprites/menu.fif");
		m_surf_c[0] = gGameWorld.gConMgr.GetSurface("sprites/c1.fif");
		m_surf_c[1] = gGameWorld.gConMgr.GetSurface("sprites/c2.fif");
		m_surf_c[2] = gGameWorld.gConMgr.GetSurface("sprites/c3.fif");
		m_surf_c[3] = gGameWorld.gConMgr.GetSurface("sprites/c4.fif");
		m_surf_levelpacks = gGameWorld.gConMgr.GetSurface("sprites/levelpacks.fif");
		loadimages = 1;
	}

	//Draw Menu Background
	if(m_credits == 1)
	{
		SDL_BlitSurface(m_surf_c[m_cp], 0, gGameWorld.backbuffer, &SDLE_Rect(40, 40, m_surf_c[m_cp]->w, m_surf_c[m_cp]->h));
		return;
	}


	//If levelpacks
	if(m_levelpacks == 1)
	{
		SDL_BlitSurface(m_surf_levelpacks, 0, gGameWorld.backbuffer, &SDLE_Rect(60, 10, m_surf_levelpacks->w, m_surf_levelpacks->h));
		
		if(m_selected == 0){gGameWorld.gFont.DrawString(gGameWorld.backbuffer, 108, 40, "Random Levels", FONT_RED);}
		else{gGameWorld.gFont.DrawString(gGameWorld.backbuffer, 108, 40, "Random Levels", FONT_BLUE);}

		int y = 40;
		for(int i = 0; levelpacks[i].name[0] != 0x00; i++)
		{
			y += 14;
			string lp = levelpacks[i].name;
			lp += " Levels";
			if(m_selected == i+1){gGameWorld.gFont.DrawString(gGameWorld.backbuffer, 108, y, lp, FONT_RED);}
			else{gGameWorld.gFont.DrawString(gGameWorld.backbuffer, 108, y, lp, FONT_BLUE);}
		}

		return;
	}


	//Draw Menu Background
	SDL_BlitSurface(m_surf_background, 0, gGameWorld.backbuffer, &SDLE_Rect(40, 40, m_surf_background->w, m_surf_background->h));


	//Draw Settings
	if(m_settings == 1)
	{
		if(m_sound == 1)
		{
			if(m_selected == 0){gGameWorld.gFont.DrawString(gGameWorld.backbuffer, 108, 106, "Sound: ON", FONT_RED);}
			else{gGameWorld.gFont.DrawString(gGameWorld.backbuffer, 108, 106, "Sound: ON", FONT_BLUE);}
		}

		if(m_sound == 0)
		{
			if(m_selected == 0){gGameWorld.gFont.DrawString(gGameWorld.backbuffer, 108, 106, "Sound: OFF", FONT_RED);}
			else{gGameWorld.gFont.DrawString(gGameWorld.backbuffer, 108, 106, "Sound: OFF", FONT_BLUE);}
		}

		if(m_colourblind == 1)
		{
			if(m_selected == 1){gGameWorld.gFont.DrawString(gGameWorld.backbuffer, 108, 120, "Colour Blind: ON", FONT_RED);}
			else{gGameWorld.gFont.DrawString(gGameWorld.backbuffer, 108, 120, "Colour Blind: ON", FONT_BLUE);}
		}

		if(m_colourblind == 0)
		{
			if(m_selected == 1){gGameWorld.gFont.DrawString(gGameWorld.backbuffer, 108, 120, "Colour Blind: OFF", FONT_RED);}
			else{gGameWorld.gFont.DrawString(gGameWorld.backbuffer, 108, 120, "Colour Blind: OFF", FONT_BLUE);}
		}
		
		if(m_system == 0)
		{
			if(m_selected == 2){gGameWorld.gFont.DrawString(gGameWorld.backbuffer, 108, 134, "System: GP2X", FONT_RED);}
			else{gGameWorld.gFont.DrawString(gGameWorld.backbuffer, 108, 134, "System: GP2X", FONT_BLUE);}
		}

		if(m_system == 1)
		{
			if(m_selected == 2){gGameWorld.gFont.DrawString(gGameWorld.backbuffer, 108, 134, "System: WIZ", FONT_RED);}
			else{gGameWorld.gFont.DrawString(gGameWorld.backbuffer, 108, 134, "System: WIZ", FONT_BLUE);}
		}

		if(m_selected == 3){gGameWorld.gFont.DrawString(gGameWorld.backbuffer, 108, 148, "Back", FONT_RED);}
		else{gGameWorld.gFont.DrawString(gGameWorld.backbuffer, 108, 148, "Back", FONT_BLUE);}

		return;
	}


	//Draw Menu Items
	if(m_selected == 0){gGameWorld.gFont.DrawString(gGameWorld.backbuffer, 128, 86, "New Game", FONT_RED);}
	else{gGameWorld.gFont.DrawString(gGameWorld.backbuffer, 128, 86, "New Game", FONT_BLUE);}
	
	if(m_selected == 1){gGameWorld.gFont.DrawString(gGameWorld.backbuffer, 136, 100, "Settings", FONT_RED);}
	else{gGameWorld.gFont.DrawString(gGameWorld.backbuffer, 136, 100, "Settings", FONT_BLUE);}

	if(m_selected == 2){gGameWorld.gFont.DrawString(gGameWorld.backbuffer, 123, 114, "High Scores", FONT_RED);}
	else{gGameWorld.gFont.DrawString(gGameWorld.backbuffer, 123, 114, "High Scores", FONT_BLUE);}

	if(m_selected == 3){gGameWorld.gFont.DrawString(gGameWorld.backbuffer, 138, 131, "Credits", FONT_RED);}
	else{gGameWorld.gFont.DrawString(gGameWorld.backbuffer, 138, 131, "Credits", FONT_BLUE);}

	if(m_selected == 4){gGameWorld.gFont.DrawString(gGameWorld.backbuffer, 128, 146, "Quit Game", FONT_RED);}
	else{gGameWorld.gFont.DrawString(gGameWorld.backbuffer, 128, 146, "Quit Game", FONT_BLUE);}


	if(gGameWorld.m_gamemode == 1)
	{
		if(gGameWorld.GetPlayer(0)->GetLife() <= 0){return;}
		char rl[32];
		sprintf(rl, "Restart Level -%i", abs((gGameWorld.GetPlayer(0)->GetScore() - gGameWorld.m_beginscore) + gGameWorld.GetPlayer(0)->GetScore()/6));

		if(m_selected == 5){gGameWorld.gFont.DrawString(gGameWorld.backbuffer, 90, 170, rl, FONT_RED);}
		else{gGameWorld.gFont.DrawString(gGameWorld.backbuffer, 90, 170, rl, FONT_BLUE);}
	}
}


//Moves menu selection
void Menu::MenuMove(bool direction)
{
	if(gGameWorld.gScores.m_enabled == 1 || m_credits == 1){return;}
	if(direction == 0 && m_selected-1 < 0){return;}
	if(m_settings == 1){if(direction == 1 && m_selected+1 > 3){return;}}

	if(gGameWorld.gMenu.m_levelpacks == 0)
	{
		if(gGameWorld.m_gamemode == 1)
		{
			if(direction == 1 && m_selected+1 > 5){return;}
		}

		if(gGameWorld.m_gamemode == 0)
		{
			if(direction == 1 && m_selected+1 > 4){return;}
		}
	}

	if(gGameWorld.gMenu.m_levelpacks == 1)
	{
		if(direction == 1 && m_selected+1 > m_maxpacks-1){return;}
	}
	
	if(direction == MENU_UP){m_selected--;}
	if(direction == MENU_DOWN){m_selected++;}
}


//Selects current menu selection
void Menu::MenuSelect()
{
	if(gGameWorld.gMenu.m_levelpacks == 1)
	{
		if(m_selected == 0)
		{
			gGameWorld.NewGame();
			m_levelpacks = 0;
			m_enabled = 0;
		}
		else
		{
			gGameWorld.NewGame();
			gGameWorld.m_levelname = levelpacks[m_selected-1].name;
			gGameWorld.LoadLevel();
			m_levelpacks = 0;
			m_enabled = 0;
		}

		return;
	}

	//Settings
	if(m_settings == 1)
	{
		if(m_selected == 0)
		{
			m_sound =! m_sound;
		}

		if(m_selected == 1)
		{
			m_colourblind =! m_colourblind;
		}

		if(m_selected == 2)
		{
			m_system =! m_system;
			
			if(m_system == 0)
			{
            	updatetime = 33;
                screentime = 40;
                return;
            }
            
            if(m_system == 1)
			{
            	updatetime = 10;
                screentime = 33;
                return;
            }
		}
		
		if(m_selected == 3)
		{
			m_selected = 1;
			m_settings = 0;
		}

		return;
	}
	
	//New Game
	if(m_selected == 0)
	{
		m_levelpacks = 1;
	}

	//Settings
	if(m_selected == 1)
	{
		m_selected = 0;
		m_settings = 1;
	}

	//HighScores
	if(m_selected == 2)
	{
		gGameWorld.gScores.SetEnabled(!gGameWorld.gScores.m_enabled);
	}

	//Credits
	if(m_selected == 3)
	{
		if(m_credits == 0){m_credits=1; return;}
		m_cp++;
		if(m_cp > 3){m_cp=0; m_credits=0;}
	}

	//Restart Level
	if(m_selected == 5)
	{
		if(gGameWorld.GetPlayer(0)->GetLife() <= 0){return;}
		//gGameWorld.GetPlayer(0)->GetScore() = gGameWorld.m_beginscore - gGameWorld.GetPlayer(0)->GetScore()/6;
		gGameWorld.GetPlayer(0)->GetScore() -= abs((gGameWorld.GetPlayer(0)->GetScore() - gGameWorld.m_beginscore) + gGameWorld.GetPlayer(0)->GetScore()/6);
		gGameWorld.LoadLevel();
		m_enabled = 0;
	}
}



