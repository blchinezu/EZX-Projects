/*
	Code written by James Fletcher (LiX). 
	Copyright 2009 - 2010. 
	ethocybin@googlemail.com
*/

#ifndef ENTBOX_H
#define ENTBOX_H

#include "BaseEntity.h"

//Types of Box
#define UNMOVABLE_BOX 0
#define MOVEABLE_BOX 1

//Box Entity Class
class EntBox : public BaseEntity
{
public:
	EntBox();
	~EntBox();
	
	void Update();
	void Render();
	void InitBox(bool itype);
	void SendEvent(int msg, void* addarg);

	//Accessors
	bool m_moveable;

private:
	SDL_Surface* m_surf[2];

	bool m_inanim;
	int m_direction;
	Vector2D m_animpos;
};

#endif



