#ifndef _ENGINE_H_
#define _ENGINE_H_

#include <SDL.h>
#include <SDL_ttf.h>
#ifdef USE_SDL_image
	#include <SDL_image.h>
#endif
#include <iostream>
#include <sstream>
#include <string>
#include "timer.h"
#include "config.h"
#include "mouse.h"
#include "graphics.h"


class Engine
{
	public:
		Engine();
		static bool	Running;
		static bool	UpdateAll;

	public:
//		static bool	Init			();
		static bool	LoadData		();
		
		static void	Sleep			(int msec);
		static void	SetWindowTitle	();
		static void	ShowSplash		();
		static void	AddBackground	();
		static void ToggleUpdateAll	();
};


#endif /* _ENGINE_H_ */
