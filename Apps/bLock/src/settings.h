#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#include "main.h"
#include "soft-keyboard.h"

#define iNONE			0
#define	iBOOL			1
#define	iINT			2
#define	iCHAR			3
#define iKEY			4

#define iXoffset		5
#define iYoffset		5

#define FlowRetStep 	5
#define MaxOutArea  	25

#define MovesTillDrag	2


class ITEM;
class SCROLLWINDOW;
class SETTINGS;

/** ITEM **/

class ITEM {
	private:
		SDL_Surface		*name, *value;
		SCROLLWINDOW	*sw;
		char			nameC[MAX_PATH_LEN*2];
		
	public:
		int		type, h, maxX;
		// Holders
		bool	hold_bool;
		int		hold_int, hold_key;
		char	hold_char[MAX_PATH_LEN];
		
		
	public:
		ITEM();
		~ITEM();
		
		void	set			(SCROLLWINDOW*, char*, bool);
		void	set			(SCROLLWINDOW*, char*, int);
		void	set			(SCROLLWINDOW*, char*, int, int);
		void	set			(SCROLLWINDOW*, char*, char*);
		void	setCommon	(SCROLLWINDOW*, char*);
		
		void	gui_val		();
		
		void	set_val		(bool);
		void	set_val		(int);
		void	set_val		(char*);
		
		void	draw		(int, int, SDL_Surface*);
		void	draw		(int, int, int, int, SDL_Surface*);
		
		int		atoi		(char*);
		bool	is_num		(char);
		void	key2char	(int, char*);
};

/** SCROLLWINDOW **/

class SCROLLWINDOW {
	private:
		SDL_Surface	*bkgImage, *ibg, *delim;

	public:
		ITEM		item[20];
		TTF_Font	*font;
		bool		click, dragging, goFlow, returning;
		int			x, y, w, h,
					yScroll, hScroll, relyFlow,
					last_relY[3], last_rel_count,
					items_counter;
		
	public:
		SCROLLWINDOW();
		~SCROLLWINDOW();
		
		void	Init	(SDL_Surface*, int, int, int, int, SDL_Surface* ibgg=NULL);
		void	Init	(SDL_Surface*, int, int, SDL_Surface* ibgg);
		void	draw	(SDL_Surface*);
		void	moveRel	(int, bool upd=true);
		void	selfFlow(bool running=true);
		bool	hover	(int, int);
};

/** SETTINGS **/

class SETTINGS {
	private:
		SDL_Event		Event;
		SCROLLWINDOW	swin;
		SDL_Surface		*wallpaper, *old_wallpaper,
						*button[2];
		
	public:
		SETTINGS(SDL_Surface*);
		~SETTINGS();
		
		bool			button_clicked;
		bool 			Running;
		bool 			Focused;
		bool 			go_active_flag;
		int				status;
		
		bool	Run				();
		void	HandleEvent		();
		void		MouseMove	(int, int, int, int);
		void		MouseDown	(int, int, int, int);
		void		MouseUp		(int, int, int, int);
		void		KeyDown		(SDLKey);
		void		KeyUp		(SDLKey);
};


#endif /* _SETTINGS_H_ */

