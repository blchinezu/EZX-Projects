/*
	Code written by James Fletcher (LiX). 
	Copyright 2009 - 2010. 
	ethocybin@googlemail.com
*/

#include "EntButterfly.h"
#include "GameWorld.h"
#include <math.h>

//Constructor
EntButterfly::EntButterfly()
{
	//Set Variables
	m_id = 0;
	m_direction = 0;
	m_flycycle = 0;
	m_inanim = 0;
	anim_regulator = 0;
	m_dead = 0;
	move_regulator = 0;
	ms_regulator = 0;
	mrv = 0;
	m_pathpos = 0;
	m_visible = 0;
}

//Destructor
EntButterfly::~EntButterfly()
{
	//
}



//Init Butterfly
void EntButterfly::InitButterfly(int id, char* path)
{
	//Load Sprites
	m_surf_up[0] = gGameWorld.gConMgr.GetSurface("sprites/butterfly/bu1.fif");
	m_surf_up[1] = gGameWorld.gConMgr.GetSurface("sprites/butterfly/bu2.fif");
	m_surf_up[2] = gGameWorld.gConMgr.GetSurface("sprites/butterfly/bu3.fif");

	m_surf_down[0] = gGameWorld.gConMgr.GetSurface("sprites/butterfly/bd1.fif");
	m_surf_down[1] = gGameWorld.gConMgr.GetSurface("sprites/butterfly/bd2.fif");
	m_surf_down[2] = gGameWorld.gConMgr.GetSurface("sprites/butterfly/bd3.fif");

	m_surf_left[0] = gGameWorld.gConMgr.GetSurface("sprites/butterfly/bl1.fif");
	m_surf_left[1] = gGameWorld.gConMgr.GetSurface("sprites/butterfly/bl2.fif");
	m_surf_left[2] = gGameWorld.gConMgr.GetSurface("sprites/butterfly/bl3.fif");

	m_surf_right[0] = gGameWorld.gConMgr.GetSurface("sprites/butterfly/br1.fif");
	m_surf_right[1] = gGameWorld.gConMgr.GetSurface("sprites/butterfly/br2.fif");
	m_surf_right[2] = gGameWorld.gConMgr.GetSurface("sprites/butterfly/br3.fif");

	m_id = id;

	m_direction = 0;
	m_flycycle = 0;
	m_inanim = 0;
	anim_regulator = 0;
	m_dead = 0;
	move_regulator = 0;
	ms_regulator = 0;
	mrv = 0;
	m_pathpos = 0;

	memset(&m_path, 0x00, sizeof(m_path));
	sprintf(m_path, path);

	m_ident = IDENT_BUTTERFLY;
	m_visible = 1;
}


//Update Butterfly Entity
void EntButterfly::Update()
{
	if(m_visible == 0){return;}
	if(gGameWorld.IsReady() == 0){return;}
	Vector2D pos;


	//Regulate fly animation
	if(GetTicks() > anim_regulator+180)
	{
		m_flycycle++;
		if(m_flycycle > 2){m_flycycle = 0;}
		anim_regulator = GetTicks();
	}


	//Not in animation
	if(m_inanim == 0)
	{
		//Get Position to Draw Graphic at (from grid pos)
		pos = gGameWorld.GetGridCell(m_pos.x, m_pos.y)->pos;

		if(m_path[0] == 'X' || m_path[0] == 0x00)
		{
			//Check if a player is three blocks from you.
			EntPlayer* p = NULL;
			for(int i = 0; i < gGameWorld.GetTotalPlayers(); i++)
			{
				if(gGameWorld.GetPlayer(i)->GetPos().EuclidDistance(m_pos) < 3){p = gGameWorld.GetPlayer(i);}
			}

			//If there is a player three blocks from you move in it's general direction!
			if(p != NULL)
			{
				int dir=-1;

				if(fabs((double)p->GetPos().x - m_pos.x) > fabs((double)p->GetPos().y - m_pos.y))
				{
					if(p->GetPos().x > m_pos.x){dir=3;}
					if(p->GetPos().x < m_pos.x){dir=2;}
				}else{
					if(p->GetPos().y > m_pos.y){dir=1;}
					if(p->GetPos().y < m_pos.y){dir=0;}
				}

				Move(dir);
				return;
			}
		}
		else
		{
			//Check if a player is three blocks from you.
			EntPlayer* p = NULL;
			for(int i = 0; i < gGameWorld.GetTotalPlayers(); i++)
			{
				if(gGameWorld.GetPlayer(i)->GetPos().EuclidDistance(m_pos) < 2){p = gGameWorld.GetPlayer(i);}
			}

			//If there is a player three blocks from you move in it's general direction!
			if(p != NULL)
			{
				int dir=-1;

				if(fabs((double)p->GetPos().x - m_pos.x) > fabs((double)p->GetPos().y - m_pos.y))
				{
					if(p->GetPos().x > m_pos.x){dir=3;}
					if(p->GetPos().x < m_pos.x){dir=2;}
				}else{
					if(p->GetPos().y > m_pos.y){dir=1;}
					if(p->GetPos().y < m_pos.y){dir=0;}
				}

				Move(dir);
				return;
			}

			if(m_path[m_pathpos] == 'u'){Move(DIRECTION_UP); m_pathpos++; return;}
			if(m_path[m_pathpos] == 'd'){Move(DIRECTION_DOWN); m_pathpos++; return;}
			if(m_path[m_pathpos] == 'l'){Move(DIRECTION_LEFT); m_pathpos++; return;}
			if(m_path[m_pathpos] == 'r'){Move(DIRECTION_RIGHT); m_pathpos++; return;}
			if(m_path[m_pathpos] == 0x00){m_pathpos=0;}
		}

		//Regulate movement
		if(m_path[0] == 'X' || m_path[0] == 0x00)
		{
			if(GetTicks() > move_regulator+mrv)
			{
				Move(qRand(0, 4));
				move_regulator = GetTicks();
				mrv = qRand(700, 2700);
			}
		}
	}

	//Is in animation
	if(m_inanim == 1)
	{
		//Get Position to Draw Graphic at (from grid pos)
		pos = m_animpos;

		//Move Butterfly
		if(GetTicks() > ms_regulator+50)
		{
			if(m_direction == DIRECTION_UP){m_animpos.y -= 1;}
			if(m_direction == DIRECTION_DOWN){m_animpos.y += 1;}
			if(m_direction == DIRECTION_LEFT){m_animpos.x -= 1;}
			if(m_direction == DIRECTION_RIGHT){m_animpos.x += 1;}
			ms_regulator = GetTicks();
		}

		//When reached destined position exit animation
		if(m_animpos.x == gGameWorld.GetGridCell(m_pos.x, m_pos.y)->pos.x &&
			m_animpos.y == gGameWorld.GetGridCell(m_pos.x, m_pos.y)->pos.y)
		{
			m_inanim = 0;
		}
	}

}

//Render Butterfly Entity
void EntButterfly::Render()
{
	if(m_visible == 0){return;}
	Vector2D pos;

	//Not in animation
	if(m_inanim == 0)
	{
		//Get Position to Draw Graphic at (from grid pos)
		pos = gGameWorld.GetGridCell(m_pos.x, m_pos.y)->pos;

		//Draw Graphic
		if(m_direction == DIRECTION_UP){SDL_BlitSurface(m_surf_up[m_flycycle], 0, gGameWorld.backbuffer, &SDLE_Rect(pos.x+1, pos.y+1, m_surf_up[m_flycycle]->w, m_surf_up[m_flycycle]->h));}
		if(m_direction == DIRECTION_DOWN){SDL_BlitSurface(m_surf_down[m_flycycle], 0, gGameWorld.backbuffer, &SDLE_Rect(pos.x+1, pos.y+1, m_surf_down[m_flycycle]->w, m_surf_down[m_flycycle]->h));}
		if(m_direction == DIRECTION_LEFT){SDL_BlitSurface(m_surf_left[m_flycycle], 0, gGameWorld.backbuffer, &SDLE_Rect(pos.x+1, pos.y+1, m_surf_left[m_flycycle]->w, m_surf_left[m_flycycle]->h));}
		if(m_direction == DIRECTION_RIGHT){SDL_BlitSurface(m_surf_right[m_flycycle], 0, gGameWorld.backbuffer, &SDLE_Rect(pos.x+1, pos.y+1, m_surf_right[m_flycycle]->w, m_surf_right[m_flycycle]->h));}
	}

	//Is in animation
	if(m_inanim == 1)
	{
		//Get Position to Draw Graphic at (from grid pos)
		pos = m_animpos;

		//Draw Graphic
		if(m_direction == DIRECTION_UP){SDL_BlitSurface(m_surf_up[m_flycycle], 0, gGameWorld.backbuffer, &SDLE_Rect(pos.x+1, pos.y+1, m_surf_up[m_flycycle]->w, m_surf_up[m_flycycle]->h));}
		if(m_direction == DIRECTION_DOWN){SDL_BlitSurface(m_surf_down[m_flycycle], 0, gGameWorld.backbuffer, &SDLE_Rect(pos.x+1, pos.y+1, m_surf_down[m_flycycle]->w, m_surf_down[m_flycycle]->h));}
		if(m_direction == DIRECTION_LEFT){SDL_BlitSurface(m_surf_left[m_flycycle], 0, gGameWorld.backbuffer, &SDLE_Rect(pos.x+1, pos.y+1, m_surf_left[m_flycycle]->w, m_surf_left[m_flycycle]->h));}
		if(m_direction == DIRECTION_RIGHT){SDL_BlitSurface(m_surf_right[m_flycycle], 0, gGameWorld.backbuffer, &SDLE_Rect(pos.x+1, pos.y+1, m_surf_right[m_flycycle]->w, m_surf_right[m_flycycle]->h));}
	}
}


//Moves butterfly in specified direction
void EntButterfly::Move(int direction)
{
	//If in animation, dead or invisible or game no ready do nothing
	if(m_dead == 1 || m_visible == 0 || gGameWorld.IsReady() == 0){return;}

	//Change Sprite Facing Direction
	m_direction = direction;

	//If your trying to move the player out of bounds, do nothing
	if(m_direction == DIRECTION_UP){if(m_pos.y <= 0){return;}}
	if(m_direction == DIRECTION_DOWN){if(m_pos.y >= 10){return;}}
	if(m_direction == DIRECTION_LEFT){if(m_pos.x <= 0){return;}}
	if(m_direction == DIRECTION_RIGHT){if(m_pos.x >= 16){return;}}

	//Have sprite animated walk to new grid square
	m_animpos = gGameWorld.GetGridCell(m_pos.x, m_pos.y)->pos;

	//Move one space in the grid in specified direction
	int ox=0, oy=0; //Original Cell
	BaseEntity* ent = gGameWorld.GetGridCell(m_pos.x, m_pos.y)->ent;
	ox = m_pos.x;
	oy = m_pos.y;
	
	if(m_direction == DIRECTION_UP){m_pos.y--;}
	if(m_direction == DIRECTION_DOWN){m_pos.y++;}
	if(m_direction == DIRECTION_LEFT){m_pos.x--;}
	if(m_direction == DIRECTION_RIGHT){m_pos.x++;}

	//If there's something in the way, dont move there!
	if(gGameWorld.GetGridCell(m_pos.x, m_pos.y)->ent != NULL && gGameWorld.GetGridCell(m_pos.x, m_pos.y)->ent->IsVisible() == 1)
	{
		gGameWorld.GetGridCell(m_pos.x, m_pos.y)->ent->SendEvent(MSG_COLLISION, this);
		
		//If collided with a player then the butterfly dies.
		if(gGameWorld.GetGridCell(m_pos.x, m_pos.y)->ent->m_ident == IDENT_MAN){m_dead = 1; m_visible = 0;}

		m_pos.x = ox; m_pos.y = oy;
		return;
	}

	//Start Animation
	m_inanim = 1;

	gGameWorld.GetGridCell(m_pos.x, m_pos.y)->ent = ent; //Move into new cell
    gGameWorld.GetGridCell(ox, oy)->ent = NULL; //Clear old cell
}


//Accessors
int EntButterfly::GetID(){return m_id;}


