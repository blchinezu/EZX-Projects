/*
	Code written by James Fletcher (LiX). 
	Copyright 2009 - 2010. 
	ethocybin@googlemail.com
*/

#ifndef ENTBUTTERFLY_H
#define ENTBUTTERFLY_H

#include "BaseEntity.h"
#include <vector>
using namespace std;

//Fly Directions
#define DIRECTION_UP 0
#define DIRECTION_DOWN 1
#define DIRECTION_LEFT 2
#define DIRECTION_RIGHT 3

//Player Entity Class
class EntButterfly : public BaseEntity
{
public:
	EntButterfly();
	~EntButterfly();

	//Functions
	void Update();
	void Render();
	void InitButterfly(int id, char* path);
	void Move(int direction);
	void MoveAllongPath(vector<Vector2D> path);

	//Accessors
	int GetID();

private:
	int m_id;
	int m_direction;
	bool m_dead;

	SDL_Surface* m_surf_up[3];
	SDL_Surface* m_surf_down[3];
	SDL_Surface* m_surf_left[3];
	SDL_Surface* m_surf_right[3];

	int m_flycycle;
	bool m_inanim; //1 - is in animation
	Vector2D m_animpos;
	int anim_regulator;
	int move_regulator;
	int ms_regulator; //movespeed_reg
	int mrv;

	char m_path[256];
	int m_pathpos;
};

#endif



