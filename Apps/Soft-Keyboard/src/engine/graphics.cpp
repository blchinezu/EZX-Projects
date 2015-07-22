#include "graphics.h"

Graphics::Graphics() { }

// Init Class Vars
bool			Graphics::NeedScreenUpdate	= false;
SDL_Surface*	Graphics::Screen			= NULL;
SDL_Surface*	Graphics::background		= NULL;
TTF_Font*		Graphics::font_big			= NULL;
TTF_Font*		Graphics::font_normal		= NULL;
TTF_Font*		Graphics::font_small		= NULL;



/* Main functions */
bool Graphics::Init() {
	#ifdef DEBUG
		std::cout<<" > Initializing Screen: "<<Config::ScreenWidth<<"x"<<Config::ScreenHeight<<" @ "<<Config::ScreenBpp<<" bits\n";
	#endif
	Screen = SDL_SetVideoMode(Config::ScreenWidth, Config::ScreenHeight, Config::ScreenBpp, SDL_HWSURFACE | SDL_DOUBLEBUF);
	if(Screen == NULL) {
		std::cout<<" >> ERROR: Failed setting video mode: "<<Config::ScreenWidth<<"x"<<Config::ScreenHeight<<" @ "<<Config::ScreenBpp<<" bits\n";
		return false; }
	return true;
}
bool Graphics::LoadData() {
	#ifdef DEBUG
		std::cout<<"   > Loading Graphics Data - BEGIN\n";
	#endif
	background		= LoadImage("background", false);
	font_big		= LoadFont("font.ttf", Config::FontSizeBig);
	font_normal		= LoadFont("font.ttf", Config::FontSizeNormal);
	font_small		= LoadFont("font.ttf", Config::FontSizeSmall);
	#ifdef DEBUG
		std::cout<<"   > Loading Graphics Data - ENDED\n";
	#endif
	if(!background || !font_big || !font_normal || !font_small) return false;
	return true;
}
void Graphics::Cleanup() {
	#ifdef DEBUG
		std::cout<<"   > Releasing Graphics Data\n";
	#endif
	Blank(Screen); Update(Screen);
	ReleaseFont(font_big);
	ReleaseFont(font_normal);
	ReleaseFont(font_small);
	ReleaseSurface(background);
	ReleaseSurface(Screen);
}
bool Graphics::UpdateScreen() { if(NeedScreenUpdate) { NeedScreenUpdate=false; return Update(Screen); }; return true; }



/* General Surface Operations */

// NewTransparentSurface
SDL_Surface* Graphics::NewTransparentSurface(int w, int h, int transp) {
	#ifdef DEBUG
		std::cout<<" NewTransparentSurface("<<w<<", "<<h<<", "<<transp<<")";
		if(transp<1) std::cout<<" >> WARNING: Surface is invisible.\n";
	#endif
	SDL_Surface* holder=NULL;
	SDL_Surface* holderOptimized=NULL;
	holder = SDL_CreateRGBSurface(SDL_HWSURFACE | SDL_SRCALPHA,w,h,Config::ScreenBpp,0,0,0,transp);
	if(holder==NULL) { std::cout<<" >> WARNING: Could not create new transparent surface.\n"; return NULL; }
	if(transp<255 && Config::AllowAlphaImages) {
		holderOptimized = SDL_DisplayFormatAlpha(holder);
	} else {
		holderOptimized = SDL_DisplayFormat(holder);
		SDL_SetColorKey( holderOptimized, SDL_SRCCOLORKEY, SDL_MapRGB(holderOptimized->format, 0xFF, 0, 0xFF));
	}
	#ifdef DEBUG
		std::cout<<" > Created transparent surface: "<<holderOptimized<<"\n";
	#endif
	return holderOptimized;
}
// Blank
void Graphics::Blank(SDL_Surface* destination) {
	#ifdef DEBUG
		std::cout<<" Blank("<<destination<<")";
	#endif
	SDL_FillRect(destination, 0, 0L);
	#ifdef DEBUG
		std::cout<<" > Made surface ("<<destination<<") black\n";
	#endif
}
// AddSurface
void Graphics::AddSurface(int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip) {
	#ifdef DEBUG
		std::cout<<" AddSurface("<<x<<", "<<y<<", "<<source<<", "<<destination<<", "<<clip<<")\n";
	#endif
	AddSurfaceCore(x, y, source, destination, clip);
}
// AddSurface
void Graphics::AddSurfaceCore(int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip) {
	SDL_Rect offset={x, y, 0, 0};
	SDL_BlitSurface(source, clip, destination, &offset);
}
// AddSurfaceCentered
void Graphics::AddSurfaceCentered(SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip)
{ AddSurface((destination->w-source->w)/2, (destination->h-source->h)/2, source, destination, clip); }
// AddSurfaceCenteredX
void Graphics::AddSurfaceCenteredX(int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip) 
{	AddSurface((destination->w-source->w)/2, y, source, destination, clip); }
// AddSurfaceCenteredY
void Graphics::AddSurfaceCenteredY(int x, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip) 
{	AddSurface(x, (destination->h-source->h)/2, source, destination, clip); }
// AddSurfaceLeft
void Graphics::AddSurfaceLeft(int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip) 
{	AddSurface(0, y, source, destination, clip); }
// AddSurfaceRight
void Graphics::AddSurfaceRight(int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip) 
{	AddSurface(destination->w-source->w, y, source, destination, clip); }
// AddSurfaceTop
void Graphics::AddSurfaceTop(int x, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip) 
{	AddSurface(x, 0, source, destination, clip); }
// AddSurfaceTopLeft
void Graphics::AddSurfaceTopLeft(SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip) 
{	AddSurface(0, 0, source, destination, clip); }
// AddSurfaceTopRight
void Graphics::AddSurfaceTopRight(SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip) 
{	AddSurface(destination->w-source->w, 0, source, destination, clip); }
// AddSurfaceBottom
void Graphics::AddSurfaceBottom(int x, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip) 
{	AddSurface(x, destination->w-source->h, source, destination, clip); }
// AddSurfaceBottomLeft
void Graphics::AddSurfaceBottomLeft(SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip) 
{	AddSurface(0, destination->h-source->h, source, destination, clip); }
// AddSurfaceBottomRight
void Graphics::AddSurfaceBottomRight(SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip) 
{	AddSurface(destination->w-source->w, destination->h-source->h, source, destination, clip); }
// AddSurfaceTransparent
void AddSurfaceTransparent(SDL_Surface* source, SDL_Surface* destination) {
	SDL_LockSurface(destination); SDL_LockSurface(source);
	memcpy(destination->pixels, source->pixels, (source->w * source->h * source->format->BytesPerPixel));
	SDL_UnlockSurface(destination); SDL_UnlockSurface(source);
}
// CopySurface
SDL_Surface* Graphics::CopySurface(SDL_Surface* source) {
	#ifdef DEBUG
		std::cout<<" CopySurface("<<source<<")\n";
	#endif
	SDL_Surface* destination = NULL;
	destination = SDL_CreateRGBSurface(SDL_HWSURFACE|SDL_SRCALPHA, source->w , source->h, 
										source->format->BitsPerPixel, 
										source->format->Rmask, source->format->Gmask, 
										source->format->Bmask, source->format->Amask);
	if(!destination) { std::cout<<" >> WARNING: Could not create new surface.\n"; return NULL; }
	SDL_LockSurface(destination);
	SDL_LockSurface(source);	
	memcpy(destination->pixels, source->pixels, (source->w * source->h * source->format->BytesPerPixel));
	SDL_UnlockSurface(destination);
	SDL_UnlockSurface(source);
	#ifdef DEBUG
		std::cout<<" > Copied surface ["<<source<<"] to surface ["<<destination<<"]\n";
	#endif
	return destination;
}
// SetClip
SDL_Rect Graphics::SetClip(int x, int y, int w, int h) {
	SDL_Rect clip; clip.x=x; clip.y=y; clip.w=w; clip.h=h; return clip;
}
// Update
bool Graphics::Update(SDL_Surface* destination) {
	#ifdef DEBUG
		std::cout<<" Update("<<destination<<")\n";
	#endif
	if(SDL_Flip(destination) == -1) { std::cout << " >> ERROR: Surface could not be updated. (Surface: "<<destination<<")\n"; return false; }
	return true;
}
// ReleaseSurface
void Graphics::ReleaseSurface(SDL_Surface* destination) {
	#ifdef DEBUG
		std::cout<<" ReleaseSurface("<<destination<<")\n";
	#endif
	if(destination) { SDL_FreeSurface(destination);
	} else {
		#ifdef DEBUG
			std::cout<<" > Surface already NULL\n";
		#endif
	}
}



/* IMAGE */

// LoadImage
SDL_Surface* Graphics::LoadImage(std::string filename, bool alpha) {
	#ifdef DEBUG
		std::cout<<" LoadImage("<<filename.c_str()<<", "<<alpha<<")";
	#endif
	SDL_Surface* loadedImage = NULL;
	SDL_Surface* optimizedImage = NULL;
	std::string fila; fila.clear(); fila.append( Config::ImagesPath ); fila.append(filename.c_str());
	#ifdef USE_SDL_image
		fila.append(".png"); loadedImage = IMG_Load(fila.c_str());
	#else
		fila.append(".bmp"); loadedImage = SDL_LoadBMP(fila.c_str());
	#endif
	if(loadedImage != NULL) {
		#ifdef USE_SDL_image
		if(alpha && Config::AllowAlphaImages) {
			optimizedImage = SDL_DisplayFormatAlpha(loadedImage);
		} else {
		#endif
			optimizedImage = SDL_DisplayFormat(loadedImage);
			SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, SDL_MapRGB(optimizedImage->format, 0xFF, 0, 0xFF));
		#ifdef USE_SDL_image
		}
		#endif
		SDL_FreeSurface(loadedImage);
	} else { std::cout << " >> WARNING: Could not load image: " << fila.c_str() << "\n"; std::cerr << ::SDL_GetError() << std::endl; return NULL; }
	#ifdef DEBUG
		std::cout<<" > Loaded image ("<<fila.c_str()<<") to surface: "<<optimizedImage<<"\n";
	#endif
	return optimizedImage;
}



/* FONT */

// LoadFont
TTF_Font* Graphics::LoadFont(std::string filename, int size) {
		#ifdef DEBUG
			std::cout<<" LoadFont("<<filename.c_str()<<", "<<size<<")";
		#endif
		std::string fila; fila.clear(); fila.append(Config::DataPath); fila.append(filename.c_str());
		TTF_Font* holder = NULL; holder = TTF_OpenFont( fila.c_str(), size );
		if(holder == NULL) { std::cout<<" >> WARNING: Could not load font: "<<fila.c_str()<<"\n"; return false; }
		#ifdef DEBUG
			std::cout<<" > Loaded font ("<<fila.c_str()<<") to surface: "<<holder<<"\n";
		#endif
		return holder;
}
// FontLineSpacing
int Graphics::FontLineSpacing(TTF_Font* font) {
		#ifdef DEBUG
			std::cout<<" FontLineSpacing("<<font<<")\n";
		#endif
		return TTF_FontLineSkip(font);
}
// Text
SDL_Surface* Graphics::Text(TTF_Font* font, std::string text, int R, int G, int B) {
		#ifdef DEBUG
			std::cout<<" Text("<<font<<", "<<text.c_str()<<", "<<R<<", "<<G<<", "<<B<<")";
		#endif
		SDL_Surface* holder = NULL;
		SDL_Color textColor = {R, G, B};
		if(Config::AllowBlendedFonts)	{ holder = TTF_RenderText_Blended(font, text.c_str(), textColor); }
		else					{ holder = TTF_RenderText_Solid(font, text.c_str(), textColor); }
		if(holder == NULL) { std::cout<<" >> WARNING: Couldn't create text surface.\n"; return false; }
		#ifdef DEBUG
			std::cout<<" > Created text surface: "<<holder<<"\n";
		#endif
		return holder;
}
// ReleaseFont
void Graphics::ReleaseFont(TTF_Font* destination) {
		#ifdef DEBUG
			std::cout<<" ReleaseFont("<<destination<<")\n";
		#endif
		if(destination) { TTF_CloseFont(destination); }
}

