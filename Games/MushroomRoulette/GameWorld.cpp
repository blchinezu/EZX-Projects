/*
	Code written by James Fletcher (LiX). 
	Copyright 2009 - 2010. 
	ethocybin@googlemail.com
*/

#include "GameWorld.h"
GameWorld gGameWorld;

#include "MushRlt.h"
#include "Rand.h"
#include "Sound.h"

#include "FontSystem.h"
bool firstgen=1;
char line1[64];
char line2[64];
char line3[64];
char line4[64];
char line5[64];


//Contructor
GameWorld::GameWorld()
{
    //Setup graphic buffers
    backbuffer = SDL_CreateRGBSurface(SDL_HWSURFACE, 320, 240, 16, 0, 0, 0, 0);
	gConMgr.AddSurface("sprites/grid.fif", "sprites/grid.fif", 0);
	gConMgr.AddSurface("sprites/levelpacks.fif", "sprites/levelpacks.fif", 0);
	gConMgr.AddSurface("sprites/molegrid.fif", "sprites/molegrid.fif", 0);
	gConMgr.AddSurface("sprites/sban.fif", "sprites/sban.fif", 0);
	gConMgr.AddSurface("sprites/box2.fif", "sprites/box2.fif", 0);
	gConMgr.AddSurface("sprites/box1.fif", "sprites/box1.fif", 0);
	gConMgr.AddSurface("sprites/m1.fif", "sprites/m1.fif", 1);
	gConMgr.AddSurface("sprites/m2.fif", "sprites/m2.fif", 1);
	gConMgr.AddSurface("sprites/m3.fif", "sprites/m3.fif", 1);
	gConMgr.AddSurface("sprites/m4.fif", "sprites/m4.fif", 1);
	gConMgr.AddSurface("sprites/m5.fif", "sprites/m5.fif", 1);
	gConMgr.AddSurface("sprites/m6.fif", "sprites/m6.fif", 1);
	gConMgr.AddSurface("sprites/getready.fif", "sprites/getready.fif", 1);
	gConMgr.AddSurface("sprites/go.fif", "sprites/go.fif", 1);
	gConMgr.AddSurface("sprites/man/pu.fif", "sprites/pu.fif", 1);
	gConMgr.AddSurface("sprites/man/pu1.fif", "sprites/pu1.fif", 1);
	gConMgr.AddSurface("sprites/man/pu2.fif", "sprites/pu2.fif", 1);
	gConMgr.AddSurface("sprites/man/pd.fif", "sprites/pd.fif", 1);
	gConMgr.AddSurface("sprites/man/pd1.fif", "sprites/pd1.fif", 1);
	gConMgr.AddSurface("sprites/man/pd2.fif", "sprites/pd2.fif", 1);
	gConMgr.AddSurface("sprites/man/pl.fif", "sprites/pl.fif", 1);
	gConMgr.AddSurface("sprites/man/pl1.fif", "sprites/pl1.fif", 1);
	gConMgr.AddSurface("sprites/man/pl2.fif", "sprites/pl2.fif", 1);
	gConMgr.AddSurface("sprites/man/pr.fif", "sprites/pr.fif", 1);
	gConMgr.AddSurface("sprites/man/pr1.fif", "sprites/pr1.fif", 1);
	gConMgr.AddSurface("sprites/man/pr2.fif", "sprites/pr2.fif", 1);
	gConMgr.AddSurface("sprites/purple/pu.fif", "sprites/purple/pu.fif", 1);
	gConMgr.AddSurface("sprites/purple/pu1.fif", "sprites/purple/pu1.fif", 1);
	gConMgr.AddSurface("sprites/purple/pu2.fif", "sprites/purple/pu2.fif", 1);
	gConMgr.AddSurface("sprites/purple/pd.fif", "sprites/purple/pd.fif", 1);
	gConMgr.AddSurface("sprites/purple/pd1.fif", "sprites/purple/pd1.fif", 1);
	gConMgr.AddSurface("sprites/purple/pd2.fif", "sprites/purple/pd2.fif", 1);
	gConMgr.AddSurface("sprites/purple/pl.fif", "sprites/purple/pl.fif", 1);
	gConMgr.AddSurface("sprites/purple/pl1.fif", "sprites/purple/pl1.fif", 1);
	gConMgr.AddSurface("sprites/purple/pl2.fif", "sprites/purple/pl2.fif", 1);
	gConMgr.AddSurface("sprites/purple/pr.fif", "sprites/purple/pr.fif", 1);
	gConMgr.AddSurface("sprites/purple/pr1.fif", "sprites/purple/pr1.fif", 1);
	gConMgr.AddSurface("sprites/purple/pr2.fif", "sprites/purple/pr2.fif", 1);
	gConMgr.AddSurface("sprites/butterfly/bu1.fif", "sprites/butterfly/bu1.fif", 1);
	gConMgr.AddSurface("sprites/butterfly/bu2.fif", "sprites/butterfly/bu2.fif", 1);
	gConMgr.AddSurface("sprites/butterfly/bu3.fif", "sprites/butterfly/bu3.fif", 1);
	gConMgr.AddSurface("sprites/butterfly/bd1.fif", "sprites/butterfly/bd1.fif", 1);
	gConMgr.AddSurface("sprites/butterfly/bd2.fif", "sprites/butterfly/bd2.fif", 1);
	gConMgr.AddSurface("sprites/butterfly/bd3.fif", "sprites/butterfly/bd3.fif", 1);
	gConMgr.AddSurface("sprites/butterfly/bl1.fif", "sprites/butterfly/bl1.fif", 1);
	gConMgr.AddSurface("sprites/butterfly/bl2.fif", "sprites/butterfly/bl2.fif", 1);
	gConMgr.AddSurface("sprites/butterfly/bl3.fif", "sprites/butterfly/bl3.fif", 1);
	gConMgr.AddSurface("sprites/butterfly/br1.fif", "sprites/butterfly/br1.fif", 1);
	gConMgr.AddSurface("sprites/butterfly/br2.fif", "sprites/butterfly/br2.fif", 1);
	gConMgr.AddSurface("sprites/butterfly/br3.fif", "sprites/butterfly/br3.fif", 1);
	gConMgr.AddSurface("sprites/well.fif", "sprites/well.fif", 1);
	gConMgr.AddSurface("sprites/well2.fif", "sprites/well2.fif", 1);
	gConMgr.AddSurface("sprites/evo/ed.fif", "sprites/evo/ed.fif", 1);
	gConMgr.AddSurface("sprites/evo/ed1.fif", "sprites/evo/ed1.fif", 1);
	gConMgr.AddSurface("sprites/evo/ed2.fif", "sprites/evo/ed2.fif", 1);
	gConMgr.AddSurface("sprites/evo/el.fif", "sprites/evo/el.fif", 1);
	gConMgr.AddSurface("sprites/evo/el1.fif", "sprites/evo/el1.fif", 1);
	gConMgr.AddSurface("sprites/evo/el2.fif", "sprites/evo/el2.fif", 1);
	gConMgr.AddSurface("sprites/evo/er.fif", "sprites/evo/er.fif", 1);
	gConMgr.AddSurface("sprites/evo/er1.fif", "sprites/evo/er1.fif", 1);
	gConMgr.AddSurface("sprites/evo/er2.fif", "sprites/evo/er2.fif", 1);
	gConMgr.AddSurface("sprites/evo/eu.fif", "sprites/evo/eu.fif", 1);
	gConMgr.AddSurface("sprites/evo/eu1.fif", "sprites/evo/eu1.fif", 1);
	gConMgr.AddSurface("sprites/evo/eu2.fif", "sprites/evo/eu2.fif", 1);
	gConMgr.AddSurface("sprites/evop/ed.fif", "sprites/evop/ed.fif", 1);
	gConMgr.AddSurface("sprites/evop/ed1.fif", "sprites/evop/ed1.fif", 1);
	gConMgr.AddSurface("sprites/evop/ed2.fif", "sprites/evop/ed2.fif", 1);
	gConMgr.AddSurface("sprites/evop/el.fif", "sprites/evop/el.fif", 1);
	gConMgr.AddSurface("sprites/evop/el1.fif", "sprites/evop/el1.fif", 1);
	gConMgr.AddSurface("sprites/evop/el2.fif", "sprites/evop/el2.fif", 1);
	gConMgr.AddSurface("sprites/evop/er.fif", "sprites/evop/er.fif", 1);
	gConMgr.AddSurface("sprites/evop/er1.fif", "sprites/evop/er1.fif", 1);
	gConMgr.AddSurface("sprites/evop/er2.fif", "sprites/evop/er2.fif", 1);
	gConMgr.AddSurface("sprites/evop/eu.fif", "sprites/evop/eu.fif", 1);
	gConMgr.AddSurface("sprites/evop/eu1.fif", "sprites/evop/eu1.fif", 1);
	gConMgr.AddSurface("sprites/evop/eu2.fif", "sprites/evop/eu2.fif", 1);
	gConMgr.AddSurface("sprites/highscores.fif", "sprites/highscores.fif", 0);
	gConMgr.AddSurface("sprites/fail.fif", "sprites/fail.fif", 1);
	gConMgr.AddSurface("sprites/menu.fif", "sprites/menu.fif", 0);
	gConMgr.AddSurface("sprites/cursor.fif", "sprites/cursor.fif", 1);
	gConMgr.AddSurface("sprites/effects/smoke1.fif", "sprites/effects/smoke1.fif", 1);
	gConMgr.AddSurface("sprites/effects/smoke2.fif", "sprites/effects/smoke2.fif", 1);
	gConMgr.AddSurface("sprites/effects/smoke3.fif", "sprites/effects/smoke3.fif", 1);
	gConMgr.AddSurface("sprites/effects/db1.fif", "sprites/effects/db1.fif", 1);
	gConMgr.AddSurface("sprites/effects/db2.fif", "sprites/effects/db2.fif", 1);
	gConMgr.AddSurface("sprites/effects/db3.fif", "sprites/effects/db3.fif", 1);
	gConMgr.AddSurface("sprites/effects/db4.fif", "sprites/effects/db4.fif", 1);
	gConMgr.AddSurface("sprites/effects/db5.fif", "sprites/effects/db5.fif", 1);
	gConMgr.AddSurface("sprites/effects/db6.fif", "sprites/effects/db6.fif", 1);
	gConMgr.AddSurface("sprites/effects/db7.fif", "sprites/effects/db7.fif", 1);
	gConMgr.AddSurface("sprites/effects/db8.fif", "sprites/effects/db8.fif", 1);
	gConMgr.AddSurface("sprites/effects/db9.fif", "sprites/effects/db9.fif", 1);
	gConMgr.AddSurface("sprites/hats/sc1.fif", "sprites/hats/sc1.fif", 1); //Skull Cap
	gConMgr.AddSurface("sprites/hats/sc2.fif", "sprites/hats/sc2.fif", 1);
	gConMgr.AddSurface("sprites/hats/sc3.fif", "sprites/hats/sc3.fif", 1);
	gConMgr.AddSurface("sprites/hats/nc1.fif", "sprites/hats/nc1.fif", 1); //Night Cap
	gConMgr.AddSurface("sprites/hats/nc2.fif", "sprites/hats/nc2.fif", 1);
	gConMgr.AddSurface("sprites/hats/nc3.fif", "sprites/hats/nc3.fif", 1);
	gConMgr.AddSurface("sprites/hats/nc4.fif", "sprites/hats/nc4.fif", 1);
	gConMgr.AddSurface("sprites/hats/mh1.fif", "sprites/hats/mh1.fif", 1); //Mexican Hat
	gConMgr.AddSurface("sprites/hats/mh2.fif", "sprites/hats/mh2.fif", 1);
	gConMgr.AddSurface("sprites/hats/mh3.fif", "sprites/hats/mh3.fif", 1);
	gConMgr.AddSurface("sprites/hats/mh4.fif", "sprites/hats/mh4.fif", 1);
	gConMgr.AddSurface("sprites/hats/fz1.fif", "sprites/hats/fz1.fif", 1); //Fez Hat
	gConMgr.AddSurface("sprites/hats/fz2.fif", "sprites/hats/fz2.fif", 1);
	gConMgr.AddSurface("sprites/hats/fz3.fif", "sprites/hats/fz3.fif", 1);
	gConMgr.AddSurface("sprites/hats/fz4.fif", "sprites/hats/fz4.fif", 1);
	gConMgr.AddSurface("sprites/hats/dv1.fif", "sprites/hats/dv1.fif", 1); //Diving Hat
	gConMgr.AddSurface("sprites/hats/dv2.fif", "sprites/hats/dv2.fif", 1);
	gConMgr.AddSurface("sprites/hats/dv3.fif", "sprites/hats/dv3.fif", 1);
	gConMgr.AddSurface("sprites/hats/dv4.fif", "sprites/hats/dv4.fif", 1);
	gConMgr.AddSurface("sprites/hats/dh1.fif", "sprites/hats/dh1.fif", 1); //Dunce Hat
	gConMgr.AddSurface("sprites/hats/dh2.fif", "sprites/hats/dh2.fif", 1);
	gConMgr.AddSurface("sprites/hats/dh3.fif", "sprites/hats/dh3.fif", 1);
	gConMgr.AddSurface("sprites/hats/dh4.fif", "sprites/hats/dh4.fif", 1);
	gConMgr.AddSurface("sprites/stalk.fif", "sprites/stalk.fif", 1);
	gConMgr.AddSurface("sprites/effects/bh1.fif", "sprites/effects/bh1.fif", 1);
	gConMgr.AddSurface("sprites/effects/bh2.fif", "sprites/effects/bh2.fif", 1);
	gConMgr.AddSurface("sprites/effects/bh3.fif", "sprites/effects/bh3.fif", 1);

	gConMgr.AddSurface("sprites/c1.fif", "sprites/c1.fif", 0);
	gConMgr.AddSurface("sprites/c2.fif", "sprites/c2.fif", 0);
	gConMgr.AddSurface("sprites/c3.fif", "sprites/c3.fif", 0);
	gConMgr.AddSurface("sprites/c4.fif", "sprites/c4.fif", 0);

	SDLE_LoadFIF("sprites/grid.fif", &m_surf_background);

	m_surf_molebackground = gConMgr.GetSurface("sprites/molegrid.fif");
	m_surf_gr[0] = gConMgr.GetSurface("sprites/getready.fif");
	m_surf_gr[1] = gConMgr.GetSurface("sprites/go.fif");

     
    //Setup GameGrid Positions
    int cx=8, cy=22;
    for(int y = 0; y < 11; y++)
    {
		for(int x = 0; x < 17; x++)
		{
			m_gamegrid[x][y].pos.x = cx;
			m_gamegrid[x][y].pos.y = cy;
			m_gamegrid[x][y].ent = NULL;
			cx += 18;
		}
             
    cy += 18;
    cx=8;
    }
     
    //Generate a random level
	m_readyreg = 0;
	m_totalplayers=0;
	m_totalmushrooms=0;
	m_totalboxes=0;
	m_totalhats = 0;
	m_totalwells = 0;
	m_bonustype = 0;
	m_totalbutterflys=0;
	hat_regulator = 0;
	hat_nextshow = qRand(180000, 420000);
	well_regulator = 0;
	well_nextshow = qRand(120000, 360000);
	m_isbonus = 0;
	m_ready = 0;
	m_level = 0;
	m_gamemode = 0;
	m_beginscore = 0;
	gentoggle=0;
	gamestart=0;
    //GenerateRandomLevel();

	//Add a player
	AddPlayer("Spaceman");

	//test
	//gScores.AddScore(20000, 300, 0);
}

//Destructor
GameWorld::~GameWorld()
{
	 //Content Manager cleans up everything for us...
	SDL_FreeSurface(backbuffer);
}

//Update
void GameWorld::Update()
{ 
    //Update Game Grid
    for(int y = 0; y < 11; y++)
    {
		for(int x = 0; x < 17; x++)
		{
			if(m_gamegrid[x][y].ent == NULL){continue;}
			m_gamegrid[x][y].ent->Update();
		}
    }

	//Randomy place a well during gameplay
	if(m_gamemode == 0)
	{
		if(GetTicks() > well_regulator+well_nextshow)
		{
			AddWell();
			well_regulator = GetTicks();
			well_nextshow = qRand(120000, 360000);
		}

		//Randomy place a hat during gameplay
		if(GetTicks() > hat_regulator+hat_nextshow)
		{
			AddHat();
			hat_regulator = GetTicks();
			hat_nextshow = qRand(180000, 420000);
		}
	}

	//Generate a new level?
	if(gentoggle == 1){NextLevel(); gentoggle=0;}
}

//Render
char intc[64]; // for into to char conversions
void GameWorld::Render()
{
	//Begin Game Screen (cherry cake image)
    if(gamestart == 0){sound.PlaySound("sounds/init.wav"); gamestart = GetTicks();}

	if(gamestart != -1)
	{
		SDL_BlitSurface(gConMgr.GetSurface("sprites/sban.fif"), 0, backbuffer, &renderarea);
		SDL_BlitSurface(backbuffer, 0, screen, &renderarea);

		SDL_Flip(screen);

		if(GetTicks() > gamestart+3000)
		{
			gamestart = -1;
		}

		return;
	}


    //Blit Background
	if(m_bonustype == 0 || m_bonustype == 1 || m_bonustype == 3){
		SDL_BlitSurface(m_surf_background, 0, backbuffer, &renderarea);}
	if(m_bonustype == 2){SDL_BlitSurface(m_surf_molebackground, 0, backbuffer, &renderarea);}


	//HUD Font
	gFont.DrawString(backbuffer, 8, 4, "Life:", FONT_NORMAL);
	sprintf(intc, "%i%%", m_players[0].GetLife());
	if(m_players[0].GetLife() >= 50){gFont.DrawString(backbuffer, 42, 4, intc, FONT_GREEN);}
	else{gFont.DrawString(backbuffer, 42, 4, intc, FONT_RED);}

	gFont.DrawString(backbuffer, 188, 4, "Score:", FONT_NORMAL);
	sprintf(intc, "%i", m_players[0].GetScore());
	gFont.DrawString(backbuffer, 234, 4, intc, FONT_BLUE);

	gFont.DrawString(backbuffer, 8, 224, "Level:", FONT_NORMAL);
	sprintf(intc, "%i", m_level);
	gFont.DrawString(backbuffer, 53, 224, intc, FONT_NORMAL);

    //Render Game Grid
    for(int y = 0; y < 11; y++)
    {
		for(int x = 0; x < 17; x++)
		{
			if(m_gamegrid[x][y].ent == NULL){continue;}
			m_gamegrid[x][y].ent->Render();
		}
    }

	//Draw get ready graphics
	if(m_isbonus == 0 && gMenu.m_enabled == 0)
	{
		if(m_ready == 0 && GetTicks() > m_readyreg+500 && GetTicks() < m_readyreg+(60*strlen(m_description))+1600)
		{
			if(m_description[0] != 0x00)
			{
				SDLE_FillRectAlpha(backbuffer, &renderarea, SDL_MapRGB(backbuffer->format, 0, 120, 230), 0.8f);
			}

			SDL_BlitSurface(m_surf_gr[0], 0, backbuffer, &SDLE_Rect(45, 100, m_surf_gr[0]->w, m_surf_gr[0]->h));

			if(m_description[0] != 0x00)
			{
				gFont.DrawString(backbuffer, 40, 140, line1, FONT_WHITE);
				gFont.DrawString(backbuffer, 40, 154, line2, FONT_WHITE);
				gFont.DrawString(backbuffer, 40, 166, line3, FONT_WHITE);
				gFont.DrawString(backbuffer, 40, 178, line4, FONT_WHITE);
				gFont.DrawString(backbuffer, 40, 192, line5, FONT_WHITE);
			}
		}

		if(m_ready == 0 && GetTicks() > m_readyreg+(60*strlen(m_description))+1600)
		{
			SDL_BlitSurface(m_surf_gr[1], 0, backbuffer, &SDLE_Rect(75, 90, m_surf_gr[1]->w, m_surf_gr[1]->h));
		}

		//Set game to ready!!
		if(m_ready == 0 && GetTicks() > m_readyreg+(60*strlen(m_description))+1900)
		{
			m_ready = 1;
			sound.PlaySound("sounds/go.wav");
		}
	}else{m_ready = 1; m_isbonus = 0;}

	//Effects!
	gEffects.RenderEffects();

	//Menu Stuff!
	if(gMenu.m_enabled == 1){gMenu.Render();}
	if(gScores.m_enabled == 1){gScores.Render();}


    //Blit and Flip'
	SDL_BlitSurface(backbuffer, 0, screen, &renderarea);
    SDL_Flip(screen);
}

//Makes all movable boxes unmovable
bool GameWorld::FreezeBoxes()
{
	int bf=0;

	for(int i = 0; i < m_totalboxes; i++)
	{
		if(m_boxes[i].m_moveable == 1){m_boxes[i].m_moveable = 0; bf++;}
	}

	if(bf!=0){sound.PlaySound("sounds/freezeboxes.wav"); return 1;}
	return 0;
}

//Checks if you ate the last red mushroom (new level) FOR NETWORK PLAY TELL HOST AND WAIT FOR RESYNC
void GameWorld::CheckIfLastRedMushroom()
{
	if(m_bonustype != 0){return;}

	int rmc;
	EntMushroom* m;

	//First Check!
	rmc = 0;

	for(int i = 0; i < m_totalmushrooms; i++)
	{
		if(&m_mushrooms[i] != NULL)
		{
			if(m_mushrooms[i].IsVisible() == 1 && m_mushrooms[i].GetType() == MUSHROOM_RED){rmc++;}
		}
	}

	if(rmc == 0){gentoggle=1; sound.PlaySound("sounds/lastred.wav"); return;}


	//Second Check!
	/*
	rmc = 0;

	for(int y = 0; y < 11; y++)
    {
		for(int x = 0; x < 17; x++)
		{
			if(m_gamegrid[x][y].ent != NULL)
			{
				if(m_gamegrid[x][y].ent->m_ident == IDENT_MUSHROOM)
				{
					m = (EntMushroom*)m_gamegrid[x][y].ent;
					if(m->GetType() == MUSHROOM_RED)
					{
						rmc++;
					}
				}
			}
		}
    }

	if(rmc == 0){gentoggle=1; sound.PlaySound("sounds/lastred.wav"); return;}
	*/
}

//Score bonus left behind when a player dies
void GameWorld::AddBlueBonus(Vector2D pos, int score)
{
	if(m_totalmushrooms >= sizeof(m_mushrooms)){return;} //Full

	EntMushroom m;

	m_mushrooms[m_totalmushrooms].SetPos(pos.x, pos.y);
	m_mushrooms[m_totalmushrooms].InitMushroom(MUSHROOM_BLUE);
	m_mushrooms[m_totalmushrooms].m_bluebonus = score;
	m_gamegrid[pos.x][pos.y].ent = (BaseEntity*)&m_mushrooms[m_totalmushrooms];

	m_totalmushrooms++;
}

//Accessors
Cell* GameWorld::GetGridCell(int x, int y){return &m_gamegrid[x][y];}
EntPlayer* GameWorld::GetPlayer(int id){return &m_players[id];}
bool GameWorld::IsReady(){return m_ready;}
int GameWorld::GetTotalPlayers(){return m_totalplayers;}
int GameWorld::GetLevel(){return m_level;}


//Adds a player to the game
void GameWorld::AddPlayer(string name)
{
	if(m_totalplayers >= sizeof(m_players)){return;} //Full
	int rx=qRand(0, 17),ry=qRand(0, 11);

	while(m_gamegrid[rx][ry].ent != NULL)
	{
		rx = qRand(0, 17);
		ry = qRand(0, 11);
	}

	m_players[m_totalplayers].SetPos(rx, ry);
	m_players[m_totalplayers].InitPlayer(name);
	m_gamegrid[rx][ry].ent = (BaseEntity*)&m_players[m_totalplayers];
	m_totalplayers++;
}

//Adds a butterfly to the game
void GameWorld::AddButterfly(int id)
{
	if(m_totalbutterflys >= sizeof(m_butterflys)){return;} //Full
	int rx=qRand(0, 17),ry=qRand(0, 11);

	while(m_gamegrid[rx][ry].ent != NULL)
	{
		rx = qRand(0, 17);
		ry = qRand(0, 11);
	}

	m_butterflys[m_totalbutterflys].SetPos(rx, ry);
	m_butterflys[m_totalbutterflys].InitButterfly(id, "");
	m_gamegrid[rx][ry].ent = (BaseEntity*)&m_butterflys[m_totalbutterflys];
	m_totalbutterflys++;
}

//Adds specified boxes to the game
void GameWorld::AddBoxes(int min, int max, bool type)
{
	if(m_totalboxes >= sizeof(m_boxes)){return;} //Full
	int rx=qRand(0, 17),ry=qRand(0, 11),rc=0;

	rc = qRand(min+1,max+1);

	for(int i = m_totalboxes; i < m_totalboxes+rc; i++)
	{
		while(m_gamegrid[rx][ry].ent != NULL)
		{
			rx = qRand(0, 17);
			ry = qRand(0, 11);
		}

		m_boxes[i].SetPos(rx, ry);
		m_boxes[i].InitBox(type);
		m_gamegrid[rx][ry].ent = (BaseEntity*)&m_boxes[i];
	}

	m_totalboxes += rc;
}

//Adds specified boxe to the game
void GameWorld::AddBoxPos(int x, int y)
{
	if(m_totalboxes >= sizeof(m_boxes)){return;} //Full

	if(m_gamegrid[x][y].ent == NULL)
	{
		m_boxes[m_totalboxes].SetPos(x, y);
		m_boxes[m_totalboxes].InitBox(1);
		m_gamegrid[x][y].ent = (BaseEntity*)&m_boxes[m_totalboxes];

		m_totalboxes++;
	}
}

//Adds specified mushrooms to the game
void GameWorld::AddMushrooms(int min, int max, int type)
{
	if(m_totalmushrooms >= sizeof(m_mushrooms)){return;} //Full
	int rx=qRand(0, 17),ry=qRand(0, 11),rc=0;

	rc = qRand(min+1,max+1);

	for(int i = m_totalmushrooms; i < m_totalmushrooms+rc; i++)
	{
		while(m_gamegrid[rx][ry].ent != NULL)
		{
			rx = qRand(0, 17);
			ry = qRand(0, 11);
		}

		m_mushrooms[i].SetPos(rx, ry);
		m_mushrooms[i].InitMushroom(type);
		m_gamegrid[rx][ry].ent = (BaseEntity*)&m_mushrooms[i];

		//Set a Decal
		SDL_BlitSurface(gConMgr.GetSurface("sprites/stalk.fif"), 0, m_surf_background, &SDLE_Rect(m_gamegrid[rx][ry].pos.x+3, m_gamegrid[rx][ry].pos.y+3, gConMgr.GetSurface("sprites/stalk.fif")->w, gConMgr.GetSurface("sprites/stalk.fif")->h));
	}

	m_totalmushrooms += rc;
}

//Adds an escape mushroom to the game
void GameWorld::AddEscapeMushroom(int x, int y)
{
	if(m_totalmushrooms >= sizeof(m_mushrooms)){return;} //Full
	int rx=qRand(0, 17),ry=qRand(0, 11);

	if(x == 0 && y == 0)
	{
		while(m_gamegrid[rx][ry].ent != NULL)
		{
			rx = qRand(0, 17);
			ry = qRand(0, 11);
		}
	}else{rx = x; ry = y;}

	m_mushrooms[m_totalmushrooms].SetPos(rx, ry);
	m_mushrooms[m_totalmushrooms].InitMushroom(6);
	m_gamegrid[rx][ry].ent = (BaseEntity*)&m_mushrooms[m_totalmushrooms];

	m_totalmushrooms++;
}


//Adds a well to the game
void GameWorld::AddWell()
{
	//If the well is allready on the map! Do nothing!
	if(m_well[0].IsVisible() == 1){return;}

	int rx=qRand(0, 17),ry=qRand(0, 11),rc=0;

	while(m_gamegrid[rx][ry].ent != NULL)
	{
		rx = qRand(0, 17);
		ry = qRand(0, 11);
	}

	m_well[0].SetPos(rx, ry);
	m_well[0].InitWell(0);
	m_gamegrid[rx][ry].ent = (BaseEntity*)&m_well[0];

	sound.PlaySound("sounds/wellshow.wav");
}

//Adds a hat to the game
void GameWorld::AddHat()
{
	//If a hat is allready on the map! Do nothing!
	if(m_hat[0].IsVisible() == 1){return;}

	int rx=qRand(0, 17),ry=qRand(0, 11),rc=0;

	while(m_gamegrid[rx][ry].ent != NULL)
	{
		rx = qRand(0, 17);
		ry = qRand(0, 11);
	}

	m_hat[0].SetPos(rx, ry);
	m_hat[0].InitHat(qRand(1, 7));
	m_gamegrid[rx][ry].ent = (BaseEntity*)&m_hat[0];

	sound.PlaySound("sounds/wellshow.wav");
}

//Add Butterfly
void GameWorld::AddButterflyX(int rx, int ry, char* path, int id)
{
	if(m_totalbutterflys >= sizeof(m_butterflys)){return;} //Full

	if(m_gamegrid[rx][ry].ent == NULL)
	{
		m_butterflys[m_totalbutterflys].SetPos(rx, ry);
		m_butterflys[m_totalbutterflys].InitButterfly(id, path);
		m_gamegrid[rx][ry].ent = (BaseEntity*)&m_butterflys[m_totalbutterflys];
		m_totalbutterflys++;
	}
}

//Add Player
void GameWorld::AddPlayerX(string name, int rx, int ry)
{
	if(m_totalplayers >= sizeof(m_players)){return;} //Full

	if(m_gamegrid[rx][ry].ent == NULL)
	{
		m_players[m_totalplayers].SetPos(rx, ry);
		m_players[m_totalplayers].InitPlayer(name);
		m_gamegrid[rx][ry].ent = (BaseEntity*)&m_players[m_totalplayers];
		m_totalplayers++;
	}
}

//Add Box
void GameWorld::AddBoxX(int x, int y, bool type)
{
	if(m_totalboxes >= sizeof(m_boxes)){return;} //Full

	if(m_gamegrid[x][y].ent == NULL)
	{
		m_boxes[m_totalboxes].SetPos(x, y);
		m_boxes[m_totalboxes].InitBox(type);
		m_gamegrid[x][y].ent = (BaseEntity*)&m_boxes[m_totalboxes];

		m_totalboxes++;
	}
}

//Add Mushroom
void GameWorld::AddMushroomX(int rx, int ry, int type)
{
	if(m_totalmushrooms >= sizeof(m_mushrooms)){return;} //Full

	if(m_gamegrid[rx][ry].ent == NULL)
	{
		m_mushrooms[m_totalmushrooms].SetPos(rx, ry);
		m_mushrooms[m_totalmushrooms].InitMushroom(type);
		m_gamegrid[rx][ry].ent = (BaseEntity*)&m_mushrooms[m_totalmushrooms];

		//Set a Decal
		SDL_BlitSurface(gConMgr.GetSurface("sprites/stalk.fif"), 0, m_surf_background, &SDLE_Rect(m_gamegrid[rx][ry].pos.x+3, m_gamegrid[rx][ry].pos.y+3, gConMgr.GetSurface("sprites/stalk.fif")->w, gConMgr.GetSurface("sprites/stalk.fif")->h));

		m_totalmushrooms++;
	}
}

//Add Well
void GameWorld::AddWellX(int x, int y)
{
	if(m_totalwells >= sizeof(m_well)){return;} //Full

	if(m_gamegrid[x][y].ent == NULL)
	{
		m_well[m_totalwells].SetPos(x, y);
		m_well[m_totalwells].InitWell(1);
		m_gamegrid[x][y].ent = (BaseEntity*)&m_well[m_totalwells];

		m_totalwells++;
	}
}

//Add Hat
void GameWorld::AddHatX(int x, int y, int hat)
{
	if(m_totalhats >= sizeof(m_hat)){return;} //Full

	if(m_gamegrid[x][y].ent == NULL)
	{
		m_hat[m_totalhats].SetPos(x, y);
		m_hat[m_totalhats].InitHat(hat);
		m_gamegrid[x][y].ent = (BaseEntity*)&m_hat[m_totalhats];

		m_totalhats++;
	}
}

//Loads a Level
void GameWorld::LoadLevel()
{
	string file;
	sprintf((char*)file.c_str(), "maps/%s%i.mrl", m_levelname.c_str(), m_level);

	//Load Data
	int id;
	int e;
	char p[256];
	int bi=0; //butterfly id

	//Clear the background of any decals
	SDL_BlitSurface(gConMgr.GetSurface("sprites/grid.fif"), 0, m_surf_background, &renderarea);

	//Reset Player Anim State
	m_players[0].ResetAnim();

	//Clear level data
    for(int y = 0; y < 11; y++)
    {
		for(int x = 0; x < 17; x++)
		{
			m_gamegrid[x][y].ent = NULL;
		}
    }

	//Reset some data
	memset(&m_description, 0x00, sizeof(m_description));
	for(int i = 0; i < MAX_CELLS; i++){m_mushrooms[i].SetVisible(0);}
	m_totalmushrooms = 0;
	m_totalboxes = 0;
	m_totalbutterflys = 0;
	m_totalhats = 0;
	m_totalwells = 0;
	m_well[0].SetVisible(0);
	m_hat[0].SetVisible(0);

	//If level doesnt exist:
	FILE* ec = fopen((char*)file.c_str(), "r");
	if(ec != NULL){fclose(ec);}else
	{
		//It doesnt exist
		gGameWorld.gScores.AddScore(m_players[0].GetScore(), gGameWorld.GetLevel(), m_players[0].GetLife());
		gGameWorld.gScores.m_enabled = 1;
		gGameWorld.gMenu.m_selected = 2;
		gGameWorld.gMenu.m_enabled = 1;
		return;
	}


	//Decrypt
	XorCrypt((char*)file.c_str(), "temp.dat", 471);

	//Open Map File
	char out[32];
	FILE* open = fopen("temp.dat", "r");
	if(open != NULL)
	{
		for(int x = 0; x < 17; x++)
		{
			for(int y = 0; y < 11; y++)
			{
				fscanf(open, "%s", out);
				id = atoi(out);

				if(id == 9)
				{
					fscanf(open, "%s", out);
					e = atoi(out);
				}

				if(id == 2)
				{
					memset(&p, 0x00, sizeof(p));
					fscanf(open, "%s", p);
				}

				//Add object to the game
				if(id == 0){AddBoxX(x, y, 1);}
				if(id == 1){AddBoxX(x, y, 0);}
				if(id == 2){AddButterflyX(x, y, p, bi); bi++;}

				if(id == 3)
				{
					m_gamegrid[x][y].ent = &m_players[0];
					m_players[0].SetPos(x, y);
				}

				if(id == 4){AddWellX(x, y);} // well
				if(id == 5){AddMushroomX(x, y, MUSHROOM_BLACK);}
				if(id == 6){AddMushroomX(x, y, MUSHROOM_RED);}
				if(id == 7){AddMushroomX(x, y, MUSHROOM_PINK);}
				if(id == 8){AddMushroomX(x, y, MUSHROOM_PURPLE);}
				if(id == 9){Vector2D tp; tp.Set(x, y); AddBlueBonus(tp, e);}
				if(id == 10){AddHatX(x, y, 4);} // mexican hat
				if(id == 11){AddHatX(x, y, 3);} // fez hat
				if(id == 12){AddHatX(x, y, 2);} // diving hat
				if(id == 13){AddHatX(x, y, 5);} // nightcap
				if(id == 14){AddHatX(x, y, 6);} // skullcap
				if(id == 15){AddHatX(x, y, 1);} // dunce hat
			}
		}

		//Load Description
		fscanf(open, "%s", m_description);
		for(int i = 0; i < sizeof(m_description); i++){if(m_description[i] == '^'){m_description[i] = ' ';}}

		//Break description down into lines
		memset(&line1, 0x00, sizeof(line1));
		memset(&line2, 0x00, sizeof(line2));
		memset(&line3, 0x00, sizeof(line3));
		memset(&line4, 0x00, sizeof(line4));
		memset(&line5, 0x00, sizeof(line5));

		int line=0;
		int lc=0;
		int lsp=0; //last space pos
		int lcsp=0;
		for(int i = 0; i < strlen(m_description); i++)
		{
			if(m_description[i] ==  ' '){lsp = i; lcsp = lc;}

			if(line == 0){line1[lc] = m_description[i];}
			if(line == 1){line2[lc] = m_description[i];}
			if(line == 2){line3[lc] = m_description[i];}
			if(line == 3){line4[lc] = m_description[i];}
			if(line == 4){line5[lc] = m_description[i];}
			
			lc++;
			
			if(lc >= 36)
			{
				i = lsp;
				if(line == 0){line1[lcsp] = 0x00;}
				if(line == 1){line2[lcsp] = 0x00;}
				if(line == 2){line3[lcsp] = 0x00;}
				if(line == 3){line4[lcsp] = 0x00;}
				if(line == 4){line5[lcsp] = 0x00;}

				line++;
				lsp=0;
				lc=0;
			}
			
			if(line > 5){break;}
		}

		fclose(open);
	}

	//Delete Temp
	ClearFile("temp.dat");

	//Get Ready!!
	if(firstgen == 0)
	{
		m_readyreg = GetTicks();
		m_ready = 0;
	}else{firstgen=0;}

	m_beginscore = m_players[0].GetScore();

	m_gamemode = 1;
}


//Generates a random level : NETWORK PLAY, WHEN THIS IS CALLED YOU JUST ASK THE HOST FOR A RESYNC
void GameWorld::GenerateRandomLevel()
{
	//Clear the background of any decals
	SDL_BlitSurface(gConMgr.GetSurface("sprites/grid.fif"), 0, m_surf_background, &renderarea);

	//Clear game grid (of all but the players)
	for(int y = 0; y < 11; y++)
    {
		for(int x = 0; x < 17; x++)
		{
			if(m_gamegrid[x][y].ent != NULL)
			{
				if(m_gamegrid[x][y].ent->m_ident != IDENT_MAN){m_gamegrid[x][y].ent = NULL;}
			}
		}
    }

	//Clear arrays
	for(int i = 0; i < MAX_CELLS; i++){m_mushrooms[i].SetVisible(0);}
	m_totalmushrooms = 0;
	m_totalboxes = 0;
	m_totalbutterflys = 0;
	m_totalhats = 0;
	m_totalwells = 0;
	m_well[0].SetVisible(0);
	m_hat[0].SetVisible(0);

	//Add Entities (If normal level)
	if(m_isbonus == 0) //Normal
	{
		//AddWell();
		AddBoxes(10, 50, 1);
		AddBoxes(3, 28, 0);
		AddMushrooms(2, 30, 1);
		AddMushrooms(2, 40, 2);
		AddMushrooms(0, 8, 3);
		AddMushrooms(0, 3, 4);
		for(int i = 0; i < qRand(0, 5); i++){AddButterfly(i);}
	}

	//Add Entities (If bonus level)
	if(m_isbonus == 1)
	{
		if(m_bonustype == 1) //Field of Red Mushrooms
		{
			AddMushrooms(10, 50, 1);
			AddEscapeMushroom(0, 0); //0,0 = Random Position
		}

		if(m_bonustype == 2) //Whack-a-Mole
		{
			//
			AddEscapeMushroom(8, 10); //Top Right Corner
		}
	}

	//Increment Level
	if(m_isbonus != 1){m_level++;}

	//Get Ready!!
	if(firstgen == 0)
	{
		m_readyreg = GetTicks();
		m_ready = 0;
	}else{firstgen=0;}
}

//New Game
void GameWorld::NewGame()
{
	//Clear the background of any decals
	SDL_BlitSurface(gConMgr.GetSurface("sprites/grid.fif"), 0, m_surf_background, &renderarea);

	//Check if player is still on the map (wont be if you died)
	bool onmap=0;

	for(int y = 0; y < 11; y++)
    {
		for(int x = 0; x < 17; x++)
		{
			if(m_gamegrid[x][y].ent != NULL)
			{
				if(m_gamegrid[x][y].ent == &m_players[0]){onmap=1;}
			}
		}
    }

	//If your not on the map let's add you back to it
	if(onmap == 0)
	{
		int rx=qRand(0, 17),ry=qRand(0, 11);

		while(m_gamegrid[rx][ry].ent != NULL)
		{
			rx = qRand(0, 17);
			ry = qRand(0, 11);
		}

		m_gamegrid[rx][ry].ent = &m_players[0];
		m_players[0].SetPos(rx, ry);
	}

	//Reset game and player
	m_level = 0;
	m_bonustype = 0;
	m_players[0].InitPlayer("Spaceman");
	m_gamemode = 0;

	hat_regulator = GetTicks();
	well_regulator = GetTicks();

	memset(&m_description, 0x00, sizeof(m_description));
	memset(&line1, 0x00, 34);
	memset(&line2, 0x00, 34);
	memset(&line3, 0x00, 34);
	memset(&line4, 0x00, 34);
	memset(&line5, 0x00, 34);
	GenerateRandomLevel();
}

//Moves game onto the next level
void GameWorld::NextLevel()
{
	if(m_gamemode == 0){GenerateRandomLevel();}
	if(m_gamemode == 1){m_level++; LoadLevel();}
}



//Debug, dumps mushrooms array.
void GameWorld::DumpMushrooms(string name)
{
	FILE* f = fopen(name.c_str(), "a");
	if(f != NULL)
	{
		for(int i = 0; i < 100; i++)
		{
			fprintf(f, "%i: %i %i, %i, %i\n", i, m_mushrooms[i].GetPos().x,
				m_mushrooms[i].GetPos().y,
				m_mushrooms[i].GetType(),
				m_mushrooms[i].IsVisible());
		}

		fclose(f);
	}
}

//Returns the nearest grid square position to the provided mouse position
Vector2D GameWorld::GetNearestGridSquarePositionToMouse(int ix, int iy)
{
	Vector2D mousepos;
	mousepos.Set(ix, iy);
	Vector2D gsp;
	Vector2D retval;
	retval.Set(-1, -1);

	for(int y = 0; y < 11; y++)
    {
		for(int x = 0; x < 17; x++)
		{
			gsp = m_gamegrid[x][y].pos;

			if(mousepos.x > gsp.x && mousepos.x < gsp.x+19
				&& mousepos.y > gsp.y && mousepos.y < gsp.y+19)
			{
				retval.Set(x, y);
				return retval;
			}
		}
    }

	return retval;
}

//Remove all red mushrooms from the map
void GameWorld::EatRedMushrooms()
{
	for(int i = 0; i < 100; i++){m_mushrooms[i].SetVisible(0);}
}

//This is a horrible Hunt & Kill maze algorithm i (shamefully) wrote (deliberate problem: it doesnt account for cross sections)
void GameWorld::GenerateMaze()
{
	//Clear the background of any decals
	SDL_BlitSurface(gConMgr.GetSurface("sprites/grid.fif"), 0, m_surf_background, &renderarea);

	//Clear game grid (of all but the players)
	for(int y = 0; y < 11; y++)
    {
		for(int x = 0; x < 17; x++)
		{
			if(m_gamegrid[x][y].ent != NULL)
			{
				if(m_gamegrid[x][y].ent->m_ident != IDENT_MAN){m_gamegrid[x][y].ent = NULL;}
			}
		}
    }

	//Clear arrays
	for(int i = 0; i < 100; i++){m_mushrooms[i].SetVisible(0);}
	m_totalmushrooms = 0;
	m_totalboxes = 0;
	m_totalbutterflys = 0;
	m_totalhats = 0;
	m_totalwells = 0;
	m_well[0].SetVisible(0);
	m_hat[0].SetVisible(0);

	//Vars
	Vector2D start_cell;
	Vector2D current_cell;
	Vector2D last_cell;
	int visited[17][11];

	//Set all cell pointers to 1 for unvisited
	for(int y = 0; y < 11; y++)
	{
		for(int x = 0; x < 17; x++)
		{
			visited[x][y] = 0;
		}
	}

	//Pick a random starting cell, set it as visited 1
	start_cell.Set(qRand(0, 17), qRand(0, 11));
	visited[start_cell.x][start_cell.y] = 1;
	last_cell.x = start_cell.x;
	last_cell.y = start_cell.y;

	int dir=0; //direction to check
	int lastdir=0; //last direction
	int checked[4]; //checked directions

loopsearch: //i'm going to hell

	checked[0]=0;
	checked[1]=0;
	checked[2]=0;
	checked[3]=0;

	//Choose a random direction to move to if the cell is unvisited, then place the borders around it
	do
	{
doloop: //there's no redemption for people like me
		current_cell.x = last_cell.x;
		current_cell.y = last_cell.y;
		dir = qRand(1, 5);
		if(dir == 1){current_cell.x--; checked[0] = 1;} //left
		if(dir == 2){current_cell.x++; checked[1] = 1;} //right
		if(dir == 3){current_cell.y--; checked[2] = 1;} //up
		if(dir == 4){current_cell.y++; checked[3] = 1;} //down

		if(current_cell.x < 0){goto doloop;}
		if(current_cell.y < 0){goto doloop;}
		if(current_cell.x > 16){goto doloop;}
		if(current_cell.y > 10){goto doloop;}

		//if no direction was found search all visited cells untill you find one with an accessable direction
		if(checked[0] == 1 && checked[1] == 1 && checked[2] == 1 && checked[3] == 1) //No unvisited direction avalible
		{
			//Find a new last cell (if there are no unvisited cells its complete)
			for(int y = 0; y < 11; y++)
			{
				for(int x = 0; x < 17; x++)
				{
					if(visited[x][y] == 0)
					{
						visited[x][y] = 1;
						last_cell.Set(x, y);
						goto loopsearch;
					}
				}
			}

			//Well Maze is pretty much done, add some entities
			AddMushrooms(10, 50, 1);
			AddMushrooms(1, 4, 4);
			for(int i = 0; i < qRand(0, 5); i++){AddButterfly(i);}
			AddEscapeMushroom(0, 0);

			//Place a blue bonus
			Vector2D p;
			p.Set(qRand(0, 17), qRand(0, 11));

			while(m_gamegrid[p.x][p.y].ent != NULL)
			{
				p.Set(qRand(0, 17), qRand(0, 11));
			}

			AddBlueBonus(p, qRand(100, 1000));

			//Give the player a wall destroy
			m_players[0].m_walldestroy = 1;

			return; //No unvisited cell found, done maze!
		}
	}
	while(visited[current_cell.x][current_cell.y] == 1);

	//Place our borders!
	if(lastdir == 3 || lastdir == 4)
	{
		if(lastdir == 3 && dir == 1) //If you turn 'left' while going 'up'
		{
			if(visited[last_cell.x+1][last_cell.y] == 0){if(last_cell.x+1 <= 16){AddBoxPos(last_cell.x+1, last_cell.y); visited[last_cell.x+1][last_cell.y] = 1;}}
			if(visited[last_cell.x+1][last_cell.y-1] == 0){if(last_cell.y-1 >= 0 && last_cell.x+1 <= 16){AddBoxPos(last_cell.x+1, last_cell.y-1); visited[last_cell.x+1][last_cell.y-1] = 1;}}
			if(visited[last_cell.x][last_cell.y-1] == 0){if(last_cell.y-1 >= 0){AddBoxPos(last_cell.x, last_cell.y-1); visited[last_cell.x][last_cell.y-1] = 1;}}
			goto dp;
		}

		if(lastdir == 3 && dir == 2) //If you turn 'right' while going 'up'
		{
			if(visited[last_cell.x-1][last_cell.y] == 0){if(last_cell.x-1 >= 0){AddBoxPos(last_cell.x-1, last_cell.y); visited[last_cell.x-1][last_cell.y] = 1;}}
			if(visited[last_cell.x-1][last_cell.y-1] == 0){if(last_cell.y-1 >= 0 && last_cell.x-1 >= 0){AddBoxPos(last_cell.x-1, last_cell.y-1); visited[last_cell.x-1][last_cell.y-1] = 1;}}
			if(visited[last_cell.x][last_cell.y-1] == 0){if(last_cell.y-1 >= 0){AddBoxPos(last_cell.x, last_cell.y-1); visited[last_cell.x][last_cell.y-1] = 1;}}
			goto dp;
		}

		if(lastdir == 4 && dir == 1) //If you turn 'left' while going 'down'
		{
			if(visited[last_cell.x+1][last_cell.y] == 0){if(last_cell.x+1 <= 16){AddBoxPos(last_cell.x+1, last_cell.y); visited[last_cell.x+1][last_cell.y] = 1;}}
			if(visited[last_cell.x+1][last_cell.y+1] == 0){if(last_cell.y+1 <= 10 && last_cell.x+1 <= 16){AddBoxPos(last_cell.x+1, last_cell.y+1); visited[last_cell.x+1][last_cell.y+1] = 1;}}
			if(visited[last_cell.x][last_cell.y+1] == 0){if(last_cell.y+1 <= 10){AddBoxPos(last_cell.x, last_cell.y+1); visited[last_cell.x][last_cell.y+1] = 1;}}
			goto dp;
		}

		if(lastdir == 4 && dir == 2) //If you turn 'right' while going 'down'
		{
			if(visited[last_cell.x-1][last_cell.y] == 0){if(last_cell.x-1 >= 0){AddBoxPos(last_cell.x-1, last_cell.y); visited[last_cell.x-1][last_cell.y] = 1;}}
			if(visited[last_cell.x-1][last_cell.y+1] == 0){if(last_cell.y+1 <= 10 && last_cell.x-1 >= 0){AddBoxPos(last_cell.x-1, last_cell.y+1); visited[last_cell.x-1][last_cell.y+1] = 1;}}
			if(visited[last_cell.x][last_cell.y+1] == 0){if(last_cell.y+1 <= 10){AddBoxPos(last_cell.x, last_cell.y+1); visited[last_cell.x][last_cell.y+1] = 1;}}
			goto dp;
		}

		if(dir == 3 || dir == 4) //if direction is up or down, place borders left and right around last_cell
		{
			if(visited[last_cell.x-1][last_cell.y] == 0){if(last_cell.x-1 >= 0){AddBoxPos(last_cell.x-1, last_cell.y); visited[last_cell.x-1][last_cell.y] = 1;}}
			if(visited[last_cell.x+1][last_cell.y] == 0){if(last_cell.x+1 <= 16){AddBoxPos(last_cell.x+1, last_cell.y); visited[last_cell.x+1][last_cell.y] = 1;}}
			//goto dp;
		}

	}

	if(lastdir == 1 || lastdir == 2)
	{
		if(lastdir == 1 && dir == 3) //If you turn 'up' while going 'left'
		{
			if(visited[last_cell.x][last_cell.y+1]){if(last_cell.y+1 <= 10){AddBoxPos(last_cell.x, last_cell.y+1); visited[last_cell.x][last_cell.y+1] = 1;}}
			if(visited[last_cell.x-1][last_cell.y+1] == 0){if(last_cell.y+1 <= 10 && last_cell.x-1 >= 0){AddBoxPos(last_cell.x-1, last_cell.y+1); visited[last_cell.x-1][last_cell.y+1] = 1;}}
			if(visited[last_cell.x-1][last_cell.y] == 0){if(last_cell.x-1 >= 0){AddBoxPos(last_cell.x-1, last_cell.y); visited[last_cell.x-1][last_cell.y] = 1;}}
			goto dp;
		}

		if(lastdir == 1 && dir == 4) //If you turn 'down' while going 'left'
		{
			if(visited[last_cell.x-1][last_cell.y] == 0){if(last_cell.x-1 >= 0){AddBoxPos(last_cell.x-1, last_cell.y); visited[last_cell.x-1][last_cell.y] = 1;}}
			if(visited[last_cell.x][last_cell.y-1]){if(last_cell.y-1 >= 0){AddBoxPos(last_cell.x, last_cell.y-1); visited[last_cell.x][last_cell.y-1] = 1;}}
			if(visited[last_cell.x-1][last_cell.y-1] == 0){if(last_cell.y-1 >= 0 && last_cell.x-1 >= 0){AddBoxPos(last_cell.x-1, last_cell.y-1); visited[last_cell.x-1][last_cell.y-1] = 1;}}
			goto dp;
		}

		if(lastdir == 2 && dir == 3) //If you turn 'up' while going 'right'
		{
			if(visited[last_cell.x+1][last_cell.y] == 0){if(last_cell.x+1 <= 16){AddBoxPos(last_cell.x+1, last_cell.y); visited[last_cell.x+1][last_cell.y] = 1;}}
			if(visited[last_cell.x+1][last_cell.y+1] == 0){if(last_cell.y+1 <= 10 && last_cell.x+1 <= 16){AddBoxPos(last_cell.x+1, last_cell.y+1); visited[last_cell.x+1][last_cell.y+1] = 1;}}
			if(visited[last_cell.x][last_cell.y+1]){if(last_cell.y+1 <= 10){AddBoxPos(last_cell.x, last_cell.y+1); visited[last_cell.x][last_cell.y+1] = 1;}}
			goto dp;
		}

		if(lastdir == 2 && dir == 4) //If you turn 'down' while going 'right'
		{
			if(visited[last_cell.x+1][last_cell.y-1] == 0){if(last_cell.y-1 >= 0 && last_cell.x+1 <= 16){AddBoxPos(last_cell.x+1, last_cell.y-1); visited[last_cell.x+1][last_cell.y-1] = 1;}}
			if(visited[last_cell.x+1][last_cell.y] == 0){if(last_cell.x+1 <= 16){AddBoxPos(last_cell.x+1, last_cell.y); visited[last_cell.x+1][last_cell.y] = 1;}}
			if(visited[last_cell.x][last_cell.y-1]){if(last_cell.y-1 >= 0){AddBoxPos(last_cell.x, last_cell.y-1); visited[last_cell.x][last_cell.y-1] = 1;}}
			goto dp;
		}

		if(dir == 1 || dir == 2) //if direction is left or right, place borders up and down around last_cell
		{
			if(visited[last_cell.x][last_cell.y-1]){if(last_cell.y-1 >= 0){AddBoxPos(last_cell.x, last_cell.y-1); visited[last_cell.x][last_cell.y-1] = 1;}}
			if(visited[last_cell.x][last_cell.y+1]){if(last_cell.y+1 <= 10){AddBoxPos(last_cell.x, last_cell.y+1); visited[last_cell.x][last_cell.y+1] = 1;}}
			//goto dp;
		}
	}

dp: //just feed me to the pigs already

	//Update last cell and direction
	last_cell.x = current_cell.x;
	last_cell.y = current_cell.y;
	lastdir = dir;

	//Set current sell as visited
	visited[current_cell.x][current_cell.y] = 1;

	//Loop the search
	goto loopsearch;
}





