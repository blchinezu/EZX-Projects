#include "graphics.h"


GRAPHICS::GRAPHICS() { Screen=NULL; };

/** General Surface Operations **/

// NewSurface
SDL_Surface* GRAPHICS::NewSurface(int w, int h, int transp) {
	if(transp<1) dbg(WARNING, "Surface is invisible.");
	
	SDL_Surface* holder=NULL;
	SDL_Surface* holderOptimized=NULL;
	holder = SDL_CreateRGBSurface(SDL_HWSURFACE | SDL_SRCALPHA,w,h,ScreenBpp,0,0,0,transp);
	if(holder==NULL) {
		dbg(WARNING, "Could not create new surface.");
		return NULL;
	}
	if(transp<255) {
		holderOptimized = SDL_DisplayFormatAlpha(holder);
		if(holderOptimized==NULL)
			dbg(WARNING, "Could not optimize new surface.");
		return holderOptimized;
	} /*else {
		holderOptimized = SDL_DisplayFormat(holder);
		SDL_SetColorKey( holderOptimized, SDL_SRCCOLORKEY, SDL_MapRGB(holderOptimized->format, 0xFF, 0, 0xFF));
	}*/
	return holder;
}

// Blank
void GRAPHICS::Blank(SDL_Surface* destination) {
	SDL_FillRect(destination, 0, 0L);
}
// AddSurface
void GRAPHICS::AddSurface(int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip) {
	AddSurfaceCore(x, y, source, destination, clip);
}
// AddSurface
void GRAPHICS::AddSurfaceCore(int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip) {
	//dbg(INFO, "Blitting surface");
	SDL_Rect offset={x, y, 0, 0};
	SDL_BlitSurface(source, clip, destination, &offset);
}
// AddSurfaceCentered
void GRAPHICS::AddSurfaceCentered(SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip)
{ AddSurface((destination->w-source->w)/2, (destination->h-source->h)/2, source, destination, clip); }
// AddSurfaceCenteredX
void GRAPHICS::AddSurfaceCenteredX(int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip) 
{	AddSurface((destination->w-source->w)/2, y, source, destination, clip); }
// AddSurfaceCenteredY
void GRAPHICS::AddSurfaceCenteredY(int x, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip) 
{	AddSurface(x, (destination->h-source->h)/2, source, destination, clip); }
// AddSurfaceLeft
void GRAPHICS::AddSurfaceLeft(int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip) 
{	AddSurface(0, y, source, destination, clip); }
// AddSurfaceRight
void GRAPHICS::AddSurfaceRight(int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip) 
{	AddSurface(destination->w-source->w, y, source, destination, clip); }
// AddSurfaceTop
void GRAPHICS::AddSurfaceTop(int x, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip) 
{	AddSurface(x, 0, source, destination, clip); }
// AddSurfaceTopLeft
void GRAPHICS::AddSurfaceTopLeft(SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip) 
{	AddSurface(0, 0, source, destination, clip); }
// AddSurfaceTopRight
void GRAPHICS::AddSurfaceTopRight(SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip) 
{	AddSurface(destination->w-source->w, 0, source, destination, clip); }
// AddSurfaceBottom
void GRAPHICS::AddSurfaceBottom(int x, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip) 
{	AddSurface(x, destination->w-source->h, source, destination, clip); }
// AddSurfaceBottomLeft
void GRAPHICS::AddSurfaceBottomLeft(SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip) 
{	AddSurface(0, destination->h-source->h, source, destination, clip); }
// AddSurfaceBottomRight
void GRAPHICS::AddSurfaceBottomRight(SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip) 
{	AddSurface(destination->w-source->w, destination->h-source->h, source, destination, clip); }
// AddSurfaceTransparent
void AddSurfaceTransparent(SDL_Surface* source, SDL_Surface* destination) {
	SDL_LockSurface(destination); SDL_LockSurface(source);
	memcpy(destination->pixels, source->pixels, (source->w * source->h * source->format->BytesPerPixel));
	SDL_UnlockSurface(destination); SDL_UnlockSurface(source);
}
// CopySurface
SDL_Surface* GRAPHICS::CopySurface(SDL_Surface* source) {
	SDL_Surface* destination = NULL;
	destination = SDL_CreateRGBSurface(SDL_HWSURFACE|SDL_SRCALPHA, source->w , source->h, 
										source->format->BitsPerPixel, 
										source->format->Rmask, source->format->Gmask, 
										source->format->Bmask, source->format->Amask);
	if(!destination) { dbg(WARNING, "Could not create new surface."); return NULL; }
	SDL_LockSurface(destination);
	SDL_LockSurface(source);	
	memcpy(destination->pixels, source->pixels, (source->w * source->h * source->format->BytesPerPixel));
	SDL_UnlockSurface(destination);
	SDL_UnlockSurface(source);
	return destination;
}
// SetClip
SDL_Rect GRAPHICS::SetClip(int x, int y, int w, int h) {
	SDL_Rect clip; clip.x=x; clip.y=y; clip.w=w; clip.h=h; return clip;
}
// Update
bool GRAPHICS::FullUpdate(SDL_Surface* destination) {
	if(SDL_Flip(destination) == -1) { dbg(ERROR, "SDL_Flip function failed."); return false; }
	return true;
}
bool GRAPHICS::Update(SDL_Surface* destination, int x, int y, int w, int h) {
	SDL_UpdateRect(destination, x, y, w, h);
	return true;
}
// ReleaseSurface
void GRAPHICS::ReleaseSurface(SDL_Surface* destination) {
	if(destination) SDL_FreeSurface(destination);
}



/** IMAGE **/

// LoadImage
SDL_Surface* GRAPHICS::LoadImage(char* filename) {
	dbg(INFO, "Loading image: ",filename);
	SDL_Surface* loadedImage = NULL;
	SDL_Surface* optimizedImage = NULL;
	loadedImage = IMG_Load(filename);
	if(loadedImage != NULL) {
		optimizedImage = SDL_DisplayFormatAlpha(loadedImage);
		SDL_FreeSurface(loadedImage);
	} else { dbg(WARNING, "Could not load image: ", filename); return NULL; }
	return optimizedImage;
}



/** FONT **/

// LoadFont
TTF_Font* GRAPHICS::LoadFont(char* filename, int size) {
		dbg(INFO, "Loading font: ",filename);
		TTF_Font* holder = NULL;
		holder = TTF_OpenFont( filename, size );
		if(holder == NULL) { dbg(WARNING, "Could not load font:",filename); return false; }
		return holder;
}
// FontLineSpacing
int GRAPHICS::FontLineSpacing(TTF_Font* font) {
		return TTF_FontLineSkip(font);
}
// Text
SDL_Surface* GRAPHICS::Text(TTF_Font* font, char* text, int R, int G, int B) {
		dbg(INFO, "Creating text surface: ",text);
		SDL_Surface* holder = NULL;
		SDL_Color textColor = {R, G, B};
		holder = TTF_RenderText_Blended(font, text, textColor);
		if(holder == NULL) { dbg(WARNING, "Can't create text surface: ", text); return false; }
		return holder;
}
// ReleaseFont
void GRAPHICS::ReleaseFont(TTF_Font* destination) {
		if(destination) TTF_CloseFont(destination);
}


/** Effects **/


// Slide Screen
void GRAPHICS::SlideScreen(int x, SDL_Surface* s2, SDL_Surface* scr) {
	if( scr->w == s2->w && scr->h == s2->h ) {
		SDL_Surface* s1 = CopySurface(scr);
		switch(x) {
			case LEFT:
				for(int i=SETT_SLIDE_SPD; i<=scr->w; i+=SETT_SLIDE_SPD) {
					SDL_Rect clip1 = SetClip(i, 0, scr->w-i, scr->h);
					AddSurfaceTopLeft(s1, scr, &clip1);
					SDL_Rect clip2 = SetClip(0, 0, i, scr->h);
					AddSurface(scr->w-i, 0, s2, Screen, &clip2);
					FullUpdate(scr);
				}
				break;
			case RIGHT:
				for(int i=SETT_SLIDE_SPD; i<=scr->w; i+=SETT_SLIDE_SPD) {
					SDL_Rect clip1 = SetClip(0, 0, scr->w-i, scr->h);
					AddSurface(i, 0, s1, scr, &clip1);
					SDL_Rect clip2 = SetClip(scr->w-i, 0, i, scr->h);
					AddSurface(0, 0, s2, Screen, &clip2);
					FullUpdate(scr);
				}
				break;
			case UP:
				for(int i=SETT_SLIDE_SPD; i<=scr->h; i+=SETT_SLIDE_SPD) {
					SDL_Rect clip1 = SetClip(0, i, scr->w, scr->h-i);
					AddSurface(0, 0, s1, scr, &clip1);
					SDL_Rect clip2 = SetClip(0, 0, scr->w, i);
					AddSurface(0, scr->h-i, s2, Screen, &clip2);
					FullUpdate(scr);
				}
				break;
			case DOWN:
				for(int i=SETT_SLIDE_SPD; i<=scr->h; i+=SETT_SLIDE_SPD) {
					SDL_Rect clip1 = SetClip(0, 0, scr->w, scr->h-i);
					AddSurface(0, i, s1, scr, &clip1);
					SDL_Rect clip2 = SetClip(0, scr->h-i, scr->w, i);
					AddSurface(0, 0, s2, Screen, &clip2);
					FullUpdate(scr);
				}
				break;
		}
		AddSurface(0, 0, s2, scr, NULL);
		FullUpdate(scr);
	} else {
		dbg(ERROR, "Could not slide screen because of different sdl surface sizes.");
	}
}

