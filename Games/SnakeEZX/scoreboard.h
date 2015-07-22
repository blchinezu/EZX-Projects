#define LEVEL_MARGIN_X 10
#define LEVEL_MARGIN_Y 5
#define SCORE_MARGIN_X 70
#define SCORE_MARGIN_Y 5

class CScoreboard
{
	private:
		CBitmap *bmp_bg, *bmp_life;
		TTF_Font *font1, *font2;
		SDL_Surface *s_level, *s_score;
		
		bool redrawBackground;
		bool redrawScore;
		bool redrawLifes;
		bool redrawLevel;
		
		int lifes;
		int score;
		int level;		
		
		void DrawBackground(SDL_Surface *screen)
		{
			redrawBackground = false;
	
			SDL_Rect r;

			// 1: draw background
			bmp_bg->draw(screen,0,0);	
	
			// 2: draw score and level indicators
			r.x = SCORE_MARGIN_X;
			r.y = SCORE_MARGIN_Y;
			r.w = s_score->w;
			r.h = s_score->h;
			SDL_BlitSurface(s_score,NULL,screen,&r);
			r.x = LEVEL_MARGIN_X;
			r.y = LEVEL_MARGIN_Y;
			r.w = s_level->w;
			r.h = s_level->h;
			SDL_BlitSurface(s_level,NULL,screen,&r);
		}

		void DrawScore(SDL_Surface *screen)
		{
			redrawScore = false; 
	
			char *text = new char[5];
			SDL_Color c = {255,255,255,0};
			SDL_Rect r;
			SDL_Surface *s;
			
			sprintf(text, "%d", score);
			s = TTF_RenderText_Blended(font2,text, c);
	
			r.x = SCORE_MARGIN_X+s_score->w; 
			r.y = SCORE_MARGIN_Y;
			r.w = 40;
			r.h = s->h;

			SDL_FillRect(screen, &r,0); // clear area	

			r.w = s->w;
			
			SDL_BlitSurface(s,NULL,screen,&r);
			SDL_FreeSurface(s);		
		}

		void DrawLevel(SDL_Surface *screen)
		{
			redrawLevel = false;
	
			char *text = new char[20];
			SDL_Color c = {255,255,255,0};
			SDL_Rect r;
			SDL_Surface *s;
			
			sprintf(text, "%d", level);
			s = TTF_RenderText_Blended(font2,text, c);
	
			r.x = LEVEL_MARGIN_X+s_level->w; 
			r.y = LEVEL_MARGIN_Y;
			r.w = 25;
			r.h = s->h;
	
			SDL_FillRect(screen, &r,0); // clear area	
	
			r.w = s->w;
			
			SDL_BlitSurface(s,NULL,screen,&r);
			SDL_FreeSurface(s);		
		}
		
		void DrawLifes(SDL_Surface *screen)
		{
			redrawLifes = false;
	
			int x1=0;
			int y1=5;
			int y2=25;
			int x2=310;
	
			int w=10; // TO DO: change 10 for bitmap->w
			int margin=3; // TO DO: change 10 for 10+bitmap->w
	
			int lifesToDraw = (lifes>10?10:lifes); 
			// TO DO: change 10 for max_lifes_to_draw
	
			x1=x2-(lifesToDraw*(margin+w)); // HACK: never draw more than 10 lifes

			int maxX1=x2-(10*(margin+w)); // TO DO: change 10 for max_lifes_to_draw
	
			SDL_Rect r;
			r.x = maxX1;
			r.y = y1; 
			r.w = x2-x1;
			r.h = y2-y1;
			SDL_FillRect(screen, &r,0); // clear area	
		
			for (int i=0; i<lifesToDraw; i++)
			{
				bmp_life->draw(screen,x1+i*(margin+w)+margin,y1);
			}		
		}	
		
	public:
		CScoreboard(TTF_Font *_font1, TTF_Font *_font2)
		{
			font1 = _font1;
			font2 = _font2;
	
			bmp_bg = new CBitmap("pixmaps/scoreboard_bg.bmp");
			bmp_life = new CBitmap("pixmaps/life.bmp");	
	
			SDL_Color color1 = {255,255,255,0};
			SDL_Color color2 = {140,140,140,0};
			s_level = TTF_RenderText_Blended(font2,"Level: ", color2);
			s_score = TTF_RenderText_Blended(font2,"Score: ", color2);
		}
		
		~CScoreboard()
		{
			delete bmp_bg;
			delete bmp_life;
			SDL_FreeSurface(s_level);
			SDL_FreeSurface(s_score);
		}
	
		void Init()
		{
			score = 0;
			level = 1;
			lifes = 3;

			redrawBackground = true;;
			redrawScore = true;
			redrawLifes = true;
			redrawLevel = true;
		}

		void Think()
		{
		}
		
		
		void Draw(SDL_Surface *screen, bool fullRedraw)
		{
			if (fullRedraw || redrawBackground) DrawBackground(screen);
			if (fullRedraw || redrawLevel) DrawLevel(screen);
			if (fullRedraw || redrawScore) DrawScore(screen);
			if (fullRedraw || redrawLifes) DrawLifes(screen);
		}
		
		void IncScore(int n)
		{
			score += n;
			redrawScore = true;	
			if (score%50==0) IncLifes(1);
		}
		
		void IncLifes(int n)
		{
			lifes += 1;
			redrawLifes = true;
		}
		
		void IncLevel()
		{
			level++;
			redrawLevel = true;
		}

		void LostLife()
		{
			lifes -= 1;
			redrawLifes = true;
		}

		int GetScore() { return score; };
		int GetLifes() { return lifes; };
		int GetLevel() { return level; };
		
		
};
