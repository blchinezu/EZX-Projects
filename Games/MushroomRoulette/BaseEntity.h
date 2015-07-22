/*
	Code written by James Fletcher (LiX). 
	Copyright 2009 - 2010. 
	ethocybin@googlemail.com
*/

#ifndef BASEENTITY_H
#define BASEENTITY_H

#include "sdl_extra.h"
#include "Vector2D.h"

#define MSG_COLLISION 1
#define IDENT_MAN 1
#define IDENT_BUTTERFLY 2
#define IDENT_MUSHROOM 3
#define IDENT_HAT 4

class BaseEntity
{
public:
	BaseEntity();
	virtual void Update();
	virtual void Render();
	virtual void SendEvent(int msg, void* addarg);
	
	//Accessors
	void SetPos(int x, int y);
	Vector2D GetPos();
	bool IsVisible();
	void SetVisible(bool in);
	int m_ident;

protected:
	Vector2D m_pos; //Graphic Position
	bool m_visible;
};



#endif



