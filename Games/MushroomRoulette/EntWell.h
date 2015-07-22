/*
	Code written by James Fletcher (LiX). 
	Copyright 2009 - 2010. 
	ethocybin@googlemail.com
*/

#ifndef ENTWELL_H
#define ENTWELL_H

#include "BaseEntity.h"

//Well Entity Class
class EntWell : public BaseEntity
{
public:
	EntWell();
	~EntWell();

	void Update();
	void Render();
	void InitWell(bool mode);
	void SendEvent(int msg, void* addarg);

private:
	SDL_Surface* m_surf_well[2];
	int anim_regulator;
	bool m_wellstate;
	int init_time; //time well was created at
	bool m_mode;
};

#endif



