class CHelpState : public CCustomState
{
	private:
		CBitmap *bg;

	public:		
		CHelpState(CSnakeGameInfo *info) : CCustomState(info)
		{
			bg = new CBitmap(info->trans->get(IMG_HELP));

		};
				
		void Render()
		{
			SDL_Color c1 = {255,255,255,0};
			SDL_Color c2 = {155,155,155,0};

			if (JustActivated())
			{
				bg->draw(screen,0,0);	

				SDL_Rect r;

				SDL_Surface *s = TTF_RenderText_Blended(info->bigFont,info->trans->get(TXT_HELP),c1);
				r.x = (320-s->w)>>1;
				r.y = 30;
				r.w = s->w;
				r.h = s->h;
				SDL_BlitSurface(s,NULL,screen,&r);

				s = TTF_RenderText_Blended(info->smallFont,info->trans->get(TXT_START),c2);
				r.x = (320-s->w)>>1;
				r.y = 240-s->h-4;
				r.w = s->w;
				r.h = s->h;
				SDL_BlitSurface(s,NULL,screen,&r);

				SDL_FreeSurface(s);						
			}
			
		};
		
		void KeyDown(int iKeyEnum)
		{
			ChangeToState(ST_MENU);
		};
		
		void JoyButtonDown(int iJoyButtonEnum, SDL_Joystick *joystick)
		{
			if (iJoyButtonEnum==GP2X_BUTTON_START || iJoyButtonEnum==GP2X_BUTTON_CLICK)
			{
				ChangeToState(ST_MENU);
			}			
		};
		

};

