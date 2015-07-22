#ifndef BOARD_H
#define BOARD_H

#include "includes.h"

enum TItem{EMPTY,FOOD,WALL,SNAKE,RESERVED};


class CBoard {
	private:
		int cols, rows, cellW, cellH, offsetX, offsetY, marginX, marginY;
		TItem **item;
		CBitmap *bmp_food;
		CBitmap *bmp_wall;
		CBitmap *bmp_background;
		int foodLeft;
		
		void CreateWalls(int n)
		{
			int x1,y1,x2,y2,created;
			bool vertical, obstacle, checked;
			created = 0;
			while (created<n)
			{
				x1 = rand()%(cols-2)+1;
				y1 = rand()%(rows-2)+1;
				x2=x1;
				y2=y1;

				vertical = (rand()%2==0);
		
				if (vertical)
				{
					while (y2-y1<3)
					{
						y1 = rand()%(rows-2)+1;
						y2 = rand()%(rows-2)+1;
					}
				}
				else
				{
					while (x2-x1<3)
					{
						x1 = rand()%(cols-2)+1;
						x2 = rand()%(cols-2)+1;
					}
				}
		
				// check there is no obstacle
				obstacle = false;
				for (int tempX=x1-1; tempX<=x2+1; tempX++)
					for (int tempY=y1-1; tempY<=y2+1; tempY++)
						if (item[tempX][tempY] != EMPTY) obstacle = true;

				if (!obstacle)
				{
					for (int tempX=x1; tempX<=x2; tempX++)
						for (int tempY=y1; tempY<=y2; tempY++)
						item[tempX][tempY] = WALL;
					created++;
				}

			} // while		
		}

	public:
		CBoard(int _cols, int _rows,
			int _cellW, int _cellH,
			int _offsetX, int _offsetY)
		{
			cols = _cols;
			rows = _rows;
			cellW = _cellW;
			cellH = _cellH;
			offsetX = _offsetX;
			offsetY = _offsetY;
			marginX=10;
			marginY=10;
	
			foodLeft = 0;

			bmp_background = new CBitmap("pixmaps/board_bg.bmp");

			bmp_food = new CBitmap("pixmaps/food.bmp");
			bmp_wall = new CBitmap("pixmaps/wall.bmp");
		
			item = new TItem*[cols];
			for (int c=0; c<cols; c++)
			{
				item[c] = new TItem[rows];
				for (int r=0; r<rows; r++)
				{
					item[c][r] = EMPTY;
				}
			}
		}

		~CBoard()
		{
			for (int c=0; c<cols; c++) delete item[c];
			delete item;	
			delete bmp_food;
			delete bmp_wall;		
			delete bmp_background;
		}

		void Draw(SDL_Surface *screen)
		{
			if (screen==NULL) {printf("Screen null\n");}
			// 1: draw background
			bmp_background->draw(screen,offsetX,offsetY);
			
			
			// 2: draw food and walls
			for (int c=0; c<cols; c++)
				for (int r=0; r<rows; r++)
					switch (item[c][r])
					{
						case FOOD: 
							DrawCell(screen,c,r,bmp_food);
							break;
						case WALL:
							DrawCell(screen,c,r,bmp_wall);
							break;
					}
		}

		void DrawCell(SDL_Surface *screen,int x, int y,CBitmap *bmp)
		{
			bmp->draw(screen,offsetX+(x*cellW)+marginX,offsetY+(y*cellH)+marginX);		
		}
		
		
		void SetCell(int x, int y,TItem type)
		{
			if (x>=0 && y>=0 && x<cols && y<rows)
			{
				if ( (item[x][y]==FOOD) && (type!=FOOD) ) foodLeft--;	
				item[x][y] = type;			
			}
		}
		
		
		void ClearCell(SDL_Surface *screen,int x, int y)
		{
			SDL_Rect r;
			r.x = offsetX+(x*cellW)+marginX;
			r.y = offsetY+(y*cellH)+marginY;
			r.w = cellW;
			r.h = cellH;
			
			SDL_FillRect(screen, &r,0); // clear area	
			
		}
		
		
		bool CanMoveTo(int x, int y)
		{
			return ( (x>=0 && y>=0 && x<cols && y<rows) && (item[x][y]==EMPTY || item[x][y]==FOOD || item[x][y]==RESERVED) );
		}
		
		
		bool ThereIsFood(int x, int y)
		{
			return ((x>=0 && y>=0 && x<cols && y<rows) && (item[x][y]==FOOD));
		}
		
		
		void PrepareLevel(int l)
		{
			// 1: empty board
			for (int c=0; c<cols; c++)
				for (int r=0; r<rows; r++)
					if (item[c][r] != RESERVED) item[c][r] = EMPTY;
	
			// 2: create walls
			CreateWalls(2*l);
	
			// 3: create food
			foodLeft = 3*l;
			int foodCount = 0;
			while (foodCount<foodLeft)
			{
				int x = rand()%cols;
				int y = rand()%rows;
		
				// TO DO: check food not be put over snake - it will crash foodLeft
				if (item[x][y] == EMPTY)
				{
					item[x][y] = FOOD;
					foodCount++;
				}
			}			
		}
		
		
		bool Finished()
		{
			return (foodLeft<=0);		
		}
};

#endif
