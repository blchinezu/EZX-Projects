#ifndef _INFO_PANEL_H_
#define _INFO_PANEL_H_

#include "main.h"
#include "power.h"

// Class
class INFO_PANEL {
	private:
		SDL_Surface	*ibg, *bkgImage,
					*button[2],
					*status_surf;
		
		int			connected, charging;
		char		usb_mode[MAX_NAME_LEN];
		
		char		status_char[MAX_NAME_LEN*2];
		int			statusX, statusY;
		TTF_Font	*font;
		
		int			buttonX, buttonY;
		
	public:
		bool		dragging, button_clicked;
		int			relX, relY;
		
	public:
		INFO_PANEL();
		~INFO_PANEL();
		
		void	Init			(SDL_Surface*);
		void	move			(int, int);
		void	moveRel			(int, int);
		void	moveAuto		(int);
		void	draw			(SDL_Surface*, bool=true);
		void	redraw			(SDL_Surface*);
		bool	hover			(int, int);
		
		void	updateData		();
		
		void	draw_status		(SDL_Surface*, bool=true);
		void	redraw_status	(SDL_Surface*);
		
		void	draw_button		(SDL_Surface*, bool, bool=true);
		bool	hover_button	(int, int);
		
		int		max_visible		();
		int		min_visible		();
};



#endif /* _INFO_PANEL_H_ */

