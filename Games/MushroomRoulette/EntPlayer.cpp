/*
	Code written by James Fletcher (LiX). 
	Copyright 2009 - 2010. 
	ethocybin@googlemail.com
*/

#include "EntPlayer.h"
#include "GameWorld.h"
#include "Sound.h"
#include "EntHat.h"

//Constructor
EntPlayer::EntPlayer()
{
	//Set Variables
	m_score=0;
	m_life=100;
	m_pathpos = 0;
	m_walldestroy=0;
	m_direction=1;
	m_walkcycle=0;
	anim_regulator=0;
	m_inanim=0;
	m_dead = 0;
	m_evolved = 0;
	m_pollenated = 0;
	m_visible = 1;
	m_hat = 0;
	
	tf=0;
}

//Destructor
EntPlayer::~EntPlayer()
{
	//SDL_FreeSurface(); //to them all
}

//Init Player Entity
void EntPlayer::InitPlayer(string name)
{
	m_evolved = gGameWorld.gScores.IsEvolved();

	//Load Sprites
	if(m_evolved == 0)
	{
		m_surf_up[0] = gGameWorld.gConMgr.GetSurface("sprites/pu.fif");
		m_surf_up[1] = gGameWorld.gConMgr.GetSurface("sprites/pu1.fif");
		m_surf_up[2] = gGameWorld.gConMgr.GetSurface("sprites/pu2.fif");

		m_surf_down[0] = gGameWorld.gConMgr.GetSurface("sprites/pd.fif");
		m_surf_down[1] = gGameWorld.gConMgr.GetSurface("sprites/pd1.fif");
		m_surf_down[2] = gGameWorld.gConMgr.GetSurface("sprites/pd2.fif");

		m_surf_left[0] = gGameWorld.gConMgr.GetSurface("sprites/pl.fif");
		m_surf_left[1] = gGameWorld.gConMgr.GetSurface("sprites/pl1.fif");
		m_surf_left[2] = gGameWorld.gConMgr.GetSurface("sprites/pl2.fif");

		m_surf_right[0] = gGameWorld.gConMgr.GetSurface("sprites/pr.fif");
		m_surf_right[1] = gGameWorld.gConMgr.GetSurface("sprites/pr1.fif");
		m_surf_right[2] = gGameWorld.gConMgr.GetSurface("sprites/pr2.fif");
	}

	if(m_evolved == 1)
	{
		m_surf_up[0] = gGameWorld.gConMgr.GetSurface("sprites/evo/eu.fif");
		m_surf_up[1] = gGameWorld.gConMgr.GetSurface("sprites/evo/eu1.fif");
		m_surf_up[2] = gGameWorld.gConMgr.GetSurface("sprites/evo/eu2.fif");

		m_surf_down[0] = gGameWorld.gConMgr.GetSurface("sprites/evo/ed.fif");
		m_surf_down[1] = gGameWorld.gConMgr.GetSurface("sprites/evo/ed1.fif");
		m_surf_down[2] = gGameWorld.gConMgr.GetSurface("sprites/evo/ed2.fif");

		m_surf_left[0] = gGameWorld.gConMgr.GetSurface("sprites/evo/el.fif");
		m_surf_left[1] = gGameWorld.gConMgr.GetSurface("sprites/evo/el1.fif");
		m_surf_left[2] = gGameWorld.gConMgr.GetSurface("sprites/evo/el2.fif");

		m_surf_right[0] = gGameWorld.gConMgr.GetSurface("sprites/evo/er.fif");
		m_surf_right[1] = gGameWorld.gConMgr.GetSurface("sprites/evo/er1.fif");
		m_surf_right[2] = gGameWorld.gConMgr.GetSurface("sprites/evo/er2.fif");
	}

	m_name = name;

	m_score=0;
	m_life=100;
	m_pathpos = 0;
	m_walldestroy=0;
	m_direction=1;
	m_walkcycle=0;
	anim_regulator=0;
	m_inanim=0;
	m_dead = 0;
	tf=0;
	m_hat = 0;
	m_pollenated = 0;
	m_visible = 1;

	m_ident = IDENT_MAN;
}

//Update Player Entity
void EntPlayer::Update()
{
	if(m_visible == 0){return;}
	Vector2D pos;


	//MoveAllongPath()
	if(m_inanim == 0) //Sync with animation so that it doesnt execute code while moving between path nodes!
	{
		if(m_path.size() != 0)
		{
			int dir=0;

			//get direction of node
			if(m_path[m_pathpos].x > m_pos.x){dir = DIRECTION_RIGHT;}
			if(m_path[m_pathpos].x < m_pos.x){dir = DIRECTION_LEFT;}
			if(m_path[m_pathpos].y < m_pos.y){dir = DIRECTION_UP;}
			if(m_path[m_pathpos].y > m_pos.y){dir = DIRECTION_DOWN;}

			//Move in direction of node
			Move(dir);

			//If we have reached the last square, clear path and break, we're done!
			if(m_pathpos >= m_path.size()-1){m_path.clear();}

			//Increment Path Position
			m_pathpos++;
		}
	}

	//Is in animation
	if(m_inanim == 1)
	{
		//Get Position to Draw Graphic at (from grid pos)
		pos = m_animpos;

		//Move Player
		if(gGameWorld.m_bonustype == 1 || gGameWorld.m_bonustype == 2)
		{
			if(m_direction == DIRECTION_UP){m_animpos.y -= 2; }
			if(m_direction == DIRECTION_DOWN){m_animpos.y += 2;}
			if(m_direction == DIRECTION_LEFT){m_animpos.x -= 2;}
			if(m_direction == DIRECTION_RIGHT){m_animpos.x += 2;}
		}else{
            if(gGameWorld.gMenu.m_system == 0)
            {
				if(m_direction == DIRECTION_UP){m_animpos.y -= 2;}
				if(m_direction == DIRECTION_DOWN){m_animpos.y += 2;}
				if(m_direction == DIRECTION_LEFT){m_animpos.x -= 2;}
            	if(m_direction == DIRECTION_RIGHT){m_animpos.x += 2;}
            }
            
            if(gGameWorld.gMenu.m_system == 1)
            {
				if(m_direction == DIRECTION_UP){m_animpos.y -= 1; }
				if(m_direction == DIRECTION_DOWN){m_animpos.y += 1;}
				if(m_direction == DIRECTION_LEFT){m_animpos.x -= 1;}
            	if(m_direction == DIRECTION_RIGHT){m_animpos.x += 1;}
            }
		}

		//When reached destined position exit animation
		if(m_animpos.x == gGameWorld.GetGridCell(m_pos.x, m_pos.y)->pos.x &&
			m_animpos.y == gGameWorld.GetGridCell(m_pos.x, m_pos.y)->pos.y)
		{
			if(m_pollenated >= 2){m_pollenated = 0;}
			if(m_pollenated != 0){m_pollenated++;}

			m_inanim = 0;
			return;
		}

		//Regulate Walk Animation
		if(GetTicks() > anim_regulator+120)
		{
			m_walkcycle++;
			if(m_walkcycle > 2){m_walkcycle = 1;}
			anim_regulator = GetTicks();
		}
	}

	//If Dead drop blue mushroom, set invisible.
	if(m_life <= 0)
	{
		gGameWorld.gScores.AddScore(m_score, gGameWorld.GetLevel(), m_life);
		gGameWorld.AddBlueBonus(m_pos, m_score/4);
		sound.PlaySound("sounds/death.wav");

		Effect* dead = gGameWorld.gEffects.GetFreeEffect();
		dead->SetSpeed(90);
		dead->SetOpacity(0.2f);
		dead->SetFrame(0, gGameWorld.gConMgr.GetSurface("sprites/effects/db1.fif"));
		dead->SetFrame(1, gGameWorld.gConMgr.GetSurface("sprites/effects/db2.fif"));
		dead->SetFrame(2, gGameWorld.gConMgr.GetSurface("sprites/effects/db3.fif"));
		dead->SetFrame(3, gGameWorld.gConMgr.GetSurface("sprites/effects/db4.fif"));
		dead->SetFrame(4, gGameWorld.gConMgr.GetSurface("sprites/effects/db5.fif"));
		dead->SetFrame(5, gGameWorld.gConMgr.GetSurface("sprites/effects/db6.fif"));
		dead->SetFrame(6, gGameWorld.gConMgr.GetSurface("sprites/effects/db7.fif"));
		dead->SetFrame(7, gGameWorld.gConMgr.GetSurface("sprites/effects/db8.fif"));
		dead->SetFrame(8, gGameWorld.gConMgr.GetSurface("sprites/effects/db9.fif"));
		dead->SetPos(gGameWorld.GetGridCell(m_pos.x, m_pos.y)->pos.x-17, gGameWorld.GetGridCell(m_pos.x, m_pos.y)->pos.y-20);
		dead->Enable();

		//m_visible = 0;
		m_dead=1;
		gGameWorld.gScores.m_enabled = 1;
		gGameWorld.gMenu.m_selected = 2;
		gGameWorld.gMenu.m_enabled = 1;
	}
}

//Render Player Entity
void EntPlayer::Render()
{
	if(m_visible == 0){return;}
	Vector2D pos;

	//Normal Sprites
	if(m_walldestroy == 0)
	{
		if(tf == 0)
		{
			if(m_evolved == 0)
			{
				m_surf_up[0] = gGameWorld.gConMgr.GetSurface("sprites/pu.fif");
				m_surf_up[1] = gGameWorld.gConMgr.GetSurface("sprites/pu1.fif");
				m_surf_up[2] = gGameWorld.gConMgr.GetSurface("sprites/pu2.fif");

				m_surf_down[0] = gGameWorld.gConMgr.GetSurface("sprites/pd.fif");
				m_surf_down[1] = gGameWorld.gConMgr.GetSurface("sprites/pd1.fif");
				m_surf_down[2] = gGameWorld.gConMgr.GetSurface("sprites/pd2.fif");

				m_surf_left[0] = gGameWorld.gConMgr.GetSurface("sprites/pl.fif");
				m_surf_left[1] = gGameWorld.gConMgr.GetSurface("sprites/pl1.fif");
				m_surf_left[2] = gGameWorld.gConMgr.GetSurface("sprites/pl2.fif");

				m_surf_right[0] = gGameWorld.gConMgr.GetSurface("sprites/pr.fif");
				m_surf_right[1] = gGameWorld.gConMgr.GetSurface("sprites/pr1.fif");
				m_surf_right[2] = gGameWorld.gConMgr.GetSurface("sprites/pr2.fif");
			}

			if(m_evolved == 1)
			{
				m_surf_up[0] = gGameWorld.gConMgr.GetSurface("sprites/evo/eu.fif");
				m_surf_up[1] = gGameWorld.gConMgr.GetSurface("sprites/evo/eu1.fif");
				m_surf_up[2] = gGameWorld.gConMgr.GetSurface("sprites/evo/eu2.fif");

				m_surf_down[0] = gGameWorld.gConMgr.GetSurface("sprites/evo/ed.fif");
				m_surf_down[1] = gGameWorld.gConMgr.GetSurface("sprites/evo/ed1.fif");
				m_surf_down[2] = gGameWorld.gConMgr.GetSurface("sprites/evo/ed2.fif");

				m_surf_left[0] = gGameWorld.gConMgr.GetSurface("sprites/evo/el.fif");
				m_surf_left[1] = gGameWorld.gConMgr.GetSurface("sprites/evo/el1.fif");
				m_surf_left[2] = gGameWorld.gConMgr.GetSurface("sprites/evo/el2.fif");

				m_surf_right[0] = gGameWorld.gConMgr.GetSurface("sprites/evo/er.fif");
				m_surf_right[1] = gGameWorld.gConMgr.GetSurface("sprites/evo/er1.fif");
				m_surf_right[2] = gGameWorld.gConMgr.GetSurface("sprites/evo/er2.fif");
			}

			tf=1;
		}
	}


	//Purple Sprites
	if(m_walldestroy == 1)
	{
		if(tf == 1)
		{
			if(m_evolved == 0)
			{
				m_surf_up[0] = gGameWorld.gConMgr.GetSurface("sprites/purple/pu.fif");
				m_surf_up[1] = gGameWorld.gConMgr.GetSurface("sprites/purple/pu1.fif");
				m_surf_up[2] = gGameWorld.gConMgr.GetSurface("sprites/purple/pu2.fif");

				m_surf_down[0] = gGameWorld.gConMgr.GetSurface("sprites/purple/pd.fif");
				m_surf_down[1] = gGameWorld.gConMgr.GetSurface("sprites/purple/pd1.fif");
				m_surf_down[2] = gGameWorld.gConMgr.GetSurface("sprites/purple/pd2.fif");

				m_surf_left[0] = gGameWorld.gConMgr.GetSurface("sprites/purple/pl.fif");
				m_surf_left[1] = gGameWorld.gConMgr.GetSurface("sprites/purple/pl1.fif");
				m_surf_left[2] = gGameWorld.gConMgr.GetSurface("sprites/purple/pl2.fif");

				m_surf_right[0] = gGameWorld.gConMgr.GetSurface("sprites/purple/pr.fif");
				m_surf_right[1] = gGameWorld.gConMgr.GetSurface("sprites/purple/pr1.fif");
				m_surf_right[2] = gGameWorld.gConMgr.GetSurface("sprites/purple/pr2.fif");
			}

			if(m_evolved == 1)
			{
				m_surf_up[0] = gGameWorld.gConMgr.GetSurface("sprites/evop/eu.fif");
				m_surf_up[1] = gGameWorld.gConMgr.GetSurface("sprites/evop/eu1.fif");
				m_surf_up[2] = gGameWorld.gConMgr.GetSurface("sprites/evop/eu2.fif");

				m_surf_down[0] = gGameWorld.gConMgr.GetSurface("sprites/evop/ed.fif");
				m_surf_down[1] = gGameWorld.gConMgr.GetSurface("sprites/evop/ed1.fif");
				m_surf_down[2] = gGameWorld.gConMgr.GetSurface("sprites/evop/ed2.fif");

				m_surf_left[0] = gGameWorld.gConMgr.GetSurface("sprites/evop/el.fif");
				m_surf_left[1] = gGameWorld.gConMgr.GetSurface("sprites/evop/el1.fif");
				m_surf_left[2] = gGameWorld.gConMgr.GetSurface("sprites/evop/el2.fif");

				m_surf_right[0] = gGameWorld.gConMgr.GetSurface("sprites/evop/er.fif");
				m_surf_right[1] = gGameWorld.gConMgr.GetSurface("sprites/evop/er1.fif");
				m_surf_right[2] = gGameWorld.gConMgr.GetSurface("sprites/evop/er2.fif");
			}

			tf=0;
		}
	}


	//Is in animation
	if(m_inanim == 1)
	{
		//Get Position to Draw Graphic at (from grid pos)
		pos = m_animpos;

		//Draw Graphic
		if(m_direction == DIRECTION_UP){
			SDL_BlitSurface(m_surf_up[m_walkcycle], 0, gGameWorld.backbuffer, &SDLE_Rect(pos.x+1, pos.y-8, m_surf_up[m_walkcycle]->w, m_surf_up[m_walkcycle]->h));
			if(m_hat != 0){
				if(m_hat == 3){pos.x+=5; pos.y-=3;}
				if(m_hat == 2){pos.x+=2; pos.y-=3;}
				if(m_hat == 6){pos.x+=5;}
				if(m_hat == 5){pos.x+=3; pos.y-=4;}
				if(m_hat == 1){pos.x+=5; pos.y-=4;}
				if(m_hat == 4){pos.x+=1; pos.y-=3;}
				SDL_BlitSurface(m_surf_hat[3], 0, gGameWorld.backbuffer, &SDLE_Rect(pos.x+1, pos.y-8, m_surf_hat[3]->w, m_surf_hat[3]->h));
			}
		}
		
		if(m_direction == DIRECTION_DOWN){
			SDL_BlitSurface(m_surf_down[m_walkcycle], 0, gGameWorld.backbuffer, &SDLE_Rect(pos.x+1, pos.y-8, m_surf_down[m_walkcycle]->w, m_surf_down[m_walkcycle]->h));
			if(m_hat != 0){
				if(m_hat == 3){pos.x+=5; pos.y-=3;}
				if(m_hat == 2){pos.x+=2; pos.y-=3;}
				if(m_hat == 5){pos.x-=1; pos.y-=4;}
				if(m_hat == 1){pos.x+=1; pos.y-=4;}
				if(m_hat == 4){pos.x+=1; pos.y-=3;}
				if(m_surf_hat[2] != NULL){SDL_BlitSurface(m_surf_hat[2], 0, gGameWorld.backbuffer, &SDLE_Rect(pos.x+1, pos.y-8, m_surf_hat[2]->w, m_surf_hat[2]->h));}
			}
		}
		
		if(m_direction == DIRECTION_LEFT){
			SDL_BlitSurface(m_surf_left[m_walkcycle], 0, gGameWorld.backbuffer, &SDLE_Rect(pos.x+1, pos.y-8, m_surf_left[m_walkcycle]->w, m_surf_left[m_walkcycle]->h));
			if(m_hat != 0){
				if(m_hat == 3){pos.x+=4; pos.y-=3;}
				if(m_hat == 2){pos.x+=2; pos.y-=5;}
				if(m_hat == 6){pos.x+=7;}
				if(m_hat == 5){pos.x+=4; pos.y-=3;}
				if(m_hat == 1){pos.x+=6; pos.y-=4;}
				if(m_hat == 4){pos.x+=1; pos.y-=3;}
				SDL_BlitSurface(m_surf_hat[0], 0, gGameWorld.backbuffer, &SDLE_Rect(pos.x+1, pos.y-8, m_surf_hat[0]->w, m_surf_hat[0]->h));
			}
		}
		
		if(m_direction == DIRECTION_RIGHT){
			SDL_BlitSurface(m_surf_right[m_walkcycle], 0, gGameWorld.backbuffer, &SDLE_Rect(pos.x+1, pos.y-8, m_surf_right[m_walkcycle]->w, m_surf_right[m_walkcycle]->h));
			if(m_hat != 0){
				if(m_hat == 3){pos.x+=3; pos.y-=3;}
				if(m_hat == 2){pos.x+=2; pos.y-=5;}
				if(m_hat == 6){pos.x+=4;}
				if(m_hat == 5){pos.x-=3; pos.y-=3;}
				if(m_hat == 1){pos.x-=1; pos.y-=5;}
				if(m_hat == 4){pos.x+=1; pos.y-=3;}
				SDL_BlitSurface(m_surf_hat[1], 0, gGameWorld.backbuffer, &SDLE_Rect(pos.x+1, pos.y-8, m_surf_hat[1]->w, m_surf_hat[1]->h));
			}
		}
	}

	//Not in Animation
	if(m_inanim == 0)
	{
		//Get Position to Draw Graphic at (from grid pos)
		pos = gGameWorld.GetGridCell(m_pos.x, m_pos.y)->pos;

		

		//Draw Graphic
		if(m_direction == DIRECTION_UP){
			SDL_BlitSurface(m_surf_up[0], 0, gGameWorld.backbuffer, &SDLE_Rect(pos.x+1, pos.y-8, m_surf_up[0]->w, m_surf_up[0]->h));
			if(m_hat != 0){
				if(m_hat == 3){pos.x+=5; pos.y-=3;}
				if(m_hat == 2){pos.x+=2; pos.y-=3;}
				if(m_hat == 6){pos.x+=5;}
				if(m_hat == 5){pos.x+=3; pos.y-=4;}
				if(m_hat == 1){pos.x+=5; pos.y-=4;}
				if(m_hat == 4){pos.x+=1; pos.y-=3;}
				SDL_BlitSurface(m_surf_hat[3], 0, gGameWorld.backbuffer, &SDLE_Rect(pos.x+1, pos.y-8, m_surf_hat[3]->w, m_surf_hat[3]->h));
			}
		}
		
		if(m_direction == DIRECTION_DOWN){
			SDL_BlitSurface(m_surf_down[0], 0, gGameWorld.backbuffer, &SDLE_Rect(pos.x+1, pos.y-8, m_surf_down[0]->w, m_surf_down[0]->h));
			if(m_hat != 0){
				if(m_hat == 3){pos.x+=5; pos.y-=3;}
				if(m_hat == 2){pos.x+=2; pos.y-=3;}
				if(m_hat == 5){pos.x-=1; pos.y-=4;}
				if(m_hat == 1){pos.x+=1; pos.y-=4;}
				if(m_hat == 4){pos.x+=1; pos.y-=3;}
				if(m_surf_hat[2] != NULL){SDL_BlitSurface(m_surf_hat[2], 0, gGameWorld.backbuffer, &SDLE_Rect(pos.x+1, pos.y-8, m_surf_hat[2]->w, m_surf_hat[2]->h));}
			}
		}
		
		if(m_direction == DIRECTION_LEFT){
			SDL_BlitSurface(m_surf_left[0], 0, gGameWorld.backbuffer, &SDLE_Rect(pos.x+1, pos.y-8, m_surf_left[0]->w, m_surf_left[0]->h));
			if(m_hat != 0){
				if(m_hat == 3){pos.x+=4; pos.y-=3;}
				if(m_hat == 2){pos.x+=2; pos.y-=5;}
				if(m_hat == 6){pos.x+=7;}
				if(m_hat == 5){pos.x+=4; pos.y-=3;}
				if(m_hat == 1){pos.x+=6; pos.y-=4;}
				if(m_hat == 4){pos.x+=1; pos.y-=3;}
				SDL_BlitSurface(m_surf_hat[0], 0, gGameWorld.backbuffer, &SDLE_Rect(pos.x+1, pos.y-8, m_surf_hat[0]->w, m_surf_hat[0]->h));
			}
		}
		
		if(m_direction == DIRECTION_RIGHT){
			SDL_BlitSurface(m_surf_right[0], 0, gGameWorld.backbuffer, &SDLE_Rect(pos.x+1, pos.y-8, m_surf_right[0]->w, m_surf_right[0]->h));
			if(m_hat != 0){
				if(m_hat == 3){pos.x+=3; pos.y-=3;}
				if(m_hat == 2){pos.x+=2; pos.y-=5;}
				if(m_hat == 6){pos.x+=4;}
				if(m_hat == 5){pos.x-=3; pos.y-=3;}
				if(m_hat == 1){pos.x-=1; pos.y-=5;}
				if(m_hat == 4){pos.x+=1; pos.y-=3;}
				SDL_BlitSurface(m_surf_hat[1], 0, gGameWorld.backbuffer, &SDLE_Rect(pos.x+1, pos.y-8, m_surf_hat[1]->w, m_surf_hat[1]->h));
			}
		}
	}

	//if pollenated
	if(m_pollenated != 0)
	{
		//Slow!!
		SDLE_ReplaceColour(gGameWorld.backbuffer, SDLE_Rect(pos.x+1, pos.y-8, 14, 22), SDL_MapRGB(gGameWorld.backbuffer->format, 255, 178, 156), SDL_MapRGB(gGameWorld.backbuffer->format, 255, 255, 0));
		SDLE_ReplaceColour(gGameWorld.backbuffer, SDLE_Rect(pos.x+1, pos.y-8, 14, 22), SDL_MapRGB(gGameWorld.backbuffer->format, 255, 154, 123), SDL_MapRGB(gGameWorld.backbuffer->format, 191, 191, 90));
	}
}


//Moves player in specified direction
void EntPlayer::Move(int direction)
{
	//If in animation, dead or invisible or game no ready do nothing
	if(m_inanim == 1 || m_dead == 1 || m_visible == 0 || gGameWorld.IsReady() == 0){return;}

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

	//If there's something in the way, send collision message and dont move
	if(gGameWorld.GetGridCell(m_pos.x, m_pos.y)->ent != NULL && gGameWorld.GetGridCell(m_pos.x, m_pos.y)->ent->IsVisible() == 1)
	{
		if(gGameWorld.GetGridCell(m_pos.x, m_pos.y)->ent->m_ident == IDENT_HAT)
		{
			EntHat* h = (EntHat*)gGameWorld.GetGridCell(m_pos.x, m_pos.y)->ent;
			m_hat = h->m_type;

			if(m_hat == 1) //Dunce Hat
			{
				m_surf_hat[0] = gGameWorld.gConMgr.GetSurface("sprites/hats/dh1.fif");
				m_surf_hat[1] = gGameWorld.gConMgr.GetSurface("sprites/hats/dh2.fif");
				m_surf_hat[2] = gGameWorld.gConMgr.GetSurface("sprites/hats/dh3.fif");
				m_surf_hat[3] = gGameWorld.gConMgr.GetSurface("sprites/hats/dh4.fif");
				m_score-=2500;
			}

			if(m_hat == 2) //Diving Hat
			{
				m_surf_hat[0] = gGameWorld.gConMgr.GetSurface("sprites/hats/dv1.fif");
				m_surf_hat[1] = gGameWorld.gConMgr.GetSurface("sprites/hats/dv2.fif");
				m_surf_hat[2] = gGameWorld.gConMgr.GetSurface("sprites/hats/dv3.fif");
				m_surf_hat[3] = gGameWorld.gConMgr.GetSurface("sprites/hats/dv4.fif");
				m_score+=6500;
			}

			if(m_hat == 3) //Fez Hat
			{
				m_surf_hat[0] = gGameWorld.gConMgr.GetSurface("sprites/hats/fz1.fif");
				m_surf_hat[1] = gGameWorld.gConMgr.GetSurface("sprites/hats/fz2.fif");
				m_surf_hat[2] = gGameWorld.gConMgr.GetSurface("sprites/hats/fz3.fif");
				m_surf_hat[3] = gGameWorld.gConMgr.GetSurface("sprites/hats/fz4.fif");
				m_score+=1000;
			}

			if(m_hat == 4) //Mexican Hat
			{
				m_surf_hat[0] = gGameWorld.gConMgr.GetSurface("sprites/hats/mh1.fif");
				m_surf_hat[1] = gGameWorld.gConMgr.GetSurface("sprites/hats/mh2.fif");
				m_surf_hat[2] = gGameWorld.gConMgr.GetSurface("sprites/hats/mh3.fif");
				m_surf_hat[3] = gGameWorld.gConMgr.GetSurface("sprites/hats/mh4.fif");
				m_score+=2000;
			}

			if(m_hat == 5) //Night Cap
			{
				m_surf_hat[0] = gGameWorld.gConMgr.GetSurface("sprites/hats/nc1.fif");
				m_surf_hat[1] = gGameWorld.gConMgr.GetSurface("sprites/hats/nc2.fif");
				m_surf_hat[2] = gGameWorld.gConMgr.GetSurface("sprites/hats/nc3.fif");
				m_surf_hat[3] = gGameWorld.gConMgr.GetSurface("sprites/hats/nc4.fif");
				m_score+=4000;
			}

			if(m_hat == 6) //Skull Cap
			{
				m_surf_hat[0] = gGameWorld.gConMgr.GetSurface("sprites/hats/sc1.fif");
				m_surf_hat[1] = gGameWorld.gConMgr.GetSurface("sprites/hats/sc2.fif");
				m_surf_hat[2] = NULL;
				m_surf_hat[3] = gGameWorld.gConMgr.GetSurface("sprites/hats/sc3.fif");
				m_score+=500;
			}

			sound.PlaySound("sounds/gravepickup.wav");
			gGameWorld.GetGridCell(m_pos.x, m_pos.y)->ent = NULL;
			m_pos.x = ox; m_pos.y = oy;
			return;
		}

		gGameWorld.GetGridCell(m_pos.x, m_pos.y)->ent->SendEvent(MSG_COLLISION, this);

		m_pos.x = ox; m_pos.y = oy;
		return;
	}

	//Start Animation
	m_inanim = 1;

	gGameWorld.GetGridCell(m_pos.x, m_pos.y)->ent = ent; //Move into new cell
    gGameWorld.GetGridCell(ox, oy)->ent = NULL; //Clear old cell
}


//Moves a player allong specified path
void EntPlayer::MoveAllongPath(vector<Vector2D> path)
{
	//Reset the current path (if there is one)
	m_path.clear();

	//Copy from path to m_path
	for(int i = 0; i < path.size(); i++){m_path.push_back(path[i]);}
	
	//Set position in path to beginning
	m_pathpos = 0;
}

//Player Events
void EntPlayer::SendEvent(int msg, void* addarg)
{
	BaseEntity* b = (BaseEntity*)addarg;

	if(b->m_ident == IDENT_BUTTERFLY) //If collision from butterfly
	{
		if(m_walldestroy == 1)
		{
			m_walldestroy = 0;
			sound.PlaySound("sounds/bde.wav");
			Effect* smoke = gGameWorld.gEffects.GetFreeEffect();
			smoke->SetSpeed(140);
			smoke->SetOpacity(0.35f);
			smoke->SetFrame(0, gGameWorld.gConMgr.GetSurface("sprites/effects/smoke1.fif"));
			smoke->SetFrame(1, gGameWorld.gConMgr.GetSurface("sprites/effects/smoke2.fif"));
			smoke->SetFrame(2, gGameWorld.gConMgr.GetSurface("sprites/effects/smoke3.fif"));
			smoke->SetPos(gGameWorld.GetGridCell(b->GetPos().x, b->GetPos().y)->pos.x-1, gGameWorld.GetGridCell(b->GetPos().x, b->GetPos().y)->pos.y-1);
			smoke->Enable();
			return;
		}

		if(m_hat != 0)
		{
			if(m_hat == 1) //Dunce Hat
			{m_score+=2500;}

			if(m_hat == 2) //Diving Hat
			{m_score-=6500;}

			if(m_hat == 3) //Fez Hat
			{m_score-=1000;}

			if(m_hat == 4) //Mexican Hat
			{m_score-=2000;}

			if(m_hat == 5) //Night Cap
			{m_score-=4000;}

			if(m_hat == 6) //Skull Cap
			{m_score-=500;}

			m_hat = 0;
			sound.PlaySound("sounds/bde.wav");
			Effect* smoke = gGameWorld.gEffects.GetFreeEffect();
			smoke->SetSpeed(140);
			smoke->SetOpacity(0.35f);
			smoke->SetFrame(0, gGameWorld.gConMgr.GetSurface("sprites/effects/smoke1.fif"));
			smoke->SetFrame(1, gGameWorld.gConMgr.GetSurface("sprites/effects/smoke2.fif"));
			smoke->SetFrame(2, gGameWorld.gConMgr.GetSurface("sprites/effects/smoke3.fif"));
			smoke->SetPos(gGameWorld.GetGridCell(b->GetPos().x, b->GetPos().y)->pos.x-1, gGameWorld.GetGridCell(b->GetPos().x, b->GetPos().y)->pos.y-1);
			smoke->Enable();
			//m_pollenated = 1;
			//sound.PlaySound("sounds/hit.wav");
		}
		else //If your wearing a hat take that away, otherwise do normal..
		{
			m_life -= (m_life/3); //Take a third health
			m_score -= (m_score/6); //Take a sixth score
			m_pollenated = 1;
			sound.PlaySound("sounds/hit.wav");
			Effect* smoke = gGameWorld.gEffects.GetFreeEffect();
			smoke->SetSpeed(170);
			smoke->SetOpacity(0.35f);
			smoke->SetFrame(0, gGameWorld.gConMgr.GetSurface("sprites/effects/bh1.fif"));
			smoke->SetFrame(1, gGameWorld.gConMgr.GetSurface("sprites/effects/bh2.fif"));
			smoke->SetFrame(2, gGameWorld.gConMgr.GetSurface("sprites/effects/bh3.fif"));
			smoke->SetPos(gGameWorld.GetGridCell(b->GetPos().x, b->GetPos().y)->pos.x+1, gGameWorld.GetGridCell(b->GetPos().x, b->GetPos().y)->pos.y-16);
			smoke->Enable();
		}
	}
}


//Accessors
int& EntPlayer::GetLife(){return m_life;}
int& EntPlayer::GetScore(){return m_score;}
string EntPlayer::GetName(){return m_name;}
int EntPlayer::GetDirection(){return m_direction;}
void EntPlayer::ResetAnim(){m_inanim=0;}


