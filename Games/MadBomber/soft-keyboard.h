/*
*	Created for 320x240 SDL touch screen surfaces
*
*	Author:			BruceLee
*	Forum topic:	http://www.motorolafans.com/forums/development/30860-sdl-softkeyboard-class.html
*	Contact email:	eu.gabii <at> yahoo <dot> com
*	
*	Source is best viewed using \t (tabs) of 4 spaces
*
*	You'll encounter many times the terms: seamless and separated. Those are 2 calling modes of SK:
*	- Seamless	= Integrate in app and send key signals as if it were from the keyboard
*	- Separated	= Pause the app and start own loop to get a string
*
*	Last updated: 16.Oct.2010
*/

#ifndef _SK_H_
#define _SK_H_

#include <iostream>
#include <string>
#include <stdlib.h>
#include <assert.h>

#include <SDL/SDL.h>
#ifdef SK_USE_SDL_image
	#include <SDL/SDL_image.h>
#endif


// SFont
#ifdef __cplusplus
	extern "C" {
#endif
	typedef struct {
		SDL_Surface*	Surface;	
		int				CharPos[512];
		int				MaxPos;
	} SFont_Font;
	SFont_Font*	SFont_InitFont		(SDL_Surface*);
	void		SFont_FreeFont		(SFont_Font*);
	void		SFont_Write			(SDL_Surface*, const SFont_Font*, int, int, const char*);
	int			SFont_TextWidth		(const SFont_Font*, const char*);
	int			SFont_TextHeight	(const SFont_Font*);
	void		SFont_WriteCenter	(SDL_Surface*, const SFont_Font*, int, const char*);
#ifdef __cplusplus
	}
#endif

// Button structure
struct SK_button { int x, y, w, h; SDLKey SDLK; char place; std::string id, ch; SDL_Rect clip; };

// SK - SoftKeyboard
class SK {
	//   ----------------------------   VARIABLES   ----------------------------
	public:
		static bool			Show;
		static bool			Show_FirstFrame;
		static bool			SkipHideFlag;
		static bool			Initialized;
		static bool			CapsLock;
		static bool			AllowAnimation;
		static bool			OpenAnim;
		static bool			CloseAnim;
		static bool			HeadIsHidding;
		static bool			HeadIsHidden;
		static bool			FinishedAnim_FF;
		static char			Mode;
		static SDLKey		ToggleButton;
		static int			CallMode;
		static int			MaxFPS;
		static int			FadeFrames;
		
		static std::string	hstr;
		static std::string	head_title;
		
		static std::string	DataPath;
		static std::string	loc_font1;		static std::string	loc_font2;
		static std::string	loc_head_bkg;	static std::string	loc_keyb_bkg;
		static std::string	loc_background;	static std::string	loc_text_box;
		static std::string	loc_head;		static std::string	loc_head_pressed;
		static std::string	loc_char;		static std::string	loc_char_pressed;
		static std::string	loc_num;		static std::string	loc_num_pressed;
		
		static SDL_Surface* SCREEN;			static SDL_Surface*	SCREEN_copy;
		static SFont_Font*	img_font1;		static SFont_Font*	img_font2;
		static SDL_Surface*	img_head_bkg;	static SDL_Surface*	img_keyb_bkg;
		static SDL_Surface*	img_background;	static SDL_Surface*	img_text_box;
		static SDL_Surface*	img_head;		static SDL_Surface*	img_head_pressed;
		static SDL_Surface*	img_char;		static SDL_Surface*	img_char_pressed;
		static SDL_Surface*	img_num;		static SDL_Surface*	img_num_pressed;
		
		static int			Head_off_X[], Head_off_Y[],
							TBox_off_X[], TBox_off_Y[],
							Keyb_off_X[], Keyb_off_Y[];

		static SK_button	button[];
		static int			buttons_nr;
		static int			buttons_grp[];
		static int			last_pressed_button;
		static int			last_rendered_pressed_button;
		
	//   ----------------------------   FUNCTIONS   ----------------------------
	public:
		// Those can be used in your source code:
			static bool		Init					();
			static void		Quit					();
			static void		ShowIt					();
			static void		ShowSeamless			();
			static void		ShowSeparated			();
			static void		HideIt					();
			static void		ToggleShow				();
			static void		ToggleShowSeamless		();
			static void		ToggleShowSeparated		();
			static void		SetTitle				(std::string);
			static void		RemoveTitle				();
			static void		GetString				(char[]);
		// Core
		static void 		LoadOffsets				();
		static bool		 	RunSeparatedLoop		();
		static void			AddButton				(char, std::string, int, int, int, int, SDLKey, std::string);
		static void			InitButtons				();
		static void			SendKeyPress			(SK_button);
		static void			ButtonClicked			(int);
		static void			ToggleCapsLock			();
		// Graphics
		static bool 		InitGraphics			();
		static bool 		DetectScreenSurface		();
		static void			SetScreenSurface		(SDL_Surface*);
		static SDL_Surface*	LoadImage				(std::string);
		static SDL_Surface*	CopySurface				(SDL_Surface*);
		static void			OpenAnimation			(char);
		static void			CloseAnimation			(char);
		static void			StopAnimation			(char);
		static void			RenderButton			(SK_button, bool);
		static void			RenderSurface			(int, int, SDL_Surface*, SDL_Surface*, SDL_Rect*);
		static void			RenderCore				();
		static void			Render					();
		static void			FreeSurface				(SDL_Surface*);
		// Mouse
		static bool			PointOverButton			(int, int, SK_button);
		static bool			PointOver				(int, int, int, int, int, int);
		static void			ProcEvent				(SDL_Event*);
		static void			ClickAt					(int, int);
};

// Timer
class SK_Timer {
    private:
		int		startTicks;
		int		pausedTicks;
		bool	paused;
		bool	started;
    public:
		SK_Timer();
    	static int		frame;
    	static Uint32	deltaTicks;
		void			start		();
		void			stop		();
		void			pause		();
		void			unpause		();
		int				get_ticks	();
		bool			is_started	();
		bool			is_paused	();
};


#endif /* _SK_H_ */
