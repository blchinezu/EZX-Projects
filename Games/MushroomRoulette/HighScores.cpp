/*
	Code written by James Fletcher (LiX). 
	Copyright 2009 - 2010. 
	ethocybin@googlemail.com
*/

#include "HighScores.h"
#include "GameWorld.h"
#include <stdio.h>
bool loadedimages=0;


//Constructor
HighScores::HighScores()
{
	//Set the array values to 0's
	for(int i = 0; i < MAX_SCORES; i++)
	{
		memset(&m_scores[i], 0x00, sizeof(hscore));
	}

	//Set Variables
	m_failed = 0;
	m_lastscore = 0;
	m_enabled = 0;

	//Load scores
	LoadScoresFromFile();
}

//Destructor
HighScores::~HighScores()
{
	//
}


//Renders HighScores screen
char text[128];
void HighScores::Render()
{
	if(loadedimages == 0)
	{
		//Load Background Surface
		m_surf_background = gGameWorld.gConMgr.GetSurface("sprites/highscores.fif");
		m_surf_fail = gGameWorld.gConMgr.GetSurface("sprites/fail.fif");
		loadedimages=1;
	}

	//Draw Highscores Background
	SDL_BlitSurface(m_surf_background, 0, gGameWorld.backbuffer, &SDLE_Rect(40, 40, m_surf_background->w, m_surf_background->h));

	//Draw the scores
	int y=70;
	int bx=60; 
	for(int i = 0; i < MAX_SCORES; i++)
	{
		if(i == 10){bx = 180; y=70;}

		if(m_scores[i].score == 0){return;}
		sprintf(text, "L%i", m_scores[i].level);
		gGameWorld.gFont.DrawString(gGameWorld.backbuffer, bx, y, text, FONT_GREEN);
		sprintf(text, "%i", m_scores[i].score);

		if(m_scores[i].score == m_lastscore)
		{
			gGameWorld.gFont.DrawString(gGameWorld.backbuffer, bx+40, y, text, FONT_RED);
		}else{
			gGameWorld.gFont.DrawString(gGameWorld.backbuffer, bx+40, y, text, FONT_BLUE);
		}

		y += 13;
	}

	//If your score was too low to be added to the highscores, you failed.
	if(m_failed == 1)
	{
		SDL_BlitSurface(m_surf_fail, 0, gGameWorld.backbuffer, &SDLE_Rect(95, 100, m_surf_fail->w, m_surf_fail->h));
	}
}


//Clears a file content
void HighScores::ClearFile(char* in)
{
	FILE* t = fopen(in, "w");
	if(t != NULL)
	{
		fprintf(t, "");
		fclose(t);
	}
}

//Encrypts data using simple xor
void HighScores::XorCrypt(char* in, char* out, int effector)
{
	char c;
	int size;

	FILE* f = fopen(in, "r");
	FILE* o = fopen(out, "w");
	if(f != NULL && o != NULL)
	{
		fseek(f, 0, SEEK_END);
		size = ftell(f);
		fseek(f, 0, SEEK_SET);

		for(int i = 0; i < size; i++)
		{
			c = fgetc(f);
			c = c ^ effector;
			fputc(c, o);
		}

		fclose(f);
		fclose(o);
	}
}

//Add's a high score to the m_scores array
bool HighScores::AddScore(int score, int level, int life)
{
	//Check if the score is higher then the lowest score on the list, if not return 0
	if(m_scores[MAX_SCORES-1].score < score)
	{
		//if it is, then replace the lowest score with the new score.
		m_scores[MAX_SCORES-1].score = score;
		m_scores[MAX_SCORES-1].health = life;
		m_scores[MAX_SCORES-1].level = level;
		m_lastscore = score;

		//Sort score list!
		SortScores();

		//Save score list to file!
		SaveScoresToFile();

		//Score has been added to top 10!
		return 1;
	}

	//Score is too low
	m_failed = 1;
	return 0;
}


//Sorts the scores into ascending order based on Level and Score
void HighScores::SortScores()
{
	hscore s;

	for(int o = 0; o < MAX_SCORES; o++)
	{
		for(int i = 0; i < MAX_SCORES-1; i++)
		{
			o++;

			if(m_scores[i].score < m_scores[i+1].score)
			{
				s = m_scores[i];
				m_scores[i] = m_scores[i+1];
				m_scores[i+1] = s;
				o = 0;
			}
		}
	}
}


//Saves m_scores to file
void HighScores::SaveScoresToFile()
{
	//Save Scores
	FILE* f = fopen("temp.dat", "w");
	if(f != NULL)
	{
		for(int i = 0; i < MAX_SCORES; i++)
		{
			fprintf(f, "%i %i %i ", m_scores[i].health, m_scores[i].level, m_scores[i].score);
		}

		fclose(f);
	}

	//Encrypt
	XorCrypt("temp.dat", "highscores.dat", ENCRYPTION_KEY);

	//remove Temp
	ClearFile("temp.dat");
}


//Loads m_scores from file
void HighScores::LoadScoresFromFile()
{
	//Decrypt
	XorCrypt("highscores.dat", "temp.dat", ENCRYPTION_KEY);

	//Load Scores
	FILE* f = fopen("temp.dat", "r");
	if(f != NULL)
	{
		for(int i = 0; i < MAX_SCORES; i++)
		{
			fscanf(f, "%i", &m_scores[i].health);
			fscanf(f, "%i", &m_scores[i].level);
			fscanf(f, "%i", &m_scores[i].score);
		}

		fclose(f);
	}

	//clear Temp
	ClearFile("temp.dat");
}


//Is Evolved
bool HighScores::IsEvolved()
{
	for(int i = 0; i < MAX_SCORES; i++)
	{
		if(m_scores[i].level >= 200){return 1;}
	}

	return 0;
}


//Set m_enabled
void HighScores::SetEnabled(bool in)
{
	m_enabled = in;
	if(m_enabled == 0){m_failed = 0; m_lastscore=0;}
}



