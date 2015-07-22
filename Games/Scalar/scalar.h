/*-----------------------------------------------------------------------------
Copyright 2003 Milan Babuskov

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
#ifndef SCALAR_H
#define SCALAR_H
#include "SDL.h"
#include "SDL_mixer.h"
#include "njamutils.h"
#include "njamfont.h"
#include "dirent.h"
//-----------------------------------------------------------------------------

	
typedef struct piece
{
	SDL_Rect image_rect;
	SDL_Rect grid_rect;
	struct piece *next;
} Piece;
//-----------------------------------------------------------------------------
typedef enum { toHiscore=0, toExit, toPlay, toRepeat, toHelp, toGrid } tToolbarOption;
//-----------------------------------------------------------------------------
class HiScore
{
public:
	int points[10];
	char names[10][15];
	int swaps[10];

	char *enterHiscore(int points, int swaps);	// returns where to put the name
	HiScore();
	~HiScore();
};
//-----------------------------------------------------------------------------
class ScalarEngine
{
private:
	HiScore m_hiscore;
 	bool m_SDL;						// whether SDL is initialized or not
	bool m_AudioAvailable;			// so we know wheter to try to play music/sfx
	bool m_GridOn;
	char linux_sdl_driver[10];
	SDL_Surface *m_Screen;

	SDL_Surface *m_Icon;
	SDL_Surface *m_PuzzleImage;
	SDL_Surface *m_LogoImage;
	SDL_Surface *m_Background;
	SDL_Surface *m_SmallImage;
	SDL_Surface *m_Toolbar;
	NjamFont *m_FontBlue;
	NjamFont *m_FontYellow;
	Mix_Music *m_MainMenuMusic;

	Piece *m_FirstPiece;
	int m_XOffset, m_YOffset;		// offset of image comparing to screen
	void Draw(bool WithBackground = true);
	void StrechDraw(SDL_Rect *src, SDL_Rect *dest, bool Darken = false);
	void NewGame();
	void Message(char *text);
	void FreePieces();
	void CreatePieces(Piece *);
	Piece *FindPiece(int x, int y);
	void Switch(Piece *one, Piece *two);
	void ShufflePieces();
	int m_ImageW, m_ImageH;

	bool countImages();
	int m_ImageCount;
	int m_ImageToPlay[3];

	int m_Clicks;
	Piece *m_SelectedPiece;
	bool m_LevelComplete;
	bool SetupGame();
	void ClickOnPlay();
	bool CheckVictory();
	void DrawHelp();
	void DrawHiscore();
	void getName(char *name);
	int current_points, current_swaps, current_level, current_try;
public:
	ScalarEngine();
	~ScalarEngine();
	bool Init(bool,bool);			// initialize everything
	void Start();					// start the music and main menu loop
};
//-----------------------------------------------------------------------------
#endif
