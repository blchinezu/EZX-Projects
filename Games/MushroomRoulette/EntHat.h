/*
	Code written by James Fletcher (LiX). 
	Copyright 2009 - 2010. 
	ethocybin@googlemail.com
*/

#ifndef ENTHAT_H
#define ENTHAT_H

#include "BaseEntity.h"

//Hat Entity Class
class EntHat : public BaseEntity
{
public:
	EntHat();
	~EntHat();

	void Render();
	void InitHat(int type);
	int m_type;

private:
	SDL_Surface* m_surf_hat[6];
};

#endif



