/*
	Code written by James Fletcher (LiX). 
	Copyright 2009 - 2010. 
	ethocybin@googlemail.com
*/

#ifndef VECTOR2D_H
#define VECTOR2D_H

//2D Vector Data Structure
struct Vector2D
{
	//Constructors
	Vector2D()
	{
		x = 0, y = 0;
	}

	Vector2D(const int ix, const int iy)
	{
		x = ix, y = iy;
	}

	//Variables
	int x, y;

	//Functions
	int ManhattanDistance(Vector2D in);
	int EuclidDistance(Vector2D in);
	Vector2D Normalize();
	void Reset();
	void Set(int ix, int iy);

	//Operator Overloads
	Vector2D& operator+=(Vector2D& in);
	Vector2D& operator-=(Vector2D& in);
	Vector2D& operator*=(Vector2D& in);
	Vector2D& operator/=(Vector2D& in);

	bool operator==(Vector2D& in);
	bool operator!=(Vector2D& in);
};

#endif

