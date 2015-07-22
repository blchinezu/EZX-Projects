void ShowSplash() {
	int fnt_size=15, i=90, j, ld_x=0, ld_y=238;
	SDL_Surface*	text		= NULL;
	SDL_Surface*	splash		= SDL_LoadBMP("pixmaps/splash_bg.bmp");
	SDL_Surface*	ld			= SDL_LoadBMP("pixmaps/ld.bmp");
	TTF_Font*		fnt			= TTF_OpenFont("fonts/pachyderm.ttf", fnt_size);
	SDL_Color		textColor1	= { 255, 255, 255 };
	SDL_Color		textColor2	= { 160, 160, 160 };
	SDL_Rect 		of;
	
	of.x=0; of.y=0; of.w=splash->w; of.h=splash->h; SDL_BlitSurface(splash, NULL, SURFACE::SCREEN, &of);
	
	text=TTF_RenderText_Blended(fnt, "Created by", textColor2); of.x=(320-text->w)/2; of.y=i; of.w=text->w; of.h=text->h;
	SDL_BlitSurface(text, NULL, SURFACE::SCREEN, &of); SDL_FreeSurface(text); i+=fnt_size+5;
	text=TTF_RenderText_Blended(fnt, "Schuan", textColor1); of.x=(320-text->w)/2; of.y=i; of.w=text->w; of.h=text->h;
	SDL_BlitSurface(text, NULL, SURFACE::SCREEN, &of); SDL_FreeSurface(text); i+=fnt_size+15;
	
	text=TTF_RenderText_Blended(fnt, "Ported by", textColor2); of.x=(320-text->w)/2; of.y=i; of.w=text->w; of.h=text->h;
	SDL_BlitSurface(text, NULL, SURFACE::SCREEN, &of); SDL_FreeSurface(text); i+=fnt_size+5;
	text=TTF_RenderText_Blended(fnt, "BruceLee", textColor1); of.x=(320-text->w)/2; of.y=i; of.w=text->w; of.h=text->h;
	SDL_BlitSurface(text, NULL, SURFACE::SCREEN, &of); SDL_FreeSurface(text); i+=fnt_size+5;
	
	SDL_Flip(SURFACE::SCREEN);
	TTF_CloseFont(fnt);

	for(j=0; j<20; j++) {
		of.x=ld_x; of.y=ld_y; of.w=ld->w; of.h=ld->h;
		SDL_BlitSurface(ld, NULL, SURFACE::SCREEN, &of);
		SDL_Flip(SURFACE::SCREEN);
		ld_x+=ld->w;
		SDL_Delay(50);
	}
	
	SDL_FreeSurface(splash);
	SDL_FreeSurface(ld);
}
