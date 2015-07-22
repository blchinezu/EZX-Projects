#ifndef SOUND_H
#define SOUND_H

/*
	Code written by James Fletcher (LiX). 
	Copyright 2009 - 2010. 
	ethocybin@googlemail.com
*/

#include <SDL.h>

//Sound Sample
struct sample
{
	Uint8 *data;
	Uint32 dpos;
	Uint32 dlen;
};

//Sound System
class cSound
{
public:
	cSound(); //Constructorv
	~cSound(); //Destructor

	void PlaySound(char *file);

private:
	//
};

extern cSound sound;

#endif












