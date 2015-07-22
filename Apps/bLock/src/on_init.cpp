#include "main.h"

bool APP::Init() {
	// Init video
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
		return dbg(DIE, "Failed initializing SDL Video.");
	
	// Set video mode
	gfx.Screen = SDL_SetVideoMode(ScreenWidth, ScreenHeight, ScreenBpp, SDL_HWSURFACE | SDL_DOUBLEBUF);
	if(gfx.Screen == NULL) {
		char buff[100];
		sprintf(buff, "Failed setting video mode: %dx%d @ %d bits", ScreenWidth, ScreenHeight, ScreenBpp);
		return dbg(DIE, buff);
	}
	
	// Init font
	if(TTF_Init() < 0)
		return dbg(DIE, "Failed initializing TTF.");

	// Disable mouse cursor
	SDL_ShowCursor(SDL_DISABLE);
	
	// Blank screen
	gfx.Blank(gfx.Screen);
	
	// Load wallpaper
	char path[MAX_PATH_LEN];
	ezx.get_wallpaper(path);
	wallpaper = gfx.LoadImage(path);
	// Show wallpaper
	SDL_Rect clip = gfx.SetClip(0, 0, ScreenWidth, ScreenHeight);
	gfx.AddSurfaceTopLeft(wallpaper, gfx.Screen, &clip);
	gfx.FullUpdate(gfx.Screen);
	
	// Load Clock data
	iclock.Init (CLOCKX,CLOCKY,wallpaper);

	// Load Unlocker data
	unLock.Init(wallpaper);
	
	// Load info panel data
	infop.Init(wallpaper);
	
	
	return true;
}

