/*
	Code written by James Fletcher (LiX). 
	Copyright 2009 - 2010. 
	ethocybin@googlemail.com

	You just get an effect pointer with GetFreeEffect(),
	set the effect variables and set active to 1 and it will play the effect
*/

#ifndef EFFECTMANAGER_H
#define EFFECTMANAGER_H

#include "sdl_extra.h"
#include "Vector2D.h"

//Effect Class
class Effect
{
public:
	Effect();
	void SetSpeed(int playspeed);
	void SetOpacity(float in);
	void SetPos(int x, int y);
	SDL_Surface* GetFrame(int i);
	void SetFrame(int i, SDL_Surface* frame);
	void Render();
	void Enable();
	bool IsActive();

private:
	int m_playspeed;
	int speed_regulator;
	SDL_Surface* m_frames[10];
	Vector2D m_pos;
	int m_playpos;
	bool m_active;
	float m_opacity;
};

//EffectManager Class
class EffectManager
{
public:
	void RenderEffects();
	Effect* GetFreeEffect();

private:
	Effect m_effects[10];
};


#endif









