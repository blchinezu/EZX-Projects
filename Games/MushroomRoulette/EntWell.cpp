/*
	Code written by James Fletcher (LiX). 
	Copyright 2009 - 2010. 
	ethocybin@googlemail.com
*/

#include "EntWell.h"
#include "GameWorld.h"
#include "sdl_extra.h"
#include "Sound.h"
#include "Rand.h"


//Constructor
EntWell::EntWell()
{
	anim_regulator = 0;
	m_wellstate = 0;
	m_mode = 0;
}

//Destructor
EntWell::~EntWell()
{

}


//Initializes the well entity
void EntWell::InitWell(bool mode)
{
	m_mode = mode;
	if(m_mode == 1){m_wellstate = 1;}
	if(m_surf_well[0] == NULL){m_surf_well[0] = gGameWorld.gConMgr.GetSurface("sprites/well.fif");}
	if(m_surf_well[1] == NULL){m_surf_well[1] = gGameWorld.gConMgr.GetSurface("sprites/well2.fif");}
	m_visible = 1;
	init_time = GetTicks();
}


//Update Function
void EntWell::Update()
{
	if(m_visible == 0){return;}

	if(m_mode == 0)
	{
		//Reulate flashing animation
		if(GetTicks() > anim_regulator+200)
		{
			m_wellstate =! m_wellstate;
			anim_regulator = GetTicks();
		}

		//If well has been visible more than 6 seconds then get rid of it!
		if(GetTicks() > init_time+6000)
		{
			gGameWorld.GetGridCell(m_pos.x, m_pos.y)->ent = NULL;
			m_visible=0;
		}
	}
}

//Render Function
void EntWell::Render()
{
	if(m_visible == 0){return;}

	//Get Position to Draw Graphic at (from grid pos)
	Vector2D pos = gGameWorld.GetGridCell(m_pos.x, m_pos.y)->pos;

	//Draw Graphic
	SDL_BlitSurface(m_surf_well[m_wellstate], 0, gGameWorld.backbuffer, &SDLE_Rect(pos.x, pos.y, m_surf_well[m_wellstate]->w, m_surf_well[m_wellstate]->h));
}


//Event System
void EntWell::SendEvent(int msg, void* addarg)
{
	BaseEntity* b = (BaseEntity*)addarg;
	if(b->m_ident != IDENT_MAN){return;} //Wells only respond to collisions from players!

	if(msg == MSG_COLLISION)
	{
		EntPlayer* plr = (EntPlayer*)addarg;
		
		//If YOU are the lucky player that got the well
		if(plr == gGameWorld.GetPlayer(0))
		{
			gGameWorld.m_isbonus = 1;
			gGameWorld.m_bonustype = qRand(1, 4);
			if(gGameWorld.m_bonustype == 3)
			{
				gGameWorld.GenerateMaze();
			}else
			{
				gGameWorld.GenerateRandomLevel();
			}
		}

		//Get rid of the well!
		gGameWorld.GetGridCell(m_pos.x, m_pos.y)->ent = NULL;
		m_visible=0;

		//Play sound
		sound.PlaySound("sounds/well.wav");
	}
}






