/*
	Code written by James Fletcher (LiX). 
	Copyright 2009 - 2010. 
	ethocybin@googlemail.com
*/

#include "Vector2D.h"
#include <math.h>


//Set's the x and y values
void Vector2D::Set(int ix, int iy)
{
	x = ix;
	y = iy;
}

//Reset's the x and y values
void Vector2D::Reset()
{
	x = 0;
	y = 0;
}

//Get's the distance between two points
int Vector2D::EuclidDistance(Vector2D input)
{
	return sqrt((double)(input.x - x)*(input.x - x) + (input.y - y)*(input.y - y));
}

//Get's the distance between two points
int Vector2D::ManhattanDistance(Vector2D in)
{
	return fabs((double)in.x-x) + fabs((double)in.y-y);
}

//Normalizes a Vector
Vector2D Vector2D::Normalize()
{
	int magnitude = sqrt((double)x*x + y*y);
	return Vector2D(x / magnitude, y / magnitude);
}

//Operator Overloads
Vector2D& Vector2D::operator+=(Vector2D& in)
{
	x += in.x;
	y += in.y;
	return *this;
}

Vector2D& Vector2D::operator-=(Vector2D& in)
{
	x -= in.x;
	y -= in.y;
	return *this;
}

Vector2D& Vector2D::operator*=(Vector2D& in)
{
	x *= in.x;
	y *= in.y;
	return *this;
}

Vector2D& Vector2D::operator/=(Vector2D& in)
{
	x /= in.x;
	y /= in.y;
	return *this;
}

bool Vector2D::operator==(Vector2D& in)
{
	if(x == in.x && y == in.y){return 1;}
	return 0;
}

bool Vector2D::operator!=(Vector2D& in)
{
	if(x != in.x && y != in.y){return 1;}
	return 0;
}

