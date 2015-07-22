#ifndef SNAKE_H
#define SNAKE_H

#define SNAKE_LENGTH 300


class CPointList {

	private:
		CPoint *p;
		int firstFree;
		int size;
	
	public:
	
		CPointList()
		{
			size = 100;
			firstFree=0;
			p = new CPoint[20];
		}
		
		void Push(CPoint _p)
		{
			if (firstFree<size-1) {
				p[firstFree]=_p;
				firstFree++;		
			} else {
				printf("CPointList: Stack overflow\n");
				exit(1);
			}
		}
	
		CPoint *Pop()
		{
			CPoint *pr;
			pr = NULL;
			
			if (firstFree>0) {
				pr = new CPoint();
				pr->x=p[firstFree-1].x;
				pr->y=p[firstFree-1].y;
				firstFree--;
			}
		
			return pr;
		}
		
		void Clear()
		{
			firstFree=0;
		}
	
};

enum TDirection {DIR_NONE, DIR_LEFT, DIR_RIGHT, DIR_UP, DIR_DOWN};

class CSnake
{
	public:
	
		int delay;
	
		CSnake(CBoard *_board, int _lifes)
		{
			board = _board;

			bmp_body_h = new CBitmap("pixmaps/body_h.bmp");
			bmp_body_v = new CBitmap("pixmaps/body_v.bmp");
			bmp_body_ul = new CBitmap("pixmaps/body_ul.bmp");
			bmp_body_ur = new CBitmap("pixmaps/body_ur.bmp");
			bmp_body_dl = new CBitmap("pixmaps/body_dl.bmp");
			bmp_body_dr = new CBitmap("pixmaps/body_dr.bmp");
			bmp_head_l = new CBitmap("pixmaps/head_l.bmp");
			bmp_head_r = new CBitmap("pixmaps/head_r.bmp");
			bmp_head_u = new CBitmap("pixmaps/head_u.bmp");
			bmp_head_d = new CBitmap("pixmaps/head_d.bmp");
			bmp_end_l = new CBitmap("pixmaps/end_l.bmp");
			bmp_end_r = new CBitmap("pixmaps/end_r.bmp");
			bmp_end_u = new CBitmap("pixmaps/end_u.bmp");
			bmp_end_d = new CBitmap("pixmaps/end_d.bmp");

			body = new CPoint[SNAKE_LENGTH];
	
			pointsToDelete = new CPointList();

			lifes = _lifes;

			Init();
		}
		
		~CSnake()
		{
			delete bmp_body_h;
			delete bmp_body_v;
			delete bmp_body_ul;
			delete bmp_body_ur;
			delete bmp_body_dl;
			delete bmp_body_dr;
			delete bmp_head_l;
			delete bmp_head_r;
			delete bmp_head_u;
			delete bmp_head_d;
			delete bmp_end_l;
			delete bmp_end_r;
			delete bmp_end_u;
			delete bmp_end_d;
	
			delete body;
		}

		void Init()
		{
			pointsToDelete->Clear();
	
			grow = 0;
			dir = DIR_RIGHT;
			collided = false;
			delay = 5;
			timeLeft = delay;

			body[0].x=10;
			body[0].y=8;
			body[1].x=11;
			body[1].y=8;
			first = 1;
			last = 0;
	
			board->SetCell(body[0].x,body[0].y,RESERVED);
			board->SetCell(body[1].x,body[1].y,RESERVED);

			for (int i=0;i<10;i++)
			{
				board->SetCell(body[0].x+i,body[0].y,RESERVED);
			}

		}
		
		void ChangeDirection(TDirection newDir)
		{
			// change direction only if new one is *not* opposite current one
			// and is different from current one
			if (!( 
				( (dir==DIR_UP) && (newDir==DIR_DOWN || newDir==DIR_UP) ) ||
				( (dir==DIR_DOWN) && (newDir==DIR_UP || newDir==DIR_DOWN) ) ||
				( (dir==DIR_LEFT) && (newDir==DIR_RIGHT || newDir==DIR_LEFT) ) ||
				( (dir==DIR_RIGHT) && (newDir==DIR_LEFT || newDir==DIR_RIGHT) ) 
		   )) {
				dir = newDir;
				timeLeft=0;
			}
		}

		void Think(CScoreboard *scoreboard)
		{
			timeLeft--;
			if (timeLeft<=0)
			{
				timeLeft=delay;
				Step(scoreboard);
			}		
		}
		
		
		void Draw(SDL_Surface *screen,bool fullRedraw)
		{
			// 1: delete points marked to be deleted
			CPoint *p = NULL;
				bool exit = false;
				while (!exit) {
					p = pointsToDelete->Pop();
					if (p==NULL) {
						exit = true;
					} else {
						board->ClearCell(screen,p->x,p->y);
						delete p;
					}
				}
				
			if (fullRedraw) {
				int i=last;
				bool done=false;
				while (Previous(i)!=first) {
					DrawPoint(screen,i);
					i=Next(i);
				}
			} else {
				// 2: redraw points marked to be redrawed
				DrawPoint(screen,last); // the new last one, changed from 'body' to 'end'
				DrawPoint(screen,first); // the new head
				DrawPoint(screen,Previous(first)); // the old first, changed from 'head' to 'body'	
			}		
		
		}
		
		bool HasCollided() { return collided; };
		bool LostLife() { return --lifes>=0; };
		int GetLifes() { return lifes; }


	private:
		
		CBitmap *bmp_body_h;
		CBitmap *bmp_body_v;
		CBitmap *bmp_body_ul;
		CBitmap *bmp_body_ur;
		CBitmap *bmp_body_dl;
		CBitmap *bmp_body_dr;
		CBitmap *bmp_head_l;
		CBitmap *bmp_head_r;
		CBitmap *bmp_head_u;
		CBitmap *bmp_head_d;
		CBitmap *bmp_end_l;
		CBitmap *bmp_end_r;
		CBitmap *bmp_end_u;
		CBitmap *bmp_end_d;
		
		CBoard *board;
		CPoint *body;
		CPointList *pointsToDelete;

		int first, last;
		int grow;
		bool collided;
		TDirection dir,desiredDir;
		int timeLeft;
		
		void DrawPoint(SDL_Surface *screen,int p)
		{
			CBitmap *bitmap;
			bitmap = bmp_body_h;
	
			if (p==first) { 
				// draw head
				int prevX=body[Previous(p)].x;
				int prevY=body[Previous(p)].y;		
				if (prevX>body[p].x)	      bitmap = bmp_head_l;
				else if (prevX<body[p].x)  bitmap = bmp_head_r;
				else if (prevY>body[p].y) 	bitmap = bmp_head_u;
				else                       bitmap = bmp_head_d;
			}
			else if (p==last) 
			{
				// draw end of the tail
				int nextX=body[Next(p)].x;
				int nextY=body[Next(p)].y;
				if (nextX>body[p].x)       bitmap = bmp_end_r;
				else if (nextX<body[p].x)  bitmap = bmp_end_l;
				else if (nextY>body[p].y)  bitmap = bmp_end_d;
				else                       bitmap = bmp_end_u;
			}	
			else
			{
				// draw body
				int prevX=body[Previous(p)].x;
				int prevY=body[Previous(p)].y;
				int nextX=body[Next(p)].x;
				int nextY=body[Next(p)].y;
		
				int tempX = body[p].x-prevX;
				int tempY = body[p].y-prevY;
		
		
				if (nextX>body[p].x) { // goes right
					if (prevY<body[p].y) {
						bitmap = bmp_body_ur;
					} else if (prevY>body[p].y) {
						bitmap = bmp_body_dr;
					} else {
						bitmap = bmp_body_h;
					}
				} else
				if (nextX<body[p].x) { // goes left
					if (prevY<body[p].y) {
						bitmap = bmp_body_ul;
					} else if (prevY>body[p].y) {
						bitmap = bmp_body_dl;
					} else {
						bitmap = bmp_body_h;
					}
				} else
				if (nextY>body[p].y) { // goes down
					if (prevX<body[p].x) {
						bitmap = bmp_body_dl;
					} else if (prevX>body[p].x) {
						bitmap = bmp_body_dr;
					} else {
						bitmap = bmp_body_v;
					}
				} else
				if (nextY<body[p].y) { // goes up
					if (prevX<body[p].x) {
						bitmap = bmp_body_ul;
					} else if (prevX>body[p].x) {
						bitmap = bmp_body_ur;
					} else {
						bitmap = bmp_body_v;
					}
				}
			}
	
			board->DrawCell(screen,body[p].x,body[p].y,bitmap);
			if (!collided) { // if has collided, draw the head but not set it!
				board->SetCell(body[p].x,body[p].y,SNAKE);
			}

		}
		
		
		void DeletePoint(int x, int y)
		{
			// 1: set the point in the board to 'empty'
			board->SetCell(x,y,EMPTY);
	
			// 2: push the point into the stack to undraw it later
			CPoint p;
			p.x = x;
			p.y = y;
			pointsToDelete->Push(p);		
		}
		
		int Previous(int p) { return (p+SNAKE_LENGTH-1)%SNAKE_LENGTH; }
		int Next(int p) { return (p+1)%SNAKE_LENGTH; }

		int lifes;

		bool Step(CScoreboard *scoreboard) // returns false if collides
		{
			if (collided) {
				return false;
			}
	
			// if snake is not moving (dir==none), exit function
			if (dir==DIR_NONE) {
				return true;
			}
	
		
			// 1: create a new point followind direction of head
			// TO DO:  check if body array is full
			// HACK:   width a high value of SNAKE_LENGTH, it will ¿never? be full
			int newX = body[first].x;
			int newY = body[first].y;	
			switch (dir)
			{
				case DIR_LEFT:
					newX--;
					break;
				case DIR_RIGHT:
					newX++;
					break;
				case DIR_UP:
					newY--;
					break;
				case DIR_DOWN:
					newY++;
					break;
			}
	
			if (!board->CanMoveTo(newX,newY))
			{
				collided = true;
			}
			first = Next(first);
			body[first].x=newX;
			body[first].y=newY;
	

		// 2: check is there is food on new position
		if (board->ThereIsFood(body[first].x,body[first].y))
		{
			grow++;
			scoreboard->IncScore(1);
		}
			
		// 3: remove last point of the tail
		//    is snake is growing, don't remove it	
		if (grow>0)
		{
			grow--;
		}		
		else
		{
			if (Next(last)!=first) // this should always be true
			{
				DeletePoint(body[last].x,body[last].y);
				last = Next(last);
			}
		}

		return !collided;
	}
		
			

};

#endif
