/*
	madbomber.c
	
	by Bill Kendrick
	bill@newbreedsoftware.com
	http://www.newbreedsoftware.com/
	
	October 25, 1999 - March 16, 2003
	
	---------------------------------------
	
	Changed from C to CPP
	Added SoftKeyboard for highscore name input.
	Merged features of the PC version into the EMBEDDED (EZX) version
	
	Ported to EZX platform by BruceLee
	Contact:	http://www.motorolafans.com/forums
	Port date:	October 23, 2010
*/


#define EZX
//#define WITHOUT_SK

#ifndef EZX
	#define VERSION "0.2.5"
#else
	#define VERSION "0.2.5-ezx"
	#define SDL_MIXER_BUG
#endif



/* #includes: */

#ifndef WITHOUT_SK
	#include "soft-keyboard.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <math.h>

#ifdef __cplusplus
	#include <SDL/SDL.h>
	#include <SDL/SDL_image.h>
	#ifndef NOSOUND
		#include <SDL/SDL_mixer.h>
	#endif
#else
	#include <SDL.h>
	#include <SDL_image.h>
	#ifndef NOSOUND
		#include <SDL_mixer.h>
	#endif
#endif


/* Image enumerations: */

enum {
	IMG_LOADING,
	IMG_BAR,
	IMG_TITLE_TITLE,
	IMG_TITLE_ONE_PLAYER,
	IMG_TITLE_ONE_PLAYER_OFF,
	IMG_TITLE_TWO_PLAYERS,
	IMG_TITLE_TWO_PLAYERS_OFF,
#ifndef EZX
	IMG_TITLE_TWO_PLAYER_VS,
	IMG_TITLE_TWO_PLAYER_VS_OFF,
#else
	IMG_TITLE_CONTINUE,
	IMG_TITLE_CONTINUE_OFF,
#endif
	IMG_TITLE_OPTIONS,
	IMG_TITLE_OPTIONS_OFF,
	IMG_TITLE_HIGHSCORE,
	IMG_TITLE_HIGHSCORE_OFF,
	IMG_TITLE_EXIT,
	IMG_TITLE_EXIT_OFF,
	IMG_OPTIONS_OPTIONS,
	IMG_OPTIONS_DETAIL,
	IMG_OPTIONS_NORMAL,
	IMG_OPTIONS_NORMAL_OFF,
	IMG_OPTIONS_ZEN,
	IMG_OPTIONS_ZEN_OFF,
	IMG_OPTIONS_PLAYER_ONE,
	IMG_OPTIONS_PLAYER_TWO,
	IMG_OPTIONS_DIFFICULTY,
	IMG_OPTIONS_HARD,
	IMG_OPTIONS_HARD_OFF,
	IMG_OPTIONS_EFFECTS,
	IMG_OPTIONS_MUSIC,
	IMG_OPTIONS_VOLUME,
	IMG_OPTIONS_0PERCENT,
	IMG_OPTIONS_0PERCENT_OFF,
	IMG_OPTIONS_25PERCENT,
	IMG_OPTIONS_25PERCENT_OFF,
	IMG_OPTIONS_50PERCENT,
	IMG_OPTIONS_50PERCENT_OFF,
	IMG_OPTIONS_75PERCENT,
	IMG_OPTIONS_75PERCENT_OFF,
	IMG_OPTIONS_100PERCENT,
	IMG_OPTIONS_100PERCENT_OFF,
	IMG_OPTIONS_OK,
	IMG_OPTIONS_OK_OFF,
	IMG_BACKGROUND,
	IMG_BUCKET1,
	IMG_BUCKET2,
	IMG_BUCKET3,
	IMG_BUCKET1RED,
	IMG_BUCKET2RED,
	IMG_BUCKET3RED,
	IMG_SMALL_BUCKET1,
	IMG_SMALL_BUCKET2,
	IMG_SMALL_BUCKET3,
	IMG_SMALL_BUCKET1RED,
	IMG_SMALL_BUCKET2RED,
	IMG_SMALL_BUCKET3RED,
	IMG_BOMB0,
	IMG_BOMB1,
	IMG_BOMB2,
	IMG_BOMB3,
	IMG_EXPLOSION1,
	IMG_EXPLOSION2,
	IMG_BOMBER_SAD,
	IMG_BOMBER_HAPPY,
	IMG_BOMBER_AMAZED,
	IMG_DROP_UP,
	IMG_DROP_RIGHT_UP,
	IMG_DROP_RIGHT,
	IMG_DROP_RIGHT_DOWN,
	IMG_DROP_DOWN,
	IMG_DROP_LEFT_DOWN,
	IMG_DROP_LEFT,
	IMG_DROP_LEFT_UP,
	IMG_0,
	IMG_1,
	IMG_2,
	IMG_3,
	IMG_4,
	IMG_5,
	IMG_6,
	IMG_7,
	IMG_8,
	IMG_9,
	IMG_0RED,
	IMG_1RED,
	IMG_2RED,
	IMG_3RED,
	IMG_4RED,
	IMG_5RED,
	IMG_6RED,
	IMG_7RED,
	IMG_8RED,
	IMG_9RED,
	IMG_HIGHSCORE,
	IMG_PAUSED,
	IMG_GAME_OVER,
	IMG_PRESS_FIRE,
	IMG_PRESS_ESCAPE,
	IMG_SPRAY_CYAN,
	IMG_SPRAY_BLACK,
	IMG_SPRAY_BLUE,
	IMG_HIGHSCORE_HIGHSCORE,
#ifndef EZX
	IMG_HIGHSCORE_LETTERS,
#endif
	NUM_IMAGES
};


/* Image filenames: */

const char * image_names[NUM_IMAGES] = {
	"data/images/loading.png",
	"data/images/bar.png",
	"data/images/title/title.png",
	"data/images/title/one-player.png",
	"data/images/title/one-player-off.png",
	"data/images/title/two-players.png",
	"data/images/title/two-players-off.png",
#ifndef EZX
	"data/images/title/two-player-vs.png",
	"data/images/title/two-player-vs-off.png",
#else
	"data/images/title/continue.png",
	"data/images/title/continue-off.png",
#endif
	"data/images/title/options.png",
	"data/images/title/options-off.png",
	"data/images/title/highscore.png",
	"data/images/title/highscore-off.png",
	"data/images/title/exit.png",
	"data/images/title/exit-off.png",
	"data/images/options/options.png",
	"data/images/options/detail.png",
	"data/images/options/normal.png",
	"data/images/options/normal-off.png",
	"data/images/options/zen.png",
	"data/images/options/zen-off.png",
	"data/images/options/player-one.png",
	"data/images/options/player-two.png",
	"data/images/options/difficulty.png",
	"data/images/options/hard.png",
	"data/images/options/hard-off.png",
	"data/images/options/effects.png",
	"data/images/options/music.png",
	"data/images/options/volume.png",
	"data/images/options/0percent.png",
	"data/images/options/0percent-off.png",
	"data/images/options/25percent.png",
	"data/images/options/25percent-off.png",
	"data/images/options/50percent.png",
	"data/images/options/50percent-off.png",
	"data/images/options/75percent.png",
	"data/images/options/75percent-off.png",
	"data/images/options/100percent.png",
	"data/images/options/100percent-off.png",
	"data/images/options/ok.png",
	"data/images/options/ok-off.png",
	"data/images/game/background.png",
	"data/images/game/bucket/bucket1.png",
	"data/images/game/bucket/bucket2.png",
	"data/images/game/bucket/bucket3.png",
	"data/images/game/bucket/bucket1red.png",
	"data/images/game/bucket/bucket2red.png",
	"data/images/game/bucket/bucket3red.png",
	"data/images/game/bucket/small-bucket1.png",
	"data/images/game/bucket/small-bucket2.png",
	"data/images/game/bucket/small-bucket2.png",
	"data/images/game/bucket/small-bucket1red.png",
	"data/images/game/bucket/small-bucket2red.png",
	"data/images/game/bucket/small-bucket2red.png",
	"data/images/game/bomb/bomb0.png",
	"data/images/game/bomb/bomb1.png",
	"data/images/game/bomb/bomb2.png",
	"data/images/game/bomb/bomb3.png",
	"data/images/game/bomb/explosion1.png",
	"data/images/game/bomb/explosion2.png",
	"data/images/game/bomber/bomber-sad.png",
	"data/images/game/bomber/bomber-happy.png",
	"data/images/game/bomber/bomber-amazed.png",
	"data/images/game/drop/drop-up.png",
	"data/images/game/drop/drop-right-up.png",
	"data/images/game/drop/drop-right.png",
	"data/images/game/drop/drop-right-down.png",
	"data/images/game/drop/drop-down.png",
	"data/images/game/drop/drop-left-down.png",
	"data/images/game/drop/drop-left.png",
	"data/images/game/drop/drop-left-up.png",
	"data/images/game/numbers/0.png",
	"data/images/game/numbers/1.png",
	"data/images/game/numbers/2.png",
	"data/images/game/numbers/3.png",
	"data/images/game/numbers/4.png",
	"data/images/game/numbers/5.png",
	"data/images/game/numbers/6.png",
	"data/images/game/numbers/7.png",
	"data/images/game/numbers/8.png",
	"data/images/game/numbers/9.png",
	"data/images/game/numbers/0red.png",
	"data/images/game/numbers/1red.png",
	"data/images/game/numbers/2red.png",
	"data/images/game/numbers/3red.png",
	"data/images/game/numbers/4red.png",
	"data/images/game/numbers/5red.png",
	"data/images/game/numbers/6red.png",
	"data/images/game/numbers/7red.png",
	"data/images/game/numbers/8red.png",
	"data/images/game/numbers/9red.png",
	"data/images/game/highscore.png",
	"data/images/game/paused.png",
	"data/images/game/game-over.png",
	"data/images/game/press-fire.png",
	"data/images/game/press-escape.png",
	"data/images/spray-cyan.png",
	"data/images/spray-black.png",
	"data/images/spray-blue.png",
	"data/images/highscore/highscore.png",
#ifndef EZX
	"data/images/highscore/letters.png"
#endif
};


/* Sound enumerations: */

enum {
	SND_SELECT,
	SND_CONFIRM,
	SND_FUSE,
	SND_EXPLOSION,
	SND_BIGEXPLOSION,
	SND_SPLASH1,
	SND_SPLASH2,
	SND_SPLASH3,
	SND_SPLASH4,
	SND_SPLASH5,
	SND_SPLASH6,
	SND_SPLASH7,
	SND_SPLASH8,
	SND_ONEUP,
	SND_LEVEL8,
	SND_HIGHSCORE,
	SND_SPRAY,
	SND_CAN_SHAKE,
	NUM_SOUNDS
};


/* Sound filenames: */

const char * sound_names[NUM_SOUNDS] = {
	"data/sounds/select.wav",
	"data/sounds/confirm.wav",
	"data/sounds/fuse.wav",
	"data/sounds/explosion.wav",
	"data/sounds/bigexplosion.wav",
	"data/sounds/splash1.wav",
	"data/sounds/splash2.wav",
	"data/sounds/splash3.wav",
	"data/sounds/splash4.wav",
	"data/sounds/splash5.wav",
	"data/sounds/splash6.wav",
	"data/sounds/splash7.wav",
	"data/sounds/splash8.wav",
	"data/sounds/oneup.wav",
	"data/sounds/level8.wav",
	"data/sounds/highscore.wav",
	"data/sounds/spray.wav",
	"data/sounds/can-shake.wav"
};


/* Music filenames: */

//#ifndef EZX

	#define MUS_TITLE "data/music/bizjung.it"
	#define MUS_HIGHSCORE "data/music/waterfal.mod"

	#define NUM_GAME_MUSICS 3

	const char * game_music_names[NUM_GAME_MUSICS] = {
		"data/music/fdn-arab.s3m",
		"data/music/gluppobe.mod",
		"data/music/astraltr.mod"
	};

/*#else

	#define MUS_TITLE "data/music/winterwi.mod"

	#define NUM_GAME_MUSICS 1

	const char * game_music_names[NUM_GAME_MUSICS] = {
		"data/music/fdn-arab.s3m"
	};

#endif*/


/* Title Options: */

enum {
	TITLE_OPTION_ONE_PLAYER,
	TITLE_OPTION_TWO_PLAYERS,
#ifndef EZX
	TITLE_OPTION_TWO_PLAYER_VS,
#else
	TITLE_OPTION_CONTINUE,
#endif
	TITLE_OPTION_OPTIONS,
	TITLE_OPTION_HIGHSCORE,
	TITLE_OPTION_EXIT,
	NUM_TITLE_OPTIONS
};


/* Title option images: */

const int title_option_images[NUM_TITLE_OPTIONS] = {
	IMG_TITLE_ONE_PLAYER,
	IMG_TITLE_TWO_PLAYERS,
#ifndef EZX
	IMG_TITLE_TWO_PLAYER_VS,
#else
	IMG_TITLE_CONTINUE,
#endif
	IMG_TITLE_OPTIONS,
	IMG_TITLE_HIGHSCORE,
	IMG_TITLE_EXIT
};


/* Option screen option enumerations: */

enum {
	OPTIONS_OPTION_DETAIL,
	OPTIONS_OPTION_PLAYER_ONE,
	OPTIONS_OPTION_PLAYER_TWO,
	OPTIONS_OPTION_EFFECTS,
	OPTIONS_OPTION_MUSIC,
	OPTIONS_OPTION_OK,
	NUM_OPTIONS_OPTIONS
};


#ifdef NOSOUND
	#define NOSOUND_CONTROLS
#endif



/* Flying number layouts: */

int flying_number_layouts[8][7] = {
	{	0,
	 0,	1,
		 0,
	 0,	1,
		 0},

	{	1,
	 0,	1,
		 1,
	 1,	0,
		 1},

	{	1,
	 0,	1,
		 1,
	 0,	1,
		 1},

	{	0,
	 1,	1,
		 1,
	 0,	1,
		 0},

	{	1,
	 1,	0,
		 1,
	 0,	1,
		 1},

	{	1,
	 1,	0,
		 1,
	 1,	1,
		 1},

	{	1,
	 0,	1,
		 0,
	 0,	1,
		 0},

	{	1,
	 1,	1,
		 1,
	 1,	1,
		 1}
};



/* Graffiti letters: */

int graffiti[36][5][2][2] = {
	{
		/* 0 */
		{ { 2, 0 }, { 0, 4 } },
		{ { 0, 4 }, { 2, 8 } },
		{ { 2, 8 }, { 4, 4 } },
		{ { 4, 4 }, { 2, 0 } },
		{ { 4, 0 }, { 0, 8 } }
	},
	{
		/* 1 */
		{ { 0, 1 }, { 2, 0 } },
		{ { 2, 0 }, { 2, 8 } },
		{ { -1, -1 }, { -1, -1 } },
		{ { -1, -1 }, { -1, -1 } },
		{ { -1, -1 }, { -1, -1 } }
	},
	{
		/* 2 */
		{ { 1, 1 }, { 2, 0 } },
		{ { 2, 0 }, { 3, 0 } },
		{ { 3, 0 }, { 4, 3 } },
		{ { 4, 3 }, { 0, 8 } },
		{ { 0, 8 }, { 4, 8 } }
	},
	{
		/* 3 */
		{ { 0, 0 }, { 4, 2 } },
		{ { 4, 2 }, { 1, 4 } },
		{ { 1, 4 }, { 4, 6 } },
		{ { 4, 6 }, { 0, 8 } },
		{ { -1, -1 }, { -1, -1 } }
	},
	{
		/* 4 */
		{ { 1, 0 }, { 0, 4 } },
		{ { 0, 4 }, { 4, 4 } },
		{ { 4, 0 }, { 2, 8 } },
		{ { -1, -1 }, { -1, -1 } },
		{ { -1, -1 }, { -1, -1 } }
	},
	{
		/* 5 */
		{ { 4, 0 }, { 0, 0 } },
		{ { 0, 0 }, { 0, 5 } },
		{ { 0, 5 }, { 3, 4 } },
		{ { 3, 4 }, { 4, 6 } },
		{ { 4, 6 }, { 0, 8 } }
	},
	{
		/* 6 */
		{ { 4, 0 }, { 0, 6 } },
		{ { 0, 6 }, { 2, 8 } },
		{ { 2, 8 }, { 4, 6 } },
		{ { 4, 6 }, { 2, 4 } },
		{ { -1, -1 }, { -1, -1 } }
	},
	{
		/* 7 */
		{ { 0, 1 }, { 4, 0 } },
		{ { 4, 0 }, { 1, 8 } },
		{ { 1, 4 }, { 4, 3 } },
		{ { -1, -1 }, { -1, -1 } },
		{ { -1, -1 }, { -1, -1 } }
	},
	{
		/* 8 */
		{ { 0, 1 }, { 4, 0 } },
		{ { 4, 0 }, { 0, 5 } },
		{ { 0, 5 }, { 2, 8 } },
		{ { 2, 8 }, { 4, 5 } },
		{ { 4, 5 }, { 0, 1 } }
	},
	{
		/* 9 */
		{ { 4, 0 }, { 1, 1 } },
		{ { 1, 1 }, { 0, 4 } },
		{ { 0, 4 }, { 2, 3 } },
		{ { 2, 3 }, { 4, 0 } },
		{ { 4, 0 }, { 3, 8 } }
	},
	{
		/* A */
		{ { 0, 8 }, { 2, 0 } },
		{ { 2, 0 }, { 4, 8 } },
		{ { 0, 7 }, { 4, 4 } },
		{ { -1, -1 }, { -1, -1 } },
		{ { -1, -1 }, { -1, -1 } }
	},
	{
		/* B */
		{ { 0, 8 }, { 0, 0 } },
		{ { 0, 0 }, { 4, 2 } },
		{ { 4, 2 }, { 0, 4 } },
		{ { 0, 4 }, { 4, 6 } },
		{ { 4, 6 }, { 0, 8 } }
	},
	{
		/* C */
		{ { 4, 0 }, { 0, 4 } },
		{ { 0, 4 }, { 4, 8 } },
		{ { -1, -1 }, { -1, -1 } },
		{ { -1, -1 }, { -1, -1 } },
		{ { -1, -1 }, { -1, -1 } }
	},
	{
		/* D */
		{ { 0, 0 }, { 0, 8 } },
		{ { 0, 8 }, { 4, 4 } },
		{ { 4, 4 }, { 0, 0 } },
		{ { -1, -1 }, { -1, -1 } },
		{ { -1, -1 }, { -1, -1 } }
	},
	{
		/* E */
		{ { 4, 0 }, { 0, 2 } },
		{ { 0, 2 }, { 3, 4 } },
		{ { 3, 4 }, { 0, 6 } },
		{ { 0, 6 }, { 4, 8 } },
		{ { -1, -1 }, { -1, -1 } }
	},
	{
		/* F */
		{ { 4, 0 }, { 0, 1 } },
		{ { 0, 1 }, { 1, 8 } },
		{ { 0, 4 }, { 4, 3 } },
		{ { -1, -1 }, { -1, -1 } },
		{ { -1, -1 }, { -1, -1 } }
	},
	{
		/* G */
		{ { 4, 0 }, { 0, 4 } },
		{ { 0, 4 }, { 4, 8 } },
		{ { 4, 8 }, { 4, 4 } },
		{ { -1, -1 }, { -1, -1 } },
		{ { -1, -1 }, { -1, -1 } }
	},
	{
		/* H */
		{ { 1, 0 }, { 0, 8 } },
		{ { 4, 0 }, { 4, 8 } },
		{ { 0, 4 }, { 5, 3 } },
		{ { -1, -1 }, { -1, -1 } },
		{ { -1, -1 }, { -1, -1 } }
	},
	{
		/* I */
		{ { 3, 0 }, { 2, 8 } },
		{ { -1, -1 }, { -1, -1 } },
		{ { -1, -1 }, { -1, -1 } },
		{ { -1, -1 }, { -1, -1 } },
		{ { -1, -1 }, { -1, -1 } }
	},
	{
		/* J */
		{ { 4, 0 }, { 4, 6 } },
		{ { 4, 6 }, { 2, 8 } },
		{ { 2, 8 }, { 0, 5 } },
		{ { -1, -1 }, { -1, -1 } },
		{ { -1, -1 }, { -1, -1 } }
	},
	{
		/* K */
		{ { 0, 0 }, { 0, 8 } },
		{ { 3, 0 }, { 0, 4 } },
		{ { 0, 4 }, { 4, 8 } },
		{ { -1, -1 }, { -1, -1 } },
		{ { -1, -1 }, { -1, -1 } }
	},
	{
		/* L */
		{ { 0, 0 }, { 0, 8 } },
		{ { 0, 8 }, { 4, 7 } },
		{ { -1, -1 }, { -1, -1 } },
		{ { -1, -1 }, { -1, -1 } },
		{ { -1, -1 }, { -1, -1 } }
	},
	{
		/* M */
		{ { 0, 8 }, { 1, 0 } },
		{ { 1, 0 }, { 2, 4 } },
		{ { 2, 4 }, { 4, 0 } },
		{ { 4, 0 }, { 4, 8 } },
		{ { -1, -1 }, { -1, -1 } }
	},
	{
		/* N */
		{ { 0, 8 }, { 0, 0 } },
		{ { 0, 0 }, { 4, 8 } },
		{ { 4, 8 }, { 4, 1 } },
		{ { -1, -1 }, { -1, -1 } },
		{ { -1, -1 }, { -1, -1 } }
	},
	{
		/* O */
		{ { 2, 0 }, { 0, 4 } },
		{ { 0, 4 }, { 2, 8 } },
		{ { 2, 8 }, { 4, 4 } },
		{ { 4, 4 }, { 2, 0 } },
		{ { -1, -1 }, { -1, -1 } }
	},
	{
		/* P */
		{ { 0, 8 }, { 0, 0 } },
		{ { 0, 0 }, { 4, 2 } },
		{ { 4, 2 }, { 0, 4 } },
		{ { -1, -1 }, { -1, -1 } },
		{ { -1, -1 }, { -1, -1 } }
	},
	{
		/* Q */
		{ { 2, 0 }, { 0, 4 } },
		{ { 0, 4 }, { 2, 8 } },
		{ { 2, 8 }, { 4, 4 } },
		{ { 4, 4 }, { 2, 0 } },
		{ { 2, 4 }, { 4, 8 } }
	},
	{
		/* R */
		{ { 0, 8 }, { 0, 0 } },
		{ { 0, 0 }, { 4, 2 } },
		{ { 4, 2 }, { 0, 4 } },
		{ { 0, 4 }, { 4, 8 } },
		{ { -1, -1 }, { -1, -1 } }
	},
	{
		/* S */
		{ { 4, 0 }, { 0, 3 } },
		{ { 0, 2 }, { 4, 5 } },
		{ { 4, 5 }, { 0, 8 } },
		{ { -1, -1 }, { -1, -1 } },
		{ { -1, -1 }, { -1, -1 } }
	},
	{
		/* T */
		{ { 0, 0 }, { 4, 0 } },
		{ { 2, 0 }, { 1, 8 } },
		{ { -1, -1 }, { -1, -1 } },
		{ { -1, -1 }, { -1, -1 } },
		{ { -1, -1 }, { -1, -1 } }
	},
	{
		/* U */
		{ { 0, 0 }, { 0, 8 } },
		{ { 0, 8 }, { 4, 7 } },
		{ { 4, 7 }, { 4, 0 } },
		{ { -1, -1 }, { -1, -1 } },
		{ { -1, -1 }, { -1, -1 } }
	},
	{
		/* V */
		{ { 0, 0 }, { 2, 8 } },
		{ { 2, 8 }, { 4, 0 } },
		{ { -1, -1 }, { -1, -1 } },
		{ { -1, -1 }, { -1, -1 } },
		{ { -1, -1 }, { -1, -1 } }
	},
	{
		/* W */
		{ { 0, 0 }, { 1, 8 } },
		{ { 1, 8 }, { 2, 5 } },
		{ { 2, 5 }, { 3, 8 } },
		{ { 3, 8 }, { 4, 0 } },
		{ { -1, -1 }, { -1, -1 } }
	},
	{
		/* X */
		{ { 0, 0 }, { 4, 8 } },
		{ { 4, 0 }, { 0, 8 } },
		{ { -1, -1 }, { -1, -1 } },
		{ { -1, -1 }, { -1, -1 } },
		{ { -1, -1 }, { -1, -1 } }
	},
	{
		/* Y */
		{ { 0, 0 }, { 2, 4 } },
		{ { 4, 0 }, { 0, 8 } },
		{ { -1, -1 }, { -1, -1 } },
		{ { -1, -1 }, { -1, -1 } },
		{ { -1, -1 }, { -1, -1 } }
	},
	{
		/* Z */
		{ { 0, 0 }, { 4, 0 } },
		{ { 4, 0 }, { 0, 8 } },
		{ { 0, 8 }, { 4, 8 } },
		{ { -1, -1 }, { -1, -1 } },
		{ { -1, -1 }, { -1, -1 } }
	}
};


/* Bomb per level: */

int bombs_per_level[8] = {
	10,
	20,
	30,
	40,
	50,
	75,
	100,
	150
};


/* Bombs: */

#define MAX_BOMBS 128

typedef struct bomb_type {
	int alive, x, y;
} bomb_type;


/* Drops: */

#define MAX_DROPS 256

typedef struct drop_type {
	int alive, timer, x, y, xm, ym;
} drop_type;


/* Spray drips: */

#define MAX_SPRAYDRIPS 16

typedef struct spraydrip_type {
	int alive, timer, img, x, y;
} spraydrip_type;


/* Globals: */

int use_sound, use_fullscreen, zen, effects_volume, music_volume,
	num_rects, highscore, bomber_x, player, frame, vs_mode, spray_count,
	quick_spray;

#ifdef EZX
	int can_continue;
#endif

int level[2], past_level_8[2], half_level[2],
	old_x[2], one_up_score[2], have_highscore[2];
int bomber_go, bomber_happy, bombs_left, no_active_bombs,
	bomber_xm, bomber_xm_time,
	left_down, right_down, bomber_left_down, bomber_right_down,
	bomber_left_left_down, bomber_right_right_down,
	img, explode_the_bombs, game_over, drop_bomb, num_players;

int x[2], width[2], num_buckets[2], score[2];

char highscorer[13];
SDL_Surface * screen, * zen_background;
SDL_Surface * images[NUM_IMAGES];
SDL_Rect option_box;
SDL_Rect rects[(MAX_BOMBS + MAX_DROPS + 4) * 2 + 128];
bomb_type bombs[MAX_BOMBS];
drop_type drops[MAX_DROPS];
spraydrip_type spraydrips[MAX_SPRAYDRIPS];

#ifndef NOSOUND
	Mix_Chunk * sounds[NUM_SOUNDS];
	Mix_Music * title_music;
//	#ifndef EZX
		Mix_Music * highscore_music;
//	#endif
	Mix_Music * game_musics[NUM_GAME_MUSICS];
#endif

#ifdef EZX
	int MULTIPLIER=2;
#else
	int MULTIPLIER=1;
#endif


/* Local function prototypes: */
#ifdef __cplusplus
extern "C" {
#endif
	int game(int mode);
	int title(void);
	void setup(void);
	void mb_shutdown(void);
	int widest(int w1, int w2);
	void addrect(SDL_Rect rect);
	void addbomb(int x);
	void addsplash(int x, int y);
	void adddrop(int x, int y, int xm);
	void playsound(int snd, int chan);
	void drawscore(int score, int player);
	int pausescreen(void);
	void explodebombs(void);
	void erase(SDL_Rect dest);
	FILE * open_option_file(char * mode);
	#ifdef EZX
		FILE * open_state_file(char * mode);
	#endif
	int optionscreen(void);
	void show_option_img(int horiz, int y, int img);
	void show_option_meter(int horiz, int value, int selected);
	void draw_flying_numbers(int number, int scale, int r, int g, int b);
	void erase_flying_numbers(int scale);
	int highscorescreen(void);
	int spraytext(char * str, int y, int img, int scale);
	void sprayline(int x1, int y1, int x2, int y2, int img);
	void spraydot(int x, int y, int img);
	void handle_spraydrips(void);
	void add_spraydrip(int x, int y, int img);
	int sign_highscore(int player);
	void my_blit(SDL_Surface * src_img, SDL_Rect * src_rect, SDL_Surface * dest_img, SDL_Rect * dest_rect);
	void my_updaterect(SDL_Surface * surf, int x, int y, int w, int h);
	void my_fillrect(SDL_Surface * surf, SDL_Rect * dest, Uint32 color);
#ifdef __cplusplus
}
#endif


/* --- MAIN --- */

int main(int argc, char * argv[])
{
	FILE * fi;
	int mode, quit, i;
	char temp[512];


	/* (Use sound, by default): */

	use_sound = 1;
	use_fullscreen = 0;


	/* Check for arguments: */

	if (argc == 2)
		{
			if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-?") == 0)
	{
		/* Display help stuff: */
	
		printf("\n"
		 "-------------------------------------"
		 "------------------------------\n"
		 "Mad Bomber version " VERSION "\n"
		 "Copyright (C) 1999-2002 by Bill Kendrick, "
		 "bill@newbreedsoftware.com\n"
	#ifdef EZX
		 "-------------------------------------"
		 "------------------------------\n"
		 "Ported to EZX A1200/E6 by BruceLee (2010)\n"
		 "http://www.motorolafans.com/forums\n"
	#endif
		 "-------------------------------------"
		 "------------------------------\n"
		 "\n"
		 "This program is free software; you can redistribute it\n"
		 "and/or modify it under the terms of the GNU General Public\n"
		 "License as published by the Free Software Foundation;\n"
		 "either version 2 of the License, or (at your option) any\n"
		 "later version.\n"
		 "\n"
		 "This program is distributed in the hope that it will be\n"
		 "useful and entertaning, but WITHOUT ANY WARRANTY; without\n"
		 "even the implied warranty of MERCHANTABILITY or FITNESS\n"
		 "FOR A PARTICULAR PURPOSE.	See the GNU General Public\n"
		 "License for more details.\n"
		 "\n"
		 "You should have received a copy of the GNU General Public\n"
		 "License along with this program; if not, write to the Free\n"
		 "Software Foundation, Inc., 59 Temple Place, Suite 330,\n"
		 "Boston, MA	02111-1307	USA\n"
		 "\n");

		printf("Usage: %s [OPTION]\n\n", argv[0]);

		printf("Game options\n"
	#ifndef NOSOUND
		 "	--disable-sound		Disable sound\n"
	#endif
	#ifndef EZX
		 "	--fullscreen			 (Try to) run in fullscreen\n"
	#endif
		 "\n"
		 "Help options\n"
		 "	-?, --help				 Show this help message\n"
		 "	--usage						Display brief usage message\n"
		 "\n"
		 "Title screen controls\n"
		 "	Up/Down						Choose option\n"
	#ifndef EZX
		 "	Space/Return			 Activate option\n"
		 "	Mouse-click				Activate option under mouse pointer\n"
		 "	Escape						 Quit program\n"
	#else
		 "	Selector					 Activate option\n"
		 "	Stylus-tap				 Activate option under stylus\n"
		 "	Cancel						 Quit program\n"
	#endif
		 "\n"
		 "Option screen controls\n"
		 "	Up/Down Arrows		 Choose option\n"
	#ifndef EZX
		 "	Space/Return			 Activate option\n"
		 "	Mouse-click				Activate option under mouse pointer\n"
		 "	Escape						 Return to title screen\n"
	#else
		 "	Selector					 Activate option\n"
		 "	Stylus-tap				 Activate option under stylus\n"
		 "	Cancel						 Return to title screen\n"
	#endif
		 "\n"
		 "Game controls\n"
		 "	Left/Right Arrows	Move buckets\n"
	#ifndef EZX
		 "	Mouse-motion			 Move buckets\n"
		 "	Space/Return			 Fire\n"
		 "	Mouse-click				Fire\n"
		 "	1/2/3/4						(in Vs. mode) Move Mad Bomber\n"
		 "										 (1 = left, fast\n"
		 "											2 = left, slow\n"
		 "											3 = right, slow\n"
		 "											4 = right, fast)\n"
		 "	Tab/P							Pause\n"
	#else
		 "	Stylus-drag				Move buckets\n"
		 "	Ok/Selector				Fire\n"
	#endif
		 "	Escape						 Quit game (return to title screen)\n"
		 "\n");
	
		exit(0);
	}
			else if (strcmp(argv[1], "--usage") == 0)
	{
		/* Display usage: */
	
		printf("Usage: %s [-?] [--help] [--usage] [--disable-sound] "
		 "[--fullscreen]\n",
		 argv[0]);
	
		exit(0);
	}
			else if (strcmp(argv[1], "--disable-sound") == 0)
	{
		/* Turn off sound: */
	
		use_sound = 0;
	}
			else if (strcmp(argv[1], "--fullscreen") == 0)
	{
		/* Turn on fullscreen mode: */
	
		use_fullscreen = 1;
	}
			else
	{
		/* Display usage (as an error!): */
	
		fprintf(stderr,
			"Usage: %s [-?] [--help] [--usage] [--disable-sound] "
			"[--fullscreen]\n",
			argv[0]);
	
		exit(1);
	}
		}
	else if (argc > 2)
		{ 
			/* Display usage (as an error!): */
		
			fprintf(stderr, "Usage: %s [-?] [--help] [--usage] [--disable-sound] "
				"[--fullscreen]\n",
				argv[0]);
		
			exit(1);
	 }


	/* Set defaults: */

	highscore = 100;
	strcpy(highscorer, "BRUCE LEE");
	zen = 0;
	width[0] = 2;
	width[1] = 2;
	effects_volume = 4;
	music_volume = 2;


	/* Load options: */

	fi = open_option_file("r");

	if (fi != NULL)
		{
			do
	{
		fgets(temp, sizeof(temp), fi);
	
		if (!feof(fi))
			{
				temp[strlen(temp) - 1] = '\0';
			
			
				/* Parse each line: */
			
				if (strstr(temp, "highscore=") == temp)
		{
			highscore = atoi(temp + 10);
		
			if (highscore == 0)
				highscore = 500;
		}
				else if (strstr(temp, "highscorer=") == temp)
		{
			if (strlen(temp) > 11 + sizeof(highscorer))
				temp[11 + sizeof(highscorer)] = '\0';
		
			strcpy(highscorer, temp + 11);
		}
				else if (strstr(temp, "details=") == temp)
		{
			if (strcmp(temp + 8, "zen") == 0)
				zen = 1;
		}
				else if (strstr(temp, "diff1=") == temp)
		{
			if (strcmp(temp + 6, "hard") == 0)
				width[0] = 1;
		}
				else if (strstr(temp, "diff2=") == temp)
		{
			if (strcmp(temp + 6, "hard") == 0)
				width[1] = 1;
		}
				else if (strstr(temp, "effects=") == temp)
		{
			effects_volume = atoi(temp + 8);
		
			if (effects_volume < 0)
				effects_volume = 0;
			else if (effects_volume > 8)
				effects_volume = 8;
		}
				else if (strstr(temp, "music=") == temp)
		{
			music_volume = atoi(temp + 6);
		
			if (music_volume < 0)
				music_volume = 0;
			else if (music_volume > 8)
				music_volume = 8;
		}
			}
	}
			while (!feof(fi));
		
			fclose(fi);
		}



#ifdef EZX
	/* Load state file: */

	can_continue = 0;


	/* Load state: */

	fi = open_state_file("r");

	if (fi != NULL)
		{
			can_continue = fgetc(fi);

			if (can_continue)
			{
				num_players = fgetc(fi);
				player = fgetc(fi);

				fread(&bomber_x, sizeof(int), 1, fi);

				fread(level, sizeof(int), 2, fi);
				fread(past_level_8, sizeof(int), 2, fi);
				fread(half_level, sizeof(int), 2, fi);
				fread(old_x, sizeof(int), 2, fi);
				fread(one_up_score, sizeof(int), 2, fi);
				fread(have_highscore, sizeof(int), 2, fi);
				fread(&bomber_go, sizeof(int), 1, fi);
				fread(&bomber_happy, sizeof(int), 1, fi);
				fread(&bombs_left, sizeof(int), 1, fi);
				fread(&no_active_bombs, sizeof(int), 1, fi);
				fread(&bomber_xm, sizeof(int), 1, fi);
				fread(&bomber_xm_time, sizeof(int), 1, fi);
				fread(&img, sizeof(int), 1, fi);
				fread(&explode_the_bombs, sizeof(int), 1, fi);
				fread(&drop_bomb, sizeof(int), 1, fi);

				fread(x, sizeof(int), 2, fi);
				fread(width, sizeof(int), 2, fi);
				fread(num_buckets, sizeof(int), 2, fi);
				fread(score, sizeof(int), 2, fi);

				fread(bombs, sizeof(bomb_type), MAX_BOMBS, fi);
				fread(bombs, sizeof(bomb_type), MAX_DROPS, fi);
			}
		
			fclose(fi);
		}

#endif



	/* Setup: */

	setup();
	quit = 0;


	/* Set volumes: */

#ifndef NOSOUND
	if (use_sound)
		{
			Mix_Volume(-1, effects_volume * 16);
			Mix_VolumeMusic(music_volume * 16);
		}
#endif

	do {	/* MAIN INTERFACE LOOP! */

		/* Display title screen: */
		mode = title();
		
		/* Decide what to based on title screen command: */
		if (mode == TITLE_OPTION_ONE_PLAYER ||
		mode == TITLE_OPTION_TWO_PLAYERS ||
		#ifndef EZX
			mode == TITLE_OPTION_TWO_PLAYER_VS)
		#else
			(mode == TITLE_OPTION_CONTINUE && can_continue))
		#endif
		{	/* Stop music: */
	
			#ifndef NOSOUND
				if (use_sound == 1) Mix_HaltMusic();
			#endif
	
			quit = game(mode);
		}
		else if (mode == TITLE_OPTION_OPTIONS)		quit = optionscreen();
		else if (mode == TITLE_OPTION_EXIT)			quit = 1;
		else if (mode == TITLE_OPTION_HIGHSCORE)	quit = highscorescreen();
	} while (quit == 0);


	/* Save options: */

	fi = open_option_file("w");
	if (fi != NULL) 
		{
			/* Comment at the top (I wish _everyone_ did this!) */
		
			fprintf(fi, "# Mad Bomber options file\n\n");
		
		
			/* High score: */
		
			fprintf(fi, "# Highscore:\n\n");
			fprintf(fi, "highscore=%d\n\n", highscore);
			fprintf(fi, "highscorer=%s\n\n\n", highscorer);
		

			/* Background details: */
		
			fprintf(fi,
				"# Set \"details\" to \"zen\" for plain background,\n"
				"# or \"normal\" for photorealistic background.\n\n"
				"# (Default: normal)\n\n");
		
			if (zen == 1)
	fprintf(fi, "details=zen\n\n\n");
			else
	fprintf(fi, "details=normal\n\n\n");
		
		
			/* Difficulty levels: */
		
			fprintf(fi,
				"# Set \"diff#\" to \"hard\" for thin buckets, or\n"
				"# \"normal\" for wide buckets.\n"
				"# Where \"#\" is \"1\" or \"2\", for player one or two.\n\n"
				"# (Default: normal)\n\n");
		
			for (i = 0; i < 2; i++)
	{
		fprintf(fi, "diff%d=", i + 1);
	
		if (width[i] == 1)
			fprintf(fi, "hard\n");
		else
			fprintf(fi, "normal\n");
	}
		
			fprintf(fi, "\n");
		
		
			/* Volume levels: */
		
			fprintf(fi,
				"# Set \"effects\" and \"music\" to a value between 0 and 8.\n"
				"# Where \"0\" is silent and \"8\" is maximum volume (loud).\n"
				"# Where \"effects\" sets sound effects volume, and\n"
				"# where \"music\" sets music volume.\n\n"
				"# (Default: 7 and 5, respectively)\n\n");			
		
			fprintf(fi, "effects=%d\n", effects_volume);
			fprintf(fi, "music=%d\n\n", music_volume);
		
		
			/* The end! */
		
			fprintf(fi, "# (File automatically created.)\n");
		
			fclose(fi);
		}


#ifdef EZX
	/* Save state file: */

	fi = open_state_file("w");

	if (fi != NULL)
		{
			fputc(can_continue, fi);

			if (can_continue)
			{
				fputc(num_players, fi);
				fputc(player, fi);

				fwrite(&bomber_x, sizeof(int), 1, fi);

				fwrite(level, sizeof(int), 2, fi);
				fwrite(past_level_8, sizeof(int), 2, fi);
				fwrite(half_level, sizeof(int), 2, fi);
				fwrite(old_x, sizeof(int), 2, fi);
				fwrite(one_up_score, sizeof(int), 2, fi);
				fwrite(have_highscore, sizeof(int), 2, fi);
				fwrite(&bomber_go, sizeof(int), 1, fi);
				fwrite(&bomber_happy, sizeof(int), 1, fi);
				fwrite(&bombs_left, sizeof(int), 1, fi);
				fwrite(&no_active_bombs, sizeof(int), 1, fi);
				fwrite(&bomber_xm, sizeof(int), 1, fi);
				fwrite(&bomber_xm_time, sizeof(int), 1, fi);
				fwrite(&img, sizeof(int), 1, fi);
				fwrite(&explode_the_bombs, sizeof(int), 1, fi);
				fwrite(&drop_bomb, sizeof(int), 1, fi);

				fwrite(x, sizeof(int), 2, fi);
				fwrite(width, sizeof(int), 2, fi);
				fwrite(num_buckets, sizeof(int), 2, fi);
				fwrite(score, sizeof(int), 2, fi);

				fwrite(bombs, sizeof(bomb_type), MAX_BOMBS, fi);
				fwrite(bombs, sizeof(bomb_type), MAX_DROPS, fi);
			}
		
			fclose(fi);
		}

#endif

	/* Shut down and quit: */

	mb_shutdown();

	return(0);
}


/* Game loop: */

int game(int mode)
{
	SDL_Event event;
	SDLKey key;
	int done, quit, i, fire, old_bomber_x, posn;
	int flying_numbers_scale;
	SDL_Rect dest;
	Uint32 last_time;

 
#ifndef EZX
	if (mode == TITLE_OPTION_TWO_PLAYER_VS) vs_mode = 1; else
#endif
		vs_mode = 0;


#ifdef EZX
	if (mode != TITLE_OPTION_CONTINUE) can_continue = 0;
#endif



	/* Draw background screen: */

	dest.x = 0;
	dest.y = 0;
	dest.w = 640;
	dest.h = 480;
	erase(dest);

 
#ifdef EZX
	if (can_continue == 0)
	{
#endif
		/* Init. game variables: */
		for (i = 0; i < 2; i++) {
			score[i] = 0;
			one_up_score[i] = 1000;
			level[i] = 1;
			half_level[i] = 0;
			past_level_8[i] = 0;
			x[i] = 32;
			old_x[i] = 32;
			have_highscore[i] = 0;
		}

		if (mode == TITLE_OPTION_TWO_PLAYERS)	num_players = 2;
		else									num_players = 1;

		num_buckets[0] = 3;
		if (num_players == 2)	num_buckets[1] = 3;
		else					num_buckets[1] = 0;

		player = 0;

		bomber_x = 640-64;
		old_bomber_x = 640-64;
		bomber_go = 0;
		bomber_happy = 1;
		bombs_left = 0;
		bomber_xm = 0;
		bomber_xm_time = 0;
		explode_the_bombs = 0;

		for (i = 0; i < MAX_BOMBS; i++) bombs[i].alive = 0;

		for (i = 0; i < MAX_DROPS; i++) drops[i].alive = 0;

#ifdef EZX
	}
#endif

	drawscore(score[player], player);
	my_updaterect(screen, 0, 0, 640, 480);


	/* Main game loop! */

	frame = 0;
	key = SDLK_LAST;
	left_down = 0;
	right_down = 0;
	bomber_left_left_down = 0;
	bomber_left_down = 0;
	bomber_right_down = 0;
	bomber_right_right_down = 0;
	drop_bomb = 0;
	done = 0;
	quit = 0;
	game_over = 0;
	flying_numbers_scale = 1;


#ifdef EZX
	can_continue = 1;
#endif


	do
		{
			num_rects = 0;
			last_time = SDL_GetTicks();
			frame++;
		
		
			/* Get events: */
		
			fire = 0;
		
			while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_KEYDOWN)
			{
				/* A keypress! */
			
				key = event.key.keysym.sym;
			
					 if (key == SDLK_ESCAPE)						done = 1;
				else if (key == SDLK_LEFT)							left_down = 1;
				else if (key == SDLK_RIGHT)							right_down = 1;
				else if (key == SDLK_1)								bomber_left_left_down = 1;
				else if (key == SDLK_2)								bomber_left_down = 1;
				else if (key == SDLK_3)								bomber_right_down = 1;
				else if (key == SDLK_4)								bomber_right_right_down = 1;
				else if (key == SDLK_SPACE || key == SDLK_RETURN)	fire = 1;
				else if (key == SDLK_TAB || key == SDLK_p || key == SDLK_PAUSE)
		{	/* Tab or P pauses: */
		
			if (game_over == 0)
				{
					done = pausescreen();
				
					if (done == 2) quit = 1;
				
					left_down = 0;
					right_down = 0;
				}
		}
			}
		else if (event.type == SDL_KEYUP)
			{ /* A key-release (for movement keys): */
				key = event.key.keysym.sym;
			 
					 if (key == SDLK_LEFT)		left_down = 0;
				else if (key == SDLK_RIGHT)		right_down = 0;
				else if (key == SDLK_1)			bomber_left_left_down = 0;
				else if (key == SDLK_2)			bomber_left_down = 0;
				else if (key == SDLK_3)			bomber_right_down = 0;
				else if (key == SDLK_4)			bomber_right_right_down = 0;
			}
		else if (event.type == SDL_MOUSEMOTION)
			{
				if(game_over) done = 1;
			
				/* Mouse movement - move the bucket: */
				x[player] = event.motion.x * MULTIPLIER - (16 * width[player]);
				if (x[player] < 32)								 x[player] = 32;
				else if (x[player] > 608 - (width[player] * 32)) x[player] = 608 - (width[player] * 32);
			}
		else if (event.type == SDL_MOUSEBUTTONDOWN)		fire = 1;
		else if (event.type == SDL_QUIT)				quit = 1;
	}

			/* Fire button activates the bomber: */		
			if (fire == 1)
	{
		if (num_buckets[0] != 0 || num_buckets[1] != 0)
			{			
				if (bomber_go == 0 && no_active_bombs == 1)
		{
			/* Decide how many we're to drop this turn: */
		
			bombs_left = bombs_per_level[level[player] - 1];
			drop_bomb = 0;
		
		
			/* (On a half-level?) */
		
			if (half_level[player])
				bombs_left = bombs_left / 2;
		
			half_level[player] = 0;
		
		
			/* Turn on the mad bomber! */
		
			bomber_go = 1;
			frame = 0;
			bomber_happy = 0;

		
			/* Erase the "Press Fire" message: */
		
			dest.x = (640 - (images[IMG_PRESS_FIRE] -> w * MULTIPLIER)) / 2;
			dest.y = 480 - (images[IMG_PRESS_FIRE] -> h * MULTIPLIER);
			dest.w = images[IMG_PRESS_FIRE] -> w * MULTIPLIER;
			dest.h = images[IMG_PRESS_FIRE] -> h * MULTIPLIER;
		
			erase(dest);
			addrect(dest);
		}
			}
	}
		
		
			/* Move bucket(s): */
		
			if (left_down == 1 && right_down == 0)
	{
		x[player] = x[player] - 24;
		if (x[player] < 32)
			x[player] = 32;
	}
			else if (right_down == 1 && left_down == 0)
	{
		x[player] = x[player] + 24;
		if (x[player] > 608 - (width[player] * 32))
			x[player] = 608 - (width[player] * 32);
	}
		
		
			/* Move the bomber: */
		 
#ifndef EZX
			if (mode != TITLE_OPTION_TWO_PLAYER_VS)
#endif
	{
		if (bomber_go == 1 && bombs_left > 0)
			{
				bomber_x = bomber_x + bomber_xm;
			
				if (bomber_x < 32)
		{
			bomber_x = 32;
			bomber_xm_time = 0;
		}
				else if (bomber_x > 576)
		{
			bomber_x = 576;
			bomber_xm_time = 0;
		}
			
				bomber_xm_time--;
				if (bomber_xm_time <= 0)
		{
			do
				{
					bomber_xm = ((rand() % 8) - 4) * (level[player] * 2);
				}
			while (bomber_xm == 0);
		
			bomber_xm_time = (rand() % (level[player] * 10)) + 5;
		}
			}
	}
#ifndef EZX
			else
	{
		if (bomber_left_left_down == 1)
			bomber_x = bomber_x - 32;
		else if (bomber_left_down == 1)
			bomber_x = bomber_x - 16;
	
		if (bomber_x < 32)
			bomber_x = 32;
	
		if (bomber_right_right_down == 1)
			bomber_x = bomber_x + 32;
		else if (bomber_right_down == 1)
			bomber_x = bomber_x + 16;
	
		if (bomber_x > 576)
			bomber_x = 576;
	}
#endif
		
		
			/* Drop bombs: */
		
			if (bomber_go == 1 && bombs_left > 0)
	{
		if ((frame % (10 - level[player])) == 0)
			{
				if ((level[player] % 2) == 1)
		drop_bomb = 1 - drop_bomb;
				else
		drop_bomb = 1;
			
				if (drop_bomb == 1)
		{
			addbomb(bomber_x);
			bombs_left--;
		}
			}
	}
		
		
			/* Go to next level? */
		
			no_active_bombs = 1;
			for (i = 0; i < MAX_BOMBS; i++)
	{
		if (bombs[i].alive)
			no_active_bombs = 0;
	}
		
			if (bomber_go == 1 && bombs_left == 0 && no_active_bombs == 1)
	{
		bomber_go = 0;
		level[player]++;
	
		if (level[player] > 8)
			{
				level[player] = 8;
				if (past_level_8[player] == 0)
		{
			past_level_8[player] = 1;
			playsound(SND_LEVEL8, 3);
		}
			}
		else
			{
				flying_numbers_scale = 1;
			}
	}
		
		
			/* Erase bombs: */
		
			for (i = 0; i < MAX_BOMBS; i++)
	{
		if (bombs[i].alive == 1)
			{
				dest.x = bombs[i].x;
				dest.y = bombs[i].y;
				dest.w = 32;
				dest.h = 32;
			
				erase(dest);
				addrect(dest);
			}
	}


			/* Erase drops: */
		
			for (i = 0; i < MAX_DROPS; i++)
	{
		if (drops[i].alive == 1)
			{
				dest.x = drops[i].x;
				dest.y = drops[i].y;
				dest.w = 8;
				dest.h = 8;
			
				erase(dest);
				addrect(dest);
			}
	}
		
		
			/* Erase bucket(s): */
		
			if (x != old_x)
	{
		dest.x = old_x[player];
		dest.y = 352;
		dest.w = 64;
		dest.h = 96;
	
		erase(dest);
		addrect(dest);
	
		old_x[player] = x[player];
	}
		
		
			/* Erase the mad bomber: */
		
			if (bomber_x != old_bomber_x)
	{
		dest.x = old_bomber_x;
		dest.y = 67;
		dest.w = 32;
		dest.h = 64;
	
		erase(dest);
		addrect(dest);
	
		old_bomber_x = bomber_x;
	}
		

			/* Erase and resize flying numbers: */
		
			if (flying_numbers_scale != 0 && game_over == 0)
	{
		erase_flying_numbers(flying_numbers_scale);
	
		if (half_level[player] == 0)
			flying_numbers_scale++;
		else
			flying_numbers_scale--;
	
		if (flying_numbers_scale > 20)
			flying_numbers_scale = 0;
	}
		
		
			/* Move bombs: */
		
			for (i = 0; i < MAX_BOMBS; i++)
	{
		if (bombs[i].alive)
			{
				/* Move bomb downwards: */
			
				bombs[i].y = bombs[i].y + (level[player] + 1) * 2;
			
			
				/* Blow bombs up when they hit bottom! */
			
				if (bombs[i].y >= 416)
		{
			explode_the_bombs = 1;
		}
			
			
				/* Catch bombs: */
			
				if (bombs[i].y >= 336 &&
			bombs[i].y <= 336 + (num_buckets[player] * 32) &&
			bombs[i].x >= x[player] - 32 &&
			bombs[i].x <= x[player] + (32 * width[player]))
		{
			/* Get rid of the bomb: */
		
			bombs[i].alive = 0;
		
		
			/* Make a splash: */
		
			addsplash(bombs[i].x + 16,
					(((bombs[i].y - 336) / 32) * 32) + 352);
			playsound(SND_SPLASH1 + level[player] - 1, 1);
	 
#ifndef NOSOUND
			posn = (bombs[i].x * 255) / screen->w;
						Mix_SetPanning(1, 255 - posn, posn);
#endif
		
		
			/* Add some score and update the score display: */
		
			score[player] = score[player] + level[player];
			drawscore(score[player], player);
		
		
			/* Did they get a high score? */
		
			if (score[player] >= highscore
#ifndef EZX
					&& mode != TITLE_OPTION_TWO_PLAYER_VS
#endif
					)
				{
					highscore = score[player];
				
				
					/* Did they just get the high score? */
				
					if (have_highscore[player] == 0)
			{
				have_highscore[player] = 1;
				playsound(SND_HIGHSCORE, 3);
			}
				}
			else
				{
					have_highscore[player] = 0;
				}
		
		
			/* Add a one-up if we hit the 1000 mark! */
		
			if (score[player] >= one_up_score[player])
				{
					/* Add another bucket! */
				
					if (num_buckets[player] < 3)
			num_buckets[player]++;
				
				
					/* Play a sound: */
				
					playsound(SND_ONEUP, 3);
				
				
					/* Set the next higher one-up threshold: */
				
					one_up_score[player] = one_up_score[player] + 1000;
				}
		}
			}
	}
		
		
			/* Move drops: */
		
			for (i = 0; i < MAX_DROPS; i++)
	{
		if (drops[i].alive)
			{
				/* Move drops: */
			
				drops[i].x = drops[i].x + drops[i].xm;
				drops[i].y = drops[i].y + drops[i].ym;
			
			
				/* Be influenced by gravity: */
			
				drops[i].ym++;
			
			
				/* Count-down: */
			
				drops[i].timer--;
			
			
				/* Kill drop */
			
				if (drops[i].y >= 440 || drops[i].timer <= 0 ||
			drops[i].x < 32 || drops[i].x >= 600)
		drops[i].alive = 0;
			}
	}
		
		
			/* Draw bucket(s): */
		
			for (i = 0; i < num_buckets[player]; i++)
	{
		dest.x = x[player];
		dest.y = 352 + (i * 32);
		dest.w = 32 * width[player];
		dest.h = 32;

		my_blit(images[IMG_BUCKET1 + (((frame / 3) + i) % 3) + (player * 3) + 12 - (width[player] * 6)], NULL, screen, &dest);
		addrect(dest);
	}
		
		
			/* Draw the mad bomber: */
		
			dest.x = bomber_x;
			dest.y = 67;
			dest.w = 32;
			dest.h = 64;
		
			if (score[player] < 10000 || vs_mode == 1) my_blit(images[IMG_BOMBER_SAD + bomber_happy], NULL, screen, &dest);
			else										 my_blit(images[IMG_BOMBER_AMAZED],				NULL, screen, &dest);
			addrect(dest);
		
		
			/* Draw a bomb in the bomber's hand: */
		
			if (bomber_happy == 0 && bomber_go == 0)
	{
		dest.x = bomber_x;
		dest.y = 99;
		dest.w = 32;
		dest.h = 32;
	
		my_blit(images[IMG_BOMB0], NULL, screen, &dest);
	}
		
		
			/* Draw bombs: */
		
			for (i = 0; i < MAX_BOMBS; i++)
	{
		if (bombs[i].alive == 1)
			{
				dest.x = bombs[i].x;
				dest.y = bombs[i].y;
				dest.w = 32;
				dest.h = 32;
			
				if (((frame + i) % 2) == 0)
		my_blit(images[IMG_BOMB0], NULL, screen, &dest);
				else
		my_blit(images[IMG_BOMB1 + (rand() % 3)],
				NULL, screen, &dest);
			
				addrect(dest);
			}
	}
		
		
			/* Draw drops: */
		
			for (i = 0; i < MAX_DROPS; i++)
	{
		if (drops[i].alive == 1)
			{
				dest.x = drops[i].x;
				dest.y = drops[i].y;
				dest.w = 8;
				dest.h = 8;
			
				if (drops[i].xm > 1)
		{
			if (drops[i].ym >= 0)
				img = IMG_DROP_RIGHT_DOWN;
			else
				img = IMG_DROP_RIGHT_UP;
		}
				else if (drops[i].xm < -1)
		{
			if (drops[i].ym >= 0)
				img = IMG_DROP_LEFT_DOWN;
			else
				img = IMG_DROP_LEFT_UP;
		}
				else
		{
			if (drops[i].ym < 0)
				img = IMG_DROP_UP;
			else
				img = IMG_DROP_DOWN;
		}
			
				my_blit(images[img], NULL, screen, &dest);
				addrect(dest);
			}
	}
		
		
			/* If everyone's died, make the bomber happy and show score(s): */
		
			if (num_buckets[0] == 0 && num_buckets[1] == 0)
	{
		/* Display "GAME OVER" text: */
	
		if (game_over == 0)
			{
				game_over = 1;
				bomber_happy = 1;

				dest.x = (640 - (images[IMG_GAME_OVER] -> w * MULTIPLIER)) / 2;
				dest.y = (480 - (images[IMG_GAME_OVER] -> h * MULTIPLIER)) / 2;
				dest.w = images[IMG_GAME_OVER] -> w * MULTIPLIER;
				dest.h = images[IMG_GAME_OVER] -> h * MULTIPLIER;
			
				my_blit(images[IMG_GAME_OVER], NULL, screen, &dest);
				addrect(dest);

#ifdef EZX
				can_continue = 0;
#endif
			}
	
	
		/* Toggle between player one's and two's scores: */
	
		if (num_players == 2 && (frame % 40) == 0)
			{
				player = 1 - player;
				drawscore(score[player], player);
			}
	
	
		/* Flash "Press ESCAPE" message: */
	
		if ((frame % 30) == 0)
			{
				dest.x = (640 - (images[IMG_PRESS_ESCAPE] -> w * MULTIPLIER)) / 2;
				dest.y = 480 - (images[IMG_PRESS_ESCAPE] -> h * MULTIPLIER);
				dest.w = images[IMG_PRESS_ESCAPE] -> w * MULTIPLIER;
				dest.h = images[IMG_PRESS_ESCAPE] -> h * MULTIPLIER;
			
				my_blit(images[IMG_PRESS_ESCAPE], NULL, screen, &dest);
				addrect(dest);
			}
		else if ((frame % 15) == 0)
			{
				dest.x = (640 - (images[IMG_PRESS_ESCAPE] -> w * MULTIPLIER)) / 2;
				dest.y = 480 - (images[IMG_PRESS_ESCAPE] -> h * MULTIPLIER);
				dest.w = images[IMG_PRESS_ESCAPE] -> w * MULTIPLIER;
				dest.h = images[IMG_PRESS_ESCAPE] -> h * MULTIPLIER;
			
				erase(dest);
				addrect(dest);
			}
	}
		

			/* Draw flying numbers: */
		
			if (flying_numbers_scale != 0 && game_over == 0)
	{
		draw_flying_numbers(level[player] - 1, flying_numbers_scale,
						(rand() % 128) + 128,	
						(rand() % 128) + 128,
						(rand() % 128) + 128);
	}
		
		
			/* Flash "Press Fire" message: */
		
			if (bomber_go == 0 && (num_buckets[0] != 0 || num_buckets[1] != 0) &&
		no_active_bombs == 1)
	{
		if ((frame % 30) == 0)
			{
				dest.x = (640 - (images[IMG_PRESS_FIRE] -> w * MULTIPLIER)) / 2;
				dest.y = 480 - (images[IMG_PRESS_FIRE] -> h * MULTIPLIER);
				dest.w = images[IMG_PRESS_FIRE] -> w * MULTIPLIER;
				dest.h = images[IMG_PRESS_FIRE] -> h * MULTIPLIER;
			
				my_blit(images[IMG_PRESS_FIRE], NULL, screen, &dest);
				addrect(dest);
			}
		else if ((frame % 15) == 0)
			{
				dest.x = (640 - (images[IMG_PRESS_FIRE] -> w * MULTIPLIER)) / 2;
				dest.y = 480 - (images[IMG_PRESS_FIRE] -> h * MULTIPLIER);
				dest.w = images[IMG_PRESS_FIRE] -> w * MULTIPLIER;
				dest.h = images[IMG_PRESS_FIRE] -> h * MULTIPLIER;
			
				erase(dest);
				addrect(dest);
			}
	}
		
		
			/* Did the bombs hit the ground? Deal with it! */
		
			if (explode_the_bombs == 1)
	{
		/* Update the screen: */
	
		SDL_UpdateRects(screen, num_rects, rects);
		num_rects = 0;
	
		explode_the_bombs = 0;
	
	
		/* Draw happy bomber: */
	
		dest.x = bomber_x;
		dest.y = 67;
		dest.w = 32;
		dest.h = 64;
	
		if (score[player] < 10000 || vs_mode == 1)
			my_blit(images[IMG_BOMBER_HAPPY], NULL, screen, &dest);
		else
			my_blit(images[IMG_BOMBER_AMAZED], NULL, screen, &dest);
		my_updaterect(screen, dest.x, dest.y, dest.w, dest.h);
	
	
		/* Explode the bombs! */
	
		explodebombs();
	
	
		/* Turn off all bombs: */
	
		for (i = 0; i < MAX_BOMBS; i++)
			bombs[i].alive = 0;
	
	
		/* Turn off the mad bomber: */
	
		bomber_go = 0;
	
	
		/* Jump back a level: */
	
		if (level[player] > 1)
			{
				level[player]--;
				half_level[player] = 1;
				flying_numbers_scale = 20;
			}
	
	
		/* Remove a bucket: */
	
		num_buckets[player]--;
	
	
		/* Switch to the other player: */
	
		if (num_players == 2)
			{
				/* If the other player is still alive... */
			
				if (num_buckets[1 - player])
		{
			/* Erase the last player's bucket bucket: */
		
			dest.x = x[player];
			dest.y = 352;
			dest.w = 64;
			dest.h = 96;
		
			erase(dest);
			addrect(dest);
		
		
			/* Switch to the new player: */
		
			player = 1 - player;
		
		
			/* Draw the new player's score: */
		
			drawscore(score[player], player);
		}
			}
	}
		
		
			/* Update the screen: */
		
			SDL_UpdateRects(screen, num_rects, rects);
		
		
#ifndef NOSOUND
			/* Play hissing noise: */
		
			if (use_sound == 1)
	{
		if (no_active_bombs == 0)
			{
				if ((frame % 5) == 0 || Mix_Playing(2) == 0)
		playsound(SND_FUSE, 2);
			}
		else
			{
	#ifndef SDL_MIXER_BUG
				if (Mix_Playing(2) != 0) Mix_HaltChannel(2);
	#endif
			}
	}
		

			/* Keep playing music: */

			if (use_sound == 1)
	{
		if (!Mix_PlayingMusic())
			{
				Mix_PlayMusic(game_musics[rand() % NUM_GAME_MUSICS], 0);
				Mix_VolumeMusic(music_volume * 16);
			}
	}
#endif
		
			/* Pause: */
		
			if (SDL_GetTicks() < last_time + 33)
	SDL_Delay(last_time + 45 - SDL_GetTicks());
		}
	while (done == 0 && quit == 0);


#ifndef NOSOUND
	/* Stop sound effects: */

	if (use_sound == 1)
		Mix_HaltChannel(-1);

	/* Stop music: */

	if (use_sound == 1)
		Mix_HaltMusic();
#endif


	/* Did anyone get the high score? */

	if (have_highscore[0])
		quit = sign_highscore(0);
	else if (num_players == 2 && have_highscore[1])
		quit = sign_highscore(1);


#ifndef NOSOUND
	/* Stop sound effects: */

	if (use_sound == 1)
		Mix_HaltChannel(-1);

	/* Stop music: */

	if (use_sound == 1)
		Mix_HaltMusic();
#endif

	return(quit);
}


/* Title loop: */

int title(void)
{
	SDL_Rect dest;
	SDL_Event event;
	SDLKey key;
	int i, y, option, old_option;
	int bx, by;
	int title_option_y[NUM_TITLE_OPTIONS];


	/* Draw title screen: */

	dest.x = 0;
	dest.y = 0;
	dest.w = 640;
	dest.h = images[IMG_TITLE_TITLE] -> h * MULTIPLIER;

	my_blit(images[IMG_TITLE_TITLE], NULL, screen, &dest);

	dest.x = 0;
	dest.y = images[IMG_TITLE_TITLE] -> h * MULTIPLIER;
	dest.w = 640;
	dest.h = (480 - (images[IMG_TITLE_TITLE] -> h * MULTIPLIER));

	my_fillrect(screen, &dest, SDL_MapRGB(screen->format, 0x00, 0x00, 0x00));


	/* Draw controls: */

	y = option_box.y + 2;

	for (i = 0; i < NUM_TITLE_OPTIONS; i++)
		{
			dest.x = (640 - (images[title_option_images[i]] -> w * MULTIPLIER)) / 2;
			dest.y = y;
			dest.w = images[title_option_images[i]] -> w * MULTIPLIER;
			dest.h = images[title_option_images[i]] -> h * MULTIPLIER;

#ifdef EZX
			if (can_continue || i != TITLE_OPTION_CONTINUE)
#endif 
				my_blit(images[title_option_images[i] + 1], NULL, screen, &dest);
		
			title_option_y[i] = y;
		 
			y = y + (images[title_option_images[i]] -> h * MULTIPLIER);
		}


	my_updaterect(screen, 0, 0, 640, 480);


	/* Title screen loop: */

	option = 0;
	old_option = -1;
	key = SDLK_LAST;

	do
		{
			while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_KEYDOWN)
			{
				key = event.key.keysym.sym;
			
				if (key == SDLK_UP && option > 0)
		option--;
				else if (key == SDLK_DOWN && option < NUM_TITLE_OPTIONS - 1)
		option++;
				else if (key == SDLK_ESCAPE)
		option = TITLE_OPTION_EXIT;

#ifdef EZX
				if (option == TITLE_OPTION_CONTINUE && !can_continue)
				{
		if (key == SDLK_UP)
			option--;
		else if (key == SDLK_DOWN)
						option++;
				}
#endif
			}
		else if (event.type == SDL_MOUSEBUTTONDOWN)
			{
				bx = event.button.x * MULTIPLIER;
				by = event.button.y * MULTIPLIER;

				if (bx >= option_box.x && bx <= option_box.x + option_box.w)
				{
					for (i = 0; i < NUM_TITLE_OPTIONS; i++) {
						if (by >= title_option_y[i] && (by <= (title_option_y[i] + images[title_option_images[i]] -> h * MULTIPLIER))) {
							#ifdef EZX
							if (i != TITLE_OPTION_CONTINUE || can_continue) {
							#endif
								option = i;
								key = SDLK_RETURN;
							#ifdef EZX
							}
							#endif
						}
					}
				}
			}
		else if (event.type == SDL_QUIT)
			{
				option = TITLE_OPTION_EXIT;
				key = SDLK_ESCAPE;
			}
	}


			/* Update the control box: */
		
			if (old_option != option)
	{
		if (old_option != -1)
			{
				/* Unhighlight the old option: */
			
				dest.x = ((640 - (images[title_option_images[old_option]] -> w) * MULTIPLIER) / 2);
				dest.y = title_option_y[old_option];
				dest.w = images[title_option_images[old_option]] -> w * MULTIPLIER;
				dest.h = images[title_option_images[old_option]] -> h * MULTIPLIER;
			
				my_blit(images[title_option_images[old_option] + 1], NULL, screen, &dest);
			}
	
	
		/* Highlight the new option: */
	
		dest.x = (640 - (images[title_option_images[option]] -> w) * MULTIPLIER) / 2;
		dest.y = title_option_y[option];
		dest.w = images[title_option_images[option]] -> w * MULTIPLIER;
		dest.h = images[title_option_images[option]] -> h * MULTIPLIER;


	
		my_blit(images[title_option_images[option]], NULL, screen, &dest);
	
	
		/* Update the screen: */
	
		my_updaterect(screen, option_box.x, option_box.y,
			 option_box.w, option_box.h);

	
		if (old_option != -1)
			{
				/* Play selection sound: */
			
				playsound(SND_SELECT, -1);
			}
	

		/* Keep track of old option: */
	
		old_option = option;
	}
		
		
			/* Keep playing music: */
		
#ifndef NOSOUND
			if (use_sound == 1)
	{
		if (!Mix_PlayingMusic())
			{
				Mix_PlayMusic(title_music, 0);
				Mix_VolumeMusic(music_volume * 16);
			}
	}
#endif
		
		
			/* Pause: */
		
			SDL_Delay(50);
		}
	while (key != SDLK_RETURN && key != SDLK_SPACE && key != SDLK_ESCAPE);


	/* Confirm sound! */

	playsound(SND_CONFIRM, -1);


	return(option);
}


/* Initialize SDL, load graphics and sound: */

void setup(void)
{
	int i;
	SDL_Rect dest;
	SDL_Surface * image;


	/* Init SDL Video: */
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		fprintf(stderr,"\nError: Could not initialize video!\nThe SDL error that occured: %s\n\n", SDL_GetError());
		exit(1);
	}


	/* Init SDL Audio: */
	if (use_sound == 1)
	{
		if (SDL_Init(SDL_INIT_AUDIO) < 0)
		{
			fprintf(stderr,"\nWarning: Could not initialize audio!\nThe SDL error that occured: %s\n\n", SDL_GetError());
			use_sound = 0;
		}
	}


	/* Open sound: */

	#ifndef NOSOUND
	if (use_sound == 1)
	{
		if (Mix_OpenAudio(22050, AUDIO_S16, 2, 256) < 0)
		{
				fprintf(stderr, "\nWarning: Could not set up audio for 22050 Hz 16-bit stereo.\nThe SDL error that occured: %s\n\n", SDL_GetError());
				use_sound = 0;
		}
	}
	#endif


	/* Open display: */

	if (use_fullscreen == 1)
		{
			screen = SDL_SetVideoMode(640/MULTIPLIER, 480/MULTIPLIER, 16, SDL_FULLSCREEN);
			if (screen == NULL)
	{
		fprintf(stderr, "\nWarning: Could not set up fullscreen video for %dx%d mode.\nThe SDL error that occured: %s\n\n", 640/MULTIPLIER, 480/MULTIPLIER, SDL_GetError());
		use_fullscreen = 0;
	}
		}

	if (use_fullscreen == 0)
		{
			screen = SDL_SetVideoMode(640/MULTIPLIER, 480/MULTIPLIER, 16, 0);
		
			if (screen == NULL)
	{
		fprintf(stderr, "\nError: Could not set up video for %dx%d mode.\nThe SDL error that occured: %s\n\n", 640/MULTIPLIER, 480/MULTIPLIER, SDL_GetError());
		exit(1);
	}
		}

	/* Set window manager stuff: */

	SDL_WM_SetCaption("Mad Bomber", "Mad Bomber");
	/* SDL_ShowCursor(0); */


#ifdef EZX
	SDL_Surface* bkg = NULL;
	SDL_Surface* new_bkg = NULL;
	SDL_Rect d;
	bkg= IMG_Load("data/images/title/title.png");
	if (image == NULL) {
		fprintf(stderr,	"\nError: Couldn't load a graphics file: %s\nThe SDL error that occured: %s\n\n", image_names[i], SDL_GetError()); exit(1);
	}
	new_bkg = SDL_DisplayFormat(bkg); if(bkg) SDL_FreeSurface(bkg);
	if (bkg == NULL) {
		fprintf(stderr,"\nError: Couldn't convert a file to the display format: %s\nThe SDL error that occured: %s\n\n", image_names[i], SDL_GetError()); exit(1);
	}
	d.x = 0;	d.y = 0;	d.w = 640;	d.h = 480;
	my_fillrect(screen, &d, SDL_MapRGB(screen->format, 0x00, 0x00, 0x00));
	d.x = 0;	d.y = 0;	d.w = 640;	d.h = bkg -> h * MULTIPLIER;
	my_blit(new_bkg, NULL, screen, &d);
	SDL_Flip(screen);
#endif
			
	/* Load graphics: */

	for (i = 0; i < NUM_IMAGES; i++)
		{
			/* Load image file: */
			image = IMG_Load(image_names[i]);
		
			if (image == NULL)
	{
		fprintf(stderr,	"\nError: Couldn't load a graphics file: %s\nThe SDL error that occured: %s\n\n", image_names[i], SDL_GetError());
					exit(1);
	}
			images[i] = SDL_DisplayFormat(image);
			if (images[i] == NULL)
	{
		fprintf(stderr,"\nError: Couldn't convert a file to the display format: %s\nThe SDL error that occured: %s\n\n", image_names[i], SDL_GetError());
		exit(1);
	}
		
			if (SDL_SetColorKey(images[i], (SDL_SRCCOLORKEY | SDL_RLEACCEL),
				SDL_MapRGB(images[i] -> format,
						 0xFF, 0xFF, 0xFF)) == -1)
	{
		fprintf(stderr,"\nError: Could not set the color key for the file: %s\nThe SDL error that occured: %s\n\n", image_names[i], SDL_GetError());
		exit(1);
	}
		 
			SDL_FreeSurface(image);
		
		
			/* Show "loading" and percentage bar: */
		
			if (i == IMG_LOADING)
	{
		dest.x = (640 - (images[IMG_LOADING] -> w * MULTIPLIER)) / 2;
		dest.y = (480 - (images[IMG_LOADING] -> h * MULTIPLIER)) / 2;
		dest.w = images[IMG_LOADING] -> w * MULTIPLIER;
		dest.h = images[IMG_LOADING] -> h * MULTIPLIER;
	
		my_blit(images[IMG_LOADING], NULL, screen, &dest);
		my_updaterect(screen, dest.x, dest.y, dest.w, dest.h);
	}
			else if (i >= IMG_BAR)
	{
		dest.x = (((640 - (images[IMG_BAR] -> w * MULTIPLIER)) * i) / (NUM_IMAGES + NUM_SOUNDS));
		dest.y = (((480 - (images[IMG_LOADING] -> h * MULTIPLIER)) / 2) + (images[IMG_LOADING] -> h * MULTIPLIER));
		dest.w = images[IMG_BAR] -> w * MULTIPLIER;
		dest.h = images[IMG_BAR] -> h * MULTIPLIER;
	
		my_blit(images[IMG_BAR], NULL, screen, &dest);
		my_updaterect(screen, dest.x, dest.y, dest.w, dest.h);
	}
		}
#ifdef EZX
	#ifndef WITHOUT_SK
		SK::Init();
	#endif
#endif


	/* Create zen background: */

	image = SDL_CreateRGBSurface(SDL_SWSURFACE, 640, 480, 16, 0, 0, 0, 0);

	dest.x = 0;
	dest.y = 0;
	dest.w = 640;
	dest.h = 480;
	my_fillrect(image, &dest, SDL_MapRGB(image->format, 0x00, 0x00, 0x00));

	dest.x = 31;
	dest.y = 39;
	dest.w = 577;
	dest.h = 88;
	my_fillrect(image, &dest, SDL_MapRGB(image->format, 165, 165, 165));

	dest.x = 31;
	dest.y = 127;
	dest.w = 577;
	dest.h = 313;
	my_fillrect(image, &dest, SDL_MapRGB(image->format, 74, 115, 24));

	my_updaterect(image, 0, 0, 640, 480);

	zen_background = SDL_DisplayFormat(image);


	/* Determine widest title screen option image: */

	option_box.w = (widest(images[IMG_TITLE_ONE_PLAYER] -> w * MULTIPLIER,
		 widest(images[IMG_TITLE_TWO_PLAYERS] -> w * MULTIPLIER,
#ifdef EZX
		 widest(images[IMG_TITLE_CONTINUE] -> w * MULTIPLIER,
#else
		 widest(images[IMG_TITLE_TWO_PLAYER_VS] -> w * MULTIPLIER,
#endif
				 widest(images[IMG_TITLE_OPTIONS] -> w * MULTIPLIER,
				widest(images[IMG_TITLE_HIGHSCORE]->w * MULTIPLIER,
			 images[IMG_TITLE_EXIT]->w * MULTIPLIER))))) + 4);

	option_box.h = ((images[IMG_TITLE_ONE_PLAYER] -> h * MULTIPLIER) +
			(images[IMG_TITLE_TWO_PLAYERS] -> h * MULTIPLIER) +
#ifdef EZX
			(images[IMG_TITLE_CONTINUE] -> h * MULTIPLIER) +
#else
			(images[IMG_TITLE_TWO_PLAYER_VS] -> h * MULTIPLIER) +
#endif
			(images[IMG_TITLE_OPTIONS] -> h * MULTIPLIER) +
			(images[IMG_TITLE_HIGHSCORE] -> h * MULTIPLIER) +
			(images[IMG_TITLE_EXIT] -> h * MULTIPLIER));


	option_box.x = (640 - option_box.w) / 2;
	option_box.y = (640 - option_box.h) / 2;


#ifndef NOSOUND
	/* Load sounds: */

	if (use_sound == 1)
		{
			for (i = 0; i < NUM_SOUNDS; i++)
	{
		sounds[i] = Mix_LoadWAV(sound_names[i]);
		if (sounds[i] == NULL)
			{
				fprintf(stderr,"\nError: Could not load the sound file: %s\nThe SDL error that occured: %s\n\n", sound_names[i], SDL_GetError());
				exit(1);
			}
	
	
		/* Draw percentage bar: */
	
		dest.x = (((640 - (images[IMG_BAR] -> w * MULTIPLIER)) * (i + NUM_IMAGES)) / (NUM_IMAGES + NUM_SOUNDS));
		dest.y = (((480 - (images[IMG_LOADING] -> h * MULTIPLIER)) / 2) + (images[IMG_LOADING] -> h * MULTIPLIER));
		dest.w = images[IMG_BAR] -> w * MULTIPLIER;
		dest.h = images[IMG_BAR] -> h * MULTIPLIER;
	
		my_blit(images[IMG_BAR], NULL, screen, &dest);
		my_updaterect(screen, dest.x, dest.y, dest.w, dest.h);
	}
		}


	/* Load music: */

	if (use_sound == 1)
		{
			title_music = Mix_LoadMUS(MUS_TITLE);
			if (title_music == NULL)
	{
		fprintf(stderr,"\nError: Could not load the music file: %s\nThe SDL error that occured: %s\n\n", MUS_TITLE, SDL_GetError());
		exit(1);
	}
		
		 
//#ifndef EZX
			highscore_music = Mix_LoadMUS(MUS_HIGHSCORE);
			if (highscore_music == NULL)
	{
		fprintf(stderr,"\nError: Could not load the music file: %s\nThe SDL error that occured: %s\n\n", MUS_TITLE, SDL_GetError());
		exit(1);
	}
//#endif
		
		
			for (i = 0; i < NUM_GAME_MUSICS; i++)
	{
		game_musics[i] = Mix_LoadMUS(game_music_names[i]);
		if (game_musics[i] == NULL)
			{
				fprintf(stderr,"\nError: Could not load the music file: %s\nThe SDL error that occured: %s\n\n", game_music_names[i], SDL_GetError());
				exit(1);
			}
	}
		}
#endif

#ifdef EZX
	if(new_bkg) SDL_FreeSurface(new_bkg);
#endif

	/* Seed random generator: */

	srand(SDL_GetTicks());


	spray_count = 0;
}


/* Clean up and prepare to quit: */

void mb_shutdown(void)
{
	int i;

	/* Not really necessary, but.. I'm nice: */

	for (i = 0; i < NUM_IMAGES; i++)
		{
			SDL_FreeSurface(images[i]);
		}


	/* Close sound: */

#ifndef NOSOUND
	if (use_sound == 1)
		Mix_CloseAudio();
#endif
#ifdef EZX
	#ifndef WITHOUT_SK
		SK::Quit();
	#endif
#endif
	SDL_Quit();
}


/* Of two widths, which is widest? */

int widest(int w1, int w2)
{
	if (w1 > w2)
		return(w1);
	else
		return(w2);
}


/* Add another rectangle: */

void addrect(SDL_Rect rect)
{
	rects[num_rects].x = rect.x / MULTIPLIER;
	rects[num_rects].y = rect.y / MULTIPLIER;
	rects[num_rects].w = rect.w / MULTIPLIER;
	rects[num_rects].h = rect.h / MULTIPLIER;

	num_rects++;
}


/* Add a bomb: */

void addbomb(int x)
{
	int i, found;


	/* Find a free slot: */

	found = -1;
	for (i = 0; i < MAX_BOMBS && found == -1; i++)
		{
			if (bombs[i].alive == 0)
	found = i;
		}


	/* Add a bomb: */

	if (found != -1)
		{
			bombs[found].alive = 1;
			bombs[found].x = x;
			bombs[found].y = 96;
		}
}


/* Make a splash (add a bunch of drops): */

void addsplash(int x, int y)
{
	int i, offset;

	for (i = 0; i < 8; i++)
		{
			offset = (rand() % 32) - 16;
		
			adddrop(x + offset, y, offset / 6);
		}
}


/* Add one drop: */

void adddrop(int x, int y, int xm)
{
	int i, found;


	/* Find a free slot: */

	found = -1;
	for (i = 0; i < MAX_DROPS && found == -1; i++)
		{
			if (drops[i].alive == 0)
	found = i;
		}


	/* Add a drop: */

	if (found != -1)
		{
			drops[found].alive = 1;
			drops[found].x = x;
			drops[found].y = y;
			drops[found].xm = xm;
			drops[found].ym = -((rand() % 6) + 2);
			drops[found].timer = (rand() % 15) + 30;
		}
}


/* Play a sound file: */

void playsound(int snd, int chan)
{
#ifndef NOSOUND
	if (use_sound == 1)
		{
#ifdef SDL_MIXER_BUG
			chan = -1;
#endif
			Mix_PlayChannel(chan, sounds[snd], 0);
			Mix_SetPanning(chan, 255, 255);
		}
#endif
}


/* Draw score: */

void drawscore(int score, int player)
{
	SDL_Rect dest;
	int i;
	char str[10];


	/* Erase where the score was: */

	dest.x = 416;
	dest.y = 2;
	dest.w = 192;
	dest.h = 32;

	erase(dest);
	addrect(dest);


	/* Draw the score: */

	sprintf(str, "%6d", score);

	for (i = 0; i < 6; i++)
		{
			dest.x = 416 + (i * 32);
			dest.y = 2;
			dest.w = 32;
			dest.h = 32;
		
			if (str[i] != ' ')
	{
		my_blit(images[IMG_0 + (str[i] - '0') + (player * 10)],
				NULL, screen, &dest);
	}
		}


	/* Draw (or erase) high score message: */

	dest.x = 416 - (images[IMG_HIGHSCORE] -> w * MULTIPLIER);
	dest.y = 2;
	dest.w = (images[IMG_HIGHSCORE] -> w * MULTIPLIER);
	dest.h = (images[IMG_HIGHSCORE] -> h * MULTIPLIER);

	if (score >= highscore)
		my_blit(images[IMG_HIGHSCORE], NULL, screen, &dest);
	else
		erase(dest);

	addrect(dest);
}


/* Pause screen: */

int pausescreen(void)
{
	SDL_Event event;
	SDLKey key;
	SDL_Rect dest;
	int done, quit;


	/* Stop sound effects: */

#ifndef NOSOUND
	if (use_sound == 1)
		Mix_HaltChannel(-1);
#endif


	/* Draw "PAUSED" message: */

	dest.x = (640 - (images[IMG_PAUSED] -> w * MULTIPLIER)) / 2;
	dest.y = (480 - (images[IMG_PAUSED] -> h * MULTIPLIER)) / 2;
	dest.w = images[IMG_PAUSED] -> w * MULTIPLIER;
	dest.h = images[IMG_PAUSED] -> h * MULTIPLIER;

	my_blit(images[IMG_PAUSED], NULL, screen, &dest);
	my_updaterect(screen, dest.x, dest.y, dest.w, dest.h);


	/* Pause loop: */

	done = 0;
	quit = 0;

	do
		{
			/* TAB or "P" key unpause: */
		
			while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_KEYDOWN)
			{
				key = event.key.keysym.sym;
			
				if (key == SDLK_TAB || key == SDLK_p)
		done = 1;
				else if (key == SDLK_ESCAPE)
		{
			done = 1;
			quit = 1;
		}
			}
		else if (event.type == SDL_QUIT)
			{
				done = 1;
				quit = 2;
			}
	}
		
			SDL_Delay(50);


#ifndef NOSOUND
			if (use_sound == 1)
	{
		if (!Mix_PlayingMusic())
			{
				Mix_PlayMusic(game_musics[rand() % NUM_GAME_MUSICS], 0);
				Mix_VolumeMusic(music_volume * 16);
			}
	}
#endif
		}
	while (done == 0);


	/* Erase "PAUSED" message: */

	dest.x = (640 - (images[IMG_PAUSED] -> w * MULTIPLIER)) / 2;
	dest.y = (480 - (images[IMG_PAUSED] -> h * MULTIPLIER)) / 2;
	dest.w = images[IMG_PAUSED] -> w * MULTIPLIER;
	dest.h = images[IMG_PAUSED] -> h * MULTIPLIER;

	erase(dest);
	my_updaterect(screen, dest.x, dest.y, dest.w, dest.h);


	/* Return whether or not we ended pause because we're quitting: */

	return(quit);
}


/* Option screen: */

int optionscreen(void)
{
	SDL_Event event;
	SDLKey key;
	SDL_Rect dest;
	int done, quit, which_option, old_which_option;
	int bx, by;


	/* Erase screen: */

	dest.x = 0;
	dest.y = 0;
	dest.w = 640;
	dest.h = 480;

	my_fillrect(screen, &dest,
				 SDL_MapRGB(screen->format, 0x00, 0x00, 0x00));


	/* Draw "OPTIONS" title: */

	dest.x = (640 - (images[IMG_OPTIONS_OPTIONS] -> w * MULTIPLIER)) / 2;
	dest.y = 0;
	dest.w = images[IMG_OPTIONS_OPTIONS] -> w * MULTIPLIER;
	dest.h = images[IMG_OPTIONS_OPTIONS] -> h * MULTIPLIER;
	my_blit(images[IMG_OPTIONS_OPTIONS], NULL, screen, &dest);


	/* Draw "Detail": */

	dest.x = 0;
	dest.y = 84;
	dest.w = images[IMG_OPTIONS_DETAIL] -> w * MULTIPLIER;
	dest.h = images[IMG_OPTIONS_DETAIL] -> h * MULTIPLIER;
	my_blit(images[IMG_OPTIONS_DETAIL], NULL, screen, &dest);

	show_option_img(1, 84, IMG_OPTIONS_NORMAL + zen * 2);


	/* Draw "Player One" and "Player Two": */

	show_option_img(
#ifndef EZX
	0
#else
	2
#endif
	, 198 - (images[IMG_OPTIONS_PLAYER_ONE] -> h * MULTIPLIER), IMG_OPTIONS_PLAYER_ONE);
	show_option_img(
#ifndef EZX
	1
#else
	3
#endif
	, 198 - (images[IMG_OPTIONS_PLAYER_TWO] -> h * MULTIPLIER), IMG_OPTIONS_PLAYER_TWO);


	/* Draw "Difficulty": */

	dest.x = 0;
	dest.y = 198;
	dest.w = images[IMG_OPTIONS_DIFFICULTY] -> w * MULTIPLIER;
	dest.h = images[IMG_OPTIONS_DIFFICULTY] -> h * MULTIPLIER;
	my_blit(images[IMG_OPTIONS_DIFFICULTY], NULL, screen, &dest);


	/* Draw players' difficulty settings: */

	if (width[0] == 1)
		show_option_img(0, 198, IMG_OPTIONS_HARD_OFF);
	else
		show_option_img(0, 198, IMG_OPTIONS_NORMAL_OFF);

	if (width[1] == 1)
		show_option_img(1, 198, IMG_OPTIONS_HARD_OFF);
	else
		show_option_img(1, 198, IMG_OPTIONS_NORMAL_OFF);


#ifndef NOSOUND_CONTROLS
	if (use_sound)
		{
			/* Draw "Effects" and "Music": */
			show_option_img(4, 312 - (images[IMG_OPTIONS_EFFECTS] -> h * MULTIPLIER), IMG_OPTIONS_EFFECTS);
			show_option_img(5, 312 - (images[IMG_OPTIONS_MUSIC] -> h * MULTIPLIER), IMG_OPTIONS_MUSIC);
		
			/* Draw "Volume": */
			dest.x = 0;
			dest.y = 312;
			dest.w = images[IMG_OPTIONS_VOLUME] -> w * MULTIPLIER;
			dest.h = images[IMG_OPTIONS_VOLUME] -> h * MULTIPLIER;
			my_blit(images[IMG_OPTIONS_VOLUME], NULL, screen, &dest);
		
			/* Draw volume settings: */
			show_option_meter(0, effects_volume, 0);
			show_option_meter(1, music_volume, 0);
		}
#endif


	/* Draw OK button: */

	dest.x = (640 - (images[IMG_OPTIONS_OK] -> w * MULTIPLIER)) / 2;
	dest.y = 480 - (images[IMG_OPTIONS_OK] -> h * MULTIPLIER);
	dest.w = images[IMG_OPTIONS_OK] -> w * MULTIPLIER;
	dest.h = images[IMG_OPTIONS_OK] -> h * MULTIPLIER;
	my_blit(images[IMG_OPTIONS_OK_OFF], NULL, screen, &dest);


	/* Update screen: */

	my_updaterect(screen, 0, 0, 640, 480);


	/* Pause loop: */

	done = 0;
	quit = 0;
	which_option = OPTIONS_OPTION_DETAIL;
	old_which_option = OPTIONS_OPTION_DETAIL;

	do
		{
			while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_KEYDOWN)
			{
				key = event.key.keysym.sym;

				if (key == SDLK_ESCAPE)
		{
			which_option = OPTIONS_OPTION_OK;
			done = 1;
		}
				else if (key == SDLK_UP)
		{
			if (which_option > 0)
				which_option--;


			/* Don't select sound options if we have no sound! */
		
#ifdef NOSOUND_CONTROLS
			if (which_option >= OPTIONS_OPTION_EFFECTS && which_option != OPTIONS_OPTION_OK) which_option = OPTIONS_OPTION_EFFECTS - 1;
#endif
		 
			if (use_sound == 0)
				{
					if (which_option >= OPTIONS_OPTION_EFFECTS &&
				which_option != OPTIONS_OPTION_OK)
			which_option = OPTIONS_OPTION_EFFECTS - 1;
				}
		}
				else if (key == SDLK_DOWN)
		{
			if (which_option < NUM_OPTIONS_OPTIONS - 1)
				which_option++;
		
		
			/* Don't select sound options if we have no sound! */
		
#ifdef NOSOUND_CONTROLS
			if (which_option >= OPTIONS_OPTION_EFFECTS &&
					which_option != OPTIONS_OPTION_OK)
				which_option = OPTIONS_OPTION_OK;
#endif
		
			if (use_sound == 0)
				{
					if (which_option >= OPTIONS_OPTION_EFFECTS &&
				which_option != OPTIONS_OPTION_OK)
			which_option = OPTIONS_OPTION_OK;
				}
		}
				else if (key == SDLK_LEFT || key == SDLK_RIGHT || key == SDLK_SPACE || key == SDLK_RETURN)
				{
			/* Any of the keys control toggles: */
		
			if (which_option == OPTIONS_OPTION_DETAIL)				zen = 1 - zen;
			else if (which_option == OPTIONS_OPTION_PLAYER_ONE)	 width[0] = 3 - width[0];
			else if (which_option == OPTIONS_OPTION_PLAYER_TWO)	 width[1] = 3 - width[1];
		
		
			if (which_option == OPTIONS_OPTION_OK)					done = 1;
		
#ifndef NOSOUND_CONTROLS
			if (use_sound)
				{
					if (key == SDLK_LEFT)
			{
				/* Left reduces volumes: */
			
				if (which_option == OPTIONS_OPTION_EFFECTS)
					{
						effects_volume--;
						if (effects_volume < 0) effects_volume = 0;
					}
				else if (which_option == OPTIONS_OPTION_MUSIC)
					{
						music_volume--;
						if (music_volume < 0) music_volume = 0;
					}
			}
					else if (key == SDLK_RIGHT)
			{
				/* Right increases volumes: */
			
				if (which_option == OPTIONS_OPTION_EFFECTS)
					{
						effects_volume++;
						if (effects_volume > 8) effects_volume = 8;
					}
				else if (which_option == OPTIONS_OPTION_MUSIC) 
					{
						music_volume++;
						if (music_volume > 8) music_volume = 8;
					}
			}
				
				
					/* Adjust sound: */
				
					if (which_option == OPTIONS_OPTION_EFFECTS)
			{
				Mix_Volume(-1, effects_volume * 16);
			}
					else if (which_option == OPTIONS_OPTION_MUSIC)
			{
				Mix_VolumeMusic(music_volume * 16);
			}
				}
#endif

			old_which_option = -1;
		}
			}
		else if (event.type == SDL_MOUSEBUTTONDOWN)
			{
				bx = event.button.x * MULTIPLIER;
				by = event.button.y * MULTIPLIER;
				/* Click - figure out where! */
			
				if (by >= 84 && by <= 84 + (images[IMG_OPTIONS_NORMAL] -> h * MULTIPLIER) && bx >= 640 - (images[IMG_OPTIONS_NORMAL] -> w * MULTIPLIER))
		{
			/* Detail: */
		
			zen = 1 - zen;
		
			which_option = OPTIONS_OPTION_DETAIL;
			if (old_which_option == which_option) old_which_option = -1;
		}
				else if (by >= 198 && by <= (198 + (images[IMG_OPTIONS_NORMAL] -> h * MULTIPLIER)))
		{
			/* Difficulty: */
		
			if (bx >= 220 && bx <= (220 + (images[IMG_OPTIONS_NORMAL] -> w * MULTIPLIER)))
				{
					/* Player one: */
				
					width[0] = 3 - width[0];
				
					which_option = OPTIONS_OPTION_PLAYER_ONE;
					if (old_which_option == which_option)
			old_which_option = -1;
				}
			else if (bx >= (640 - (images[IMG_OPTIONS_NORMAL] -> w * MULTIPLIER)))
				{
					/* Player two: */
				
					width[1] = 3 - width[1];
				
					which_option = OPTIONS_OPTION_PLAYER_TWO;
					if (old_which_option == which_option)
			old_which_option = -1;
				}
		}
				else if (by >= 480 - (images[IMG_OPTIONS_OK] -> h * MULTIPLIER) &&
					 bx >= (640 - (images[IMG_OPTIONS_OK] -> w * MULTIPLIER)) / 2 &&
					 bx <= ((640 - (images[IMG_OPTIONS_OK] -> w * MULTIPLIER)) / 2) + (images[IMG_OPTIONS_OK] -> w * MULTIPLIER))
		{
			/* OK button: */
		
			which_option = OPTIONS_OPTION_OK;
			if (old_which_option == which_option)
				old_which_option = -1;
		
			done = 1;
		}
	
	
#ifndef NOSOUND_CONTROLS
				if (use_sound == 1)
		{
			if (by >= 312 && by <= (312 + (images[IMG_OPTIONS_0PERCENT] -> h * MULTIPLIER)))
			{
				/* Volume controls: */
			
				if (bx >= 220 && bx <= (220 + ((images[IMG_OPTIONS_0PERCENT] -> w * MULTIPLIER) * 9)))
			{
				/* Effects: */
			
				effects_volume = ((bx - 220) / (images[IMG_OPTIONS_0PERCENT] -> w * MULTIPLIER));
				Mix_Volume(-1, effects_volume * 16);
			
				which_option = OPTIONS_OPTION_EFFECTS;
				if (old_which_option == which_option) old_which_option = -1;
			}
				else if (bx >= (640 - ((images[IMG_OPTIONS_0PERCENT] -> w * MULTIPLIER) * 9)))
			{
				/* Music: */
			
				music_volume = ((bx - (640 - ((images[IMG_OPTIONS_0PERCENT] -> w * MULTIPLIER) * 9))) / (images[IMG_OPTIONS_0PERCENT] -> w * MULTIPLIER));
			
				Mix_VolumeMusic(music_volume * 16);

				which_option = OPTIONS_OPTION_MUSIC;
				if (old_which_option == which_option) old_which_option = -1;
			}
				}
		}
#endif
			}
		else if (event.type == SDL_QUIT)
			{
				done = 1;
				quit = 1;
			}
	}
		
		
			/* Draw a new option? */
		
			if (which_option != old_which_option)
	{
		/* Erase the previously-selected option: */
	
		if (old_which_option == OPTIONS_OPTION_DETAIL)
			show_option_img(1, 84, IMG_OPTIONS_NORMAL_OFF + zen * 2);
		else if (old_which_option == OPTIONS_OPTION_PLAYER_ONE)
			{
				if (width[0] == 1)	show_option_img(0, 198, IMG_OPTIONS_HARD_OFF);
				else					show_option_img(0, 198, IMG_OPTIONS_NORMAL_OFF);
			}
		else if (old_which_option == OPTIONS_OPTION_PLAYER_TWO)
			{
				if (width[1] == 1)	show_option_img(1, 198, IMG_OPTIONS_HARD_OFF);
				else					show_option_img(1, 198, IMG_OPTIONS_NORMAL_OFF);
			}
		else if (old_which_option == OPTIONS_OPTION_EFFECTS)
			{
								show_option_meter(0, effects_volume, 0);
			}
		else if (old_which_option == OPTIONS_OPTION_MUSIC)
			{
								show_option_meter(1, music_volume, 0);
			}
		else if (old_which_option == OPTIONS_OPTION_OK)
			{
				dest.x = (640 - (images[IMG_OPTIONS_OK] -> w * MULTIPLIER)) / 2;
				dest.y = 480 - (images[IMG_OPTIONS_OK] -> h * MULTIPLIER);
				dest.w = images[IMG_OPTIONS_OK] -> w * MULTIPLIER;
				dest.h = images[IMG_OPTIONS_OK] -> h * MULTIPLIER;
			
				my_blit(images[IMG_OPTIONS_OK_OFF], NULL, screen, &dest);
				my_updaterect(screen, dest.x, dest.y, dest.w, dest.h);
				my_updaterect(screen, 0, 0, 640, 480);
			}
	
	
		/* Draw the newly-selected option: */
	
		if (which_option == OPTIONS_OPTION_DETAIL)
			show_option_img(1, 84, IMG_OPTIONS_NORMAL + zen * 2);
		else if (which_option == OPTIONS_OPTION_PLAYER_ONE)
			{
				if (width[0] == 1)
		show_option_img(0, 198, IMG_OPTIONS_HARD);
				else
		show_option_img(0, 198, IMG_OPTIONS_NORMAL);
			}
		else if (which_option == OPTIONS_OPTION_PLAYER_TWO)
			{
				if (width[1] == 1)
		show_option_img(1, 198, IMG_OPTIONS_HARD);
				else
		show_option_img(1, 198, IMG_OPTIONS_NORMAL);
			}
		else if (which_option == OPTIONS_OPTION_EFFECTS)
			{
				show_option_meter(0, effects_volume, 1);
			}
		else if (which_option == OPTIONS_OPTION_MUSIC)
			{
				show_option_meter(1, music_volume, 1);
			}
		else if (which_option == OPTIONS_OPTION_OK)
			{
				dest.x = (640 - (images[IMG_OPTIONS_OK] -> w * MULTIPLIER)) / 2;
				dest.y = 480 - (images[IMG_OPTIONS_OK] -> h * MULTIPLIER);
				dest.w = images[IMG_OPTIONS_OK] -> w * MULTIPLIER;
				dest.h = images[IMG_OPTIONS_OK] -> h * MULTIPLIER;

				my_blit(images[IMG_OPTIONS_OK], NULL, screen, &dest);
				my_updaterect(screen, dest.x, dest.y, dest.w, dest.h);
				my_updaterect(screen, 0, 0, 640, 480);
			}
	
	
		/* Play selection sound: */
	
		playsound(SND_SELECT, -1);
	
		old_which_option = which_option;
	}
		
		
#ifndef NOSOUND
			/* Keep playing music: */
		
			if (use_sound == 1)
	{
		if (!Mix_PlayingMusic())
			{
				Mix_PlayMusic(title_music, 0);
				Mix_VolumeMusic(music_volume * 16);
			}
	}
#endif


			/* Pause: */
		
			SDL_Delay(50);
		}
	while (done == 0);


	/* Return whether or not we ended pause because we're quitting: */

	return(quit);
}


/* Explode all of the bombs (self-contained animation loop): */

void explodebombs(void)
{
	int i, bottom_most, lowest;
	SDL_Rect dest;
	int posn;


	/* Explode each bomb, one at a time: */

	do
		{
			/* Find the bottom-most bomb: */
		
			bottom_most = -1;
			lowest = 0;
		
			for (i = 0; i < MAX_BOMBS; i++)
	{
		if (bombs[i].alive == 1)
			{
				if (bombs[i].y > lowest)
		{
			lowest = bombs[i].y;
			bottom_most = i;
		}
			}
	}
		
		
			/* Explode it! */
		
			if (bottom_most != -1)
	{
		/* Turn the bomb off: */
	
		bombs[bottom_most].alive = 0;
	
	
		/* Play an explosion sound: */
	
		playsound(SND_EXPLOSION, 1);

#ifndef NOSOUND
		posn = (bombs[bottom_most].x * 255) / screen->w;
		Mix_SetPanning(1, 255 - posn, posn);
#endif
	
	
		/* Draw the explosion animation: */
	
		for (i = 0; i < 4; i++)
			{
				dest.x = bombs[bottom_most].x;
				dest.y = bombs[bottom_most].y;
				dest.w = 32;
				dest.h = 32;
			
				my_blit(images[IMG_EXPLOSION1 + (i % 2)],
						NULL, screen, &dest);
				my_updaterect(screen, dest.x, dest.y, dest.w, dest.h);
			
				SDL_Delay(30);
			}
	
	
		/* Erase the explosion animation: */
	
		dest.x = bombs[bottom_most].x;
		dest.y = bombs[bottom_most].y;
		dest.w = 32;
		dest.h = 32;
	
		erase(dest);
		my_updaterect(screen, dest.x, dest.y, dest.w, dest.h);
	
	
		/* Make sure the bomber doesn't get cropped: */
	
		dest.x = bomber_x;
		dest.y = 67;
		dest.w = 32;
		dest.h = 64;
	
		if (score[player] < 10000 || vs_mode == 1)
			my_blit(images[IMG_BOMBER_HAPPY],
					NULL, screen, &dest);
		else
			my_blit(images[IMG_BOMBER_AMAZED],
					NULL, screen, &dest);
		my_updaterect(screen, dest.x, dest.y, dest.w, dest.h);


		/* Make sure the buckets don't get cropped: */
	
		for (i = 0; i < num_buckets[player]; i++)
			{
				dest.x = x[player];
				dest.y = 352 + i * 32;
				dest.w = 32 * width[player];
				dest.h = 32;
			
				my_blit(images[IMG_BUCKET1 + (((frame / 3) + i) % 3) +
						(player * 3) + 12 - (width[player] * 6)],
						NULL, screen, &dest);
				my_updaterect(screen, dest.x, dest.y, dest.w, dest.h);
			}
	}
		}
	while (bottom_most != -1);

	Mix_SetPanning(1, 255, 255);


	/* Play a BIG explosion sound: */

	playsound(SND_BIGEXPLOSION, 1);


	/* Flash screen: */

	dest.x = 0;
	dest.y = 0;
	dest.w = 640;
	dest.h = 480;


	/* (draw white) */

	my_fillrect(screen, &dest,
				 SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF));
	my_updaterect(screen, 0, 0, 640, 480);


	/* (wait a brief moment) */

	SDL_Delay(50);


	/* (return to normal) */

	/* ((redraw background)) */

	erase(dest);


	/* ((redraw bomber)) */

	dest.x = bomber_x;
	dest.y = 67;
	dest.w = 32;
	dest.h = 64;

	if (score[player] < 10000 || vs_mode == 1)
		my_blit(images[IMG_BOMBER_HAPPY],
				NULL, screen, &dest);
	else
		my_blit(images[IMG_BOMBER_AMAZED],
				NULL, screen, &dest);


	/* ((redraw buckets)) */

	for (i = 0; i < num_buckets[player] - 1; i++)
		{
			dest.x = x[player];
			dest.y = 352 + i * 32;
			dest.w = 32 * width[player];
			dest.h = 32;
		
			my_blit(images[IMG_BUCKET1 + (((frame / 3) + i) % 3) +
					(player * 3) + 12 - (width[player] * 6)],
					NULL, screen, &dest);
		}


	/* ((redraw score)) */

	drawscore(score[player], player);


	/* ((update it all)) */

	my_updaterect(screen, 0, 0, 640, 480);


#ifndef NOSOUND
	if (use_sound == 1)
		{
			do
	{
		/* Wait... */
		SDL_Delay(30);
	}
			while (Mix_Playing(1));
		}
#endif
}


/* Erase part of the screen back to the background: */

void erase(SDL_Rect dest)
{
	/* Copy the background bitmap onto the screen: */

	if (zen == 0)
		{
			my_blit(images[IMG_BACKGROUND],
				&dest, screen, &dest);
		}
	else
		{
			my_blit(zen_background, &dest, screen, &dest);
		}
}


/* Open the option file: */

FILE * open_option_file(char * mode)
{
	char * filename;
	FILE * fi;


	filename = ".madbomber";


	/* Try opening the file: */

	fi = fopen(filename, mode);

	if (fi == NULL)
		{
			fprintf(stderr, "\nWarning: Could not open the options file "); 
		
			if (strcmp(mode, "r") == 0)			fprintf(stderr, "for read:");
			else if (strcmp(mode, "w") == 0)	fprintf(stderr, "for write:");
		
		 fprintf(stderr, " %s\nThe error that occured was: %s\n\n", filename, strerror(errno));
		}

	return(fi);
}


#ifdef EZX
/* Open the state file: */

FILE * open_state_file(char * mode)
{
	char * filename;
	FILE * fi;

	filename = ".madbomber-state";


	/* Try opening the file: */

	fi = fopen(filename, mode);

	if (fi == NULL)
		{
			fprintf(stderr, "\nWarning: Could not open the state file "); 
		
			if (strcmp(mode, "r") == 0)			fprintf(stderr, "for read:");
			else if (strcmp(mode, "w") == 0)	fprintf(stderr, "for write:");
		
		 fprintf(stderr, " %s\nThe error that occured was: %s\n\n", filename, strerror(errno));
		}

	return(fi);
}

#endif



/* Show an option image: */

void show_option_img(int horiz, int y, int img)
{
	SDL_Rect dest;

#ifndef EZX
	if(horiz == 2 || horiz == 4) horiz = 0; else
	if(horiz == 3 || horiz == 5) horiz = 1;
#endif

	/* Determine horizontal location: */

	if (horiz == 0) dest.x = 220; else
	if (horiz == 2) dest.x = 250; else
	if (horiz == 3) dest.x = 640 - (images[img] -> w * MULTIPLIER) - 20; else
	if (horiz == 4) dest.x = 270; else
	if (horiz == 5) dest.x = 640 - (images[img] -> w * MULTIPLIER) - 46; else
					dest.x = 640 - (images[img] -> w * MULTIPLIER);

	dest.y = y;
	dest.w = images[img] -> w * MULTIPLIER;
	dest.h = images[img] -> h * MULTIPLIER;

	/* Draw or erase the spot: */
	my_blit(images[img], NULL, screen, &dest);
	my_updaterect(screen, dest.x, dest.y, dest.w, dest.h);
}


/* Display a volume meter: */

void show_option_meter(int horiz, int value, int selected)
{
	int img, i, x;
	SDL_Rect dest;


	/* Determine horizontal location: */

	if (horiz == 0)
		x = 220;
	else
		x = 640 - ((images[IMG_OPTIONS_0PERCENT] -> w * MULTIPLIER) * 9);


	/* Erase meter: */

	dest.x = x;
	dest.y = 312;
	dest.w = (images[IMG_OPTIONS_0PERCENT] -> w * MULTIPLIER) * 9;
	dest.h = images[IMG_OPTIONS_0PERCENT] -> h * MULTIPLIER;

	my_fillrect(screen, &dest, SDL_MapRGB(screen->format, 0x00, 0x00, 0x00));


	/* Draw the meter: */

	for (i = 0; i <= value; i++)
		{
			img = IMG_OPTIONS_0PERCENT + ((i / 2) * 2) + (1 - selected);
		
			dest.x = x + i * (images[IMG_OPTIONS_0PERCENT] -> w * MULTIPLIER);
			dest.y = 312;
			dest.w = images[IMG_OPTIONS_0PERCENT] -> w * MULTIPLIER;
			dest.h = images[IMG_OPTIONS_0PERCENT] -> h * MULTIPLIER;
		
			my_blit(images[img], NULL, screen, &dest);
		}


	/* Update the screen: */

	my_updaterect(screen, x, 312, (images[IMG_OPTIONS_0PERCENT] -> w * MULTIPLIER) * 9, images[IMG_OPTIONS_0PERCENT] -> h * MULTIPLIER);
}


/* Draw flying numbers on the screen: */

void draw_flying_numbers(int number, int scale, int r, int g, int b)
{
	SDL_Rect dest;
	int i;

	for (i = 0; i < 7; i++)
		{
			/* If this "light" of the LED is on, then draw it: */
		
			if (flying_number_layouts[number][i])
	{
		/* Determine where the "light" is: */
	
		if (i == 0 || i == 1 || i == 3 || i == 4 || i == 6)
			dest.x = 320 + (-scale * 5);
		else
			dest.x = 320 + (scale * 5);
	
		if (i == 0 || i == 1 || i == 2)
			dest.y = 240 + (-scale * 10);
		else if (i == 3 || i == 4 || i == 5)
			dest.y = 240;
		else if (i == 6)
			dest.y = 240 + (scale * 10);
	
	
		/* ...And what shape it is: */
	
		if (i == 0 || i == 3 || i == 6)
			{
				dest.w = scale * 8;
				dest.h = scale * 2;
			}
		else
			{
				dest.w = scale * 2;
				dest.h = scale * 8;
			}
	
	
		/* Draw it: */
	
		my_fillrect(screen, &dest,
					 SDL_MapRGB(screen->format, r, g, b));
		addrect(dest);
	}
		}
}


/* Erase flying numbers: */

void erase_flying_numbers(int scale)
{
	SDL_Rect dest;

	dest.x = 320 + (-scale * 5);
	dest.y = 240 + (-scale * 10);
	dest.w = scale * 15;
	dest.h = scale * 22;

	erase(dest);
	addrect(dest);
}



/* High Score Screen: */

int highscorescreen(void)
{
	int done, i, img, y;
	char temp[10];
	SDL_Event event;
	SDLKey key;
	SDL_Rect src, dest;


	/* Draw game background: */

	my_blit(images[IMG_BACKGROUND], NULL, screen, NULL);
	my_updaterect(screen, 0, 0, 640, 480);


	/* Clear all spray drips: */

	for (i = 0; i < MAX_SPRAYDRIPS; i++)
		{
			spraydrips[i].alive = 0;
		}


	/* Draw text onto it: */

	quick_spray = 0;

	done = spraytext("HIGH SCORE", 175, IMG_SPRAY_CYAN, 5);

	if (done == 0)
		done = spraytext(highscorer, 225, IMG_SPRAY_BLACK, 7);

	if (done == 0)
		{
			sprintf(temp, "%d", highscore);
			done = spraytext(temp, 300, IMG_SPRAY_BLUE, 7);
		}


	/* Which bomber will we draw? */

	if (highscore >= 10000)
		img = IMG_BOMBER_AMAZED;
	else if (highscore > 5000)
		img = IMG_BOMBER_SAD;
	else
		img = IMG_BOMBER_HAPPY;

	y = 0;


	if (done == 0)
		{	
			do
	{
		/* Handle events: */
	
		while (SDL_PollEvent(&event))
			{
				if (event.type == SDL_KEYDOWN)
		{
			key = event.key.keysym.sym;
		
			if (key == SDLK_ESCAPE)
				done = 1;
		}
				else if (event.type == SDL_QUIT)
		done = 2;
				else if (event.type == SDL_MOUSEBUTTONDOWN)
		done = 1;
			}
	
	
		/* Handle spray drips: */
	
		handle_spraydrips();
	
	
		/* Draw bomber: */
	
		if (y < 64)
			{
				y++;

				src.x = 0;
				src.y = 0;
				src.w = 32;
				src.h = y;
			
				dest.x = 500;
				if (y < 61)
		dest.y = 126 - y;
				else
		dest.y = 67;
			
				dest.w = 32;
				dest.h = y;
			
			
				my_blit(images[IMG_BACKGROUND], &dest, screen, &dest);
				my_blit(images[img], &src, screen, &dest);
				my_updaterect(screen, dest.x, dest.y, dest.w, dest.h);
			}
	

#ifndef NOSOUND
		/* Keep playing music: */
	
		if (use_sound == 1)
			{
				if (!Mix_PlayingMusic())
		{
			Mix_PlayMusic(title_music, 0);
			Mix_VolumeMusic(music_volume * 16);
		}
			}
#endif
	
	
		SDL_Delay(33);
	}
			while (done == 0);
		}


	/* Return whether or not we're done because the user requested a quit: */

	if (done == 0 || done == 1)
		return(0);
	else
		return(1);
}


/* Animate spraying of text onto the screen: */

int spraytext(char * str, int y, int img, int scale)
{
	int ltr, line, c, x, z;
	SDL_Event event;
	SDLKey key;


	/* Play shaking-can noise: */

#ifndef NOSOUND
	if (use_sound == 1)
		{
			if (quick_spray == 0)
	{
		playsound(SND_CAN_SHAKE, 2);
	
		do
			{
				SDL_Delay(100);
			}
		while (Mix_Playing(2));
	}
		}
#endif


	/* Place our virtual cursor (we want each line centered): */

	x = (640 - (strlen(str) * (scale * 6))) / 2;


	/* Draw each letter: */

	for (ltr = 0; ltr < strlen(str); ltr++)
		{
			c = str[ltr];
		
		
			/* Where in our graffiti array is this particular character? */
		
			if (c >= 'A' && c <= 'Z')
	c = c - 'A' + 10;
			else if (c >= '0' && c <= '9')
	c = c - '0';
			else
	c = -1;
		
		
			/* If it's a valid character, draw it! */
		
			if (c != -1)
	{
		for (line = 0; line < 5 && graffiti[c][line][0][0] != -1; line++)
			{
				z = rand() % 6;
			
				sprayline(graffiti[c][line][0][0] * scale + x,
			graffiti[c][line][0][1] * scale + y + z,
			graffiti[c][line][1][0] * scale + x,
			graffiti[c][line][1][1] * scale + y + z, img);
			}
	}
		
		
			/* Move the virtual cursor over one character: */
		
			x = x + (scale * 6);
	
#ifndef EZX
			SDL_Delay(30);
#endif
		
		
			/* Handle events: */
		
			while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_KEYDOWN)
			{
				key = event.key.keysym.sym;
			
				if (key == SDLK_ESCAPE)
		return(1);
				else
		quick_spray = 1;
			}
		else if (event.type == SDL_QUIT)
			return(2);
		else if (event.type == SDL_MOUSEBUTTONDOWN)
			quick_spray = 1;
	}
		}

	SDL_Delay(100);

#ifndef NOSOUND
	#ifndef SDL_MIXER_BUG
		if (use_sound == 1) Mix_HaltChannel(2);
	#endif
#endif

	return(0);
}


/* Draw a line with spray paint: */

void sprayline(int x1, int y1, int x2, int y2, int img)
{
	int x, y, z, steep, e, dx, dy, i;

	/* Play spraying noise: */
	#ifndef NOSOUND
		if (use_sound == 1) { if (quick_spray == 0) { if (!Mix_Playing(2)) playsound(SND_SPRAY, 2);}; }
	#endif

	/* Draw the line: */
	if (x1 == x2) { /* Vertical line: */
		if (y1 > y2) {
			z = y1;
			y1 = y2;
			y2 = z;
		}
		for (y = y1; y <= y2; y++) spraydot(x1, y, img);
	} else
	if (y1 == y2) { /* Horizontal line: */
		if (x1 > x2) {
			z = x1;
			x1 = x2;
			x2 = z;
		}
		for (x = x1; x <= x2; x++) spraydot(x, y1, img);
	} else { /* Angled line: */
		steep = 0;
	
		dx = abs(x2 - x1);
		x = ((x2 - x1) > 0) ? 1 : -1;
	
		dy = abs(y2 - y1);
		y = ((y2 - y1) > 0) ? 1 : -1;
	
		if (dy > dx) {
			steep = 1;
	
			z = x1;
			x1 = y1;
			y1 = z;
	
			z = dy;
			dy = dx;
			dx = z;
	
			z = y;
			y = x;
			x = z;
		}
		
		e = 2 * dy - dx;
		
		for (i = 0; i < dx; i++) {
			if (steep == 1) spraydot(y1, x1, img);
			else			spraydot(x1, y1, img);
	
			while (e >= 0) {
				y1 = y1 + y;
				e = e - 2 * dx;
			}
	
			x1 = x1 + x;
			e = e + 2 * dy;
		}

		spraydot(x2, y2, img);
	}

#ifdef EZX
	SDL_Flip(screen);
#endif
}


/* Spray one dot: */

void spraydot(int x, int y, int img)
{
	SDL_Rect src, dest;

	/* Pick a semi-random spot with a random spray bit: */
	dest.x = x + (rand() % 3);
	dest.y = y + (rand() % 4);
	dest.w = 4;
	dest.h = 4;

	src.x = (rand() % ((images[img] -> w) - 4));
	src.y = (rand() % ((images[img] -> h) - 4));
	src.w = 4;
	src.h = 4;
 
	/* Draw it: */
	my_blit(images[img], &src, screen, &dest);
	my_updaterect(screen, dest.x, dest.y, dest.w, dest.h);


	/* Pause: */
	spray_count = spray_count++;

	if ((spray_count % 3) == 0 && quick_spray == 0) SDL_Delay(10);

	/* Make a drip: */
	if ((rand() % 10) == 0) add_spraydrip(dest.x, dest.y, img);

	/* Handle spray drips: */
	handle_spraydrips();
}


void handle_spraydrips(void)
{
	int i;
	SDL_Rect src, dest;

	for (i = 0; i < MAX_SPRAYDRIPS; i++) {
		if (spraydrips[i].alive) {
			/* Move drip: */
			spraydrips[i].y++;
	
			/* Make drip run out of time: */
			spraydrips[i].timer--;
	
			if (spraydrips[i].timer <= 0)
				spraydrips[i].alive = 0;
	
			/* Draw spraydrip: */
			src.x = (rand() % ((images[spraydrips[i].img] -> w) - 2));
			src.y = (rand() % ((images[spraydrips[i].img] -> h) - 2));
			src.w = 2;
			src.h = 2;
	
			dest.x = spraydrips[i].x;
			dest.y = spraydrips[i].y;
			dest.w = 2;
			dest.h = 2;
	
			my_blit(images[spraydrips[i].img], &src, screen, &dest);
			my_updaterect(screen, dest.x, dest.y, dest.w, dest.h);
		}
	}
}


/* Turn a spraydrip on: */

void add_spraydrip(int x, int y, int img)
{
	int found;

	found = (rand() % MAX_SPRAYDRIPS);

	spraydrips[found].alive = 1;
	spraydrips[found].timer = (rand() % 10) + 5;
	spraydrips[found].img = img;
	spraydrips[found].x = x;
	spraydrips[found].y = y;
}


/* Have player sign their name for a high score: */

int sign_highscore(int player)
{
#ifndef EZX
	SDL_Rect src, dest;
	int done, l, draw_text, i;
	SDL_Event event;
	SDLKey key;


	/* Draw high score signature screen: */

	dest.x = 0;
	dest.y = 0;
	dest.w = 640;
	dest.h = 480;

	my_fillrect(screen, &dest, SDL_MapRGB(screen->format, 0, 0, 0));


	dest.x = 0;
	dest.y = 0;
	dest.w = 640;
	dest.h = images[IMG_HIGHSCORE_HIGHSCORE] -> h;

	my_blit(images[IMG_HIGHSCORE_HIGHSCORE], NULL, screen, &dest);

	my_updaterect(screen, 0, 0, 640, 480);

	strcpy(highscorer, "");
	l = 0;

	done = 0;
	draw_text = 1;

	do { /* Handle events: */

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_KEYDOWN)
			{
				key = event.key.keysym.sym;
	
				if (key == SDLK_ESCAPE) return(0);
				else if (key == SDLK_DELETE || key == SDLK_BACKSPACE) {
					if (l > 0) {
						l--;
						highscorer[l] = '\0';
		
						draw_text = 1;
						playsound(SND_CONFIRM, 0);
					}
				}
				else if ((key >= SDLK_a && key <= SDLK_z) || (key >= SDLK_0 && key <= SDLK_9) || key == SDLK_SPACE) {
					if (l < sizeof (highscorer)) {
						if (key >= SDLK_a && key <= SDLK_z)
		
						highscorer[l] = key - SDLK_a + 'A';
						l++;
						highscorer[l] = '\0';
		
						draw_text = 1;
						playsound(SND_SELECT, 0);
					}
				}
				else if (key == SDLK_RETURN) {
					done = 1;
					playsound(SND_EXPLOSION, 0);
				}
			}
			else if (event.type == SDL_QUIT)
			return(2);
		}
			/* Draw text? */

		if (draw_text == 1) {
			/* Erase first: */

			dest.x = 0;
			dest.y = 200;
			dest.w = 640;
			dest.h = images[IMG_HIGHSCORE_LETTERS] -> h;

			my_fillrect(screen, &dest, SDL_MapRGB(screen->format, 0, 0, 0));

			for (i = 0; i < strlen(highscorer); i++)
			{
				if (highscorer[i] != ' ') {
				if (highscorer[i] >= '0' && highscorer[i] <= '9')			src.x = ((highscorer[i] - '0') * (images[IMG_HIGHSCORE_LETTERS] -> w) / 36);
				else if (highscorer[i] >= 'A' && highscorer[i] <= 'Z')	src.x = ((highscorer[i] - 'A' + 10) * (images[IMG_HIGHSCORE_LETTERS] -> w) / 36);

				src.y = 0;
				src.w = (images[IMG_HIGHSCORE_LETTERS] -> w) / 36;
				src.h = images[IMG_HIGHSCORE_LETTERS] -> h;

				dest.x = ((640 - (strlen(highscorer) * ((images[IMG_HIGHSCORE_LETTERS] -> w) / 36))) / 2) + (i * (images[IMG_HIGHSCORE_LETTERS] -> w) / 36);
				dest.y = 200;
				dest.w = src.w;
				dest.h = src.h;

				my_blit(images[IMG_HIGHSCORE_LETTERS], &src, screen, &dest);
				}
			}


			/* Update the screen: */

			dest.x = 0;
			dest.y = 200;
			dest.w = 640;
			dest.h = images[IMG_HIGHSCORE_LETTERS] -> h;

			my_updaterect(screen, 0, 200, 640, dest.h);

			draw_text = 0;
		}



		#ifndef NOSOUND
			/* Keep playing music: */

			if (use_sound == 1)
		{
			if (!Mix_PlayingMusic())
			{
				Mix_PlayMusic(highscore_music, 0);
				Mix_VolumeMusic(music_volume * 16);
			}
		}
		#endif


		/* Pause: */

		SDL_Delay(50);
	}
	while (done == 0);
#else
	int done=1;
	
	#ifndef WITHOUT_SK
		SDL_Rect dest1, dest2;
		int i, alnum, not_empty, empty=0;
		SDL_Event event;
		SDLKey key;

		dest1.x = 0;		dest1.y = 0;	dest1.w = 640;		dest1.h = 480;
		dest2.x = 0;		dest2.y = 0;	dest2.w = 640;		dest2.h = images[IMG_HIGHSCORE_HIGHSCORE] -> h * MULTIPLIER;

		strcpy(highscorer, "");

		do {
			done=1; alnum=1; not_empty=1;
	
			my_fillrect(screen, &dest1, SDL_MapRGB(screen->format, 0, 0, 0));
			my_blit(images[IMG_HIGHSCORE_HIGHSCORE], NULL, screen, &dest2);
			my_updaterect(screen, 0, 0, 640, 480);

			SK::GetString(highscorer);
			if(strlen(highscorer)==0) { not_empty=0; empty++; SK::SetTitle("Please enter your name:"); } else {
				for(i=0; i<strlen(highscorer); i++) if(!isalnum(highscorer[i]) && highscorer[i]!=' ') { alnum=0; SK::SetTitle("Only alphanumeric characters!"); }
				if(alnum) for(i=0; i<strlen(highscorer); i++) highscorer[i]=toupper(highscorer[i]);
			}
	
			if(alnum && not_empty)	done=0;
			else if (empty>=3)		{ strcpy(highscorer, "EZX Player"); done=0; }
		} while (done!=0);
		SK::RemoveTitle();
	#else
		strcpy(highscorer, "EZX");
	#endif
#endif

	if (done == 0 || done == 1)	return(0);
	else						return(1);
}


void my_blit(SDL_Surface * src_img, SDL_Rect * src_rect, SDL_Surface * dest_img, SDL_Rect * dest_rect) {
	SDL_Rect src_rect2, dest_rect2;
	if (src_rect != NULL) {
		src_rect2.x = (*src_rect).x / MULTIPLIER;		src_rect2.y = (*src_rect).y / MULTIPLIER;
		src_rect2.w = (*src_rect).w / MULTIPLIER;		src_rect2.h = (*src_rect).h / MULTIPLIER;
		src_rect = &src_rect2;
	}
	if (dest_rect != NULL) {
		dest_rect2.x = (*dest_rect).x / MULTIPLIER;		dest_rect2.y = (*dest_rect).y / MULTIPLIER;
		dest_rect2.w = (*dest_rect).w / MULTIPLIER;		dest_rect2.h = (*dest_rect).h / MULTIPLIER;
		dest_rect = &dest_rect2;
	}
	SDL_BlitSurface(src_img, src_rect, dest_img, dest_rect);
}


void my_updaterect(SDL_Surface * surf, int x, int y, int w, int h) {
	SDL_UpdateRect(surf, x / MULTIPLIER, y / MULTIPLIER, w / MULTIPLIER, h / MULTIPLIER);
}


void my_fillrect(SDL_Surface * surf, SDL_Rect * dest, Uint32 color) {
	SDL_Rect dest2;
	dest2.x = dest->x / MULTIPLIER;	dest2.y = dest->y / MULTIPLIER;
	dest2.w = dest->w / MULTIPLIER;	dest2.h = dest->h / MULTIPLIER;
	dest = &dest2;

	SDL_FillRect(surf, dest, color);	
}
