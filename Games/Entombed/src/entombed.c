/*
  entombed.c

  "Entombed!" - Escape the maze and avoid the zombies.
  Based on "Entombed" for the Atari 2600, (c) 1982 U.S. Games
  
  by Bill Kendrick
  bill@newbreedsoftware.com
  http://www.newbreedsoftware.com/

  May 19, 2002 - June 29, 2007
*/

#define VERSION "2007.06.29"


//#define DEBUG

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "SDL.h"
#ifndef NOSOUND
#include "SDL_mixer.h"
#endif
#ifndef EMBEDDED
#include "SDL_image.h"
#endif
#include <SDL_ttf.h>


#define FPS 1000 / 30

#define MAP_WIDTH 10
#define MAP_HEIGHT 75
#define SCREEN_HEIGHT 12


#ifdef EMBEDDED
#define HALFSIZE
#endif
#define WIDTH 320
#define HEIGHT 240
#define SPRITESIZE 16
#define SPRITESIZE_27 13
#define SPRITESIZE_15 7
#define SPRITESIZE_4 2
#define SPRITESIZE_2 1
#define SPRITESIZE_160 80
#define MAKEBREAK_DIST 17

#define PAUSED_X 85
#define PAUSED_Y 50

#define PAUSED_W 150
#define PAUSED_H 48

#define CREDITS_X 150
#define CREDITS_Y 125

#define CREDITS_W 150
#define CREDITS_H 37

#define PLAYERS_X 152
#define PLAYERS_Y 50

#define PLAYERS_W 20
#define PLAYERS_H 25
 
#define STARTING_MAKEBREAK 2
#define STARTING_LIVES 3


enum {
  IMG_TITLE,
  IMG_BRICK1,
  IMG_BRICK2,
  IMG_BRICK3,
  IMG_BRICK4,
  IMG_BRICK5,
  IMG_MAN1,
  IMG_MAN2,
  IMG_WOMAN1,
  IMG_WOMAN2,
  IMG_ZOMBIE1,
  IMG_ZOMBIE2,
  IMG_ZOMBIE_MAD1,
  IMG_ZOMBIE_MAD2,
  IMG_MAKEBREAK1,
  IMG_MAKEBREAK2,
  IMG_MAKEBREAK3,
  IMG_MAKEBREAK4,
  IMG_MAKEBREAK5,
  IMG_CREDITS1,
  IMG_CREDITS2,
  IMG_CREDITS3,
  IMG_CREDITS4,
  IMG_CREDITS5,
  IMG_1,
  IMG_2,
  IMG_NUMBERS,
  IMG_PAUSED,
  NUM_IMAGES
};

#define NUM_BRICKS 5
#define NUM_CREDITS 5

enum {
  DIR_UP,
  DIR_RIGHT,
  DIR_DOWN,
  DIR_LEFT
};


const char * image_names[NUM_IMAGES] = {
  DATA_PREFIX "images/title.jpg",
  DATA_PREFIX "images/brick1.png",
  DATA_PREFIX "images/brick2.png",
  DATA_PREFIX "images/brick3.png",
  DATA_PREFIX "images/brick4.png",
  DATA_PREFIX "images/brick5.png",
  DATA_PREFIX "images/man1.png",
  DATA_PREFIX "images/man2.png",
  DATA_PREFIX "images/woman1.png",
  DATA_PREFIX "images/woman2.png",
  DATA_PREFIX "images/zombie1.png",
  DATA_PREFIX "images/zombie2.png",
  DATA_PREFIX "images/zombie_mad1.png",
  DATA_PREFIX "images/zombie_mad2.png",
  DATA_PREFIX "images/makebreak1.png",
  DATA_PREFIX "images/makebreak2.png",
  DATA_PREFIX "images/makebreak3.png",
  DATA_PREFIX "images/makebreak4.png",
  DATA_PREFIX "images/makebreak5.png",
  DATA_PREFIX "images/credits1.png",
  DATA_PREFIX "images/credits2.png",
  DATA_PREFIX "images/credits3.png",
  DATA_PREFIX "images/credits4.png",
  DATA_PREFIX "images/credits5.png",
  DATA_PREFIX "images/1.png",
  DATA_PREFIX "images/2.png",
  DATA_PREFIX "images/numbers.png",
  DATA_PREFIX "images/paused.png"
};


enum {
  /* SND_WALK, */
  SND_MAKEBREAK_GET,
  SND_MAKEBREAK_USE,
  SND_ZOMBIE_APPEAR,
  SND_ZOMBIE_EAT,
  SND_ZOMBIE_MAD,
  SND_DIE,
  SND_GAMEOVER,
  NUM_SOUNDS
};

const char * sound_names[NUM_SOUNDS] = {
  /* DATA_PREFIX "sounds/walk.wav" */
  DATA_PREFIX "sounds/makebreak_get.wav",
  DATA_PREFIX "sounds/makebreak_use.wav",
  DATA_PREFIX "sounds/zombie_appear.wav",
  DATA_PREFIX "sounds/zombie_eat.wav",
  DATA_PREFIX "sounds/zombie_mad.wav",
  DATA_PREFIX "sounds/die.wav",
  DATA_PREFIX "sounds/gameover.wav"
};


enum {
  MUS_TITLE,
  MUS_GAME,
  NUM_MUSICS
};

const char * music_names[NUM_MUSICS] = {
  DATA_PREFIX "music/AMAZE.S3M",
  DATA_PREFIX "music/Suggestive_vibes_01.mod"
};


/* Prototypes: */

int title(void);
int game(void);
void setup(int argc, char * argv[]);
void shutdown(void);
void usage(FILE * f);
void seticon(void);
void reset_level(void);
void playsound(int snd);
int mirror_x(int real_x);
Uint32 getpixel(SDL_Surface * surface, int x, int y);
void putpixel(SDL_Surface * surface, int x, int y, Uint32 pixel);
void copy_credits(SDL_Surface * dst, SDL_Surface * src);
void draw_num(int x, int y, int n);
void debug(char * str);
void toggle(int mask[MAP_WIDTH - 2]);
int pause(void);


/* Globals: */

int use_sound, use_fullscreen, use_blend;
SDL_Surface * screen, * images[NUM_IMAGES];
SDL_Surface * black_surf;
#ifndef NOSOUND
Mix_Chunk * sounds[NUM_SOUNDS];
Mix_Music * musics[NUM_MUSICS];
#endif

// for the volume controller
  char	vol_string[5];
  SDL_Surface* vol_surface=NULL;
  TTF_Font* font;
  SDL_Color FontColor = {255, 255, 255};
  int	act_volup=0,
  		act_voldn=0,
  		vol_step=5,
  		min_vol=0,
  		max_vol=MIX_MAX_VOLUME,
  		vol=10;
  		
Uint8 map[MAP_HEIGHT][MAP_WIDTH + 1];
Uint32 player_rgbs[2];
int level, makebreak[2], lives[2];
int map_position;
int pl_x[2], pl_y[2], old_pl_x[2], old_pl_y[2];
int zombie_x, zombie_y, zombie_alive, zombie_dir, zombie_fadein, zombie_mad;
int potion_x, potion_y, potion_alive, potion_xm;
int num_players;


/* --- MAIN! --- */

int main(int argc, char * argv[])
{
  int done;
  
  
  num_players = 1;
 
  setup(argc, argv);
  
  done = 0;
  do
    {
      done = title();
      
      if (!done)
	{
	  done = game();
	}
    }
  while (!done);
  
  shutdown();

  return 0;
}


int title(void)
{
  int done, quit;
  Uint32 last_time;
  SDLKey key;
  SDL_Event event;
  int x, y, which_credit, rand_x;
  Uint8 r, g, b, a, grey, r1, g1, b1, r2, g2, b2, r3, g3, b3;
  SDL_Surface * credits_orig, * many_players_orig;
  SDL_Rect dest;
  int smoking, show_num_countdown;
  


#define SMOKING_START 80
#define SMOKING_SMOKE 20
  
  
  
  credits_orig = SDL_CreateRGBSurface(screen->flags,
				      CREDITS_W, CREDITS_H,
				      screen->format->BitsPerPixel,
				      screen->format->Rmask,
				      screen->format->Gmask,
				      screen->format->Bmask,
				      0 /* screen->format->Amask */);
  
  if (credits_orig == NULL)
    {
      fprintf(stderr, "Can't make new surface!!!\n%s\n", SDL_GetError());

      exit(1);
    }
  
  
  if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
    {
      many_players_orig = SDL_CreateRGBSurface(/* screen->flags, */
					       SDL_SRCALPHA,
					       PLAYERS_W, PLAYERS_H,
					       screen->format->BitsPerPixel,
					       screen->format->Rmask,
					       screen->format->Gmask,
					       screen->format->Bmask,
					       0xFF000000);
    }
  else
    {
      many_players_orig = SDL_CreateRGBSurface(/* screen->flags, */
					       SDL_SRCALPHA,
					       PLAYERS_W, PLAYERS_H,
					       screen->format->BitsPerPixel,
					       screen->format->Rmask,
					       screen->format->Gmask,
					       screen->format->Bmask,
					       0x000000FF);
    }
  
  if (many_players_orig == NULL)
    {
      fprintf(stderr, "Can't make new surface!!!\n%s\n", SDL_GetError());

      exit(1);
    }
					   
  
  

  /* Draw title: */
  
  SDL_BlitSurface(images[IMG_TITLE], NULL, screen, NULL);
  SDL_Flip(screen);
  
  
  which_credit = 0;
  smoking = SMOKING_START;
  
  copy_credits(credits_orig, images[IMG_CREDITS1 + which_credit]);

 
  show_num_countdown = 0;
  
  done = 0;
  quit = 0;
  
  do
    {
      last_time = SDL_GetTicks();
      
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_KEYDOWN)
			{
				key = event.key.keysym.sym;

				if (key == SDLK_ESCAPE ) {
					done = 1;
					quit = 1;
				}
				else if (key == SDLK_RETURN)
					done = 1;
				else if (key == SDLK_LEFT || key == SDLK_RIGHT) {
					num_players = 1;
					show_num_countdown = 10;
					playsound(SND_DIE);
				} 
				else if (key == SDLK_PLUS)
					act_volup=1;
				else if (key == SDLK_MINUS)
					act_voldn=1;
			}
			else if (event.type == SDL_KEYUP)
			{
				key = event.key.keysym.sym;
				
				if (key == SDLK_PLUS)   act_volup=0; else
				if (key == SDLK_MINUS)  act_voldn=0;
			}
			else if (event.type == SDL_QUIT)
			{
				done = 1;
				quit = 1;
			}
		}
      
      
      /* Animate credits: */
      
      /* (Smoke) */

      smoking--;
      
      if (smoking < SMOKING_SMOKE)
	{
	  for (y = 0; y < CREDITS_H - 1; y++)
	    {
	      rand_x = (rand() % 3) - 1;
	      
	      for (x = 1; x < CREDITS_W - 1; x++)
		{
		  SDL_GetRGB(getpixel(credits_orig, x - 1 + rand_x, y + 1),
			     credits_orig->format, &r1, &g1, &b1);

		  SDL_GetRGB(getpixel(credits_orig, x + 0 + rand_x, y + 1),
			     credits_orig->format, &r2, &g2, &b2);

		  SDL_GetRGB(getpixel(credits_orig, x + 1 + rand_x, y + 1),
			     credits_orig->format, &r3, &g3, &b3);
		  
		  grey = (Uint8) ((((Uint32) r1) +
				   ((Uint32) r2) +
				   ((Uint32) r3)) >> 2);
		  
		  
		  putpixel(credits_orig, x, y - (rand() % 2),
			   SDL_MapRGB(credits_orig->format, grey, grey, grey));
		}
	    }

	  if (smoking == 0)
	    {
	      which_credit++;

	      if (which_credit >= NUM_CREDITS)
		which_credit = 0;
	      
	      copy_credits(credits_orig, images[IMG_CREDITS1 + which_credit]);
	      
	      smoking = SMOKING_START;
	    }
	}

      
      /* Draw credits: */
      
      for (y = 0; y < CREDITS_H; y++)
        {
          for (x = 0; x < CREDITS_W; x++)
            {
              SDL_GetRGB(getpixel(credits_orig, x, y),
			 credits_orig->format, &r, &g, &b);

	      grey = r;

	      SDL_GetRGB(getpixel(images[IMG_TITLE],
				  CREDITS_X + x, CREDITS_Y + y),
			 images[IMG_TITLE]->format, &r, &g, &b);

	      if (r + grey > 255)
		r = 255;
	      else
		r = r + grey;

	      if (g + grey > 255)
		g = 255;
	      else
		g = g + grey;

	      if (b + grey > 255)
		b = 255;
	      else
		b = b + grey;
	      
	      
	      putpixel(screen, x + CREDITS_X, y + CREDITS_Y,
		       SDL_MapRGB(screen->format, r, g, b));
            }
        }
      
      
      
      /* Animate number of players: */
      
      for (y = 0; y < PLAYERS_H - 2; y++)
	{
	  for (x = 0 - 1; x < PLAYERS_W - 1; x++)
	    {
	      SDL_GetRGBA(getpixel(many_players_orig, x - 1, y + 1),
			  many_players_orig->format, &r1, &g1, &b1, &a);
	      
	      SDL_GetRGBA(getpixel(many_players_orig, x + 0, y + 1),
			  many_players_orig->format, &r2, &g2, &b2, &a);
	      
	      SDL_GetRGBA(getpixel(many_players_orig, x + 1, y + 1),
			  many_players_orig->format, &r3, &g3, &b3, &a);
	     
	      if (show_num_countdown == 0)
	      {
	        r = (r1 + r2 + r3) / 3;
	        g = (g1 + g2 + g3) >> 2;
	        b = (b1 + b2 + b3) >> 3;
	      }
	      else
	      {
		r = g = b = (r1 + r2 + r3 + g1 + g2 + g3 + b1 + b2 + b3) / 10;
	      }
	      
	      putpixel(many_players_orig, x - 1 + (rand() % 3), y - (rand() % 2),
		       SDL_MapRGBA(many_players_orig->format, r, g, b, 0));
	    }
	}
      
      dest.x = (many_players_orig->w - images[IMG_1]->w) / 2;
      dest.y = many_players_orig->h - images[IMG_1]->h - 1;
     
      if (show_num_countdown == 0)
      {
        SDL_BlitSurface(images[IMG_1 + num_players - 1], NULL,
		        many_players_orig, &dest);
      }
      else
      {
	show_num_countdown--;
      }
      

      /* Draw number of players players: */
      
      dest.x = PLAYERS_X;
      dest.y = PLAYERS_Y;
      dest.w = PLAYERS_W;
      dest.h = PLAYERS_H;
      
      SDL_BlitSurface(images[IMG_TITLE], &dest, screen, &dest);
      
      SDL_BlitSurface(many_players_orig, NULL, screen, &dest);
      
      // Manip Sound Volume ------------
		if(act_volup) {
			vol+=vol_step;
			if(vol>max_vol) vol=max_vol;
		} else
		if(act_voldn) {
			vol-=vol_step;
			if(vol<min_vol) vol=min_vol;
		}
		if(act_volup || act_voldn) {
			Mix_Volume(-1, vol);
			Mix_VolumeMusic(vol);
			if(vol_surface) SDL_FreeSurface(vol_surface);
  			sprintf(vol_string,"%d",vol);
			vol_surface = TTF_RenderText_Blended(font, vol_string, FontColor);
		}
		
	  dest.x = screen->w-20;
	  dest.y = 0;
	  dest.w = 20;
	  dest.h = vol_surface->h;
	  SDL_FillRect(screen, &dest, 0L);
	  SDL_BlitSurface(vol_surface, NULL, screen, &dest);
	
      SDL_Flip(screen);


#ifndef NOSOUND
      /* Keep playing music: */

      if (use_sound)
        {
	  if (!Mix_PlayingMusic())
	    {
	       Mix_PlayMusic(musics[MUS_TITLE], 0);
		   Mix_VolumeMusic(vol);
	    }
        }
#endif


      /* Pause: */
      
      if (SDL_GetTicks() < last_time + FPS)
	SDL_Delay(last_time + FPS - SDL_GetTicks());
    }
  while (!done);


#ifndef NOSOUND
  /* Stop music: */

  if (use_sound)
    Mix_HaltMusic();
#endif


  SDL_FreeSurface(credits_orig);
  SDL_FreeSurface(many_players_orig);
  
  if(TTF_WasInit()!=0) TTF_Quit();
  Mix_Volume(-1, max_vol);
  Mix_VolumeMusic(max_vol);

  return(quit);
}


int game(void)
{
  int done, quit, do_die;
  int x, y, do_makebreak[2], player, old_zombie_x, old_zombie_y, zombie_bump;
  SDL_Rect dest, src;
  Uint32 last_time, now_time;
  SDL_Event event;
  SDLKey key, dirkey[2], last_dirkey[2];

  
  level = 1;
  reset_level();
  debug("Back in game()");
  

  for (player = 0; player < num_players; player++)
  {
    makebreak[player] = STARTING_MAKEBREAK;
    lives[player] = STARTING_LIVES;
    do_makebreak[player] = 0;
    dirkey[player] = SDLK_UNKNOWN;
  }
  
  last_dirkey[0] = SDLK_DOWN;
  last_dirkey[1] = SDLK_DOWN;
  
  done = 0;
  quit = 0;
  do_die = -1;


  debug("Game loop begins:");
  do
    {
      debug("----------------------------------------");

      last_time = SDL_GetTicks();
      
		while (SDL_PollEvent(&event) > 0)
		{
			if (event.type == SDL_KEYDOWN)
			{
				debug("Keypress!");
				key = event.key.keysym.sym;

				if (key == SDLK_ESCAPE)
					done = 1;
				else if (key == SDLK_PLUS)
				{
					if (makebreak[0] > 0) do_makebreak[0] = 1;
				}
				else if (key == SDLK_LEFT || key == SDLK_RIGHT || key == SDLK_UP || key == SDLK_DOWN)
				{
					dirkey[0] = key;
					last_dirkey[0] = key;
				}
				else if (key == SDLK_SPACE)
				{
					done = quit = pause();

					for (player = 0; player < num_players; player++)
					dirkey[player] = SDLK_UNKNOWN;
				}
			}
			else if (event.type == SDL_KEYUP)
			{
				key = event.key.keysym.sym;

				if (key == dirkey[0])
					dirkey[0] = SDLK_UNKNOWN;
				else if (key == dirkey[1])
					dirkey[1] = SDLK_UNKNOWN;
			}
			else if (event.type == SDL_QUIT)
			{
				done = 1;
				quit = 1;
			}
		}
      

      /* Move the map: */

      map_position = map_position + ((level + 1) / 2);
      
      
      if (map_position >= (MAP_HEIGHT - SCREEN_HEIGHT) * SPRITESIZE)
	{
	  level++;
	  
	  reset_level();
	}


      debug("...Moving players");
      /* Move player 1: */
      
      old_pl_x[0] = pl_x[0];
      old_pl_y[0] = pl_y[0];
      
      if (dirkey[0] == SDLK_LEFT)
	pl_x[0] -= (SPRITESIZE_4 << 1);
      else if (dirkey[0] == SDLK_RIGHT)
	pl_x[0] += (SPRITESIZE_4 << 1);
      
      if (dirkey[0] == SDLK_UP)
	pl_y[0] -= SPRITESIZE_4;
      else if (dirkey[0] == SDLK_DOWN)
	pl_y[0] += SPRITESIZE_4;
      
      
      /* Move player 2: */
      
      old_pl_x[1] = pl_x[1];
      old_pl_y[1] = pl_y[1];
      
      if (dirkey[1] == SDLK_KP4)
	pl_x[1] -= (SPRITESIZE_4 << 1);
      else if (dirkey[1] == SDLK_KP6)
	pl_x[1] += (SPRITESIZE_4 << 1);
      
      if (dirkey[1] == SDLK_KP8)
	pl_y[1] -= SPRITESIZE;
      else if (dirkey[1] == SDLK_KP2)
	pl_y[1] += SPRITESIZE;



      /* Keep players from going below the screen: */

      for (player = 0; player < num_players; player++)
        {
	  if (pl_y[player] > map_position + SCREEN_HEIGHT * SPRITESIZE - SPRITESIZE_27)
	    {
	      pl_y[player] = map_position + SCREEN_HEIGHT * SPRITESIZE - SPRITESIZE_27;
	    }
        }


      debug("...Bump");

      /* Bump into things: */
      
      for (player = 0; player < num_players; player++)
        {
	  x = mirror_x(pl_x[player]);

          if (map[(pl_y[player] +  SPRITESIZE_4) / SPRITESIZE][(x +  SPRITESIZE_4) / SPRITESIZE] ||
	      map[(pl_y[player] + SPRITESIZE_27) / SPRITESIZE][(x +  SPRITESIZE_4) / SPRITESIZE] ||
	      map[(pl_y[player] +  SPRITESIZE_4) / SPRITESIZE][(x + SPRITESIZE_27) / SPRITESIZE] ||
	      map[(pl_y[player] + SPRITESIZE_27) / SPRITESIZE][(x + SPRITESIZE_27) / SPRITESIZE])
	    {
	      pl_x[player] = old_pl_x[player];
	      pl_y[player] = old_pl_y[player];
	    }
        }
      
     
      debug("...Make-or-break");

      /* Handle make-or-break: */
      
      for (player = 0; player < num_players; player++)
      {
        if (do_makebreak[player])
	  {
	    y = pl_y[player];


	    x = pl_x[player];
	    
	    if (player == 0)
	      {
	        if (last_dirkey[0] == SDLK_LEFT)
	          x -= MAKEBREAK_DIST;
	        else if (last_dirkey[0] == SDLK_RIGHT)
	          x += MAKEBREAK_DIST;
	  
	        if (last_dirkey[0] == SDLK_UP)
	          y -= MAKEBREAK_DIST;
	        else if (last_dirkey[0] == SDLK_DOWN)
	          y += MAKEBREAK_DIST;
	      }
	    else if (player == 1)
	      {
	        if (last_dirkey[1] == SDLK_KP4)
	          x -= MAKEBREAK_DIST;
	        else if (last_dirkey[1] == SDLK_KP6)
	          x += MAKEBREAK_DIST;
	  
	        if (last_dirkey[1] == SDLK_KP8)
	          y -= MAKEBREAK_DIST;
	        else if (last_dirkey[1] == SDLK_KP2)
	          y += MAKEBREAK_DIST;
	      }
	   
	    x = mirror_x(x);

	  
	    x = (x + SPRITESIZE_15) / SPRITESIZE;
	    y = (y + SPRITESIZE_15) / SPRITESIZE;

	    if (x >= MAP_WIDTH)
	      x = MAP_WIDTH - 1;
	 

	    if (x > 1)
	    {
	      map[y][x] = !map[y][x];
	      makebreak[player]--;
	      playsound(SND_MAKEBREAK_USE);
	    }
	  
	    do_makebreak[player] = 0;
	  }
        }
      
      
      /* Die, if pushed off the top: */
      
      for (player = 0; player < num_players; player++)
        {
          if (pl_y[player] - map_position < (SPRITESIZE >> 1))
	    {
	      do_die = player;
	    }
	}


      debug("...Zombie");
      /* Handle zombie: */

      if (zombie_alive)
        {
	  debug("   IS ALIVE!");

	  /* Move zombie: */
	  
	  old_zombie_x = zombie_x;
	  old_zombie_y = zombie_y;
	  
	  if (zombie_dir == DIR_RIGHT)
	    zombie_x = zombie_x + (level + 1);
	  else if (zombie_dir == DIR_LEFT)
	    zombie_x = zombie_x - (level + 1);
	  else if (zombie_dir == DIR_DOWN)
	    zombie_y = zombie_y + (level + 1);
	  else if (zombie_dir == DIR_UP)
	    zombie_y = zombie_y - (level + 1);
	  
	  
	  /* Keep on the screen! */

	  if (zombie_x < (SPRITESIZE << 1))
	    {
	      zombie_x = (SPRITESIZE << 1);
	      zombie_dir = DIR_RIGHT;
	    }
	  else if (zombie_x > ((MAP_WIDTH * 2) - 3) * SPRITESIZE)
	    {
	      zombie_x = ((MAP_WIDTH * 2) - 3) * SPRITESIZE;
	      zombie_dir = DIR_LEFT;
	    }

	  if (zombie_y > SCREEN_HEIGHT * SPRITESIZE + map_position)
	    {
	      zombie_y = zombie_y - SPRITESIZE_2;
	      zombie_dir = DIR_UP;
	    }


	  zombie_bump = 0;


	  /* Bump into things: */

	  if (!zombie_mad)
	    {
	      x = mirror_x(zombie_x);
	      
	      if (map[(zombie_y +  SPRITESIZE_4) / SPRITESIZE][(x + SPRITESIZE_4) / SPRITESIZE] ||
		  map[(zombie_y + SPRITESIZE_27) / SPRITESIZE][(x + SPRITESIZE_4) / SPRITESIZE] ||
		  map[(zombie_y +  SPRITESIZE_4) / SPRITESIZE][(x + SPRITESIZE_27) / SPRITESIZE] ||
		  map[(zombie_y + SPRITESIZE_27) / SPRITESIZE][(x + SPRITESIZE_27) / SPRITESIZE])
		{
		  zombie_x = old_zombie_x;
		  zombie_y = old_zombie_y;
		  
		  zombie_bump = 1;
		}
	    }
	  

	  /* Change direction: */
	  
	  if ((rand() % 20) == 0 || zombie_bump)
	    {
	      zombie_dir = (rand() % 4);
	    }

	  
	  /* Fall off the top of the screen: */
	  
	  if (zombie_y < map_position)
	    zombie_alive = 0;

	  
	  /* Handle fading in: */
	  
	  if (zombie_fadein > 0)
	    zombie_fadein--;
	  
	  
	  /* Get mad! */
	  
	  if (zombie_fadein == 0 && !zombie_mad &&
	      (rand() % 100) == 0)
	    {
	      zombie_mad = 1;
	      playsound(SND_ZOMBIE_MAD);
	    }
	  
	  
	  /* Kill players: */

	  if (zombie_fadein == 0)
	    {
	      for (player = 0; player < num_players; player++)
		{
		  if (pl_x[player] >= zombie_x - SPRITESIZE_27 &&
		      pl_x[player] <= zombie_x + SPRITESIZE_27 &&
		      pl_y[player] >= zombie_y - SPRITESIZE_27 &&
		      pl_y[player] <= zombie_y + SPRITESIZE_27)
		    {
		      /* DIE */
		      
		      playsound(SND_ZOMBIE_EAT);
		      do_die = player;
		    }
		}
	    }
        }
      else
	{
	  /* Add a new zombie now and then! */
	  
          if ((map_position % 200) > 190 && !zombie_alive)
            {
	      debug("Activating zombie");

              zombie_alive = 1;
	      zombie_fadein = 32;
	      zombie_mad = 0;

	      playsound(SND_ZOMBIE_APPEAR);
	      
	      
	      /* Pick position: */

	      zombie_y = ((map_position / SPRITESIZE) + SCREEN_HEIGHT) * SPRITESIZE;
	      do
		{
		  zombie_x = (rand() % (MAP_WIDTH * 2)) * SPRITESIZE;
		}
	      while (map[zombie_y / SPRITESIZE][mirror_x(zombie_x) / SPRITESIZE]);
	      
	      zombie_dir = DIR_UP;
	    }
	}


      /* Handle potion: */

      if (potion_alive)
        {
	  /* Fall off the top of the screen: */
	  
	  if (potion_y < map_position)
	    potion_alive = 0;

	  
	  /* Give player potion!: */

	  for (player = 0; player < num_players; player++)
	    {
	      if (pl_x[player] >= potion_x - SPRITESIZE_27 &&
	          pl_x[player] <= potion_x + SPRITESIZE_27 &&
	          pl_y[player] >= potion_y - SPRITESIZE_27 &&
	          pl_y[player] <= potion_y + SPRITESIZE_27)
	        {
	          playsound(SND_MAKEBREAK_GET);
	          makebreak[player] = makebreak[player] + 3;
		  
		  potion_alive = 0;
	        }
	    }


	  /* Move potion: */

	  potion_x = potion_x + potion_xm;
	  
	  if (map[potion_y / SPRITESIZE][mirror_x(potion_x -
				          ((SPRITESIZE >> 1) * abs(potion_xm))) / SPRITESIZE])
	  {
	    potion_x = potion_x - potion_xm;
	    potion_xm = -potion_xm;
	  }
        }
      else
      {
        /* Add a make-or-break potion now and then! */

        if (map_position > 200 && ((map_position + 200) % 400) == 199)
        {
	  debug("Activating potion");

	  potion_alive = 1;

	  /* Pick position: */

	  potion_y = ((map_position / SPRITESIZE) + SCREEN_HEIGHT + 1) * SPRITESIZE;
	  do
	    {
	      potion_x = (rand() % (MAP_WIDTH * 2)) * SPRITESIZE;
	    }
	  while (map[potion_y / SPRITESIZE][mirror_x(potion_x) / SPRITESIZE]);

	  potion_xm = level;
	  if (potion_xm > SPRITESIZE_4)
	    potion_xm = SPRITESIZE_4;
	}
      }
      
      
      /* Erase: */
      
      SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
     
      
      /* Draw zombie: */

      if (zombie_alive)
        {
          debug("...Drawing zombie");
      
	  if (zombie_fadein == 0)
	    {
	      dest.x = zombie_x;
	      dest.y = zombie_y - map_position;
	      
	      if (zombie_mad == 0)
		{
		  SDL_BlitSurface(images[IMG_ZOMBIE1 +
					((map_position >> 4) % 2)],
				  NULL,
				  screen,
				  &dest);
		}
	      else
		{
		  SDL_BlitSurface(images[IMG_ZOMBIE_MAD1 +
					((map_position >> 4) % 2)],
				  NULL,
				  screen,
				  &dest);
		}
	    }
	  else
	    {
	      for (y = (map_position % 2); y < SPRITESIZE; y = y + 2)
		{
		  dest.x = zombie_x + ((rand() % 5) - 2);
		  dest.y = zombie_y - map_position + y;
		  
		  src.x = 0;
		  src.y = y;
		  src.w = 32;
		  src.h = 1;
		  
		  SDL_BlitSurface(images[IMG_ZOMBIE1 +
					((map_position >> 4) % 2)],
				  &src,
				  screen,
				  &dest);
		}
	    }
	}
      
      
      /* Draw potion: */
     
      if (potion_alive)
        {
	  debug("...Drawing potion");

	  dest.x = potion_x;
	  dest.y = potion_y - map_position;
	  
	  SDL_BlitSurface(images[IMG_MAKEBREAK1 + ((map_position / 4) % 5)],
			  NULL,
			  screen, &dest);
	}

      
      /* Draw map: */
     
      debug("...Drawing map");

      for (y = 0; y < SCREEN_HEIGHT + 1; y++)
	{
	  for (x = 0; x < MAP_WIDTH; x++)
	    {
	      if (map[y + (map_position / SPRITESIZE)][x])
		{
		  dest.x = x * SPRITESIZE;
		  dest.y = y * SPRITESIZE - (map_position % SPRITESIZE);
		  
		  SDL_BlitSurface(images[(IMG_BRICK1 +
					  ((level - 1) % NUM_BRICKS))], NULL,
				  screen, &dest);


		  dest.x = (MAP_WIDTH * 2 - x - 1) * SPRITESIZE;
		  dest.y = y * SPRITESIZE - (map_position % SPRITESIZE);
		  
		  SDL_BlitSurface(images[(IMG_BRICK1 +
					  ((level - 1) % NUM_BRICKS))], NULL,
				  screen, &dest);
		}
	    }
	}


      debug("...Drawing blend");

      /* Draw the blend: */

      if (use_blend)
      {
        for (y = 0; y < HEIGHT; y = y + 10)
        {
	  dest.x = 0;
	  dest.y = y;
	  dest.w = WIDTH;
	  dest.h = 10;

	  SDL_SetAlpha(black_surf, SDL_SRCALPHA, ((HEIGHT / 2) - (y / 3)));

	  SDL_BlitSurface(black_surf, NULL, screen, &dest);
        }
      }
      
     
      debug("...Erasing left-overs (top)");
      
      /* Erase any left-overs at the top: */
      
      dest.x = 0;
      dest.y = 0;
      dest.w = WIDTH;
      dest.h = (SPRITESIZE >> 1);
      
      SDL_FillRect(screen, &dest, SDL_MapRGB(screen->format, 0, 0, 0));

      
      debug("...Erasing left-overs (bottom)");
      
      /* Erase any left-overs at the bottom: */
      
      dest.x = 0;
      dest.y = (SCREEN_HEIGHT * SPRITESIZE);
      dest.w = WIDTH;
      dest.h = HEIGHT - (SCREEN_HEIGHT * SPRITESIZE);
      
      SDL_FillRect(screen, &dest, SDL_MapRGB(screen->format, 0, 0, 0));

      
      /* Remove dead player's life: */

      if (do_die != -1)
	lives[do_die]--;


      debug("...Drawing lives");
      
      /* Draw lives: */

      for (player = 0; player < num_players; player++)
      {
        for (x = 0; x < lives[player]; x++)
	{
	  dest.x = (x * (SPRITESIZE << 1)) + (SPRITESIZE << 2) - (SPRITESIZE_4);
	  if (player == 1)
	    dest.x = WIDTH - dest.x;
	  
	  dest.y = SPRITESIZE_4;

	  dest.w = (SPRITESIZE >> 2);
	  dest.h = (SPRITESIZE >> 2);

	  SDL_FillRect(screen, &dest, player_rgbs[player]);
	}
      }
      

      debug("...Drawing players");

      /* Draw players: */

      if (do_die == -1)
      {
        for (player = 0; player < num_players; player++)
          {
            dest.x = pl_x[player];
            dest.y = pl_y[player] - map_position;
            dest.w = SPRITESIZE;
            dest.h = SPRITESIZE;
	  
            SDL_BlitSurface(images[IMG_MAN1 + ((map_position / 16) % 2) +
				  (player * 2)], NULL,
		            screen, &dest);
	  }
      }
      
     
      debug("...Erasing left-overs (bottom)");

      /* Erase any left-overs at the bottom: */
      
      dest.x = 0;
      dest.y = SCREEN_HEIGHT * SPRITESIZE;
      dest.w = WIDTH;
      dest.h = HEIGHT - (SCREEN_HEIGHT * SPRITESIZE);
      
      //SDL_FillRect(screen, &dest, SDL_MapRGB(screen->format, 0, 0, 0));


      debug("...Left digits");

      /* Draw player 1's number of make-or-breaks: */

      draw_num(SPRITESIZE_160, SCREEN_HEIGHT * SPRITESIZE + 4, makebreak[0]);

      
      debug("...Right digits");
      
      /* Draw player 2's number of make-or-breaks, or
         player 1's progress, if in one-player mode: */

      if (num_players == 2)
        draw_num(WIDTH - SPRITESIZE_160 - SPRITESIZE, SCREEN_HEIGHT * SPRITESIZE + SPRITESIZE_4, makebreak[1]);
      else
	draw_num(WIDTH - SPRITESIZE_160 - SPRITESIZE, SCREEN_HEIGHT * SPRITESIZE + SPRITESIZE_4,
	         (map_position * 6) / (MAP_HEIGHT * SPRITESIZE) + ((level - 1) * 6));

      /* Flip screen: */
      debug("...Flip!");
      SDL_Flip(screen);


      debug("...Test for player death");

      /* Someone died! */

      if (do_die != -1)
      {
	/* Reset players: */
	
	pl_x[0] = (SPRITESIZE << 1);
	pl_x[1] = WIDTH - (SPRITESIZE << 1) - SPRITESIZE;
	for (player = 0; player < 2; player++)
	{
	  pl_y[player] = (SCREEN_HEIGHT / 2) * SPRITESIZE + map_position;
	}


	/* Turn off zombie: */
	
	zombie_alive = 0;
	zombie_x = 0;
	zombie_y = 0;

	
	/* Turn off potion: */

	potion_alive = 0;
	potion_x = 0;
	potion_y = 0;


	/* Clear part of the map: */

	for (y = (map_position / SPRITESIZE);
	     y <= (map_position / SPRITESIZE) + SCREEN_HEIGHT;
	     y++)
	{
	  for (x = 2; x < MAP_WIDTH; x++)
	  {
	    map[y][x] = 0;
	  }
	}


        /* Die, or game over? */
	
	if (lives[do_die] > 0)
	{
	  playsound(SND_DIE);
	}
	else
	{
	  playsound(SND_GAMEOVER);
	  done = 1;
	}
	
	SDL_Delay(1000);
	
	
	do_die = -1;
      }


#ifndef NOSOUND
      debug("...Play music");

      /* Keep playing music: */

      if (use_sound)
	{
	  if (!Mix_PlayingMusic())
	    {
	      Mix_PlayMusic(musics[MUS_GAME], 0);
		  Mix_VolumeMusic(vol);
	    }
	}
#endif
      
      
      debug("...Wait");
      
      /* Pause: */
      
      now_time = SDL_GetTicks();
      
      if (now_time < last_time + FPS)
	SDL_Delay(last_time + FPS - now_time);
    }
  while (!done);


#ifndef NOSOUND
  /* Stop music: */

  if (use_sound)
    Mix_HaltMusic();
#endif



  return(quit);
}


void setup(int argc, char * argv[])
{
  int i;
  SDL_Surface * image;


  use_sound = 1;
  use_fullscreen = 0;
  use_blend = 1;


  /* Deal with command-line args: */
  
  for (i = 1; i < argc; i++)
  {
    if (strcmp(argv[i], "--nosound") == 0 ||
	strcmp(argv[i], "-s") == 0)
    {
      use_sound = 0;
    }
    else if (strcmp(argv[i], "--noblend") == 0 ||
	strcmp(argv[i], "-b") == 0)
    {
      use_blend = 0;
    }
    else if (strcmp(argv[i], "--fullscreen") == 0 ||
	     strcmp(argv[i], "-f") == 0)
    {
      use_fullscreen = 1;
    }
    else if (strcmp(argv[i], "--usage") == 0 ||
	     strcmp(argv[i], "-u") == 0)
    {
      usage(stdout);
      exit(0);
    }
    else if (strcmp(argv[i], "--help") == 0 ||
	     strcmp(argv[i], "-h") == 0)
    {
      printf("\nEntombed - version " VERSION "\n");
      usage(stdout);
      printf("You and your team of archeologists have fallen into the\n"
	     "\"catacombs of the zombies.\"  There's no time for research,\n"
	     "though; the walls are closing in, and the zombies have sprung\n"
	     "to life!\n"
	     "\n"
	     "Fortunately, you've discovered an ancient mystical potion which\n"
      	     "allows you to create and destroy walls.  Bottles of this potion\n"
	     "are strewn about the catacombs.  Grab them, and you can break\n"
	     "through walls when you get stuck, or create a wall behind you,\n"
	     "if you're being chased.\n"
	     "\n"
	     "The longer you survive, the faster you have to move.\n"
	     "(See README.txt for more)\n"
	     "\n");
      printf("\"Entombed!\" is a free, Open Source game written using libSDL.\n"
	     "This program is free software; you can redistribute it and/or\n"
	     "modify it under the terms of the GNU General Public License.\n"
	     "(See COPYING.txt for more)\n"
	     "\n"
	     "It is based directly on the Atari 2600 game \"Entombed,\"\n"
	     "(c) 1982 U.S. Games Corporation.\n"
	     "\n"
	     "By Bill Kendrick, (c) 2002-2003\n"
	     "bill@newbreedsoftware.com\n"
	     "http://www.newbreedsoftware.com/entombed/\n"
	     "\n"
	     "Music by DEAf and Tim Pratley, (c) 1997\n"
	     "(See AUTHORS.txt for more)\n"
	     "\n");
      exit(0);
    }
    else
    {
      usage(stderr);
      exit(1);
    }
  }


  /* Init SDL Video: */

  if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
      fprintf(stderr,
              "\nError: I could not initialize video!\n"
              "The Simple DirectMedia error that occured was:\n"
              "%s\n\n", SDL_GetError());
      exit(1);
    }


  /* Init SDL Audio: */

  if (use_sound == 1)
    {
      if (SDL_Init(SDL_INIT_AUDIO) < 0)
        {
          fprintf(stderr,
                  "\nWarning: I could not initialize audio!\n"
                  "The Simple DirectMedia error that occured was:\n"
                  "%s\n\n", SDL_GetError());
          use_sound = 0;
        }
    }


  /* Open sound: */

#ifndef NOSOUND
  if (use_sound == 1)
    {
      if (Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 1024) < 0)
//	  if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, AUDIO_S16, MIX_DEFAULT_CHANNELS, 2048) < 0)
        {
          fprintf(stderr,
                  "\nWarning: I could not set up audio for 44100 Hz "
                  "16-bit stereo.\n"
                  "The Simple DirectMedia error that occured was:\n"
                  "%s\n\n", SDL_GetError());
          use_sound = 0;
        }
    }
#endif


  // Load font:
  if(TTF_Init()<0) { printf("ERROR: TTF_Init()\n"); exit(0); }
  font = TTF_OpenFont("data/font/font.ttf", 10);
  if(font == NULL) printf("Could not load data/font/font.ttf\n");
  sprintf(vol_string,"%d",vol);
  vol_surface = TTF_RenderText_Blended(font, vol_string, FontColor);
  Mix_Volume(-1, vol);
  Mix_VolumeMusic(vol);
  
  /* Open display: */

  if (use_fullscreen == 1)
    {
      screen = SDL_SetVideoMode(WIDTH, HEIGHT, 16, SDL_FULLSCREEN);
      if (screen == NULL)
        {
          fprintf(stderr,
                  "\nWarning: I could not set up fullscreen video for "
                  "%dx%d mode.\n"
                  "The Simple DirectMedia error that occured was:\n"
                  "%s\n\n", WIDTH, HEIGHT, SDL_GetError());
          use_fullscreen = 0;
        }
    }

  if (use_fullscreen == 0)
    {
#ifndef EMBEDDED
      screen = SDL_SetVideoMode(WIDTH, HEIGHT, 16, 0);

      if (screen == NULL)
        {
          fprintf(stderr,
                  "\nError: I could not set up video for %dx%d mode.\n"
                  "The Simple DirectMedia error that occured was:\n"
                  "%s\n\n", WIDTH, HEIGHT, SDL_GetError());
          exit(1);
        }
#else
      screen = SDL_SetVideoMode(320, 240, 16, 0);

      if (screen == NULL)
        {
          fprintf(stderr,
                  "\nError: I could not set up video for 320x240 mode.\n"
                  "The Simple DirectMedia error that occured was:\n"
                  "%s\n\n", SDL_GetError());
          exit(1);
        }
#endif
    }

  seticon();


  /* Set window manager stuff: */

  SDL_WM_SetCaption("Entombed!", "Entombed!");
  /* SDL_ShowCursor(0); */


  /* Load graphics: */

  for (i = 0; i < NUM_IMAGES; i++)
    {
      /* Load image file: */

#ifndef EMBEDDED
      image = IMG_Load(image_names[i]);

      if (image == NULL)
        {
          fprintf(stderr,
                  "\nError: I couldn't load a graphics file:\n"
                  "%s\n"
                  "The Simple DirectMedia error that occured was:\n"
                  "%s\n\n", image_names[i], SDL_GetError());
          exit(1);
        }

      images[i] = SDL_DisplayFormatAlpha(image);

      if (images[i] == NULL)
        {
	  fprintf(stderr,
	          "\nError: I couldn't convert a graphics file:\n"
		  "%s\n"
		  "The Simple DirectMedia error that occured was:\n"
		  "%s\n\n", image_names[i], SDL_GetError());
	  exit(1);
	}
#else
      images[i] = SDL_LoadBMP(image_names[i]);

      if (image == NULL)
        {
          fprintf(stderr,
                  "\nError: I couldn't load a graphics file:\n"
                  "%s\n"
                  "The Simple DirectMedia error that occured was:\n"
                  "%s\n\n", image_names[i], SDL_GetError());
          exit(1);
        }
#endif
    }


#ifndef NOSOUND

  if (use_sound == 1)
    {
      /* Load sounds: */
	    
      for (i = 0; i < NUM_SOUNDS; i++)
        {
          sounds[i] = Mix_LoadWAV(sound_names[i]);
          if (sounds[i] == NULL)
            {
              fprintf(stderr,
                      "\nError: I could not load the sound file:\n"
                      "%s\n"
                      "The Simple DirectMedia error that occured was:\n"
                      "%s\n\n", sound_names[i], SDL_GetError());
              exit(1);
            }
        }


      /* Load music: */

      for (i = 0; i < NUM_MUSICS; i++)
        {
          musics[i] = Mix_LoadMUS(music_names[i]);
          if (musics[i] == NULL)
            {
              fprintf(stderr,
                      "\nError: I could not load the sound file:\n"
                      "%s\n"
                      "The Simple DirectMedia error that occured was:\n"
                      "%s\n\n", music_names[i], SDL_GetError());
              exit(1);
            }
        }
    }
  
#endif
  
  
  /* Create black surface: */

  if (use_blend)
  {
    black_surf = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCALPHA,
		                      WIDTH, 10,
				      screen->format->BitsPerPixel,
				      screen->format->Rmask,
				      screen->format->Gmask,
				      screen->format->Bmask,
				      screen->format->Amask);

    if (black_surf == NULL)
    {
      fprintf(stderr,
	      "\nWarning: I couldn't create the blending surface!\n"
	      "The Simple DirectMedia error that occured was:\n"
	      "%s\n\n", SDL_GetError());
      use_blend = 0;
    }
  
    SDL_FillRect(black_surf, NULL, SDL_MapRGB(black_surf->format, 0, 0, 0));
  }
	 
  
  
  /* Set player colors: */

  player_rgbs[0] = SDL_MapRGB(screen->format, 192, 0, 0);
  player_rgbs[1] = SDL_MapRGB(screen->format, 0, 0, 192);


  /* Seed random generator: */

  srand(SDL_GetTicks());
}


void shutdown(void)
{
}


void usage(FILE * f)
{
  fprintf(f, "\n");
  fprintf(f, "Usage: entombed [--help | --usage]\n");
  fprintf(f, "       entombed [--nosound] [--fullscreen] [--noblend]\n");
  fprintf(f, "\n");
}



void seticon(void)
{
#ifndef EMBEDDED
  int masklen;
  Uint8 * mask;
  SDL_Surface * icon;


  /* Load icon into a surface: */

  icon = IMG_Load(DATA_PREFIX "images/icon.png");
  if (icon == NULL)
    {
      fprintf(stderr,
              "\nError: I could not load the icon image: %s\n"
              "The Simple DirectMedia error that occured was:\n"
              "%s\n\n", DATA_PREFIX "images/icon.png", SDL_GetError());
      exit(1);
    }


  /* Create mask: */

  masklen = (((icon -> w) + 7) / 8) * (icon -> h);
  mask = malloc(masklen * sizeof(Uint8));
  memset(mask, 0xFF, masklen);


  /* Set icon: */

//  SDL_WM_SetIcon(icon, mask);


  /* Free icon surface & mask: */

  free(mask);
  SDL_FreeSurface(icon);
#endif
}


void reset_level(void)
{
  int x, y, player;
  int mask[WIDTH - 2];
  
  
  debug("Creating level...");
  
  for (y = 0; y < MAP_HEIGHT; y++)
    {
      for (x = 0; x < 2; x++)
	{
	  map[y][x] = 1;
	}

      for (x = 2; x <= MAP_WIDTH; x++)
	{
	  map[y][x] = 0;
	}
    }
  

  /* Start with a random on-off pattern: */

  mask[0] = rand() % 2;
  for (x = 3; x < MAP_WIDTH; x++)
  {
    mask[x - 2] = !mask[x - 3];
  }

  
  for (y = SCREEN_HEIGHT; y < MAP_HEIGHT; y++)
    {
      toggle(mask);

      for (x = 2; x < MAP_WIDTH; x++)
	{
	  map[y][x] = mask[x - 2];
	}
    }
  

  /* Set scroll position: */
  
  map_position = 0;
 

  /* Set player positions: */
  
  pl_x[0] = (SPRITESIZE << 1);
  pl_x[1] = WIDTH - (SPRITESIZE << 1) - SPRITESIZE;

  for (player = 0; player < 2; player++)
    {
      pl_y[player] = (SCREEN_HEIGHT / 2) * SPRITESIZE;
    }


  /* Disable zombies: */

  zombie_alive = 0;
  zombie_x = 0;
  zombie_y = 0;


  /* Disable potion: */

  potion_alive = 0;
  potion_x = 0;
  potion_y = 0;


  debug("...Level created");
}


void toggle(int mask[MAP_WIDTH - 2])
{
  int i;

  for (i = 0; i < MAP_WIDTH - 2; i++)
  {
    if ((rand() % 10) < 3)
      mask[i] = !mask[i];
  }
}


void playsound(int snd)
{
#ifndef NOSOUND
  if (use_sound)
    {
      Mix_PlayChannel(-1, sounds[snd], 0);
      Mix_Volume(-1, vol);
    }
#endif
}


int mirror_x(int real_x)
{
  int x;
  
  if (real_x >= MAP_WIDTH * SPRITESIZE - SPRITESIZE_27)
    {
      x = MAP_WIDTH * SPRITESIZE - SPRITESIZE - (real_x - MAP_WIDTH * SPRITESIZE);
    }
  else
    {
      x = real_x;
    }

  return(x);
}


/* Get a pixel: */

Uint32 getpixel(SDL_Surface * surface, int x, int y)
{
  int bpp;
  Uint8 * p;
  
  
  /* Determine bytes-per-pixel for the surface in question: */
  
  bpp = surface->format->BytesPerPixel;
  
  
  /* Set a pointer to the exact location in memory of the pixel
   *      in question: */
  
  p = (Uint8 *) (surface->pixels +       /* Start at beginning of RAM */
		 (y * surface->pitch) +  /* Go down Y lines */
		 (x * bpp));             /* Go in X pixels */
  
  
  /* Assuming the X/Y values are within the bounds of this surface... */
  
  if (x >= 0 && y >= 0 && x < surface -> w && y < surface -> h)
    {
      /* Return the correctly-sized piece of data containing the
       *          pixel's value (an 8-bit palette value, or a 16-, 24- or 32-bit
       *                   RGB value) */
      
      if (bpp == 1)         /* 8-bit display */
	return *p;
      else if (bpp == 2)    /* 16-bit display */
	return *(Uint16 *)p;
      else if (bpp == 3)    /* 24-bit display */
	{
	  /* Depending on the byte-order, it could be stored RGB or BGR! */
	  
	  if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
	    return p[0] << 16 | p[1] << 8 | p[2];
	  else
	    return p[0] | p[1] << 8 | p[2] << 16;
	}
      else if (bpp == 4)    /* 32-bit display */
	return *(Uint32 *)p;
      else
	return 0;           /* (Should never occur) */
    }
  else
    return 0;               /* (Out of bounds?  Just return zero) */
}


/* Draw a single pixel into the surface: */

void putpixel(SDL_Surface * surface, int x, int y, Uint32 pixel)
{
  int bpp;
  Uint8 * p;
  
  
  /* Assuming the X/Y values are within the bounds of this surface... */
  
  if (x >= 0 && y >= 0 && x < surface->w && y < surface->h)
    {
      /* Determine bytes-per-pixel for the surface in question: */
      
      bpp = surface->format->BytesPerPixel;
      
      
      /* Set a pointer to the exact location in memory of the pixel
       *          in question: */
      
      p = (Uint8 *) (surface->pixels +       /* Start at beginning of RAM */
							                           (y * surface->pitch) +  /* Go down Y lines */
		     (x * bpp));             /* Go in X pixels */
      
      
      /* Set the (correctly-sized) piece of data in the surface's RAM
       *          to the pixel value sent in: */
      
      if (bpp == 1)
	*p = pixel;
      else if (bpp == 2)
	*(Uint16 *)p = pixel;
      else if (bpp == 3)
	{
	  if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
	    {
	      p[0] = (pixel >> 16) & 0xff;
	      p[1] = (pixel >> 8) & 0xff;
	      p[2] = pixel & 0xff;
	    }
	  else
	    {
	      p[0] = pixel & 0xff;
	      p[1] = (pixel >> 8) & 0xff;
	      p[2] = (pixel >> 16) & 0xff;
	    }
	}
      else if (bpp == 4)
	{
	  *(Uint32 *)p = pixel;
	}
    }
}



void copy_credits(SDL_Surface * dst, SDL_Surface * src)
{
  SDL_Rect dst_rect;
  
  dst_rect.x = 0;
  dst_rect.y = CREDITS_H - src->h - 1;
  
  SDL_FillRect(dst, NULL, SDL_MapRGB(dst->format, 0, 0, 0));
  SDL_BlitSurface(src, NULL, dst, &dst_rect);
}


/* Draw a 2-digit number on the screen: */

void draw_num(int x, int y, int n)
{
  SDL_Rect dest, src;


  if (n > 99)
    n = 99;
  else if (n < 0)
    n =0;


  /* First digit: */

  dest.x = x;
  dest.y = y;
  
  src.x = (images[IMG_NUMBERS]->w / 10) * (n / 10);
  src.y = 0;
  src.w = (images[IMG_NUMBERS]->w / 10);
  src.h = images[IMG_NUMBERS]->h;

  SDL_BlitSurface(images[IMG_NUMBERS], &src, screen, &dest);

  
  /* Second digit: */

  dest.x = x + (images[IMG_NUMBERS]->w / 10);
  dest.y = y;
  
  src.x = (images[IMG_NUMBERS]->w / 10) * (n % 10);
  src.y = 0;
  src.w = (images[IMG_NUMBERS]->w / 10);
  src.h = images[IMG_NUMBERS]->h;

  SDL_BlitSurface(images[IMG_NUMBERS], &src, screen, &dest);
}


void debug(char * str)
{
#ifdef DEBUG
  fflush(stderr);
  fprintf(stderr, "%s\n", str);
  fflush(stderr);
  SDL_Delay(100);
#endif
}


int pause(void)
{
  int done, quit;
  int x, y;
  int smoking;
  Uint8 r, g, b, a,
	r1, g1, b1,
	r2, g2, b2,
	r3, g3, b3;
  SDL_Event event;
  Uint32 now_time, last_time;
  SDLKey key;
  SDL_Surface * paused_orig, * backbuff;
  SDL_Rect src, dest;
  

  backbuff = SDL_CreateRGBSurface(screen->flags,
				      PAUSED_W, PAUSED_H,
				      screen->format->BitsPerPixel,
				      screen->format->Rmask,
				      screen->format->Gmask,
				      screen->format->Bmask,
				      0 /* screen->format->Amask */);
  
  if (backbuff == NULL)
    {
      fprintf(stderr, "Can't make new surface!!!\n%s\n", SDL_GetError());

      exit(1);
    }

  src.x = PAUSED_X;
  src.y = PAUSED_Y;
  src.w = PAUSED_W;
  src.h = PAUSED_H;

  SDL_BlitSurface(screen, &src, backbuff, NULL);

  if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
    {
      paused_orig = SDL_CreateRGBSurface(/* screen->flags, */
					       SDL_SRCALPHA,
					       PAUSED_W, PAUSED_H,
					       screen->format->BitsPerPixel,
					       screen->format->Rmask,
					       screen->format->Gmask,
					       screen->format->Bmask,
					       0xFF000000);
    }
  else
    {
      paused_orig = SDL_CreateRGBSurface(/* screen->flags, */
					       SDL_SRCALPHA,
					       PAUSED_W, PAUSED_H,
					       screen->format->BitsPerPixel,
					       screen->format->Rmask,
					       screen->format->Gmask,
					       screen->format->Bmask,
					       0x000000FF);
    }
  
  if (paused_orig == NULL)
    {
      fprintf(stderr, "Can't make new surface!!!\n%s\n", SDL_GetError());

      exit(1);
    }

  done = 0;
  quit = 0;
  smoking = 0;


  do
  {
    last_time = SDL_GetTicks();
    
    while (SDL_PollEvent(&event))
    {
      if (event.type == SDL_QUIT)
      {
	done = 1;
	quit = 2;
      }
      else if (event.type == SDL_KEYDOWN)
      {
	key = event.key.keysym.sym;
	if (key == SDLK_ESCAPE){
		done = 1;
		quit = 2;}
	
		else if (key == SDLK_SPACE)
	  smoking = 20;
      }
    }
      

    if (smoking > 1)
    {
      smoking--;

      if (smoking == 1)
	done = 1;
    }

    
    /* Animate number of players: */
      
    for (y = 0; y < PAUSED_H - 2; y++)
    {
      for (x = 0 - 1; x < PAUSED_W - 1; x++)
      {
        SDL_GetRGBA(getpixel(paused_orig, x - 1, y + 1),
		    paused_orig->format, &r1, &g1, &b1, &a);
	      
        SDL_GetRGBA(getpixel(paused_orig, x + 0, y + 1),
		    paused_orig->format, &r2, &g2, &b2, &a);
	      
        SDL_GetRGBA(getpixel(paused_orig, x + 1, y + 1),
		    paused_orig->format, &r3, &g3, &b3, &a);
	    
	if (smoking == 0)
	{
	  r = (r1 + r2 + r3) / 3;
	  g = (g1 + g2 + g3) >> 2;
	  b = (b1 + b2 + b3) >> 3;
	}
	else
	{
	  r = g = b = (r1 + r2 + r3 + g1 + g2 + g3 + b1 + b2 + b3) / 10;
	}
	      
	putpixel(paused_orig, x - 1 + (rand() % 3), y - (rand() % 2),
		 SDL_MapRGBA(paused_orig->format, r, g, b, 0));
      }
    }
      
    dest.x = (paused_orig->w - images[IMG_PAUSED]->w) / 2;
    dest.y = paused_orig->h - images[IMG_PAUSED]->h - 1;
    
    if (smoking == 0)
    {
      SDL_BlitSurface(images[IMG_PAUSED], NULL,
		      paused_orig, &dest);
    }
      

    /* Draw number of players players: */
      
    dest.x = PAUSED_X;
    dest.y = PAUSED_Y;
    dest.w = PAUSED_W;
    dest.h = PAUSED_H;
      
    SDL_BlitSurface(backbuff, NULL,
		    screen, &dest);
      
    SDL_BlitSurface(paused_orig, NULL,
		    screen, &dest);
      
    SDL_UpdateRect(screen, PAUSED_X, PAUSED_Y, PAUSED_W, PAUSED_H);
    

    /* Pause: */
      
    now_time = SDL_GetTicks();
      
    if (now_time < last_time + FPS)
      SDL_Delay(last_time + FPS - now_time);
  }
  while (!done);

  return(quit);
}

