#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include "main.h"

#define LEFT	1
#define RIGHT	2
#define UP		3
#define DOWN	4


class GRAPHICS {
	public:
		SDL_Surface*	Screen;
		
	public:
		GRAPHICS();

	// General Surface Operations
		SDL_Surface*	NewSurface				(int, int, int=255);
	
		void			Blank					(SDL_Surface*);
		void			AddSurface				(int, int, SDL_Surface*, SDL_Surface*, SDL_Rect*);
		void			AddSurfaceCore			(int, int, SDL_Surface*, SDL_Surface*, SDL_Rect*);
		void			AddSurfaceCentered		(SDL_Surface*, SDL_Surface*, SDL_Rect*);
		void			AddSurfaceCenteredX		(int, SDL_Surface*, SDL_Surface*, SDL_Rect*);
		void			AddSurfaceCenteredY		(int, SDL_Surface*, SDL_Surface*, SDL_Rect*);
		void			AddSurfaceLeft			(int, SDL_Surface*, SDL_Surface*, SDL_Rect*);
		void			AddSurfaceRight			(int, SDL_Surface*, SDL_Surface*, SDL_Rect*);
		void			AddSurfaceTop			(int, SDL_Surface*, SDL_Surface*, SDL_Rect*);
		void			AddSurfaceTopLeft		(SDL_Surface*, SDL_Surface*, SDL_Rect*);
		void			AddSurfaceTopRight		(SDL_Surface*, SDL_Surface*, SDL_Rect*);
		void			AddSurfaceBottom		(int, SDL_Surface*, SDL_Surface*, SDL_Rect*);
		void			AddSurfaceBottomLeft	(SDL_Surface*, SDL_Surface*, SDL_Rect*);
		void			AddSurfaceBottomRight	(SDL_Surface*, SDL_Surface*, SDL_Rect*);
		void			AddSurfaceTransparent	(SDL_Surface*, SDL_Surface*);
		SDL_Surface*	CopySurface				(SDL_Surface*);
		SDL_Rect		SetClip					(int, int, int, int);
		bool			Update					(SDL_Surface*, int, int, int, int);
		bool			FullUpdate				(SDL_Surface*);
		void			ReleaseSurface			(SDL_Surface*);
		
	// Images
		SDL_Surface*	LoadImage				(char*);
	
	// Fonts
		TTF_Font*		LoadFont				(char*, int);
		int				FontLineSpacing			(TTF_Font*);
		SDL_Surface*	Text					(TTF_Font*, char*, int, int, int);
		void			ReleaseFont				(TTF_Font*);
		
	// Effects
		void			SlideScreen				(int, SDL_Surface*, SDL_Surface* scr);

};

#endif /* _GRAPHICS_H_ */
