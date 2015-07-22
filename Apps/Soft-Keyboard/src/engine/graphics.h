#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include "engine.h"

class Graphics
{
	public:
		Graphics();
		
		static bool				NeedScreenUpdate;
		static SDL_Surface*		Screen;
		static SDL_Surface*		background;
		static TTF_Font*		font_big;
		static TTF_Font*		font_normal;
		static TTF_Font*		font_small;

	public:
		// General Surface Operations
			static SDL_Surface*	NewTransparentSurface	(int w, int h, int transp);
			static void			Blank					(SDL_Surface* destination);
			static void			AddSurface				(int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip);
			static void			AddSurfaceCore			(int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip);
			static void			AddSurfaceCentered		(SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip);
			static void			AddSurfaceCenteredX		(int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip);
			static void			AddSurfaceCenteredY		(int x, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip);
			static void			AddSurfaceLeft			(int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip);
			static void			AddSurfaceRight			(int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip);
			static void			AddSurfaceTop			(int x, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip);
			static void			AddSurfaceTopLeft		(SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip);
			static void			AddSurfaceTopRight		(SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip);
			static void			AddSurfaceBottom		(int x, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip);
			static void			AddSurfaceBottomLeft	(SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip);
			static void			AddSurfaceBottomRight	(SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip);
			static void			AddSurfaceTransparent	(SDL_Surface* source, SDL_Surface* destination);
			static SDL_Surface*	CopySurface				(SDL_Surface* source);
			static SDL_Rect		SetClip					(int x, int y, int w, int h);
			static bool			Update					(SDL_Surface* destination);
			static void			ReleaseSurface			(SDL_Surface* destination);
		
		// Images
			static SDL_Surface* LoadImage(std::string filename, bool alpha);
		
		// Fonts
			static TTF_Font*	LoadFont				(std::string filename, int size);
			static int			FontLineSpacing			(TTF_Font* font);
			static SDL_Surface*	Text					(TTF_Font* font, std::string text, int R, int G, int B);
			static void			ReleaseFont				(TTF_Font* destination);
		
		// Windows
			static SDL_Surface*	NewWindow				(int width, int height, int transp);
		
		// Others
			static bool			UpdateScreen			();		// Update the whole screen
			static void			ShowSplash				();		// Show Splash Screen
			static void			AddBackground			();		// Add background surface to Screen surface
		
		// Init | LoadData | Cleanup
			static bool			Init					();
			static bool			LoadData				();
			static void			Cleanup					();
};

#endif /* _GRAPHICS_H_ */
