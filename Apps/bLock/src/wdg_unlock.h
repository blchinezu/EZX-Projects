#ifndef _unLOCK_H_
#define _unLOCK_H_

#include "main.h"

#define	ARR_MAX_REL_X	UNLOCK_ARR_M_X - arrow->w

// Class
class unLOCK {
	private:
		SDL_Surface		*ibg, *bkgImage, *arrow;
		
	public:
		bool			dragging, draggingArrow, returningArrow;
		int				relX, relY, relAX, relAY;
		
	public:
		unLOCK();
		~unLOCK();
		
		void	Init			(SDL_Surface*);
		void	move			(int, int);
		void	moveRel			(int, int);
		void	moveAuto		(int);
		void	draw			(SDL_Surface*, bool=true);
		void	redraw			(SDL_Surface*);
		bool	hover			(int, int);
		
		void	moveArrow		(int);
		void	moveRelArrow	(int);
		void	returnArrowStep	();
		void	drawArrow		(SDL_Surface*, bool=true);
		void	redrawArrow		(SDL_Surface*);
		bool	hoverArrow		(int, int);
		
		int		max_visible		();
		int		min_visible		();
};



#endif /* _unLOCK_H_ */

