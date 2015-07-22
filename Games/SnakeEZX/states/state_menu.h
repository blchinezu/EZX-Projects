
class CMenuState : public CCustomState
{
	private:
		int noptions;
		int sel;
		char **options;
		CBitmap *bg;

	public:		
		CMenuState(CSnakeGameInfo *info) : CCustomState(info)
		{
			noptions = 4;
			options = new char*[noptions];
			options[0] = info->trans->get(TXT_PLAY);
			options[1] = info->trans->get(TXT_OPTIONS);
			options[2] = info->trans->get(TXT_HELP);
			options[3] = info->trans->get(TXT_QUIT);
			sel = 0;	
			bg = new CBitmap("pixmaps/menu_bg.bmp");
		};
				
		void Render()
		{
			if (JustActivated())
			{
				bg->draw(screen,0,0);	
			}
			
			SDL_Color c1 = {255,255,255,0};
			SDL_Color c2 = {155,155,155,0};
			
			for (int i=0; i<noptions; i++)
			{
				SDL_Surface *s = TTF_RenderText_Blended(info->bigFont,options[i],sel==i?c1:c2);

				SDL_Rect r;
				r.x = (320-s->w)>>1;
				r.y = 100+i*25;
				r.w = s->w;
				r.h = 20;

				SDL_FillRect(screen, &r,0); // clear area	
				SDL_BlitSurface(s,NULL,screen,&r);

				SDL_FreeSurface(s);						
			}
		};
		
		void KeyDown(int iKeyEnum)
		{
			if (iKeyEnum==SDLK_RETURN)
			{
				ok();
			}
			if (iKeyEnum==SDLK_DOWN)
			{
				selectNext();
			}
			if (iKeyEnum==SDLK_UP)
			{
				selectPrev();
			}
			if (iKeyEnum==SDLK_ESCAPE)
			{
				sel=3; ok();
			}
		};
		
		void JoyButtonDown(int iJoyButtonEnum, SDL_Joystick *joystick)
		{
			if (iJoyButtonEnum==GP2X_BUTTON_START || iJoyButtonEnum==GP2X_BUTTON_CLICK)
			{
				ok();
			}		
			if (iJoyButtonEnum==GP2X_BUTTON_DOWN)
			{
				selectNext();
			}		
			if (iJoyButtonEnum==GP2X_BUTTON_UP)
			{
				selectPrev();
			}		
		};
		
	private:
		void selectNext()
		{
			++sel%=noptions;
		}
		
		void selectPrev()
		{
			sel+=noptions;
			--(sel)%=noptions;
		}
	
		void ok()
		{
			switch (sel)
			{
				case 0: ChangeToState(ST_STARTING_LEVEL);
					break;
				case 1: ChangeToState(ST_OPTIONS);
					break;
				case 2: ChangeToState(ST_HELP);
					break;
				case 3: game->Quit();
					break;
			}
		}
};

