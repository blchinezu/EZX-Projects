/*
	Code written by James Fletcher (LiX). 
	Copyright 2009 - 2010. 
	ethocybin@googlemail.com
*/

#ifndef ENTMUSHROOM_H
#define ENTMUSHROOM_H

#include "BaseEntity.h"

//Types of Mushroom
#define MUSHROOM_RED 1
#define MUSHROOM_PINK 2
#define MUSHROOM_BLACK 3
#define MUSHROOM_PURPLE 4
#define MUSHROOM_BLUE 5
#define MUSHROOM_ESCAPE 6

//Mushroom Entity Class
class EntMushroom : public BaseEntity
{
public:
	EntMushroom();
	~EntMushroom();

	//Functions
	void Update();
	void Render();
	void InitMushroom(int itype);
	void SendEvent(int msg, void* addarg);

	//Accessors
	int GetType();
	int m_bluebonus;

private:
	int m_type;
	SDL_Surface* m_surf[6];

	//Whack-a-Mole
	bool IsMoleGameOver();
	bool cutfeed;
	int main_regulator;
	int beep_regulator;
	int mole_regulator[18];
	int next_update[18];
	Vector2D mole_pos[18];
};

#endif



