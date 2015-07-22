/*
	Code written by James Fletcher (LiX). 
	Copyright 2009 - 2010. 
	ethocybin@googlemail.com

	This is the A* path finding method.

	I removed the H variable from the search because there's no diagonals
	in the game, thus G and H are the same values if using a ManhattanDistance().
*/

#include "PathFinder.h"
PathFinder gPathFinder;

#include "GameWorld.h"


//Sorts the list by the lowest F value
void PathFinder::SortOpenList()
{
	node t;

	for(int o = 0; o < openlist.size(); o++)
	{
		for(int i = 0; i < openlist.size()-1; i++)
		{
			o++;

			//if(i > 1) //Why did I do this? I forgot..
			//{
				if(openlist[i].f() > openlist[i+1].f())
				{
					t = openlist[i];
					openlist[i] = openlist[i+1];
					openlist[i+1] = t;
					o = 0;
				}
			//}
		}
	}
}

//Check if already in openlist
int PathFinder::IsInOpenList(int x, int y)
{
	for(int i = 0; i < openlist.size(); i++)
	{
		if(openlist[i].pos.x == x && openlist[i].pos.y == y){return i;}
	}

	return -1;
}

//Check if already in closedlist
int PathFinder::IsInClosedList(int x, int y)
{
	for(int i = 0; i < closedlist.size(); i++)
	{
		if(closedlist[i].pos.x == x && closedlist[i].pos.y == y){return i;}
	}

	return -1;
}

//Dumps the OpenList and ClosedList to file
void PathFinder::DumpPathLists()
{
	FILE* f = fopen("graphsearch.txt", "a");
	if(f != NULL)
	{
		fprintf(f, "\n\n\n\n\nOpenList ::\n");

		for(int i = 0; i < openlist.size(); i++)
		{
			fprintf(f, "%i %i, %i %i, %i, %i, %i\n", openlist[i].pos.x, openlist[i].pos.y, 
				openlist[i].parent.x, openlist[i].parent.y,
				openlist[i].g, openlist[i].h, openlist[i].f());
		}

		fprintf(f, "\n\n\n\n\nClosedList ::\n");

		for(int i = 0; i < closedlist.size(); i++)
		{
			fprintf(f, "%i %i, %i %i, %i, %i, %i\n", closedlist[i].pos.x, closedlist[i].pos.y, 
				closedlist[i].parent.x, closedlist[i].parent.y,
				closedlist[i].g, closedlist[i].h, closedlist[i].f());
		}

		fclose(f);
	}
}


//Returns the path between the specified grid locations.
bool PathFinder::GetPathTo(vector<Vector2D>* pathout, Vector2D start, Vector2D target)
{
	node n;
	node nn;
	Vector2D np;
	bool needsort=0;
	bool exitloop=0;
	int isinclosedlist=0;
	int isinopenlist=0;
	BaseEntity* ent=NULL;

	//Clear openlist and closedlist
	openlist.clear();
	closedlist.clear();

	//Add the start node to the openlist
	memset(&n, 0, sizeof(node));
	n.pos = start;
	openlist.push_back(n);

	while(exitloop == 0)
	{
		//Take the node with the lowest F value off the openlist and put on closedlist
		n = openlist.front();
		closedlist.push_back(openlist.front());
		openlist.erase(openlist.begin());

		/* If all four squares on either side are walkable and not on the closed list then
		   add to the openlist and record g and h values */
		for(int i = 0; i < 4; i++)
		{
			if(i == 0){np.Set(n.pos.x-1, n.pos.y);} //left
			if(i == 1){np.Set(n.pos.x+1, n.pos.y);} //right
			if(i == 2){np.Set(n.pos.x, n.pos.y-1);} //top
			if(i == 3){np.Set(n.pos.x, n.pos.y+1);} //bottom

			//If any of the check are out of range ignore
			if(np.x < 0){continue;}
			if(np.x > 16){continue;}
			if(np.y < 0){continue;}
			if(np.y > 10){continue;}

			//It it's NOT on the closed list and it's NOT occupied by an entity
			isinclosedlist = IsInClosedList(np.x, np.y);
			ent = gGameWorld.GetGridCell(np.x, np.y)->ent;
			if(np == target){ent = NULL;} //If the square is the destination square pretend nothings there (if there is)
			if(isinclosedlist == -1 && ent == NULL)
			{			
				//If it isnt in the openlist add it
				isinopenlist = IsInOpenList(np.x, np.y);
				if(isinopenlist == -1)
				{
					nn.pos = np;
					nn.g = n.g + 1;
					nn.h = np.ManhattanDistance(target); //Euclid distance works a bit better
					nn.parent = n.pos;
					openlist.push_back(nn);
					needsort = 1;
				}

				//If it is in the open list check to see if this path is better
				if(isinopenlist != -1)
				{
					if((n.g + 1) < openlist[isinopenlist].g)
					{
						openlist[isinopenlist].parent = n.pos;
						openlist[isinopenlist].g = n.g + 1;
						needsort = 1;
					}
				}

				if(needsort == 1){SortOpenList(); needsort=0;} //Sort the open list by F if needed
			}
		}

		//Has target been added to closed list?
		if(IsInClosedList(target.x, target.y) != -1){exitloop=1;}

		//Target not found!
		if(openlist.size() <= 0){return 0;} //Fail! Just give up.
	}

	//Grab the path from the results!
	Vector2D c; //Current Grid Position
	c.x = target.x; //Starting from the end and working backwards!
	c.y = target.y; //Starting from the end and working backwards!
	int ci=0; //Current Grid Position Index in Closed List
	vector<Vector2D> thepath; //Where the path grid positions get dumped in reverse sequence!

	do
	{
		thepath.push_back(c); //Push back position
		ci = IsInClosedList(c.x, c.y); //Get index to position in closed array
		c.x = closedlist[ci].parent.x; //Set next recorded position to be that of the parent (x)
		c.y = closedlist[ci].parent.y; //Set next recorded position to be that of the parent (y)
		ci++;
	}
	while(c.x != start.x || c.y != start.y); //end once we reach the starting square ¬
	//(we don't add this to the path list because the player is already at this square!!)

	//Output thepath to the user (in the right order)
	for(int i = 0; i < thepath.size(); i++)
	{
		pathout->push_back(thepath[(thepath.size()-1)-i]);
	}

	//Everything went just fine!
	return 1;
}







