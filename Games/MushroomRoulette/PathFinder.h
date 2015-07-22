/*
	Code written by James Fletcher (LiX). 
	Copyright 2009 - 2010. 
	ethocybin@googlemail.com
*/

#ifndef PATHFINDER_H
#define PATHFINDER_H

#include <vector>
using namespace std;

#include "Vector2D.h"

//PathFinder Node
struct node
{
	Vector2D pos;
	int g,h;
	int f(){return g+h;}
	Vector2D parent;
};

//PathFinder Class
class PathFinder
{
public:
	bool GetPathTo(vector<Vector2D>* pathout, Vector2D start, Vector2D target);

private:
	vector<node> openlist;
	vector<node> closedlist;

	void SortOpenList();
	int IsInOpenList(int x, int y);
	int IsInClosedList(int x, int y);
	void DumpPathLists();
};

extern PathFinder gPathFinder;

#endif





