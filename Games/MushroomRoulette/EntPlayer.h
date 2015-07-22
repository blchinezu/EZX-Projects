/*
	Code written by James Fletcher (LiX). 
	Copyright 2009 - 2010. 
	ethocybin@googlemail.com
*/

#ifndef ENTPLAYER_H
#define ENTPLAYER_H

#include "BaseEntity.h"
#include <string>
#include <vector>
using namespace std;

//Walk Directions
#define DIRECTION_UP 0
#define DIRECTION_DOWN 1
#define DIRECTION_LEFT 2
#define DIRECTION_RIGHT 3

//Player Entity Class
class EntPlayer : public BaseEntity
{
public:
	EntPlayer();
	~EntPlayer();

	//Functions
	void Update();
	void Render();
	void InitPlayer(string name);
	void Move(int direction);
	void MoveAllongPath(vector<Vector2D> path);
	void SendEvent(int msg, void* addarg);

	//Accessors
	int& GetLife();
	int& GetScore();
	string GetName();
	int GetDirection();
	bool m_walldestroy;
	void ResetAnim();

private:
	string m_name;
	int m_score;
	int m_life;
	bool m_dead;
	bool m_evolved;
	int m_pollenated;
	int m_hat;
	int tf; //Toggle Flip (for setting image pointer)

	int m_direction;

	vector<Vector2D> m_path;
	int m_pathpos;

	SDL_Surface* m_surf_up[3];
	SDL_Surface* m_surf_down[3];
	SDL_Surface* m_surf_left[3];
	SDL_Surface* m_surf_right[3];
	SDL_Surface* m_surf_hat[4];

	int m_walkcycle;
	bool m_inanim; //1 - is in animation
	Vector2D m_animpos;
	int anim_regulator;
};

#endif



