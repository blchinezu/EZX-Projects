#ifndef _APP_H_
#define _APP_H_

#include "main.h"

// Modes
#define	ACTIVE		1
#define	IDLE		0

class APP {
	private:
		SDL_Event		Event;
		int				last_relX, last_relY;
		
	public:
		APP();
		~APP();
		bool 			Running;
		bool 			Focused;
		bool 			go_active_flag;
		int				status;
		SDL_Surface*	wallpaper;
		
		bool	Run				();
		bool	Init			();
		void	HandleEvent		();
		void		MouseMove	(int, int, int, int);
		void		MouseDown	(int, int, int, int);
		void		MouseUp		(int, int, int, int);
		void		HandleActiveEvent	();
		void		KeyDown		(SDLKey);
		void		KeyUp		(SDLKey);
		void	Loop			();
		void	Render			();
		void	Quit			();
		
		void	go_active		();
		void	go_idle			();
};


#endif /* _APP_H_ */

