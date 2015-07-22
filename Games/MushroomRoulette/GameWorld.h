/*
	Code written by James Fletcher (LiX). 
	Copyright 2009 - 2010. 
	ethocybin@googlemail.com
*/

#ifndef GAMEWORLD_H
#define GAMEWORLD_H

#include "sdl_extra.h"
#include <vector>
#include "EntPlayer.h"
#include "EntMushroom.h"
#include "EntBox.h"
#include "EntButterfly.h"
#include "EntWell.h"
#include "EntHat.h"
#include "Vector2D.h"
#include "sdl_extra.h"
#include "ContentManager.h"
#include "FontSystem.h"
#include "HighScores.h"
#include "EffectManager.h"
#include "Menu.h"

using namespace std;
#define MAX_CELLS 187 //(17*11)

//GameGrid Cell Structure
struct Cell
{
	BaseEntity* ent;
	Vector2D pos;
};

//GameWorld class
class GameWorld
{
public:
	GameWorld();
	~GameWorld();

    //Graphics
    SDL_Surface* backbuffer;

	//Functions
	void Update();
	void Render();
	void NewGame();
	void GenerateRandomLevel();
	void NextLevel();
	bool FreezeBoxes();
	void CheckIfLastRedMushroom();
	void GenerateMaze();
	void LoadLevel();

	//Spawn
	void AddBlueBonus(Vector2D pos, int score);

	//Input
	Vector2D GetNearestGridSquarePositionToMouse(int ix, int iy);

	//Accessors
	Cell* GetGridCell(int x, int y);
	EntPlayer* GetPlayer(int id);
	bool IsReady();
	int GetTotalPlayers();
	int GetLevel();

	//Public Variables
	bool m_isbonus;
	int m_bonustype;
	bool gentoggle;
	int m_gamemode;
	int m_beginscore; //Score when you begin each level
	string m_levelname; //Name of levelpack loaded
	int gamestart; //Game Loaded

	//Public Classes
	ContentManager gConMgr; //Content Manager
	FontSystem gFont;
	HighScores gScores;
	Menu gMenu;
	EffectManager gEffects;

private:
	//Game Grid and Entities
	Cell m_gamegrid[17][11];
	EntPlayer m_players[5]; //Up to 5 players at a time
	int m_totalplayers;
	EntMushroom m_mushrooms[MAX_CELLS]; //if you change this you have to update the GameWorld.cpp
	int m_totalmushrooms;
	EntBox m_boxes[MAX_CELLS];
	int m_totalboxes;
	EntButterfly m_butterflys[MAX_CELLS];
	int m_totalbutterflys;

	//Hat's randomly appear during gameplay
	EntHat m_hat[MAX_CELLS];
	int m_totalhats;
	int hat_regulator;
	int hat_nextshow;

	//Wells randomly appear during game play 
	EntWell m_well[MAX_CELLS];
	int m_totalwells;
	int well_regulator;
	int well_nextshow;

	//Client Player Info
	string m_name;
	int m_level;

	//Get ready, GO! message on new level
	bool m_ready;
	int m_readyreg;
	char m_description[256];

	//Graphics
	SDL_Surface* m_surf_background;
	SDL_Surface* m_surf_molebackground;
	SDL_Surface* m_surf_gr[2];

	//Spawn Functions
	void AddBoxes(int min, int max, bool type);
	void AddMushrooms(int min, int max, int type);
	void AddPlayer(string name);
	void AddButterfly(int id);
	void AddWell();
	void AddHat();
	void AddEscapeMushroom(int x, int y);
	void AddBoxPos(int x, int y);

	//Load Level Stuff
	void AddHatX(int x, int y, int hat);
	void AddButterflyX(int x, int y, char* path, int id);
	void AddPlayerX(string name, int x, int y);
	void AddBoxX(int x, int y, bool type);
	void AddWellX(int x, int y);
	void AddMushroomX(int x, int y, int type);

	//Debug
	void DumpMushrooms(string name);
	void EatRedMushrooms();
};

extern GameWorld gGameWorld;

#endif



