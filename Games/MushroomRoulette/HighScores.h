/*
	Code written by James Fletcher (LiX). 
	Copyright 2009 - 2010. 
	ethocybin@googlemail.com

	This is a top 20 score system.
*/

#ifndef HIGHSCORES_H
#define HIGHSCORES_H

#include "EntPlayer.h"
#include "sdl_extra.h"

#define MAX_SCORES 20
#define ENCRYPTION_KEY 25348

//Score Structure
struct hscore
{
	int health;
	int level;
	int score;
};

//HighScores Class
class HighScores
{
public:
	HighScores();
	~HighScores();

	//Functions
	void Render();
	bool AddScore(int score, int level, int life);
	bool IsEvolved();

	void SetEnabled(bool in);
	bool m_enabled;

private:
	hscore m_scores[MAX_SCORES];
	SDL_Surface* m_surf_background;
	SDL_Surface* m_surf_fail;
	bool m_failed;
	int m_lastscore;

	void SortScores();
	void SaveScoresToFile();
	void LoadScoresFromFile();

	void XorCrypt(char* in, char* out, int effector);
	void ClearFile(char* in);
};

#endif







