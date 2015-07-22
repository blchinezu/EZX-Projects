/*
	Code written by James Fletcher (LiX). 
	Copyright 2009 - 2010. 
	ethocybin@googlemail.com

	You just get an effect pointer with GetFreeEffect(),
	set the effect variables and set active to 1 and it will play the effect
*/

#include "EffectManager.h"
#include "sdl_extra.h"
#include "GameWorld.h"

//Effect constructor
Effect::Effect()
{
	m_playspeed=0;
	m_playpos=0;
	m_active=0;
	speed_regulator=0;
	for(int i = 0; i < 10; i++){m_frames[i] = NULL;}
}

//Accessors
bool Effect::IsActive(){return m_active;}
void Effect::SetOpacity(float in){m_opacity = in;}
void Effect::Enable(){m_active = 1; speed_regulator = SDL_GetTicks();}
void Effect::SetPos(int x, int y){m_pos.Set(x, y);}

//Set's effect animation speed
void Effect::SetSpeed(int playspeed)
{
	m_playspeed = playspeed;
}

//Get's a frame of the effect
SDL_Surface* Effect::GetFrame(int i)
{
	return m_frames[i];
}

//Set's a frame of the effect
void Effect::SetFrame(int i, SDL_Surface* frame)
{
	m_frames[i] = frame;
}

//Renders an effect
void Effect::Render()
{
	if(SDL_GetTicks() > speed_regulator+m_playspeed)
	{
		m_playpos++;

		if(m_frames[m_playpos] == NULL)
		{
			m_playspeed=0;
			m_playpos=0;
			m_active=0;
			speed_regulator=0;
			for(int i = 0; i < 10; i++){m_frames[i] = NULL;}
			return;
		}

		speed_regulator = SDL_GetTicks();
	}

	if(m_opacity == 0)
	{
		SDL_BlitSurface(m_frames[m_playpos], 0, gGameWorld.backbuffer, &SDLE_Rect(m_pos.x, m_pos.y, m_frames[m_playpos]->w, m_frames[m_playpos]->h));
	}else{
		SDLE_OpacityBlit(m_frames[m_playpos], gGameWorld.backbuffer, m_pos.x, m_pos.y, m_opacity);
	}
}

//Renders all effects
void EffectManager::RenderEffects()
{
	for(int i = 0; i < 10; i++)
	{
		if(m_effects[i].IsActive() == 1)
		{
			m_effects[i].Render();
		}
	}
}

//Get a pointer to an effect
Effect* EffectManager::GetFreeEffect()
{
	for(int i = 0; i < 10; i++)
	{
		if(m_effects[i].IsActive() == 0){return &m_effects[i];}
	}

	return NULL;
}









