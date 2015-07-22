#ifndef _iCLOCK_H_
#define _iCLOCK_H_

#include "main.h"

#include <qdatetime.h>

// Class
class iCLOCK {
	private:
		QString			month;
		int 			year, day;
		int				hour, minute;
		
		SDL_Surface		*ibg, *bkgImage, *time_surface, *date_surface;
		TTF_Font		*font[2];
		
	public:
		bool			dragging;
		int				relX, relY;
		
	public:
		iCLOCK();
		~iCLOCK();
		
		void	Init		(int, int, SDL_Surface*);
		void	updateTime	();
		void	move		(int, int);
		void	moveRel		(int, int);
		void	draw		(SDL_Surface*, bool=true);
		void	redraw		(SDL_Surface*);
		bool	hover		(int, int);
		
		int		height		();
		int		width		();
};



#endif /* _iCLOCK_H_ */

