
class CMessageBox {
	private:
		TTF_Font *font1;
		TTF_Font *font2;
		SDL_Surface *text1;
		SDL_Surface *text2;
		CBitmap *bmp_bg;
		
	public:
		
		CMessageBox(TTF_Font *_font1, TTF_Font *_font2,
			char  *_mainText=NULL, char *_secondaryText=NULL)
		{
			font1 = _font1;
			font2 = _font2;
			text1 = NULL;
			text2 = NULL;
			if (_mainText!=NULL) SetMainText(_mainText);
			if (_secondaryText!=NULL) SetSecondaryText(_secondaryText);		
			bmp_bg = new CBitmap("pixmaps/dialog_bg.bmp");
		}

		~CMessageBox()
		{
			SDL_FreeSurface(text1);	
			SDL_FreeSurface(text2);			
			delete bmp_bg;
		}
		
		void SetMainText(char *text)
		{
			SDL_Color c = {255,255,255,0};
			text1 = TTF_RenderText_Blended(font1,text, c);		
		}
		
		void SetSecondaryText(char *text)
		{
			SDL_Color c = {140,140,140,0};
			text2 = TTF_RenderText_Blended(font2,text, c);
		}
		
		void Draw(SDL_Surface *screen)
		{
			int marginTop = 20;
			int marginBottom = 20;

			int w=bmp_bg->w();
			int h=bmp_bg->h();
		
			int x = (screen->w-w)/2;
			int y = (screen->h-h)/2;
	
			// 2: display box
			bmp_bg->draw(screen,x,y);

			SDL_Rect textRect1 = {x+(w-text1->w)/2,y+marginTop,0,0};
			SDL_Rect textRect2 = {x+(w-text2->w)/2,y+h-marginBottom-text2->h,0,0};
	
			SDL_BlitSurface(text1,NULL,screen,&textRect1);
			SDL_BlitSurface(text2,NULL,screen,&textRect2);			
		}
};
