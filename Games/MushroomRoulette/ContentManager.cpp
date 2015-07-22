/*
	Code written by James Fletcher (LiX). 
	Copyright 2009 - 2010. 
	ethocybin@googlemail.com
*/

#include "ContentManager.h"
#include "sdl_extra.h"


//Constructor
ContentManager::ContentManager()
{
	//
}

//Destructor
ContentManager::~ContentManager()
{
	//Free Surfaces
	for(int i = 0; i < msa.size(); i++)
	{
		SDL_FreeSurface(msa[i].surface);
	}

	msa.clear();
}


//Get's a surface from the m_surf array
SDL_Surface* ContentManager::GetSurface(string name)
{
	for(int i = 0; i < msa.size(); i++)
	{
		if(msa[i].name == name)
		{
			return msa[i].surface;
		}
	}

	return NULL;
}


//Loads a bitmap and sets name into array
void ContentManager::AddSurface(string bitmap, string name, bool transparent)
{
	CSurface t;
	msa.push_back(t);
	msa.back().name = name;
	SDLE_LoadFIF((char*)bitmap.c_str(), &msa.back().surface);

	if(transparent == 1){SDLE_KeyGreenToAlpha(msa.back().surface);}
}


//Dump list of surfaces to file
void ContentManager::DumpSurfaceList()
{
	FILE* f = fopen("content.txt", "w");
	if(f != NULL)
	{
		fprintf(f, "Total: %i\n\n", msa.size());

		for(int i = 0; i < msa.size(); i++)
		{
			fprintf(f, "%i - %s\n", i, msa[i].name.c_str());
		}

		fclose(f);
	}
}

//Encrypts data using simple xor
void ContentManager::XorCrypt(char* in, char* out, int effector)
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







