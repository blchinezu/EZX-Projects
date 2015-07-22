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

#ifndef _SK_CPP_
#define _SK_CPP_

#define SK_USE_SDL_image	// Uncomment this to use lSDL_image and load PNG images or comment it to exclude lSDL_image and load BMP images.
// FIXME FIXME FIXME - PROBLEMA ARTEFACTE FONT2 BMP - FIXME FIXME FIXME

#include "soft-keyboard.h"


//   ----------------------------   VARIABLES   ----------------------------

int				SK::FadeFrames			= 0;			// FIXME - useless for now
bool			SK::AllowAnimation		= true;					// The name says it all
int				SK::MaxFPS				= 25;					// The maximum frames that can be processed per second (in 'separated' mode)
char			SK::Mode				= 'C';					// C = Char; N = Num
bool			SK::CapsLock			= false;				// CapsLock flag (change if you want to start with CapsLock activated)
int				SK::CallMode			= 0;					// 0 = Seamless (sending key signals); 1 = Separated (Pause game/app while getting a string)
SDLKey			SK::ToggleButton		= SDLK_RETURN;			// Button used in source to show the keyboard (not used if SK is triggered by mouse event)

// It's best to let the following flags unchanged
bool			SK::Show				= false;				// Mark if SK is showed
bool			SK::Show_FirstFrame		= true;					// Mark the first frame when showing SK
bool			SK::SkipHideFlag		= false;				// Flag for skipping the HideIt() function
bool			SK::Initialized			= false;				// Initialization flag
bool			SK::OpenAnim			= false;				// Opening animation flag
bool			SK::CloseAnim			= false;				// Closing animation flag
bool			SK::FinishedAnim_FF		= true;					// Flag marking the end of the animation
bool			SK::HeadIsHidding		= false;				// Flag for head title hidding animation by button
bool			SK::HeadIsHidden		= false;				// Flag for head title hidding by button

std::string		SK::hstr;										// Contains the text while writing in 'Separated' mode
std::string		SK::head_title;									// Contains the top title
Uint32			SK_Timer::deltaTicks;							// Time passed since the last update (used for animation)
SK_Timer		delta;											// Timer

std::string		SK::DataPath			= "data/sk/";			// Data path. It must have "/" as the last char
std::string		SK::loc_font1			= "font1";				// Images names without extension. SK is loading bmp or png depending on: #define SK_USE_SDL_image
std::string		SK::loc_font2			= "font2";
std::string		SK::loc_background		= "bkg";
std::string		SK::loc_head_bkg		= "head-bkg";
std::string		SK::loc_keyb_bkg		= "keyb-bkg";
std::string		SK::loc_text_box		= "tbox";
std::string		SK::loc_head			= "head";
std::string		SK::loc_head_pressed	= "head-pressed";
std::string		SK::loc_char			= "keyb-char";
std::string		SK::loc_char_pressed	= "keyb-char-pressed";
std::string		SK::loc_num				= "keyb-num";
std::string		SK::loc_num_pressed		= "keyb-num-pressed";

SFont_Font*		SK::img_font1			= NULL;
SFont_Font*		SK::img_font2			= NULL;
SDL_Surface*	SK::SCREEN				= NULL;
SDL_Surface*	SK::SCREEN_copy			= NULL;
SDL_Surface*	SK::img_background		= NULL;
SDL_Surface*	SK::img_head_bkg		= NULL;
SDL_Surface*	SK::img_keyb_bkg		= NULL;
SDL_Surface*	SK::img_text_box		= NULL;
SDL_Surface*	SK::img_head			= NULL;
SDL_Surface*	SK::img_head_pressed	= NULL;
SDL_Surface*	SK::img_char			= NULL;
SDL_Surface*	SK::img_char_pressed	= NULL;
SDL_Surface*	SK::img_num				= NULL;
SDL_Surface*	SK::img_num_pressed		= NULL;

// These offset vars are set in SK::SetOffsets();
int				SK::Head_off_X[4],		SK::Head_off_Y[4],	// Head bar surface offsets
				SK::TBox_off_X[4],		SK::TBox_off_Y[4],	// Text Box surface offsets
				SK::Keyb_off_X[4],		SK::Keyb_off_Y[4];	// Keyboard surface offsets
				// Info for Head (the rest are the same)
				// [0]	img_head offset
				// [1]	img_head temporary offset
				// [2]	img_head velocity
				// [3]	img_head temporary velocity

// It's best to leave those unchanged too
int				SK::buttons_nr=0,						// The total number of buttons
				SK::last_pressed_button=0,				// Marks the last pressed button
				SK::last_rendered_pressed_button=0;		// Marks the last rendered button
int				SK::buttons_grp[10];					// Marks the beginning and end of each button group in button[]
SK_button		SK::button[100];						// Contains all the buttons



//   ----------------------------   FUNCTIONS   ----------------------------

bool SK::Init() {
	if(!DetectScreenSurface()) return false;
	if(!InitGraphics()) return false;
	LoadOffsets();
	InitButtons();
	RemoveTitle();
	Initialized=true;
	return true;
}
void SK::Quit() {
	SFont_FreeFont(img_font1);	SFont_FreeFont(img_font2);
	FreeSurface(img_background);	FreeSurface(img_text_box);
	FreeSurface(img_head);			FreeSurface(img_head_pressed);
	FreeSurface(img_char);			FreeSurface(img_char_pressed);
	FreeSurface(img_num);			FreeSurface(img_num_pressed);
	Initialized=false;
}
void SK::ShowIt()				{ if(!Show) { Show=true; Show_FirstFrame=true; HeadIsHidding=false; HeadIsHidden=false; }; }
void SK::ShowSeamless()			{ CallMode=0; ShowIt(); }
void SK::ShowSeparated()		{ CallMode=1; ShowIt(); }
void SK::HideIt()				{ if(SkipHideFlag) { SkipHideFlag=false; } else { CloseAnimation('A'); CapsLock=false; Mode='C'; }; }
void SK::ToggleShow()			{ if(Show) HideIt(); else ShowIt(); }
void SK::ToggleShowSeamless()	{ if(Show) HideIt(); else ShowSeamless(); }
void SK::ToggleShowSeparated()	{ if(Show) HideIt(); else ShowSeparated(); }
void SK::SetTitle(std::string s){ RemoveTitle(); head_title.append(s.c_str()); }
void SK::RemoveTitle()			{ head_title.clear(); }
void SK::GetString(char str[]) {
	hstr.clear(); hstr.append(str);
		ShowSeparated(); Render();
	sprintf(str,"%s",hstr.c_str());
}

// Core
void SK::LoadOffsets() {
	// Image offset on screen (variable while the app is running)
		Head_off_X[0]=(SCREEN->w-img_head->w)/2;		Head_off_Y[0]=0;
		Keyb_off_X[0]=(SCREEN->w-img_char->w)/2;		Keyb_off_Y[0]=SCREEN->h-img_char->h;
		TBox_off_X[0]=(SCREEN->w-img_text_box->w)/2;	TBox_off_Y[0]=Keyb_off_Y[1]-30;
	// Upper image offsets (constant)
		Head_off_X[1]=Head_off_X[0];					Head_off_Y[1]=Head_off_Y[0];
		TBox_off_X[1]=TBox_off_X[0];					TBox_off_Y[1]=TBox_off_Y[0];
		Keyb_off_X[1]=Keyb_off_X[0];					Keyb_off_Y[1]=Keyb_off_Y[0];
	// Image animation velocity (Bigger values = faster movement)
		Head_off_X[2]=0;/*not used*/					Head_off_Y[2]=60;
		Keyb_off_X[2]=0;/*not used*/					Keyb_off_Y[2]=210;
		TBox_off_X[2]=600;								TBox_off_Y[2]=0;/*not used*/
	// Image animation temp velocity (don't change)
		Head_off_X[3]=0;								Head_off_Y[3]=0;
		Keyb_off_X[3]=0;								Keyb_off_Y[3]=0;
		TBox_off_X[3]=0;								TBox_off_Y[3]=0;
}
void SK::AddButton(char place, std::string ID, int x, int y, int w, int h, SDLKey SDLK, std::string ch) {
	buttons_nr++; int oX, oY;
	if(place=='H')	{ oX=Head_off_X[0]; oY=Head_off_Y[0]; } else
	if(place=='K')	{ oX=Keyb_off_X[0]; oY=Keyb_off_Y[0]; } else
					{ oX=0;			 oY=0;			};
	button[buttons_nr].place=place;	button[buttons_nr].id=ID;
	button[buttons_nr].x=oX+x;		button[buttons_nr].y=oY+y;
	button[buttons_nr].w=w;			button[buttons_nr].h=h;
	button[buttons_nr].SDLK=SDLK;	button[buttons_nr].ch=ch;
	button[buttons_nr].clip.x=x;	button[buttons_nr].clip.y=y;
	button[buttons_nr].clip.w=w;	button[buttons_nr].clip.h=h;
}
void SK::InitButtons() {
	buttons_nr=0;
	// Declare all the buttons (X,Y values are relative to the loaded image origin)
	//		  Place	, IDentifier	, X		, Y		, W		, H		, SDL key code		, Char
	buttons_grp[0]=buttons_nr+1;
	AddButton( 'H'	, "HideHead"	, 297	, 0		, 23	, 21	, SDLK_UNKNOWN		, ""	); // Head - Top Bar
	buttons_grp[1]=buttons_nr;
	
	buttons_grp[2]=buttons_nr+1;
	AddButton( 'K'	, "Q"			, 1		, 3		, 30	, 24	, SDLK_q			, "q"	); // Key - Char
	AddButton( 'K'	, "W"			, 33	, 3		, 30	, 24	, SDLK_w			, "w"	);
	AddButton( 'K'	, "E"			, 65	, 3		, 30	, 24	, SDLK_e			, "e"	);
	AddButton( 'K'	, "R"			, 97	, 3		, 30	, 24	, SDLK_r			, "r"	);
	AddButton( 'K'	, "T"			, 129	, 3		, 30	, 24	, SDLK_t			, "t"	);
	AddButton( 'K'	, "Y"			, 161	, 3		, 30	, 24	, SDLK_y			, "y"	);
	AddButton( 'K'	, "U"			, 193	, 3		, 30	, 24	, SDLK_u			, "u"	);
	AddButton( 'K'	, "I"			, 225	, 3		, 30	, 24	, SDLK_i			, "i"	);
	AddButton( 'K'	, "O"			, 257	, 3		, 30	, 24	, SDLK_o			, "o"	);
	AddButton( 'K'	, "P"			, 289	, 3		, 30	, 24	, SDLK_p			, "p"	);
	AddButton( 'K'	, "A"			, 17	, 29	, 30	, 24	, SDLK_a			, "a"	);
	AddButton( 'K'	, "S"			, 49	, 29	, 30	, 24	, SDLK_s			, "s"	);
	AddButton( 'K'	, "D"			, 81	, 29	, 30	, 24	, SDLK_d			, "d"	);
	AddButton( 'K'	, "F"			, 113	, 29	, 30	, 24	, SDLK_f			, "f"	);
	AddButton( 'K'	, "G"			, 145	, 29	, 30	, 24	, SDLK_g			, "g"	);
	AddButton( 'K'	, "H"			, 177	, 29	, 30	, 24	, SDLK_h			, "h"	);
	AddButton( 'K'	, "J"			, 209	, 29	, 30	, 24	, SDLK_j			, "j"	);
	AddButton( 'K'	, "K"			, 241	, 29	, 30	, 24	, SDLK_k			, "k"	);
	AddButton( 'K'	, "L"			, 273	, 29	, 30	, 24	, SDLK_l			, "l"	);
	AddButton( 'K'	, "Z"			, 49	, 56	, 30	, 24	, SDLK_z			, "z"	);
	AddButton( 'K'	, "X"			, 81	, 56	, 30	, 24	, SDLK_x			, "x"	);
	AddButton( 'K'	, "C"			, 113	, 56	, 30	, 24	, SDLK_c			, "c"	);
	AddButton( 'K'	, "V"			, 145	, 56	, 30	, 24	, SDLK_v			, "v"	);
	AddButton( 'K'	, "B"			, 177	, 56	, 30	, 24	, SDLK_b			, "b"	);
	AddButton( 'K'	, "N"			, 209	, 56	, 30	, 24	, SDLK_n			, "n"	);
	AddButton( 'K'	, "M"			, 241	, 56	, 30	, 24	, SDLK_m			, "m"	);
	AddButton( 'K'	, "Space"		, 43	, 82	, 234	, 25	, SDLK_SPACE		, " "	);
	AddButton( 'K'	, "Enter"		, 281	, 83	, 37	, 23	, SDLK_RETURN		, "\n"	);
	AddButton( 'K'	, "Backspace"	, 279	, 56	, 40	, 24	, SDLK_BACKSPACE	, ""	); // must be third from bottom to top
	AddButton( 'K'	, "NumMode"		, 2		, 83	, 37	, 23	, SDLK_UNKNOWN		, ""	);
	AddButton( 'K'	, "CapsLock"	, 1		, 56	, 40	, 24	, SDLK_CAPSLOCK		, ""	); // must be last
	buttons_grp[3]=buttons_nr;
	
	buttons_grp[4]=buttons_nr+1;
	AddButton( 'K'	, "1"			, 1		, 3		, 30	, 24	, SDLK_1			, "1"	); // Key - Num
	AddButton( 'K'	, "2"			, 33	, 3		, 30	, 24	, SDLK_2			, "2"	);
	AddButton( 'K'	, "3"			, 65	, 3		, 30	, 24	, SDLK_3			, "3"	);
	AddButton( 'K'	, "4"			, 97	, 3		, 30	, 24	, SDLK_4			, "4"	);
	AddButton( 'K'	, "5"			, 129	, 3		, 30	, 24	, SDLK_5			, "5"	);
	AddButton( 'K'	, "6"			, 161	, 3		, 30	, 24	, SDLK_6			, "6"	);
	AddButton( 'K'	, "7"			, 193	, 3		, 30	, 24	, SDLK_7			, "7"	);
	AddButton( 'K'	, "8"			, 225	, 3		, 30	, 24	, SDLK_8			, "8"	);
	AddButton( 'K'	, "9"			, 257	, 3		, 30	, 24	, SDLK_9			, "9"	);
	AddButton( 'K'	, "0"			, 289	, 3		, 30	, 24	, SDLK_0			, "0"	);
	AddButton( 'K'	, "!"			, 1		, 29	, 30	, 24	, SDLK_EXCLAIM		, "!"	);
	AddButton( 'K'	, "@"			, 33	, 29	, 30	, 24	, SDLK_AT			, "@"	);
	AddButton( 'K'	, "#"			, 65	, 29	, 30	, 24	, SDLK_HASH			, "#"	);
	AddButton( 'K'	, "$"			, 97	, 29	, 30	, 24	, SDLK_DOLLAR		, "$"	);
	AddButton( 'K'	, "%"			, 129	, 29	, 30	, 24	, SDLK_UNKNOWN		, "%"	);
	AddButton( 'K'	, "^"			, 161	, 29	, 30	, 24	, SDLK_CARET		, "^"	);
	AddButton( 'K'	, "&"			, 193	, 29	, 30	, 24	, SDLK_AMPERSAND	, "&"	);
	AddButton( 'K'	, "*"			, 225	, 29	, 30	, 24	, SDLK_ASTERISK		, "*"	);
	AddButton( 'K'	, "("			, 257	, 29	, 30	, 24	, SDLK_UNKNOWN		, "("	);
	AddButton( 'K'	, ")"			, 289	, 29	, 30	, 24	, SDLK_UNKNOWN		, ")"	);
	AddButton( 'K'	, "?"			, 1		, 56	, 30	, 24	, SDLK_QUESTION		, "?"	);
	AddButton( 'K'	, "-"			, 33	, 56	, 30	, 24	, SDLK_MINUS		, "-"	);
	AddButton( 'K'	, "+"			, 65	, 56	, 30	, 24	, SDLK_PLUS			, "+"	);
	AddButton( 'K'	, "="			, 97	, 56	, 30	, 24	, SDLK_EQUALS		, "="	);
	AddButton( 'K'	, "<"			, 129	, 56	, 30	, 24	, SDLK_LESS			, "<"	);
	AddButton( 'K'	, ">"			, 161	, 56	, 30	, 24	, SDLK_GREATER		, ">"	);
	AddButton( 'K'	, "["			, 193	, 56	, 30	, 24	, SDLK_UNKNOWN		, "["	);
	AddButton( 'K'	, "]"			, 225	, 56	, 30	, 24	, SDLK_UNKNOWN		, "]"	);
	AddButton( 'K'	, "{"			, 257	, 56	, 30	, 24	, SDLK_UNKNOWN		, "{"	);
	AddButton( 'K'	, "}"			, 289	, 56	, 30	, 24	, SDLK_UNKNOWN		, "}"	);
	AddButton( 'K'	, "."			, 41	, 82	, 22	, 25	, SDLK_PERIOD		, "."	);
	AddButton( 'K'	, ","			, 65	, 82	, 22	, 25	, SDLK_COMMA		, ","	);
	AddButton( 'K'	, ":"			, 89	, 82	, 22	, 25	, SDLK_COLON		, ":"	);
	AddButton( 'K'	, ";"			, 113	, 82	, 22	, 25	, SDLK_SEMICOLON	, ";"	);
	AddButton( 'K'	, "\'"			, 137	, 82	, 22	, 25	, SDLK_QUOTE		, "\'"	);
	AddButton( 'K'	, "\""			, 161	, 82	, 22	, 25	, SDLK_QUOTEDBL		, "\""	);
	AddButton( 'K'	, "_"			, 185	, 82	, 22	, 25	, SDLK_UNDERSCORE	, "_"	);
	AddButton( 'K'	, "/"			, 209	, 82	, 22	, 25	, SDLK_SLASH		, "/"	);
	AddButton( 'K'	, "|"			, 233	, 82	, 22	, 25	, SDLK_UNKNOWN		, "|"	);
	AddButton( 'K'	, "\\"			, 257	, 82	, 22	, 25	, SDLK_BACKSLASH	, "\\"	);
	AddButton( 'K'	, "Enter"		, 281	, 83	, 37	, 23	, SDLK_RETURN		, "\n"	);
	AddButton( 'K'	, "CharMode"	, 2		, 83	, 36	, 23	, SDLK_UNKNOWN		, ""	);
	buttons_grp[5]=buttons_nr;
}
void SK::ButtonClicked(int i) {
	if(CallMode==0) {
		if((i>=buttons_grp[2] && i<=buttons_grp[3]-2) || (i>=buttons_grp[4] && i<=buttons_grp[5]-1)) {
			if(ToggleButton && button[i].SDLK==ToggleButton) { SendKeyPress(button[i]); SkipHideFlag=true; }
			else SendKeyPress(button[i]);
		} else {
			if(button[i].id=="CharMode")	{ Mode='C'; } else
			if(button[i].id=="NumMode")		{ Mode='N'; } else
			if(button[i].id=="HideHead" && !HeadIsHidden)	{ HeadIsHidding=true; /*head_title.clear(); last_pressed_button=-1;*/ } else
			if(button[i].id=="CapsLock")	{ SendKeyPress(button[i]); ToggleCapsLock(); } else
			{ std::cout<<"SK: Undefined action for: "<<button[i].id<<"\n"; last_pressed_button=-1; }
		}
	} else if(CallMode==1) {
		if((i>=buttons_grp[2] && i<=buttons_grp[3]-4) || (i>=buttons_grp[4] && i<=buttons_grp[5]-2)) {
			if(CapsLock && i>=buttons_grp[2] && i<=buttons_grp[3]-5) hstr.append(button[i].id);
			else hstr.append(button[i].ch);
		} else {
			if(button[i].id=="CharMode")	{ Mode='C'; } else
			if(button[i].id=="NumMode")		{ Mode='N'; } else
			if(button[i].id=="HideHead" && !HeadIsHidden)	{ HeadIsHidding=true; /*head_title.clear(); last_pressed_button=-1;*/ } else
			if(button[i].id=="Backspace")	{ char tt[100]; sprintf(tt,"%s",hstr.c_str()); tt[strlen(tt)-1]='\0'; hstr.clear(); hstr.append(tt); } else
			if(button[i].id=="Enter")		{ HideIt(); } else
			if(button[i].id=="CapsLock")	{ SendKeyPress(button[i]); ToggleCapsLock(); } else
			{ std::cout<<"SK: Undefined action for: "<<button[i].id<<"\n"; last_pressed_button=-1; }
		}
	}
	if(last_pressed_button==-1) last_pressed_button=0; else last_pressed_button=i;
}
void SK::SendKeyPress(SK_button but) {
	SDL_Event event;
	event.key.keysym.sym=but.SDLK;
	event.type=SDL_KEYDOWN;	SDL_PushEvent(&event);
	event.type=SDL_KEYUP;	SDL_PushEvent(&event);
}
void SK::ToggleCapsLock() {
	if(CapsLock) { CapsLock=false; SDL_SetModState(KMOD_NONE); }
	else { CapsLock=true; SDL_SetModState(KMOD_NONE); }
}
bool SK::RunSeparatedLoop() {
	Show_FirstFrame=true;
	SK_Timer SK_maxfps;
	if(!Initialized) { if(!Init()) return false; }
	
	SDL_Event SK_Event;
	while(Show) {
		SK_maxfps.start();
		while(SDL_PollEvent(&SK_Event)) { ProcEvent(&SK_Event); }
		RenderCore(); SDL_Flip(SCREEN);
		
		SK_Timer::frame++;
		if(SK_maxfps.get_ticks()<1000/SK::MaxFPS) { SDL_Delay((1000/SK::MaxFPS)-SK_maxfps.get_ticks()); }
	}
	return true;
}
void SK::ProcEvent(SDL_Event* event) {
	if(Show)
	{
		switch(event->type) {
			case SDL_MOUSEBUTTONDOWN:	{ if(event->button.button==SDL_BUTTON_LEFT) ClickAt(event->button.x, event->button.y); break; }
		}
		if(CallMode==1 && !Show_FirstFrame) { // FIXME FIXME FIXME - De facut evaluare butoane tastatura - FIXME FIXME FIXME
			switch(event->type) {
				case SDL_KEYDOWN:		{ if((ToggleButton && event->key.keysym.sym==ToggleButton) || event->key.keysym.sym==SDLK_RETURN) HideIt(); break; }
			}
		} // XXX FIXME - closing separated with togglebutton
	}
}

// Graphics
bool SK::InitGraphics() { // FIXME - De terminat treaba cu comparatiile
	img_font1			= SFont_InitFont(LoadImage(loc_font1)); img_font2			= SFont_InitFont(LoadImage(loc_font2));
	img_background		= LoadImage(loc_background);	img_text_box		= LoadImage(loc_text_box);
	img_head			= LoadImage(loc_head);			img_head_pressed	= LoadImage(loc_head_pressed);
	img_char			= LoadImage(loc_char);			img_char_pressed	= LoadImage(loc_char_pressed);
	img_num				= LoadImage(loc_num);			img_num_pressed		= LoadImage(loc_num_pressed);
	img_head_bkg		= LoadImage(loc_head_bkg);		img_keyb_bkg		= LoadImage(loc_keyb_bkg);
	#ifndef SK_USE_SDL_image
		if(img_background) SDL_SetAlpha(img_background, SDL_SRCALPHA, 128);
	#endif
	if(!img_font1 || !img_font2 || !img_head_bkg || !img_keyb_bkg || !img_background || !img_text_box || !img_head || !img_head_pressed || !img_char || !img_char_pressed || !img_num || !img_num_pressed) {
		std::cout<<"\n SK::InitGraphics() >> ERROR: Could not load all the theme images.\n"; return false;
	};/*
	if((img_char->w!=img_num->w)||(img_char->h!=img_num->h)) { // FIXME de comparat si cu 'pressed' FIXME
		std::cout<<"\n SK::InitGraphics() >> WARNING: The keyboard images don't have the same resolution.\n\t\tVisual artifacts may appear.\n";
	};
	if( (SCREEN->w < (img_char->w | img_num->w)) || (SCREEN->h < (img_char->h | img_num->h)) ) {
		std::cout<<"\n SK::InitGraphcs() >> WARNING: The keyboard images are larger than the SCREEN ("<<SCREEN->w<<"x"<<SCREEN->h<<").\n";
	};
	if(SCREEN->w <img_head->w || SCREEN->h < img_head->h) {
		std::cout<<"\n SK::InitGraphcs() >> WARNING: The top bar image is larger than the SCREEN ("<<SCREEN->w<<"x"<<SCREEN->h<<").\n";
	};*/
	return true;
}
bool SK::DetectScreenSurface() {
	SCREEN=SDL_GetVideoSurface();
	if(!SCREEN) { std::cout<<"SK::DetectScreenSurface >> ERROR: Could not detect the screen surface.\n\t\tTry setting it manually.\n"; return false; }
	return true;
}
void SK::SetScreenSurface(SDL_Surface* scr) {
	SCREEN=scr;
}
SDL_Surface* SK::CopySurface(SDL_Surface* source) {
	SDL_Surface* destination = NULL;
	destination = SDL_CreateRGBSurface(SDL_HWSURFACE|SDL_SRCALPHA, source->w , source->h, 
										source->format->BitsPerPixel, 
										source->format->Rmask, source->format->Gmask, 
										source->format->Bmask, source->format->Amask);
	if(!destination) { std::cout<<" SK::CopySurface() >> WARNING: Could not create new surface.\n"; return NULL; }
	SDL_LockSurface(destination);
	SDL_LockSurface(source);	
	memcpy(destination->pixels, source->pixels, (source->w * source->h * source->format->BytesPerPixel));
	SDL_UnlockSurface(destination);
	SDL_UnlockSurface(source);
	return destination;
}
SDL_Surface* SK::LoadImage(std::string filename) {
	SDL_Surface* loadedImage=NULL;
	SDL_Surface* optimizedImage=NULL;
	std::string fila; fila.clear(); fila.append(DataPath); fila.append(filename.c_str());
	#ifdef SK_USE_SDL_image
		fila.append(".png"); loadedImage = IMG_Load(fila.c_str());
	#else
		fila.append(".bmp"); loadedImage = SDL_LoadBMP(fila.c_str());
	#endif
	if(loadedImage) {
		#ifdef SK_USE_SDL_image
			optimizedImage=SDL_DisplayFormatAlpha(loadedImage);
		#else
			optimizedImage=SDL_DisplayFormat(loadedImage);
		#endif
		SDL_FreeSurface(loadedImage);
		if(optimizedImage) {
			SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, SDL_MapRGB( optimizedImage->format, 0xFF, 0, 0xFF ) );
		} else { std::cout<<"\n SK::LoadImage() >> WARNING: Could not optimize the image: "<<fila.c_str()<<"\n"; return NULL; }
	} else { std::cout<<"\n SK::LoadImage() >> WARNING: Could not load the image: "<<fila.c_str()<<"\n"; return NULL; }
	return optimizedImage;
}
void SK::RenderButton(SK_button B, bool pressed) {
	if(Show) {
		SDL_Surface* theme;
		if(B.place=='H')					{ if(pressed) theme=img_head_pressed; else theme=img_head; } else
		if(B.place=='K'){	if(Mode=='C')	{ if(pressed) theme=img_char_pressed; else theme=img_char; } else
							if(Mode=='N')	{ if(pressed) theme=img_num_pressed;  else theme=img_num;  };
		};
		RenderSurface(B.x, B.y, theme, SCREEN, &B.clip);
	}
}
void SK::RenderSurface(int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip) {
	SDL_Rect offset; offset.x = x; offset.y = y; SDL_BlitSurface(source, clip, destination, &offset);
}
void SK::OpenAnimation(char x) {
	if(AllowAnimation) {
		if(!OpenAnim) {
			OpenAnim=true; FinishedAnim_FF=false;
			if(!CloseAnim) {
				delta.start();
				if(x=='A'||x=='K') Keyb_off_Y[0]=SCREEN->h;
				if(x=='A'||x=='H') Head_off_Y[0]=0-img_head_bkg->h;
				if(x=='A'||x=='T') TBox_off_X[0]=0-img_text_box->w;
	
				if(x=='A'||x=='K') Keyb_off_Y[3]=Keyb_off_Y[2];
				if(x=='A'||x=='H') Head_off_Y[3]=Head_off_Y[2];
				if(x=='A'||x=='T') TBox_off_X[3]=TBox_off_X[2];
			} else { CloseAnim=false; }
		};
		if(OpenAnim) {
			int stop=0, stop_mark=1; if(x=='A') stop_mark=3;
			SK_Timer::deltaTicks=delta.get_ticks();
			if(x=='A'||x=='K') { Keyb_off_Y[0] += -Keyb_off_Y[3] * ( SK_Timer::deltaTicks / 1000.f );}
			if(x=='A'||x=='H') { Head_off_Y[0] += Head_off_Y[3] * ( SK_Timer::deltaTicks / 1000.f );}
			if(x=='A'||x=='T') { TBox_off_X[0] += TBox_off_X[3] * ( SK_Timer::deltaTicks / 1000.f );}
			if(x=='A'||x=='K') { if(Keyb_off_Y[0]<=Keyb_off_Y[1]) { Keyb_off_Y[0]=Keyb_off_Y[1]; stop++; };}
			if(x=='A'||x=='H') { if(head_title.length()<=0) stop++; else { if(Head_off_Y[0]>=Head_off_Y[1]) { Head_off_Y[0]=Head_off_Y[1]; stop++; }; };}
			if(x=='A'||x=='T') { if(CallMode==0) stop++; else { if(TBox_off_X[0]>=TBox_off_X[1]) { TBox_off_X[0]=TBox_off_X[1]; stop++; }; };}
			if(stop==stop_mark) StopAnimation(x); else delta.start();
		}
	}
}
void SK::CloseAnimation(char x) {
	if(AllowAnimation) {
		if(!CloseAnim) {
			CloseAnim=true;  FinishedAnim_FF=false;
			if(!OpenAnim) {
				delta.start();
				if(x=='A'||x=='K') Keyb_off_Y[0]=Keyb_off_Y[1];
				if(x=='A'||x=='H') Head_off_Y[0]=Head_off_Y[1];
				if(x=='A'||x=='T') TBox_off_X[0]=TBox_off_X[1];
	
				if(x=='A'||x=='K') Keyb_off_Y[3]=Keyb_off_Y[2];
				if(x=='A'||x=='H') Head_off_Y[3]=Head_off_Y[2];
				if(x=='A'||x=='T') TBox_off_X[3]=TBox_off_X[2];
			} else { OpenAnim=false; }
		};
		if(CloseAnim) {
			int stop=0, stop_mark=1; if(x=='A') stop_mark=3;
			SK_Timer::deltaTicks=delta.get_ticks();
			if(x=='A'||x=='K') { Keyb_off_Y[0] -= -Keyb_off_Y[3] * ( SK_Timer::deltaTicks / 1000.f );}
			if(x=='A'||x=='H') { Head_off_Y[0] -= Head_off_Y[3] * ( SK_Timer::deltaTicks / 1000.f );}
			if(x=='A'||x=='T') { TBox_off_X[0] -= TBox_off_X[3] * ( SK_Timer::deltaTicks / 1000.f );}
			if(x=='A'||x=='K') { if(Keyb_off_Y[0]>SCREEN->h) { Keyb_off_Y[0]=SCREEN->h; stop++; };}
			if(x=='A'||x=='H') { if(head_title.length()<=0) stop++; else { if(Head_off_Y[0]<0-img_head_bkg->h) { Head_off_Y[0]=0-img_head_bkg->h; stop++; }; };}
			if(x=='A'||x=='T') { if(CallMode==0) stop++; else { if(TBox_off_X[0]<=0-img_text_box->w) { TBox_off_X[0]=0-img_text_box->w; stop++; }; };}
			if(stop==stop_mark) StopAnimation(x); else delta.start();
		//	std::cout<<"char="<<x<<"\ndeltaTicks="<<SK_Timer::deltaTicks<<"\nHead_off_Y[0]="<<Head_off_Y[0]<<"\nstop="<<stop<<"\nstop_mark="<<stop_mark<<"\n\n";
		//	SDL_Delay(100); // FIXME - REMOVE
		}
	} else { StopAnimation(x); }
}
void SK::StopAnimation(char x) {
	if(AllowAnimation) {
		if(CloseAnim && x=='A') Show=false;
		OpenAnim=false; CloseAnim=false; FinishedAnim_FF=true;
		delta.stop();
		if(HeadIsHidding) { if(x=='H') { HeadIsHidding=false; HeadIsHidden=true; } else if(x=='A'){ HeadIsHidding=false; HeadIsHidden=false; }; }
		if(x=='A'||x=='H') { Head_off_X[0]=Head_off_X[1];	Head_off_Y[0]=Head_off_Y[1]; }
		if(x=='A'||x=='T') { TBox_off_X[0]=TBox_off_X[1];	TBox_off_Y[0]=TBox_off_Y[1]; }
		if(x=='A'||x=='K') { Keyb_off_X[0]=Keyb_off_X[1];	Keyb_off_Y[0]=Keyb_off_Y[1]; }

		if(x=='A'||x=='H') { Head_off_Y[3]=0; }
		if(x=='A'||x=='K') { Keyb_off_Y[3]=0; }
	} else { Show=false; }
//	std::cout<<"\t\t\tStopped animation\n"; // FIXME - REMOVE
}
void SK::RenderCore() {	/// XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX 
	if(Show && Initialized) {
		if(Show_FirstFrame && AllowAnimation) OpenAnimation('A'); else {
			if(AllowAnimation) {
				if(OpenAnim) OpenAnimation('A'); else
				if(CloseAnim && !HeadIsHidding) CloseAnimation('A');
				if(HeadIsHidding) CloseAnimation('H');
			} else {
				if(HeadIsHidding) { HeadIsHidding=false; HeadIsHidden=true; }
			}
		}
	}
	if(Show && Initialized) {
		int i, count=0; char tt[50], ht[50];
		sprintf(tt,"%s",hstr.c_str()); sprintf(ht,"%s",head_title.c_str());
		// SEAMLESS MODE rendering process
		if(CallMode==0) {
			if(Show_FirstFrame) { Show_FirstFrame=false; }
			if(head_title.length()>0 && !HeadIsHidden) {
				for(i=(SCREEN->w-img_head->w)/2; i<=SCREEN->w-((SCREEN->w-img_head->w)/2); i++)
					RenderSurface(i, Head_off_Y[0], img_head_bkg, SCREEN, NULL);
				while(SFont_TextWidth(img_font2, ht)>SCREEN->w) { ht[strlen(ht)-1]='\0'; head_title.clear(); head_title.append(ht); count++; }
				if(count>0)
					std::cout<<"\n SK::RenderCore() >> WARNING: The top title width is bigger than the screen width. Removed "<<count<<" characters from it.\n";
				SFont_Write(SCREEN, img_font2, (SCREEN->w-SFont_TextWidth(img_font2, ht))/2,
												Head_off_Y[0]+((img_head->h-SFont_TextHeight(img_font2))/2)+1,ht);
				RenderSurface(Head_off_X[0], Head_off_Y[0], img_head, SCREEN, NULL);
			}
			for(i=(SCREEN->w-img_char->w)/2; i<=SCREEN->w-((SCREEN->w-img_char->w)/2);i++)
				RenderSurface(i, Keyb_off_Y[0]-(img_keyb_bkg->h-img_char->h), img_keyb_bkg, SCREEN, NULL);
			if(Mode=='C')	RenderSurface(Keyb_off_X[0], Keyb_off_Y[0], img_char, SCREEN, NULL); else
			if(Mode=='N')	RenderSurface(Keyb_off_X[0], Keyb_off_Y[0], img_num, SCREEN, NULL);
			if(last_pressed_button!=0) {
				RenderButton(button[last_pressed_button], true);
				last_pressed_button=0;
			};
		// SEPARATED MODE rendering process
		} else if(CallMode==1) {
			if(Show_FirstFrame) { SCREEN_copy=CopySurface(SCREEN); /*RenderSurface(0, 0, img_background, SCREEN_copy, NULL);*/ Show_FirstFrame=false; }
			RenderSurface(0, 0, SCREEN_copy, SCREEN, NULL);
			if(head_title.length()>0 && !HeadIsHidden) {
				for(i=(SCREEN->w-img_head->w)/2; i<=SCREEN->w-((SCREEN->w-img_head->w)/2); i++)
					RenderSurface(i, Head_off_Y[0], img_head_bkg, SCREEN, NULL);
				while(SFont_TextWidth(img_font2, ht)>SCREEN->w) { ht[strlen(ht)-1]='\0'; head_title.clear(); head_title.append(ht); count++; }
				if(count>0)
					std::cout<<"\n SK::RenderCore() >> WARNING: The top title width is bigger than the screen width. Removed "<<count<<" characters from it.\n";
				SFont_Write(SCREEN, img_font2, (SCREEN->w-SFont_TextWidth(img_font2, ht))/2, 
												Head_off_Y[0]+((img_head->h-SFont_TextHeight(img_font2))/2)+1,ht);
				RenderSurface(Head_off_X[0], Head_off_Y[0], img_head, SCREEN, NULL);
			};
				TBox_off_Y[0]=Keyb_off_Y[1]-30;
				RenderSurface(TBox_off_X[0], TBox_off_Y[0], img_text_box, SCREEN, NULL);
				while(SFont_TextWidth(img_font1, tt)>SCREEN->w-((SCREEN->w-img_text_box->w)+6)) { tt[strlen(tt)-1]='\0'; hstr.clear(); hstr.append(tt); }
				SFont_Write(SCREEN, img_font1, (TBox_off_X[0]+(img_text_box->w/2)-(SFont_TextWidth(img_font2, tt))/2),
												TBox_off_Y[0]+((img_text_box->h-SFont_TextHeight(img_font1))/2)+1, tt);
			for(i=(SCREEN->w-img_char->w)/2; i<=SCREEN->w-((SCREEN->w-img_char->w)/2);i++)
				RenderSurface(i, Keyb_off_Y[0]-(img_keyb_bkg->h-img_char->h), img_keyb_bkg, SCREEN, NULL);
			if(Mode=='C')	RenderSurface(Keyb_off_X[0], Keyb_off_Y[0], img_char, SCREEN, NULL); else
			if(Mode=='N')	RenderSurface(Keyb_off_X[0], Keyb_off_Y[0], img_num, SCREEN, NULL);
			if(last_pressed_button!=0) {
				RenderButton(button[last_pressed_button], true);
				last_pressed_button=0;
			};
		}
		if(CapsLock && Mode=='C') RenderButton(button[buttons_grp[3]], true);
	}
}
void SK::Render() {
	if(Show && Initialized) {
		if(CallMode==0) RenderCore(); else
		if(CallMode==1) RunSeparatedLoop();
	}
}
void SK::FreeSurface(SDL_Surface* surface) {
	if(surface) SDL_FreeSurface(surface);
}

// Mouse
bool SK::PointOverButton(int cx, int cy, SK_button B) { return PointOver(cx, cy, B.x, B.y, B.w, B.h); }
bool SK::PointOver(int cx, int cy, int x, int y, int w, int h) {	if((cx<x) || (cx>(x+w)) || (cy<y) || (cy>(y+h))) return false; return true; }
void SK::ClickAt(int mX, int mY) {
	if(Show) {
		int i;
		if(PointOver(mX, mY, Keyb_off_X[0], Keyb_off_Y[0], img_char->w, img_char->h)) {
			// Clicked Keyboard
			if(Mode=='C') {
				for(i=buttons_grp[2];i<=buttons_grp[3];i++)
					if(PointOverButton(mX, mY, button[i])) { ButtonClicked(i); break; }
			} else if(Mode=='N') {
				for(i=buttons_grp[4];i<=buttons_grp[5];i++)
					if(PointOverButton(mX, mY, button[i])) { ButtonClicked(i); break; }
			}
		} else
		if(PointOver(mX, mY, Head_off_X[0], Head_off_Y[0], img_head->w, img_head->h) && head_title.length()>0) {
			// Clicked Top Bar
			for(i=buttons_grp[0];i<=buttons_grp[1];i++)
				if(PointOverButton(mX, mY, button[i])) { ButtonClicked(i); break; }
		} else
		if(PointOver(mX, mY, TBox_off_X[0], TBox_off_Y[0], img_text_box->w, img_text_box->h) && CallMode==1) {
			// Clicked TextBox
		} else {
			HideIt();
		}
	}
}

// Timer
SK_Timer::SK_Timer()			{ startTicks = 0; pausedTicks = 0; paused = false; started = false; }
int		SK_Timer::frame			= 0;
void 	SK_Timer::start			() { started = true; paused = false; startTicks = SDL_GetTicks(); }
void 	SK_Timer::stop			() { started = false; paused = false; }
void 	SK_Timer::pause			() { if( ( started == true ) && ( paused == false ) ) { paused = true; pausedTicks = SDL_GetTicks() - startTicks; }; }
void 	SK_Timer::unpause		() { if( paused == true ) { paused = false; startTicks = SDL_GetTicks() - pausedTicks; pausedTicks = 0; }; }
int  	SK_Timer::get_ticks		() { if( started == true ) { if( paused == true ) { return pausedTicks; } else { return SDL_GetTicks() - startTicks; }; }; return 0; };
bool 	SK_Timer::is_started	() { return started; }
bool 	SK_Timer::is_paused		() { return paused; }

// SFont
static Uint32 GetPixel(SDL_Surface *Surface, Sint32 X, Sint32 Y) {
	Uint8  *bits; Uint32 Bpp;
	assert(X>=0); assert(X<Surface->w);
	Bpp = Surface->format->BytesPerPixel; bits = ((Uint8 *)Surface->pixels)+Y*Surface->pitch+X*Bpp;
	// Get the pixel
	switch(Bpp) {
		case 1:	return *((Uint8 *)Surface->pixels + Y * Surface->pitch + X); break;
		case 2:	return *((Uint16 *)Surface->pixels + Y * Surface->pitch/2 + X); break;
		case 3: { // Format/endian independent 
			Uint8 r, g, b; r = *((bits)+Surface->format->Rshift/8); g = *((bits)+Surface->format->Gshift/8); b = *((bits)+Surface->format->Bshift/8);
			return SDL_MapRGB(Surface->format, r, g, b); }; break;
		case 4: return *((Uint32 *)Surface->pixels + Y * Surface->pitch/4 + X); break;
	}; return -1;
}
SFont_Font* SFont_InitFont(SDL_Surface* Surface) {
	int x = 0, i = 0; Uint32 pixel; SFont_Font* Font; Uint32 pink;
	if (Surface == NULL) return NULL;
	Font = (SFont_Font *) malloc(sizeof(SFont_Font)); Font->Surface = Surface;
	SDL_LockSurface(Surface);
	pink = SDL_MapRGB(Surface->format, 0, 255, 255);
	while (x < Surface->w) {
		if (GetPixel(Surface, x, 0) == pink) { 
			Font->CharPos[i++]=x;
			while((x < Surface->w) && (GetPixel(Surface, x, 0)== pink)) x++;
			Font->CharPos[i++]=x;
		}; x++;
	}
	Font->MaxPos = x-1; pixel = GetPixel(Surface, 0, Surface->h-1);
	SDL_UnlockSurface(Surface); SDL_SetColorKey(Surface, SDL_SRCCOLORKEY, pixel);
	return Font;
}
void SFont_FreeFont(SFont_Font* FontInfo) { if(FontInfo->Surface) { SDL_FreeSurface(FontInfo->Surface); free(FontInfo); }; }
void SFont_Write(SDL_Surface *Surface, const SFont_Font *Font, int x, int y, const char *text) {
	const char* c; int charoffset; SDL_Rect srcrect, dstrect;
	if(text == NULL) return;
	srcrect.y = 1; dstrect.y = y; srcrect.h = dstrect.h = Font->Surface->h - 1; // these values won't change in the loop
	for(c = text; *c != '\0' && x <= Surface->w ; c++) {
		charoffset = ((int) (*c - 33)) * 2 + 1;
		if (*c == ' ' || charoffset < 0 || charoffset > Font->MaxPos) { // skip spaces and nonprintable characters
			 x += Font->CharPos[2]-Font->CharPos[1]; continue;
		}
		srcrect.w = dstrect.w = (Font->CharPos[charoffset+2] + Font->CharPos[charoffset+1])/2 - (Font->CharPos[charoffset] + Font->CharPos[charoffset-1])/2;
		srcrect.x = (Font->CharPos[charoffset]+Font->CharPos[charoffset-1])/2;
		dstrect.x = x - (float)(Font->CharPos[charoffset] - Font->CharPos[charoffset-1])/2;
		dstrect.y = y;
		SDL_BlitSurface(Font->Surface, &srcrect, Surface, &dstrect); 
		x += Font->CharPos[charoffset+1] - Font->CharPos[charoffset];
	}
}
int SFont_TextWidth(const SFont_Font *Font, const char *text) {
	const char* c; int charoffset=0; int width = 0;
	if(text == NULL) return 0;
	for(c = text; *c != '\0'; c++) {
		charoffset = ((int) *c - 33) * 2 + 1;
		if (*c == ' ' || charoffset < 0 || charoffset > Font->MaxPos) { // skip spaces and nonprintable characters
			width += Font->CharPos[2]-Font->CharPos[1]; continue;
		}; width += Font->CharPos[charoffset+1] - Font->CharPos[charoffset];
	}; return width;
}
int SFont_TextHeight(const SFont_Font* Font) { return Font->Surface->h - 1; }
void SFont_WriteCenter(SDL_Surface *Surface, const SFont_Font *Font, int y, const char *text) {
	SFont_Write(Surface, Font, Surface->w/2 - SFont_TextWidth(Font, text)/2, y, text);
}

#endif /* _SK_CPP_ */
