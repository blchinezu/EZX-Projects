/*
	Code written by James Fletcher (LiX). 
	Copyright 2009 - 2010. 
	ethocybin@googlemail.com
*/

#ifndef CONTENTMANAGER_H
#define CONTENTMANAGER_H

#include "sdl_extra.h"
#include <string>
#include <vector>
using namespace std;


//Surface with attached string name
struct CSurface
{
	string name;
	SDL_Surface* surface;
};

//Content Manager
class ContentManager
{
public:
	ContentManager();
	~ContentManager();

	//Functions
	SDL_Surface* GetSurface(string name);
	void AddSurface(string bitmap, string name, bool transparent);
	void XorCrypt(char* in, char* out, int effector);

	//Debug
	void DumpSurfaceList();

private:
	vector<CSurface> msa; //Surface Array
};

#endif




