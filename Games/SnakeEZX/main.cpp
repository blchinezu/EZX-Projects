#define ST_STARTING_LEVEL 1
#define ST_PLAYING 2
#define ST_PAUSE 3
#define ST_LOST_LIFE 4
#define ST_LOST_LAST_LIFE 5
#define ST_EXIT_REQUESTED 6
#define ST_MENU 7
#define ST_OPTIONS 8
#define ST_HELP 9

#include "includes.h"
#include <iostream>

int main(int argc, char* argv[])
{	
	std::cout<<"Starting Snake v1.1 [EZX]...\n Author:  Schan\n Porting: BruceLee\n\n";
	// some inits...
	srand(time(0));
	if (TTF_Init() > 0) {
		fprintf(stderr, "Unable to TTF SDL: %s\n", TTF_GetError());
		exit(1);
	}
	
	// create game 
	CGameEngine *game = new CGameEngine(30,false,10);
	game->SetTitle("Loading...");
	game->Init();

	SDL_ShowCursor(false);

	int lang = 0;
	// load options
	FILE *fp = fopen("snake.cfg","r");
	if (fp!=NULL)
	{
		char str[32];
		int v;
		fscanf(fp,"%s %d",&str,&v);
		if (strcmp(str,"lang")==0)
		{
			lang = v;
		}
		fclose(fp);
	}	


	// create info object.
	// need game->Init(). actually, need a video mode set (made in game->Init());
	CSnakeGameInfo *info = new CSnakeGameInfo(lang);

				
	// create and add states. need info created
	CGameState *startingLevelState = new CStartingLevelState(info);
	CGameState *playingState = new CPlayingState(info);
	CGameState *pauseState = new CPauseState(info);
	CGameState *exitState = new CExitRequestedState(info);
	CGameState *lostLifeState = new CLostLifeState(info);
	CGameState *lostLastLifeState = new CLostLastLifeState(info);
	CGameState *menuState = new CMenuState(info);
	CGameState *optionsState = new COptionsState(info);
	CGameState *helpState = new CHelpState(info);
	
	game->AddState(startingLevelState,ST_STARTING_LEVEL);
	game->AddState(playingState,ST_PLAYING);
	game->AddState(pauseState,ST_PAUSE);
	game->AddState(lostLifeState,ST_LOST_LIFE);
	game->AddState(lostLastLifeState,ST_LOST_LAST_LIFE);
	game->AddState(exitState,ST_EXIT_REQUESTED);
	game->AddState(menuState,ST_MENU);
	game->AddState(optionsState,ST_OPTIONS);
	game->AddState(helpState,ST_HELP);
	
	// Show Splash
	ShowSplash();
	
	// start game
	game->SetTitle("Snake!");
	game->Start(ST_MENU);
	game->SetTitle("Quiting...");

	// clean
	delete startingLevelState;
	delete playingState;
	delete pauseState;
	delete exitState;
	delete lostLifeState;
	delete lostLastLifeState;
	delete menuState;
	delete optionsState;
	delete helpState;
/*	chdir("/usr/gp2x");
  	execl("/usr/gp2x/gp2xmenu", "/usr/gp2x/gp2xmenu", NULL);*/
	return 0;

}
