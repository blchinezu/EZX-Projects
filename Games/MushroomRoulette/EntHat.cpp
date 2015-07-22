/*
	Code written by James Fletcher (LiX). 
	Copyright 2009 - 2010. 
	ethocybin@googlemail.com
*/

#include "EntHat.h"
#include "GameWorld.h"
#include "sdl_extra.h"

//Constructor
EntHat::EntHat()
{
	m_visible = 0;
}

//Destructor
EntHat::~EntHat()
{
	//
}

//Init's the hat
void EntHat::InitHat(int type)
{
	//Load Graphics
	m_surf_hat[0] = gGameWorld.gConMgr.GetSurface("sprites/hats/dh2.fif");
	m_surf_hat[1] = gGameWorld.gConMgr.GetSurface("sprites/hats/dv2.fif");
	m_surf_hat[2] = gGameWorld.gConMgr.GetSurface("sprites/hats/fz2.fif");
	m_surf_hat[3] = gGameWorld.gConMgr.GetSurface("sprites/hats/mh3.fif");
	m_surf_hat[4] = gGameWorld.gConMgr.GetSurface("sprites/hats/nc2.fif");
	m_surf_hat[5] = gGameWorld.gConMgr.GetSurface("sprites/hats/sc3.fif");

	m_type = type;
	m_ident = IDENT_HAT;
	m_visible = 1;
}

//Renders the hat
void EntHat::Render()
{
	if(m_visible == 0){return;}

	//Get Position to Draw Graphic at (from grid pos)
	Vector2D pos = gGameWorld.GetGridCell(m_pos.x, m_pos.y)->pos;

	//Correct Position
	if(m_type == 1){pos.x+=4; pos.y+=4;}
	if(m_type == 2){pos.x+=3; pos.y+=2;}
	if(m_type == 3){pos.x+=5; pos.y+=6;}
	if(m_type == 4){pos.x+=2; pos.y+=5;}
	if(m_type == 5){pos.x+=2; pos.y+=3;}
	if(m_type == 6){pos.x+=7; pos.y+=7;}

	//Draw Graphic
	SDL_BlitSurface(m_surf_hat[m_type-1], 0, gGameWorld.backbuffer, &SDLE_Rect(pos.x, pos.y, m_surf_hat[m_type-1]->w, m_surf_hat[m_type-1]->h));

	//If player comes withing 3 squares
	if(gGameWorld.GetPlayer(0)->GetPos().EuclidDistance(m_pos) < 3)
	{
		if(m_type == 2){gGameWorld.gFont.DrawString(gGameWorld.backbuffer, pos.x-10, pos.y-20, "6500", FONT_BLUE);} //Diving Hat
		if(m_type == 5){gGameWorld.gFont.DrawString(gGameWorld.backbuffer, pos.x-10, pos.y-20, "4000", FONT_BLUE);} //Night Cap
		if(m_type == 4){gGameWorld.gFont.DrawString(gGameWorld.backbuffer, pos.x-10, pos.y-20, "2000", FONT_BLUE);} //Mexican Hat
		if(m_type == 3){gGameWorld.gFont.DrawString(gGameWorld.backbuffer, pos.x-10, pos.y-20, "1000", FONT_BLUE);} //Fez Hat
		if(m_type == 6){gGameWorld.gFont.DrawString(gGameWorld.backbuffer, pos.x-10, pos.y-20, "500", FONT_BLUE);} //Skull Cap
		if(m_type == 1){gGameWorld.gFont.DrawString(gGameWorld.backbuffer, pos.x-15, pos.y-20, "-2500", FONT_BLUE);} //Dunce Hat
	}
}


