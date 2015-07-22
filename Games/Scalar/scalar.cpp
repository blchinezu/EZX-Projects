//-----------------------------------------------------------------------------
//	Filename:	scalar.cpp
//	Created:	14. Nov 2003. by Milan Babuskov
//
//  Purpose:	Initialization of the game
//				Managing resources
//				Handling Main Menu
//
//  Tabs should be at 4 spaces.
//  Each section is separated with line: //-------...
/*-----------------------------------------------------------------------------
Copyright 2003-2004 Milan Babuskov

This file is part of Scalar (http://scalar.sourceforge.net).

Scalar is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

Scalar is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Scalar in file COPYING; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
-----------------------------------------------------------------------------*/
// needed for chdir
#ifdef __linux__
#include <unistd.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include "SDL.h"
//#include "SDL_mixer.h"
#include "SDL_thread.h"
#include "SDL_image.h"
#include "scalar.h"
#include "soft-keyboard.h"

#define SCALAR_VERSION "1.02 EZX"
//-----------------------------------------------------------------------------

char last_image[2000];

int main(int argc, char *argv[])
{
 	bool Fullscreen = false;
	bool UseDGA = false;
 	if (argc > 1)
 	{
 		for (int i=1; i<argc; i++)
	 	{
	 		bool ok = true;
	 		if (argv[i][0] == '-')
			{
				if (argv[i][1] == 'f')
					Fullscreen = true;	// use -f windowed mode
				else if (argv[i][1] == 'd')
					UseDGA = true;
				else
					ok = false;
			}
			else
				ok = false;

			if (!ok)
			{
				printf("Usage:  scalar -[f|d]\n\n");
				printf("-f  start in Fullscreen.\n");;
				printf("-d  use DGA driver on Linux, (much faster, but must run as root).\n");
				return 1;
			}
		}
	}

	// cd to game directory
#ifdef __linux__
	int i=0, l=0;
	while (argv[0][i])
	{
		if (argv[0][i] == '/')
			l = i;
		i++;
	}

	if (l)
	{
		argv[0][l] = '\0';
		chdir(argv[0]);
		argv[0][l] = '/';
	}
#endif

	ScalarEngine Engine;
	if (!Engine.Init(Fullscreen, UseDGA))
	{
		LogFile("Failed.\n");
		return 1;
	}

	try
	{
		Engine.Start();
	}
	catch (...)
	{
		LogFile("Exception caught\n");
	}
	return 0;
}
//-----------------------------------------------------------------------------
ScalarEngine::ScalarEngine()
{
 	m_Screen = NULL;
 	m_FontBlue = NULL;
 	m_FontYellow = NULL;
//	m_AudioAvailable = false;
	m_SDL = false;

 	m_Icon = NULL;
	m_PuzzleImage = NULL;
	m_Background = NULL;
	m_SmallImage = NULL;
	m_Toolbar = NULL;

//    m_MainMenuMusic = NULL;

	m_GridOn = true;
	m_FirstPiece = 0;
}
//-----------------------------------------------------------------------------
void ScalarEngine::FreePieces()
{
	while (m_FirstPiece)
	{
		Piece *temp = m_FirstPiece;
		m_FirstPiece = m_FirstPiece->next;
		delete temp;
	}
}
//-----------------------------------------------------------------------------
ScalarEngine::~ScalarEngine()
{
	LogFile("Closing the game ... ");
#ifdef __linux__
	char bufsdl[30];
	sprintf(bufsdl, "SDL_VIDEODRIVER=%s", linux_sdl_driver);
	putenv(bufsdl);
#endif

	FreePieces();
	
 	SK::Quit();

	if (m_Icon)				SDL_FreeSurface(m_Icon);
	if (m_PuzzleImage)		SDL_FreeSurface(m_PuzzleImage);
	if (m_LogoImage)		SDL_FreeSurface(m_LogoImage);
	if (m_Background)		SDL_FreeSurface(m_Background);
	if (m_SmallImage)		SDL_FreeSurface(m_SmallImage);
	if (m_Toolbar)			SDL_FreeSurface(m_Toolbar);

 	if (m_FontBlue)			delete m_FontBlue;
 	if (m_FontYellow)		delete m_FontYellow;

//	if (m_MainMenuMusic)	Mix_FreeMusic(m_MainMenuMusic);

// 	if (m_AudioAvailable)	Mix_CloseAudio();

	if (m_SDL)
		SDL_Quit();

	LogFile("OK.\n");
}
//---------------------------------------------------------------------------
bool ScalarEngine::Init(bool Fullscreen, bool UseDGA)
{
#ifdef __linux__
	sprintf(linux_sdl_driver, "x11\0");
	char *driver_name = getenv("SDL_VIDEODRIVER");
	if (driver_name)
		sprintf(linux_sdl_driver, "%s\0", driver_name);

	if (UseDGA)
	{
		printf("You must be root to run DGA\nType ./njam -? for help\n");
		putenv("SDL_VIDEODRIVER=dga");
	}
#else
	if (UseDGA)
		printf("Parameter -d is ignored for non-Linux platforms.\n");
#endif

	if (!countImages())
		return false;

 	LogFile("Initializing SDL: VIDEO & TIMER ... ", true);
	if ( SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER ) < 0 )
	{
		fprintf(stderr, "Error initializing SDL: %s\n", SDL_GetError());
		return false;
	}
	else
        m_SDL = true;
// Remove comment if you want window title and icon
/*	LogFile("OK.\nSetting window title ... ");
	SDL_WM_SetCaption("Scalar " SCALAR_VERSION "    http://scalar.sourceforge.net", NULL);

 	// load .bmp for icon
 	LogFile("done.\nLoading icon ... ");
 	m_Icon = SDL_LoadBMP("data/scalaricon.bmp");
 	if (!m_Icon)
 	{
 		LogFile("FAILED.\n");
		LogFile((const char *)SDL_GetError());
 		return false;
 	}
 	LogFile("OK.\n");
	SDL_WM_SetIcon(m_Icon, NULL);
*/
	LogFile("\nSetting video mode: 320x240x32 ... ");
	Uint32 flags = SDL_ANYFORMAT | SDL_SWSURFACE;

	if (Fullscreen)
		flags |= SDL_FULLSCREEN;

	m_Screen = SDL_SetVideoMode(320, 240, 32, flags);
    if ( m_Screen == NULL )
	{
        fprintf(stderr, "Couldn't set 320x240x32 video mode: %s\n", SDL_GetError());
        return false;
    }
	else
		LogFile("OK\n");

	// check & output capabilites:
	const SDL_VideoInfo *info = SDL_GetVideoInfo();
	printf("VIDEO INFORMATION:\n");
	printf("  hw_available = %d\n", info->hw_available);
	printf("  wm_available = %d\n", info->wm_available);
	printf("  blit_hw = %d\n", info->blit_hw);
	printf("  blit_hw_CC = %d\n", info->blit_hw_CC);
	printf("  blit_hw_A = %d\n", info->blit_hw_A);
	printf("  blit_sw = %d\n", info->blit_sw);
	printf("  blit_sw_CC = %d\n", info->blit_sw_CC);
	printf("  blit_sw_A = %d\n", info->blit_sw_A);
	printf("END VIDEO INFORMATION.\n");

	// Loading various resources...
	LogFile("Loading resources:\n");

	if (!NjamLoad(&m_Background, "data/back.png"))
		return false;

	if (!NjamLoad(&m_SmallImage, "data/small.png"))
		return false;

	if (!NjamLoad(&m_LogoImage, "data/logo.png"))
		return false;

	if (!NjamLoad(&m_Toolbar, "data/toolbar.png"))
		return false;

	LogFile("Loading Fonts:\n");
	m_FontBlue = new NjamFont("data/font-blue.png", 7, 10);
	m_FontYellow = new NjamFont("data/font-uni.png", 7, 10);

/*    // open 44.1KHz, signed 16bit, system byte order, stereo audio, using 1024 byte chunks
    LogFile("Opening audio ... ");
    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 1, 2048) == -1)
	{
		LogFile("Failed.\n");
        printf("Mix_OpenAudio: %s\n", Mix_GetError());
 	}
 	else
 	{
 		m_AudioAvailable = true;
 		LogFile("OK.\n");

		printf("Reserving 2 channels for sfx ... \n");
		int reserved = Mix_ReserveChannels(2);
		if(reserved != 2)
		{
		    printf("Reserved %d channels from default mixing.\n",reserved);
		    printf("2 channels were not reserved!\n");
		}

	   	printf("Loading song: data/moondan.mod ... \n");
	    m_MainMenuMusic = Mix_LoadMUS("data/moondan.mod");;
	    if(!m_MainMenuMusic)
	    	printf("ERROR: Mix_LoadMUS(): %s\n", Mix_GetError());
 	}*/
 	LogFile("Initializing Soft-Keyboard ... \n");
 	SK::Init();
 	return true;
}
//-----------------------------------------------------------------------------
void ScalarEngine::ShufflePieces()
{
	int pieces=0;
	for (Piece *temp = m_FirstPiece; temp; temp = temp->next)
		pieces++;

	for (Piece *temp = m_FirstPiece; temp; temp = temp->next)
	{
		// get random piece
		Piece *t = m_FirstPiece;
		for (int p = NjamRandom(pieces); p && t; p--)
			t = t->next;

		if (t && t != temp)
			Switch(temp, t);
	}
}
//-----------------------------------------------------------------------------
void ScalarEngine::CreatePieces(Piece *parent)
{
	const int minsize = 28; // fost 70

	if (!parent)	// create first piece
	{
		m_FirstPiece = new Piece;
		m_FirstPiece->next = 0;
		SDL_Rect *r = &m_FirstPiece->grid_rect;
		r->x = m_XOffset;
		r->y = m_YOffset;
		r->w = m_ImageW;
		r->h = m_ImageH;
		CreatePieces(m_FirstPiece);

		// after all pieces are created set 'em up
		for (Piece *temp = m_FirstPiece; temp; temp = temp->next)
		{
			SDL_Rect *r = &temp->grid_rect;
			NjamSetRectx(temp->image_rect, r->x - m_XOffset, r->y - m_YOffset, r->w, r->h);
		}
		return;
	}

	int w = parent->grid_rect.w;
	int h = parent->grid_rect.h;

	if (w < minsize * 2  ||  h < minsize * 2)
		return;

	bool vertical = (w > h);

	// sometimes don't split
	if (NjamRandom(3) == 1 && w*h < 50000 && parent != m_FirstPiece) // fost 50000
		return;

	// split into 2 pieces, call recursive for each
	Piece *temp = new Piece;
	temp->next = m_FirstPiece;	// stack it
	m_FirstPiece = temp;
	if (vertical)
	{
		int base = w - 2 * minsize;
		int size = NjamRandom(base) + minsize;

		temp->grid_rect.h = h;
		temp->grid_rect.y = parent->grid_rect.y;

		temp->grid_rect.w = w - size;
		temp->grid_rect.x = parent->grid_rect.x + size;
		parent->grid_rect.w = size;
	}
	else
	{
		int base = h - 2 * minsize;
		int size = NjamRandom(base) + minsize;

		temp->grid_rect.w = w;
		temp->grid_rect.x = parent->grid_rect.x;

		temp->grid_rect.h = h - size;
		temp->grid_rect.y = parent->grid_rect.y + size;
		parent->grid_rect.h = size;
	}
	
	CreatePieces(parent);
	CreatePieces(temp);
}
//-----------------------------------------------------------------------------
void ScalarEngine::StrechDraw(SDL_Rect *src, SDL_Rect *dest, bool Darken)
{
	SDL_Surface *surface = m_PuzzleImage;
	if ( SDL_MUSTLOCK(surface) && SDL_LockSurface(surface) < 0 )
	{
		fprintf(stderr, "Can't lock surface: %s\n", SDL_GetError());
		return;
	}

	if ( SDL_MUSTLOCK(m_Screen) && SDL_LockSurface(m_Screen) < 0 )
	{
		fprintf(stderr, "Can't lock m_Screen: %s\n", SDL_GetError());
		return;
	}

	double kx = (double)(src->w-1)/(double)(dest->w-1);
	double ky = (double)(src->h-1)/(double)(dest->h-1);
	double sx = src->x;
	double sy = src->y;

	for (double x=0; x < dest->w; x++)
	{
		for (double y=0; y < dest->h; y++)
		{
			// get pixel that matches in source
			Uint32 pix = GetPixel(surface, (int)(sx + x * kx), (int)(sy + y * ky));

			if (Darken)	// get half intensity
			{
				Uint8 r, g, b;
				SDL_GetRGB(pix, surface->format, &r, &g, &b);
				pix = SDL_MapRGB(m_Screen->format, r/2, g/2, b/2);
			}

			// paint it
			PutPixel(m_Screen, (int)(dest->x + x), (int)(dest->y + y), pix);
		}
	}

    if ( SDL_MUSTLOCK(surface) )
        SDL_UnlockSurface(surface);

    if ( SDL_MUSTLOCK(m_Screen) )
        SDL_UnlockSurface(m_Screen);
}
//-----------------------------------------------------------------------------
void ScalarEngine::Draw(bool WithBackground)
{
	if (WithBackground)
		PatternFill(m_Background, m_Screen);	// draw background
	else
		SDL_FillRect(m_Screen, 0, SDL_MapRGB(m_Screen->format, 255, 255, 255));	// draw white

	// draw border
	SDL_Rect r;
	NjamSetRect(r, 0, 0, 800, 50);
	SDL_FillRect(m_Screen, &r, 0);

	// draw toolbar
	NjamSetRect(r, 10, 10);
	SDL_BlitSurface(m_Toolbar, NULL, m_Screen, &r);

	// more border
	for (int i=0; i<800; i+=10)
	{
		NjamSetRect(r, i, 0);
		SDL_BlitSurface(m_SmallImage, NULL, m_Screen, &r);
		NjamSetRect(r, i, 40);
		SDL_BlitSurface(m_SmallImage, NULL, m_Screen, &r);
		NjamSetRect(r, i, 590);
		SDL_BlitSurface(m_SmallImage, NULL, m_Screen, &r);
	}

	for (int i=0; i<600; i+=10)
	{
		NjamSetRect(r, 0, i);
		SDL_BlitSurface(m_SmallImage, NULL, m_Screen, &r);
		NjamSetRect(r, 790, i);
		SDL_BlitSurface(m_SmallImage, NULL, m_Screen, &r);
	}

	char buff[90];
	sprintf(buff, "SWAPS LEFT: %03d", m_Clicks);
	for(int i=11; i<strlen(buff); i++) if(buff[i]=='-') buff[i]='0';
	m_FontYellow->WriteTextXY(m_Screen, 515, 13, buff);

	if (WithBackground && current_level > -1)
	{
		sprintf(buff, "POINTS: %d", current_points);
		m_FontYellow->WriteTextXY(m_Screen, 200, 13, buff);
	}

	if (WithBackground)
	{
		Uint32 white = SDL_MapRGB(m_Screen->format, 255, 255, 255);
		Uint32 yellow = SDL_MapRGB(m_Screen->format, 255, 255, 0);

		// draw puzzle
		Piece *temp = m_FirstPiece;
		while (temp)
		{
			SDL_Rect *grid, *image, src, dest;
			grid = &(temp->grid_rect);
			image = &(temp->image_rect);

			int bx=grid->x, by=grid->y, bw=grid->w, bh=grid->h;
/*			grid->x/=2.5;
			grid->y/=2.5;
			grid->w/=2.5;
			grid->h/=2.5;
*/
			if (m_GridOn || m_SelectedPiece == temp)
			{
				if (m_SelectedPiece == temp)
					SDL_FillRect(m_Screen, grid, yellow);
				else
					SDL_FillRect(m_Screen, grid, white);

				NjamSetRectx(src, image->x+1, image->y+1, image->w-2, image->h-2);
				NjamSetRectx(dest, grid->x+1,  grid->y+1,  grid->w-2, grid->h-2);
				StrechDraw(&src, &dest, m_SelectedPiece == temp);
			}
			else
				StrechDraw(image, grid);
/*
			grid->x=bx;
			grid->y=by;
			grid->w=bw;
			grid->h=bh;
*/
			temp = temp->next;
		}
	}
}
//-----------------------------------------------------------------------------
void ScalarEngine::Switch(Piece *one, Piece *two)
{
	SDL_Rect tmp;
	NjamCopyRect(one->image_rect, tmp);
	NjamCopyRect(two->image_rect, one->image_rect);
	NjamCopyRect(tmp, two->image_rect);
}
//-----------------------------------------------------------------------------
Piece *ScalarEngine::FindPiece(int x, int y)
{
	for (Piece *temp = m_FirstPiece; temp; temp=temp->next)
	{
		SDL_Rect *r = &temp->grid_rect;
		if (r->x <= x && r->x + r->w > x && r->y <= y && r->y + r->h > y)
			return temp;
	}

	return 0;
}
//-----------------------------------------------------------------------------
void ScalarEngine::Message(char *text)
{
	int l=strlen(text);

	// render background
	Uint32 FillColor;
	SDL_Rect dest;
	FillColor = SDL_MapRGB(m_Screen->format, 0, 0, 0);
	NjamSetRect(dest, (800-l*(7*2.5)-50)/2, 275, l*(7*2.5)+50, 55);
	SDL_FillRect(m_Screen, &dest, FillColor);

	FillColor = SDL_MapRGB(m_Screen->format, 180, 0, 0);
	NjamSetRect(dest, (800-l*(7*2.5)-30)/2, 285, l*(7*2.5)+30, 35);
	SDL_FillRect(m_Screen, &dest, FillColor);

	m_FontYellow->WriteTextXY(m_Screen, (800-l*(7*2.5))/2, 290, text);
	SDL_Flip(m_Screen);

	int x, y;
	GetClick(x, y);
}
//-----------------------------------------------------------------------------
void ScalarEngine::NewGame()
{
	FreePieces();
	CreatePieces(0);
	m_SelectedPiece = 0;
	ShufflePieces();
	m_Clicks = 0;
	for (Piece *temp = m_FirstPiece; temp; temp = temp->next)
		m_Clicks++;
	m_Clicks = (int)((double)m_Clicks * 1.4);
}
//---------------------------------------------------------------------------
bool ScalarEngine::countImages()
{
	// open folder with images
	DIR *dir;
	if ((dir = opendir("images")) == NULL)
	{
		printf("Unable to open image directory.\n");
		return false;
	}

	m_ImageCount = 0;
	struct dirent *ent;
	while ((ent = readdir(dir)) != NULL)
	{
		if (ent->d_name[0] == '.')	// skip entries that begin with .
			continue;
		m_ImageCount++;
	}

	if (dir && closedir(dir) != 0)
	{
		printf("Unable to close directory\n");
		return false;
	}

	if (m_ImageCount < 3)
	{
		printf("Not enought images, need at least 3.\n");
		return false;
	}

	return true;
}
//-----------------------------------------------------------------------------
//! sets up the new picture
bool ScalarEngine::SetupGame()
{
	if (current_level == -1)
	{
		printf("WARNING: current_level == -1\n");
		return false;
	}
	m_LevelComplete = false;
	current_try = 1;
	if (m_PuzzleImage)
		SDL_FreeSurface(m_PuzzleImage);

	m_PuzzleImage = 0;

	// open folder with images
	DIR *dir;
	if ((dir = opendir("images")) == NULL)
	{
		printf("Unable to open image directory.\n");
		return false;
	}

	int c = 0;
	// load next image
	struct dirent *ent;
	while ((ent = readdir(dir)) != NULL)
	{
		if (ent->d_name[0] == '.') continue;		// skip entries that begin with .
		
		char buffer[2000]; sprintf(buffer, "%s/%s", "images", ent->d_name);
		if (!strcmp(last_image, buffer)) continue;	// skip previous loaded image
		
		if (c == m_ImageToPlay[current_level])
		{
			sprintf(last_image, "%s/%s", "images", ent->d_name);
			if (!NjamLoad(&m_PuzzleImage, buffer))
				m_PuzzleImage = 0;
			break;
		}
		c++;
	}

	if (closedir(dir) != 0)
	{
		printf("Unable to close directory\n");
		return false;
	}

	if (m_PuzzleImage == 0)
		return false;

	// scale image if too big
	double w = m_PuzzleImage->w;
	double h = m_PuzzleImage->h;
	if (w > 300 || h > 204)
	{
		double vk = w / (double)300;
		double hk = h / (double)204;
		double k = (vk > hk ? vk : hk);

		// reduce image by factor k
		SDL_Surface *temp = CreateSurface(int(w/k), int(h/k));
		if (!temp)
			return false;

		CopySurfaceStreched(m_PuzzleImage, temp);
		SDL_FreeSurface(m_PuzzleImage);
		m_PuzzleImage = SDL_DisplayFormat(temp);
		if (!m_PuzzleImage)
			return false;
	}

	m_ImageW = m_PuzzleImage->w;
	m_ImageH = m_PuzzleImage->h;
	m_XOffset = 4 + (312 - m_ImageW)/2;
	m_YOffset = 20 + (216 - m_ImageH)/2;

	NewGame();
	return true;
}
//-----------------------------------------------------------------------------
bool ScalarEngine::CheckVictory()
{
	for (Piece *temp = m_FirstPiece; temp; temp=temp->next)
	{
		SDL_Rect *g = &temp->grid_rect;
		SDL_Rect *i = &temp->image_rect;
		if (g->w != i->w || g->h != i->h || g->x - m_XOffset != i->x || g->y - m_YOffset != i->y)
			return false;
	}
	return true;
}
//-----------------------------------------------------------------------------
void ScalarEngine::DrawHelp()
{
	Draw(false);

	SDL_Rect r;
	NjamSetRect(r, 20, 60);
	SDL_BlitSurface(m_LogoImage, 0, m_Screen, &r);
	m_FontYellow->WriteTextXY(m_Screen, 200, 13, "V" SCALAR_VERSION);
	m_FontBlue->WriteTextColRow(m_Screen, 13, 3, "created by Milan Babuskov");
	m_FontBlue->WriteTextColRow(m_Screen, 13, 4, "ported by BruceLee");

	// welcome message
	m_FontBlue->WriteTextColRow(m_Screen, 3, 6, "The goal of the game is to assemble the");
	m_FontBlue->WriteTextColRow(m_Screen, 2, 7, "picture from randomly shuffled pieces.");
	m_FontBlue->WriteTextColRow(m_Screen, 2, 8, "Move the pieces by swapping their");
	m_FontBlue->WriteTextColRow(m_Screen, 2, 9, "positions. Number of swappings is limited");
	m_FontBlue->WriteTextColRow(m_Screen, 2, 10, "for each level. Swap the pieces by");
	m_FontBlue->WriteTextColRow(m_Screen, 2, 11, "clicking on them. Cancel the swap by");
	m_FontBlue->WriteTextColRow(m_Screen, 2, 12, "clicking on the same piece twice.");

	m_FontBlue->WriteTextColRow(m_Screen, 3,14, "In each game you should assemble three");
	m_FontBlue->WriteTextColRow(m_Screen, 2,15, "images. For each image you get as many");
	m_FontBlue->WriteTextColRow(m_Screen, 2,16, "points as swappings you have got left,");
	m_FontBlue->WriteTextColRow(m_Screen, 2,17, "plus the following bonuses:");
	
	m_FontBlue->WriteTextColRow(m_Screen, 2,19, "30 points for level done in first try");
	m_FontBlue->WriteTextColRow(m_Screen, 2,20, "10 points for level done in second try");

	m_FontBlue->WriteTextColRow(m_Screen, 3,22, "Click anywhere on the screen to continue");

	SDL_Flip(m_Screen);
}
//-----------------------------------------------------------------------------
void ScalarEngine::DrawHiscore()
{
	Draw(false);

	SDL_Rect r;
	NjamSetRect(r, 20, 60);
	SDL_BlitSurface(m_LogoImage, 0, m_Screen, &r);
	m_FontBlue->WriteTextColRow(m_Screen, 18, 3, "* TOP 10 PLAYERS *");

	// welcome message
	m_FontBlue->WriteTextColRow(m_Screen, 5, 7, "Name             Swaps      Points");
	char buff[80];
	for (int i=0; i<10; ++i)
	{
		sprintf(buff, "%-15s  %5d       %5d", m_hiscore.names[i], m_hiscore.swaps[i], m_hiscore.points[i]);
		m_FontBlue->WriteTextColRow(m_Screen, 5, 8+i, buff);
	}

	sprintf(buff, "Your score: %d", current_points);
	m_FontBlue->WriteTextColRow(m_Screen, 15,19, buff);
	m_FontBlue->WriteTextColRow(m_Screen, 3,22, "Click anywhere on the screen to continue");
	SDL_Flip(m_Screen);
}
//-----------------------------------------------------------------------------
void ScalarEngine::Start()
{
	char InfoText[6][30] = { "HIGHSCORES", "EXIT", "NEXT LEVEL", "RESTART", "HELP", "GRID ON/OFF" };

 	// play main menu .mod
// 	if (m_AudioAvailable && Mix_PlayMusic(m_MainMenuMusic, -1) == -1)
//        printf("ERROR: scalar.cpp: Mix_PlayMusic: %s\n", Mix_GetError());

	m_Clicks = 0;
	current_level = -1;
	current_points = current_swaps = 0;

	bool HelpOn = true;
	DrawHelp();
	m_LevelComplete = true;		// so that the player can start "next" level

	bool Redraw = false;
	int LastInfo = -1;
	while (true)	// loop main menu
	{
		// handle a click
		int x, y;
		int mouse = GetClick(x, y, false);
		x*=2.5;
		y*=2.5;
		if(mouse == 4) {	// Grid Toggle Key
							m_GridOn=!m_GridOn;
							Redraw = true;
							continue;
						} else
		if(mouse == 5) {	// Restart Key
							Message("GAME OVER. CLICK TO START A NEW GAME.");
							current_level = -1;
							ClickOnPlay();
							Redraw = true;
							continue;
						} else
		if(mouse == 6) {	// Next Level Key
							ClickOnPlay();
							Redraw = true;
							continue;
						}
		if (mouse > 0)
			Redraw = true;
		else
		{
			int option = -1;
			if (y > 10 && y < 40)
			{
				option = (x - 10) / 30;
				if (option < 0 || option > 5)
					option = -1;
			}

			if (option != LastInfo)
			{
				LastInfo = option;

				SDL_Rect r;				// DRAW OPTION
				NjamSetRect(r, 190, 10, 300, 30);
				SDL_FillRect(m_Screen, &r, 0);
				if (LastInfo > -1)
					m_FontYellow->WriteTextXY(m_Screen, 200, 13, InfoText[LastInfo]);
				else if (current_level > -1)
				{
					char buff[80];
					sprintf(buff, "POINTS: %d", current_points);
					m_FontYellow->WriteTextXY(m_Screen, 200, 13, buff);
				}
				SDL_Flip(m_Screen);
				SDL_Delay(10);
			}
		}

		if (mouse == 3)	// right button
		{
			// LATER: use this to flip?
			// return;
		}
		else if (mouse == 1)	// left button
		{
			if (y > 10 && y < 40)
			{
				int option = (x - 10) / 30;

				if (option != toHelp)
					HelpOn = false;
				switch (option)
				{
					case toHiscore:
						HelpOn = !HelpOn;
						if (HelpOn)
						{
							Redraw = false;
							DrawHiscore();
						}
						break;
					case toExit:
						return;
					case toPlay:
						ClickOnPlay();
						break;
					case toRepeat:
						//NewGame();		// <- old "restart level" code
						//Redraw = true;
						Message("GAME OVER. CLICK TO START A NEW GAME.");
						current_level = -1;
						ClickOnPlay();
						break;
					case toHelp:
						HelpOn = !HelpOn;
						if (HelpOn)
						{
							Redraw = false;
							DrawHelp();
						}
						break;
					case toGrid:
						m_GridOn = !m_GridOn;		break;
					default:
						break;
				}
				m_SelectedPiece = 0;
			}
			else if (HelpOn)
			{
				HelpOn = false;
			}
			else if(current_level!=-1)
			{
				Piece *p = FindPiece(x/2.5, y/2.5);
				if (p)
				{
					if (m_SelectedPiece == 0)
						m_SelectedPiece = p;
					else if (p == m_SelectedPiece)
						m_SelectedPiece = 0;
					else
					{
						current_swaps++;
						m_Clicks--;
						Switch(p, m_SelectedPiece);
						m_SelectedPiece = 0;
						if (CheckVictory())
						{
							bool t = m_GridOn;
							m_GridOn = false;
							Draw();
							Message("WELL DONE!");
							m_LevelComplete = true;
							m_GridOn = t;

							current_points += m_Clicks;
							if (current_try == 1)
							{
								Message("30 BONUS POINTS FOR FIRST TRY");
								current_points += 30;
							}
							if (current_try == 2)
							{
								Message("10 BONUS POINTS FOR SECOND TRY");
								current_points += 10;
							}
							current_level++;
							if (current_level == 3)
							{
								if (current_points > m_hiscore.points[9] ||
									current_points == m_hiscore.points[9] && current_swaps <= m_hiscore.swaps[9])
								{
									char *name = m_hiscore.enterHiscore(current_points, current_swaps);
									getName(name);	// show: well done screen, and ask for name
									HelpOn = true;
									if (HelpOn)
									{
										Redraw = false;
										DrawHiscore();
									}
								}
								else
									Message("GAME OVER");
								current_level = -1;
							}
							SetupGame();
						}
						else if (m_Clicks == 0)
						{
							Draw();
							Message("YOU FAILED!  THE LEVEL WILL RESTART.");
							current_try++;
							SDL_Flip(m_Screen);
							NewGame();
							Redraw = true;
						}
					}
				}
			}
		}

		// draw screen
		if (Redraw)
		{
			Draw();
			SDL_Flip(m_Screen);
			Redraw = false;
		}
	}
}
//---------------------------------------------------------------------------
void ScalarEngine::ClickOnPlay()
{
	if (current_level == -1)	// setup new stuff
	{
		current_points = current_swaps = current_level = 0;
		m_LevelComplete = true;
		m_ImageToPlay[0] = NjamRandom(m_ImageCount);
		do {
			m_ImageToPlay[1] = NjamRandom(m_ImageCount);
		} while (m_ImageToPlay[1] == m_ImageToPlay[0]);
		do {
			m_ImageToPlay[2] = NjamRandom(m_ImageCount);
		} while (m_ImageToPlay[2] == m_ImageToPlay[0] || m_ImageToPlay[2] == m_ImageToPlay[1]);
	}

	if (m_LevelComplete)
	{
		if (!SetupGame())
			return;
	}
	else
		Message("YOU MUST FIRST COMPLETE THIS LEVEL!");
}
//---------------------------------------------------------------------------
void ScalarEngine::getName(char *name)
{
	Uint32 white = SDL_MapRGB(m_Screen->format, 255, 255, 255);
	Uint32 maroon = SDL_MapRGB(m_Screen->format, 180, 0, 0);
	Uint32 t = SDL_GetTicks();
	bool CursorOn = true;

	name[0] = '\0';
	// Touchscreen name input - start
		char allowed[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789. :)(!*";
		bool done=false;
		int i, j, ok;
		SK::SetTitle("NEW HIGHSCORE");
		do {
			ok=0;
			
			// Get name throught SK
			SK::GetString(name);
			
			// Check if it contains only allowed characters
			for(i=0; i<strlen(name); i++)
				for(j=0; j<strlen(allowed); j++)
					if(name[i]==allowed[j])
						ok++;
						
			// Check name
			if(ok==strlen(name)) {
				if(strlen(name)>0) done=true;						// It's ok
				else SK::SetTitle("Please enter your name:");		// Name is empty
			} else SK::SetTitle("Only alpha numeric characters!");	// Name contains other characters than the allowed ones
			
		} while(!done);
		SK::RemoveTitle();
	// Touchscreen name input - stop
	
	
	// Uncomment the following and remove the 'touchscreen name input' section if you want the normal highscore input by buttons instead of the soft-keyboard
	/*while (true)
	{
		/*SDL_Rect dest;
		NjamSetRect(dest, 286, 349, 228, 77);
		SDL_FillRect(m_Screen, &dest, 0);
		NjamSetRect(dest, 287, 350, 226, 75);
		SDL_FillRect(m_Screen, &dest, maroon);
		m_FontYellow->WriteTextXY(m_Screen, 323, 355, "NEW HISCORE");
		NjamSetRect(dest, 292, 388, 216, 32);
		SDL_FillRect(m_Screen, &dest, 0);
		m_FontYellow->WriteTextXY(m_Screen, 294, 390, name);

		int l = 0;	// get len
		while (name[l])
			l++;

		if (SDL_GetTicks() > t + 400)	// make the cursor blink
		{
			t = SDL_GetTicks();
			CursorOn = !CursorOn;
		}

		if (CursorOn)
		{
			NjamSetRect(dest, 294 + 14 * l, 392, 12, 25);	// draw cursor
			SDL_FillRect(m_Screen, &dest, white);
		}

		SDL_Flip(m_Screen);

		SDLKey key = NjamGetch(false);
		char allowed[] = "0123456789abcdefghijklmnopqrstuvwxyz. :)(!*";
		bool ok = false;
		for (int k=0; allowed[k]; k++)
			if (allowed[k] == key)
				ok = true;

		if (ok && l < 14)
		{
			char c = key;
			if (c >= 'a' && c <= 'z')
				c -= ('a' - 'A');
			name[l] = c;
			name[l+1] = '\0';
		}

		if (key == SDLK_BACKSPACE && l > 0)			// backspace
			name[l-1] = '\0';

		if ((key == SDLK_KP_ENTER || key == SDLK_RETURN) && name[0])	// entered
			break;
	}*/
}
//---------------------------------------------------------------------------
