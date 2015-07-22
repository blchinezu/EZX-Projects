/*
	Code written by James Fletcher (LiX). 
	Copyright 2009 - 2010. 
	ethocybin@googlemail.com
*/

#include "EntMushroom.h"
#include "GameWorld.h"
#include "EntPlayer.h"
#include "Sound.h"
#include "Rand.h"

//Constructor
EntMushroom::EntMushroom()
{
	m_visible = 1;
	m_ident = IDENT_MUSHROOM;
}

//Destructor
EntMushroom::~EntMushroom()
{
	//
}

//Init Mushroom Entity
void EntMushroom::InitMushroom(int itype)
{
	m_surf[0] = gGameWorld.gConMgr.GetSurface("sprites/m1.fif");
    m_surf[1] = gGameWorld.gConMgr.GetSurface("sprites/m2.fif");
    m_surf[2] = gGameWorld.gConMgr.GetSurface("sprites/m3.fif");
    m_surf[3] = gGameWorld.gConMgr.GetSurface("sprites/m4.fif");
	m_surf[4] = gGameWorld.gConMgr.GetSurface("sprites/m5.fif");
	m_surf[5] = gGameWorld.gConMgr.GetSurface("sprites/m6.fif");
	main_regulator = GetTicks();
	beep_regulator = GetTicks();
	cutfeed=0;
	for(int i = 0; i < 6; i++){mole_regulator[i]=0; next_update[i]=0;}
    m_type = itype;
	m_visible = 1;

	m_ident = IDENT_MUSHROOM;
}

//Is the mole game over?
bool EntMushroom::IsMoleGameOver()
{
	int c=0;

	for(int i = 0; i < 18; i++)
	{
		if(mole_pos[i].x == 0 && mole_pos[i].y == 0){c++;}
	}

	if(c >= 17){return 1;}
	return 0;

	/*int c=0;

	for(int x = 1; x < 16; x++)
	{
		for(int y = 1; y < 10; x++)
		{
			if(gGameWorld.GetGridCell(x, y)->ent != NULL)
			{
				if(gGameWorld.GetGridCell(x, y)->ent->m_ident == IDENT_MUSHROOM){c++;}
			}
		}
	}*/

	if(c == 0){return 1;}
	return 0;
}

//Update Function
void EntMushroom::Update()
{
	//If escape mushroom
	if(m_type == MUSHROOM_ESCAPE)
	{
		//Whack-a-Mole Game Logic
		if(gGameWorld.m_bonustype == 2)
		{
			for(int i = 0; i < 18; i++)
			{
				if(GetTicks() > main_regulator+29000){cutfeed=1;} //Time up!
				
				//Beep Counter Regulator
				/*if(GetTicks() > beep_regulator)
				{
					if(cutfeed == 1 && IsMoleGameOver() == 1){}else
					{
						if(GetTicks() >= main_regulator+28000){sound.PlaySound("sounds/blip.wav");}
						beep_regulator = GetTicks()+200;
					}
				}*/

				if(GetTicks() > mole_regulator[i]+next_update[i])
				{
					//If occupied, remove.
					if(mole_pos[i].x != 0 && mole_pos[i].y != 0 && gGameWorld.GetGridCell(mole_pos[i].x, mole_pos[i].y)->ent != NULL && gGameWorld.GetGridCell(mole_pos[i].x, mole_pos[i].y)->ent->m_ident == IDENT_MUSHROOM)
					{
						gGameWorld.GetGridCell(mole_pos[i].x, mole_pos[i].y)->ent = NULL;
						mole_pos[i].Set(0,0);
						mole_regulator[i] = GetTicks();
						next_update[i] = qRand(1500, 6000);
						return;
					}

					if(cutfeed == 1)
					{
						mole_regulator[i] = GetTicks();
						next_update[i] = qRand(1500, 6000);
						return;
					}

					//Otherwise place it somewhere
					Vector2D rp;
					rp.x = qRand(1, 16);
					rp.y = qRand(1, 10);

					while(gGameWorld.GetGridCell(rp.x, rp.y)->ent != NULL)
					{
						rp.x = qRand(1, 16);
						rp.y = qRand(1, 10);
					}

					gGameWorld.AddBlueBonus(rp, qRand(-150, 150));
					mole_pos[i].Set(rp.x, rp.y);

					//Reset Regulator
					mole_regulator[i] = GetTicks();
					next_update[i] = qRand(1500, 6000);
				}
			}
		}
	}
}

//Render Function
char inc[63];
void EntMushroom::Render()
{
	if(m_visible == 0){return;}

	//Get Position to Draw Graphic at (from grid pos)
	Vector2D pos = gGameWorld.GetGridCell(m_pos.x, m_pos.y)->pos;

	//If Escape Mushroom
	if(m_type == MUSHROOM_ESCAPE)
	{
		if(gGameWorld.GetPlayer(0)->GetPos().EuclidDistance(m_pos) < 3)
		{
			gGameWorld.gFont.DrawString(gGameWorld.backbuffer, pos.x-22, pos.y-20, "Exit Here", FONT_BLUE);
		}
	}

	//If Death/Score Mushroom
	if(m_type == MUSHROOM_BLUE)
	{
		if(gGameWorld.GetPlayer(0)->GetPos().EuclidDistance(m_pos) < 3)
		{
			sprintf(inc, "%i", m_bluebonus);
			gGameWorld.gFont.DrawString(gGameWorld.backbuffer, pos.x-((strlen(inc)*8)*0.5)+9, pos.y-20, inc, FONT_GREEN);
		}
	}

	//Colour Blind Aids
	if(gGameWorld.gMenu.m_colourblind == 1)
	{
		if(gGameWorld.GetPlayer(0)->GetPos().EuclidDistance(m_pos) < 3)
		{
			if(m_type == MUSHROOM_RED){gGameWorld.gFont.DrawString(gGameWorld.backbuffer, pos.x+4, pos.y-20, "R", FONT_WHITE);}
			if(m_type == MUSHROOM_PINK){gGameWorld.gFont.DrawString(gGameWorld.backbuffer, pos.x+4, pos.y-20, "P", FONT_WHITE);}
			if(m_type == MUSHROOM_BLACK){gGameWorld.gFont.DrawString(gGameWorld.backbuffer, pos.x+4, pos.y-20, "B", FONT_WHITE);}
			if(m_type == MUSHROOM_PURPLE){gGameWorld.gFont.DrawString(gGameWorld.backbuffer, pos.x+4, pos.y-20, "S", FONT_WHITE);}
		}
	}

	//Draw Graphic
	if(m_type == MUSHROOM_RED){SDL_BlitSurface(m_surf[0], 0, gGameWorld.backbuffer, &SDLE_Rect(pos.x+3, pos.y-4, m_surf[0]->w, m_surf[0]->h));}
    if(m_type == MUSHROOM_PINK){SDL_BlitSurface(m_surf[1], 0, gGameWorld.backbuffer, &SDLE_Rect(pos.x+3, pos.y-4, m_surf[1]->w, m_surf[1]->h));}
    if(m_type == MUSHROOM_BLACK){SDL_BlitSurface(m_surf[2], 0, gGameWorld.backbuffer, &SDLE_Rect(pos.x+3, pos.y-4, m_surf[2]->w, m_surf[2]->h));}
    if(m_type == MUSHROOM_PURPLE){SDL_BlitSurface(m_surf[3], 0, gGameWorld.backbuffer, &SDLE_Rect(pos.x+3, pos.y-4, m_surf[3]->w, m_surf[3]->h));}
	if(m_type == MUSHROOM_BLUE){SDL_BlitSurface(m_surf[4], 0, gGameWorld.backbuffer, &SDLE_Rect(pos.x+3, pos.y-4, m_surf[4]->w, m_surf[4]->h));}
	if(m_type == MUSHROOM_ESCAPE){SDL_BlitSurface(m_surf[5], 0, gGameWorld.backbuffer, &SDLE_Rect(pos.x+3, pos.y-4, m_surf[5]->w, m_surf[5]->h));}
}


//Mushroom Events
char sf[256];
void EntMushroom::SendEvent(int msg, void* addarg)
{
	BaseEntity* b = (BaseEntity*)addarg;
	if(b->m_ident != IDENT_MAN){return;} //Mushrooms only respond to collisions from players!

	if(msg == MSG_COLLISION)
	{
		if(m_type == MUSHROOM_RED)
		{
			EntPlayer* plr = (EntPlayer*)addarg;
			plr->GetLife() += 10;
			plr->GetScore() += 50;
			m_visible = 0;
			gGameWorld.GetGridCell(m_pos.x, m_pos.y)->ent = NULL;
			sprintf(sf, "sounds/crunch/c%i.wav", qRand(1,10));
			sound.PlaySound(sf);
			gGameWorld.CheckIfLastRedMushroom();
		}

		if(m_type == MUSHROOM_PINK)
		{
			EntPlayer* plr = (EntPlayer*)addarg;
			plr->GetLife() -= 40;
			plr->GetScore() += 150;
			m_visible = 0;
			gGameWorld.GetGridCell(m_pos.x, m_pos.y)->ent = NULL;

			sprintf(sf, "sounds/crunch/c%i.wav", qRand(1,10));
			sound.PlaySound(sf);
		}

		if(m_type == MUSHROOM_BLACK)
		{
			if(gGameWorld.FreezeBoxes() == 0)
			{
				sprintf(sf, "sounds/crunch/c%i.wav", qRand(1,10));
				sound.PlaySound(sf);
			}

			m_visible = 0;
			gGameWorld.GetGridCell(m_pos.x, m_pos.y)->ent = NULL;
		}

		if(m_type == MUSHROOM_PURPLE)
		{
			EntPlayer* plr = (EntPlayer*)addarg;
			plr->m_walldestroy = 1;
			m_visible = 0;
			gGameWorld.GetGridCell(m_pos.x, m_pos.y)->ent = NULL;
			sound.PlaySound("sounds/purplepickup.wav");
		}

		if(m_type == MUSHROOM_BLUE)
		{
			EntPlayer* plr = (EntPlayer*)addarg;
			plr->GetScore() += m_bluebonus;
			m_visible = 0;
			gGameWorld.GetGridCell(m_pos.x, m_pos.y)->ent = NULL;

			if(m_bluebonus >= 0){sound.PlaySound("sounds/gravepickup.wav");}
			if(m_bluebonus < 0){sound.PlaySound("sounds/hit.wav");}
		}

		if(m_type == MUSHROOM_ESCAPE)
		{
			gGameWorld.m_bonustype = 0;
			gGameWorld.GetGridCell(m_pos.x, m_pos.y)->ent = NULL;
			gGameWorld.gentoggle = 1;
			sound.PlaySound("sounds/gravepickup.wav");
		}
	}
}



//Accessors
int EntMushroom::GetType(){return m_type;}



