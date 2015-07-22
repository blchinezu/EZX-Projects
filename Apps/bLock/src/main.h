#ifndef _MAIN_H_
#define _MAIN_H_


/** Uncomment to enable debugging **/
//#define DEBUG

/** Application info **/
#define AppName			"bLock"
#define AppVersion		"v.1.0"
#define AppAuthor		"BruceLee <eu.gabii@yahoo.com>"
#define AppDate			"19.08.2011"

/** Device screen mode **/
#define ScreenWidth		240
#define ScreenHeight	320
#define ScreenBpp		32

/** Paths **/
#define Path_Data		"data"
#define Path_Config		"config"

/** String lenght limits **/
#define MAX_NAME_LEN	20
#define MAX_PATH_LEN	250
#define MAX_UN_LEN		160 	// Max United String Lenght

#define VIB_TIME		50

/** Configuration defaults **/
#define DEFAULT_LIGHTUPKEY		SDLK_RETURN
#define DEFAULT_LIGHTTIME		5

#define DEFAULT_SCREENMAXFPS	55
#define DEFAULT_LIMITFPS		true
#define DEFAULT_LCDOFFSECONDS	30

#define DEFAULT_FONT_PATH		"/usr/language/fonts/AMCSL.TTF"
#define DEFAULT_FONT_COLOR_R	255
#define DEFAULT_FONT_COLOR_G	255
#define DEFAULT_FONT_COLOR_B	255
#define DEFAULT_FONT_SMALL		16
#define DEFAULT_FONT_NORMAL		20
#define DEFAULT_FONT_BIG		47

#define DEFAULT_UNLOCK_VISIBLE	false
#define DEFAULT_UNLOCK_MIN_VIS	15
#define DEFAULT_UNLOCK_ARR_RET	8
#define DEFAULT_UNLOCK_ARR_X	20
#define DEFAULT_UNLOCK_ARR_Y	17
#define DEFAULT_UNLOCK_ARR_M_X	220

#define DEFAULT_INFO_VISIBLE	false
#define DEFAULT_INFO_MIN_VIS	24
#define DEFAULT_INFO_STAT_Y		42
#define DEFAULT_INFO_BUT_Y		2

#define DEFAULT_CLOCKX			0
#define DEFAULT_CLOCKY			120

#define DEFAULT_ENABLE_FADE		true
#define DEFAULT_FADE_TIME		200
#define DEFAULT_FADE_STEPS		10

#define DEFAULT_SETT_SLIDE_SPD	25


/** Configuration read aliases **/
#define LIMITFPS		cfg.readBoolEntry("FPS","LimitFPS",DEFAULT_LIMITFPS)
#define SCREENMAXFPS	cfg.readNumEntry("FPS","ScreenMaxFPS",DEFAULT_SCREENMAXFPS)
#define LIGHTTIME		cfg.readNumEntry("FPS","LightTime",DEFAULT_LIGHTTIME)

#define LCDOFFSECONDS	cfg.readNumEntry("SCREEN","LCDOffSeconds",DEFAULT_LCDOFFSECONDS)
#define LIGHTUPKEY		cfg.readNumEntry("SCREEN","LightUpKey",DEFAULT_LIGHTUPKEY)
#define ENABLE_FADE		cfg.readBoolEntry("SCREEN","EnableFade",DEFAULT_ENABLE_FADE)
#define FADE_TIME		cfg.readNumEntry("SCREEN","FadeTime",DEFAULT_FADE_TIME)
#define FADE_STEPS		cfg.readNumEntry("SCREEN","FadeSteps",DEFAULT_FADE_STEPS)

#define CLOCKX			cfg.readNumEntry("WDG_CLOCK","X",DEFAULT_CLOCKX)
#define CLOCKY			cfg.readNumEntry("WDG_CLOCK","Y",DEFAULT_CLOCKY)

#define FONT_SMALL		cfg.readNumEntry("FONT","Small",DEFAULT_FONT_SMALL)
#define FONT_NORMAL		cfg.readNumEntry("FONT","Normal",DEFAULT_FONT_NORMAL)
#define FONT_BIG		cfg.readNumEntry("FONT","Big",DEFAULT_FONT_BIG)
#define FONT_PATH		cfg.readEntry("FONT","Path",DEFAULT_FONT_PATH)
#define FONT_COLOR_R	cfg.readNumEntry("FONT","Color_R",DEFAULT_FONT_COLOR_R)
#define FONT_COLOR_G	cfg.readNumEntry("FONT","Color_G",DEFAULT_FONT_COLOR_G)
#define FONT_COLOR_B	cfg.readNumEntry("FONT","Color_B",DEFAULT_FONT_COLOR_B)

#define INFO_VISIBLE	cfg.readBoolEntry("WDG_INFO","StartupVisible",DEFAULT_INFO_VISIBLE)
#define INFO_MIN_VIS	cfg.readNumEntry("WDG_INFO","MinVisible",DEFAULT_INFO_MIN_VIS)
#define INFO_STAT_Y		cfg.readNumEntry("WDG_INFO","StatusY",DEFAULT_INFO_STAT_Y)
#define INFO_BUT_Y		cfg.readNumEntry("WDG_INFO","ButtonY",DEFAULT_INFO_BUT_Y)

#define UNLOCK_VISIBLE	cfg.readBoolEntry("WDG_UNLOCK","StartupVisible",DEFAULT_UNLOCK_VISIBLE)
#define UNLOCK_MIN_VIS	cfg.readNumEntry("WDG_UNLOCK","MinVisible",DEFAULT_UNLOCK_MIN_VIS)
#define UNLOCK_ARR_X	cfg.readNumEntry("WDG_UNLOCK","ArrowX",DEFAULT_UNLOCK_ARR_X)
#define UNLOCK_ARR_Y	cfg.readNumEntry("WDG_UNLOCK","ArrowY",DEFAULT_UNLOCK_ARR_Y)
#define UNLOCK_ARR_M_X	cfg.readNumEntry("FONT","ArrowXMax",DEFAULT_UNLOCK_ARR_M_X)
#define UNLOCK_ARR_RET	cfg.readNumEntry("WDG_UNLOCK","ArrowRetSpeed",DEFAULT_UNLOCK_ARR_RET)

#define SETT_SLIDE_SPD	cfg.readNumEntry("SETTINGS","SlideSpeed",DEFAULT_SETT_SLIDE_SPD)


/** Configuration write aliases **/
#define SET_LIMITFPS(var)		cfg.writeEntry("FPS","LimitFPS",var)
#define SET_SCREENMAXFPS(var)	cfg.writeEntry("FPS","ScreenMaxFPS",var)
#define SET_LIGHTTIME(var)		cfg.writeEntry("FPS","LightTime",var)

#define SET_LCDOFFSECONDS(var)	cfg.writeEntry("SCREEN","LCDOffSeconds",var)
#define SET_LIGHTUPKEY(var)		cfg.writeEntry("SCREEN","LightUpKey",var)
#define SET_ENABLE_FADE(var)	cfg.writeEntry("SCREEN","EnableFade",var)
#define SET_FADE_TIME(var)		cfg.writeEntry("SCREEN","FadeTime",var)
#define SET_FADE_STEPS(var)		cfg.writeEntry("SCREEN","FadeSteps",var)

#define SET_CLOCKX(var)			cfg.writeEntry("WDG_CLOCK","X",var)
#define SET_CLOCKY(var)			cfg.writeEntry("WDG_CLOCK","Y",var)

#define SET_FONT_SMALL(var)		cfg.writeEntry("FONT","Small",var)
#define SET_FONT_NORMAL(var)	cfg.writeEntry("FONT","Normal",var)
#define SET_FONT_BIG(var)		cfg.writeEntry("FONT","Big",var)
#define SET_FONT_PATH(var)		cfg.writeEntry("FONT","Path",var)
#define SET_FONT_COLOR_R(var)	cfg.writeEntry("FONT","Color_R",var)
#define SET_FONT_COLOR_G(var)	cfg.writeEntry("FONT","Color_G",var)
#define SET_FONT_COLOR_B(var)	cfg.writeEntry("FONT","Color_B",var)

#define SET_INFO_VISIBLE(var)	cfg.writeEntry("WDG_INFO","StartupVisible",var)
#define SET_INFO_MIN_VIS(var)	cfg.writeEntry("WDG_INFO","MinVisible",var)
#define SET_INFO_STAT_Y(var)	cfg.writeEntry("WDG_INFO","StatusY",var)
#define SET_INFO_BUT_Y(var)		cfg.writeEntry("WDG_INFO","ButtonY",var)

#define SET_UNLOCK_VISIBLE(var)	cfg.writeEntry("WDG_UNLOCK","StartupVisible",var)
#define SET_UNLOCK_MIN_VIS(var)	cfg.writeEntry("WDG_UNLOCK","MinVisible",var)
#define SET_UNLOCK_ARR_X(var)	cfg.writeEntry("WDG_UNLOCK","ArrowX",var)
#define SET_UNLOCK_ARR_Y(var)	cfg.writeEntry("WDG_UNLOCK","ArrowY",var)
#define SET_UNLOCK_ARR_M_X(var)	cfg.writeEntry("FONT","ArrowXMax",var)
#define SET_UNLOCK_ARR_RET(var)	cfg.writeEntry("WDG_UNLOCK","ArrowRetSpeed",var)

#define SET_SETT_SLIDE_SPD(var)	cfg.writeEntry("SETTINGS","SlideSpeed",var)



/** Includes **/
#include <stdio.h>
#include <stdlib.h>
#include <fstream.h>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "timer.h"			// timer class to count the time between 2 cycles
#include "debug.h"			// debugging functions
#include "graphics.h"		// rendering functions
#include "ezx.h"			// ezx platform functions
#include "app.h"			// main application class
#include "settings.h"		// settings gui class
#include "wdg_clock.h"		// clock widget
#include "wdg_unlock.h"		// unlocker widget
#include "wdg_info.h"		// information panel widget


/** Global available classes **/
extern	class GRAPHICS		gfx;
extern	class ZConfig		cfg;
extern	class iCLOCK		iclock;
extern	class unLOCK		unLock;
extern	class INFO_PANEL	infop;
extern	class CEZX			ezx;
extern	class APP			app;


#endif /* _MAIN_H_ */
