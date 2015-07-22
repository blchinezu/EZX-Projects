/*
	Code written by James Fletcher (LiX). 
	Copyright 2009 - 2010. 
	ethocybin@googlemail.com
*/

#include "EntBox.h"
#include "GameWorld.h"
#include "EntPlayer.h"
#include "Sound.h"

//Constructor
EntBox::EntBox()
{
	m_inanim=0;
	m_visible = 1;
}

//Destructor
EntBox::~EntBox()
{
	//
}

//Init Box Entity
void EntBox::InitBox(bool itype)
{
	m_surf[0] = gGameWorld.gConMgr.GetSurface("sprites/box2.fif");
    m_surf[1] = gGameWorld.gConMgr.GetSurface("sprites/box1.fif");
    m_moveable = itype;
	m_inanim = 0;
	m_visible = 1;
}


//Update Box Entity
void EntBox::Update()
{
	if(m_visible == 0){return;}

	if(m_inanim == 1)
	{
		//Get Position to Draw Graphic at (from grid pos)
		Vector2D pos = m_animpos;

		//Move box in direction
		if(m_direction == DIRECTION_UP){m_animpos.y--;}
		if(m_direction == DIRECTION_DOWN){m_animpos.y++;}
		if(m_direction == DIRECTION_LEFT){m_animpos.x--;}
		if(m_direction == DIRECTION_RIGHT){m_animpos.x++;}

		//When reached destined position exit animation
		if(m_animpos.x == gGameWorld.GetGridCell(m_pos.x, m_pos.y)->pos.x &&
			m_animpos.y == gGameWorld.GetGridCell(m_pos.x, m_pos.y)->pos.y)
		{
			m_inanim = 0;
		}
	}
}

//Render Box Entity
void EntBox::Render()
{
	if(m_visible == 0){return;}

	if(m_inanim == 1)
	{
		//Get Position to Draw Graphic at (from grid pos)
		Vector2D pos = m_animpos;

		//Draw Graphic
		SDL_BlitSurface(m_surf[m_moveable], 0, gGameWorld.backbuffer, &SDLE_Rect(m_animpos.x, m_animpos.y, m_surf[m_moveable]->w, m_surf[m_moveable]->h));
	}

	if(m_inanim == 0)
	{
		//Get Position to Draw Graphic at (from grid pos)
		Vector2D pos = gGameWorld.GetGridCell(m_pos.x, m_pos.y)->pos;

		//Draw Graphic
		SDL_BlitSurface(m_surf[m_moveable], 0, gGameWorld.backbuffer, &SDLE_Rect(pos.x, pos.y, m_surf[m_moveable]->w, m_surf[m_moveable]->h));
	}
}

//Box Events
void EntBox::SendEvent(int msg, void* addarg)
{
	BaseEntity* b = (BaseEntity*)addarg;
	if(b->m_ident != IDENT_MAN){return;} //Boxes only respond to collisions from players!

	if(msg == MSG_COLLISION)
	{
		EntPlayer* plr = (EntPlayer*)addarg;

		if(plr->m_walldestroy == 1)
		{
		 gGameWorld.GetGridCell(m_pos.x, m_pos.y)->ent = NULL; //Remove from cell
		 plr->m_walldestroy = 0;
		 m_visible = 0;
		 sound.PlaySound("sounds/boxdestroy.wav");
		 Effect* smoke = gGameWorld.gEffects.GetFreeEffect();
		 smoke->SetSpeed(140);
		 smoke->SetOpacity(0.35f);
		 smoke->SetFrame(0, gGameWorld.gConMgr.GetSurface("sprites/effects/smoke1.fif"));
		 smoke->SetFrame(1, gGameWorld.gConMgr.GetSurface("sprites/effects/smoke2.fif"));
		 smoke->SetFrame(2, gGameWorld.gConMgr.GetSurface("sprites/effects/smoke3.fif"));
		 smoke->SetPos(gGameWorld.GetGridCell(m_pos.x, m_pos.y)->pos.x-1, gGameWorld.GetGridCell(m_pos.x, m_pos.y)->pos.y-1);
		 smoke->Enable();
		 return;
		}

		if(m_moveable == 0){return;} //Box is not movable

		m_direction = plr->GetDirection();

		int nx = m_pos.x;
		int ny = m_pos.y;

		if(m_direction == DIRECTION_UP){ny--;}
		if(m_direction == DIRECTION_DOWN){ny++;}
		if(m_direction == DIRECTION_LEFT){nx--;}
		if(m_direction == DIRECTION_RIGHT){nx++;}

		//If pushing box out of bounds do nothing
		if(m_direction == DIRECTION_UP){if(m_pos.y <= 0){return;}}
		if(m_direction == DIRECTION_DOWN){if(m_pos.y >= 10){return;}}
		if(m_direction == DIRECTION_LEFT){if(m_pos.x <= 0){return;}}
		if(m_direction == DIRECTION_RIGHT){if(m_pos.x >= 16){return;}}

		//If pushing box into another entity do nothing
		if(gGameWorld.GetGridCell(nx, ny)->ent != NULL){return;}

		BaseEntity* ent = gGameWorld.GetGridCell(m_pos.x, m_pos.y)->ent; //Get Entity
		gGameWorld.GetGridCell(nx, ny)->ent = ent; //Move to new Cell
		gGameWorld.GetGridCell(m_pos.x, m_pos.y)->ent = NULL; //Remove from old cell

		//Update position
		m_animpos = gGameWorld.GetGridCell(m_pos.x, m_pos.y)->pos;
		m_pos.x = nx;
		m_pos.y = ny;

		m_inanim = 1;
		sound.PlaySound("sounds/movebox.wav");
	}
}




