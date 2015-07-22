class COptionsState : public CCustomState
{
	private:
		int noptions;
		int sel;
		char **options;
		CBitmap *bg;

	public:		
		COptionsState(CSnakeGameInfo *info) : CCustomState(info)
		{
			noptions = 4;
			options = new char*[noptions];
			options[0] = info->trans->get(TXT_LANGUAGE);		
			options[1] = "   ";
			options[2] = "   ";
			options[3] = info->trans->get(TXT_BACK);
			sel = 0;	
			bg = new CBitmap("pixmaps/options_bg.bmp");

		};
				
		void Render()
		{
			SDL_Color c1 = {255,255,255,0};
			SDL_Color c2 = {155,155,155,0};

			if (JustActivated())
			{
				bg->draw(screen,0,0);	

				SDL_Rect r;

				SDL_Surface *s = TTF_RenderText_Blended(info->bigFont,info->trans->get(TXT_OPTIONS),c1);
				r.x = (320-s->w)>>1;
				r.y = 30;
				r.w = s->w;
				r.h = s->h;
				SDL_BlitSurface(s,NULL,screen,&r);

				SDL_FreeSurface(s);						
				
			}
			
			
			for (int i=0; i<noptions; i++)
			{
				SDL_Surface *s = TTF_RenderText_Blended(info->bigFont,options[i],sel==i?c1:c2);

				SDL_Rect r;
				r.x = (320-s->w)>>1;
				r.y = 100+i*25;
				r.w = s->w;
				r.h = 20;

				SDL_FillRect(screen, &r, 0); // clear area	
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
			if (strcmp(options[sel],"   ") == 0) selectNext();
		}
		
		void selectPrev()
		{
			sel+=noptions;
			--(sel)%=noptions;
			if (strcmp(options[sel],"   ") == 0) selectPrev();
		}
	
		void ok()
		{
			switch (sel)
			{
				case 0: // change language
					info->trans->nextLanguage();
					break;
				case 1: // rotate screen
					break;
				case 2: 
					break;
				case 3: ChangeToState(ST_MENU);
					break;
			}
			
			// save options
			FILE *fp = fopen("snake.cfg","w");
			fprintf(fp,"%s %d\n", "lang", info->trans->getLang());
			fclose(fp);		
			

			SDL_Rect r;
			r.x = 0;
			r.y =220;
			r.w = 320;
			r.h = 40;
			SDL_FillRect(screen, &r, 0); // clear area	

			SDL_Color c2 = {155,155,155,0};
			SDL_Surface *s = TTF_RenderText_Blended(info->smallFont,info->trans->get(TXT_RESTART),c2);
			r.x = (320-s->w)>>1;
			r.y = 240-s->h-4;
			r.w = s->w;
			r.h = s->h;
			SDL_BlitSurface(s,NULL,screen,&r);

			SDL_FreeSurface(s);						
			
		}


};

