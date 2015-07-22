/*
	Code written by James Fletcher (LiX). 
	Copyright 2009 - 2010. 
	ethocybin@googlemail.com
*/

#include "BaseEntity.h"

BaseEntity::BaseEntity(){m_visible = 0; m_ident=0;}

//Virtual Function
void BaseEntity::Update(){}
void BaseEntity::Render(){}
void BaseEntity::SendEvent(int msg, void* addarg){}

//Accessors
void BaseEntity::SetPos(int x, int y){m_pos.x = x; m_pos.y = y;}
Vector2D BaseEntity::GetPos(){return m_pos;}
bool BaseEntity::IsVisible(){return m_visible;}
void BaseEntity::SetVisible(bool in){m_visible = in;}



