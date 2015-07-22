/*
	Code written by James Fletcher (LiX). 
	Copyright 2008 - 2009. 
	LiX@Ethocybin.com
*/

#include "sdl_extra.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>


#define archive_size 1000
int rand_list[archive_size]; //Uniniting it helps our case
int rand_pos=0;

int rand_regulator;
int rand_interval;

int retval = 0;
int gfl_start;
int rnd;


//Get's ticks
int GetTicks()
{
	return SDL_GetTicks();
}

//Get's random from list
int GetFromList()
{
	gfl_start = GetTicks();
	do
	{
		retval = rand_list[rand()%archive_size];
		if(GetTicks() >= gfl_start+100){return rand();}
	}
	while(retval == 0);

	return retval;
}


//Adds random to list
int rnd_pos;
void rnd_AddList()
{
	if(GetTicks() < rand_regulator+rand_interval){return;}

	for(int i = 0; i < archive_size; i++)
	{
		if(rand_list[i] == 0)
		{
			rnd_pos = i;
		} 
	}

	if(rnd_pos != 0)
	{
		rand_list[rnd_pos] = rand();
	}else{
		rand_list[rand()%archive_size] = rand();
	}

	//Set new Srand
	rnd = rand()%300;

	if(rnd >= 0 && rnd <= 100){srand(time(0));}
	if(rnd > 100 && rnd <= 200){srand(GetTicks());}
	if(rnd > 200 && rnd <= 300){srand(GetFromList());}
	rand_pos = GetFromList();

	rand_interval = qRand(100, 500);
	rand_regulator = GetTicks();
}


//Quick Random
int qRand(const int min, const int max)
{
	return (unsigned int) (rand()%(max-min))+min;
}

//Quick random float
float qRandFloat(const float min, const float max)
{
	return (float) (rand() / (static_cast<float>(RAND_MAX) + 1.0)) * (max - min) + min;
}


