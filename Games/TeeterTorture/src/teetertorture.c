/*
  teetertorture.c
  
  Teeter Torture
  A clone of the 1982 Exidy arcade game of the same name.
  
  Slide a cannon left and right on a teeter toter, shooting at 'weight'
  monsters.  Don't let the teeter toter fall too far to either side, or
  TNT will explode.
  
  By Bill Kendrick
  bill@newbreedsoftware.com
  http://www.newbreedsoftware.com/

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  (See COPYING.txt)
  
  August 9, 2005 - October 18, 2005
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#define boolean unsigned char
#define FALSE 0
#define TRUE 1

#define TEXT_CENTERED -1

#define FPS 60
#define MAX_CANNON_SPEED 12
#define CANNON_WEIGHT 50
#define MONSTER_WEIGHT 10

#define MAX_LEVEL 20

#define NUM_EXPLOSION_BITS 64

#define MAX_RECTS 512

#define CANNON_LIMIT (160 - images[IMG_WHEEL1]->w - images[IMG_CANNON]->w / 2)

enum {
  IMG_TITLE,
  IMG_CANNON,
  IMG_BULLET,
  IMG_WHEEL1,
  IMG_WHEEL2,
  IMG_WHEEL3,
  IMG_BADDIE1,
  IMG_PANEL,
  IMG_AXIS,
  IMG_TNT,
  IMG_FUSE,
  IMG_DETONATOR,
  IMG_PLUNGER,
  IMG_LETTERS,
  IMG_LETTERS_YELLOW,
  IMG_NUMBERS,
  IMG_NUMBERS_YELLOW,
  NUM_IMAGES
};

char * img_filenames[NUM_IMAGES] = {
  "title.png",
  "cannon.png",
  "bullet.png",
  "wheel1.png",
  "wheel2.png",
  "wheel3.png",
  "baddie1.png",
  "panel.png",
  "axis.png",
  "tnt.png",
  "fuse.png",
  "detonator.png",
  "plunger.png",
  "letters.png",
  "letters-yellow.png",
  "numbers.png",
  "numbers-yellow.png"
};

enum {
  CMD_GAME,
  CMD_MENU,
  CMD_QUIT
};


Uint8 level_colors[MAX_LEVEL / 2][3] = {
  {0, 0, 0},
  {0, 64, 64},
  {0, 64, 128},
  {89, 67, 67},
  {90, 100, 90},
  {142, 11, 11},
  {85, 84, 6},
  {34, 33, 134},
  {84, 84, 21},
  {126, 22, 27}
};

typedef struct monster_s {
  boolean alive;
  boolean falling;
  int x, y;
  boolean attached;
} monster_t;

typedef struct explosion_s {
  int active;
  int expl_type;
  int x, y;
  int xm, ym;
} explosion_t;

enum {
  EXPL_MONSTER,
  EXPL_PLAYER
};

enum {
  SCREEN_TITLE,
  SCREEN_INSTRUCTIONS,
  SCREEN_HIGH_SCORE,
  NUM_SCREENS
};


/* Local function prototypes: */

boolean setup(int argc, char * argv[]);
void do_shutdown(void);
int title_loop(void);
boolean game_loop(void);
int teeter_height(int x);
void adjust_teeter(int position, int weight);
void init_monsters(boolean complete_reset);
void draw_text(int x, int y, char * str, boolean yellow);
void explode_player(int x);
void add_explosion(int x1, int y1, int x2, int y2, int expl_type);
int huge_explosion(void);
void clear_screen(Uint8 r, Uint8 g, Uint8 b);
void add_rect(SDL_Rect rect);
void draw_left_detonator(boolean draw);
void draw_right_detonator(boolean draw);
void erase_rect(SDL_Rect rect);
void draw_score(boolean draw);
void draw_cannon(boolean draw);
void draw_explosions(boolean draw);
void draw_monsters(boolean draw);
void draw_teetertotter(boolean draw);
void show_title(void);
void show_instructions(void);
void show_high_score(void);

/* Globals: */

int Swidth=320,
	Sheight=240;
SDL_Surface * screen;
SDL_Surface * images[NUM_IMAGES];
boolean use_sound, fullscreen;
int teeter;
monster_t monsters[18];
int player_exploding;
explosion_t explosion[NUM_EXPLOSION_BITS];
SDL_Rect rects[MAX_RECTS];
int num_rects;
Uint8 bkgd_r, bkgd_g, bkgd_b;
int level, score, lives;
int cannon_x;
int wheel_frame;
char high_score_names[5][4];
int high_score_scores[5];



/* --- Main function --- */

int main(int argc, char * argv[])
{
  boolean done;
  int cmd;
  
  if (setup(argc, argv))
    {
      do
	{
	  done = FALSE;
	  cmd = title_loop();
	  
	  if (cmd == CMD_GAME)
	    {
	      done = game_loop();
	    }
	  else if (cmd == CMD_QUIT)
	    {
	      done = TRUE;
	    }
	}
      while (!done);
    }
  
  do_shutdown();
  
  return(0);
}


boolean game_loop(void)
{
  boolean done, quit;
  Uint32 prev_time, cur_time;
  SDL_Event event;
  SDL_Rect dest;
  SDLKey key;
  boolean key_left, key_right;
  boolean want_shot;
  int i, j;
  int sx, sy, w;
  boolean bullet_on;
  int bullet_x, bullet_y;
  int cannon_xm;
  int want_cannon_x;
  boolean follow_want;
  int frame;
  int non_attached_monsters, attached_monsters;
  int cmd;
  
  
  /* Unset key-held flags: */
  
  key_left = FALSE;
  key_right = FALSE;
  
  
  /* Reset game: */
  
  level = 1;
  score = 0;
  lives = 3;
  bullet_on = FALSE;
  bullet_x = 0;
  bullet_y = 0;
  cannon_x = 0;
  cannon_xm = 0; 
  follow_want = FALSE;
  want_cannon_x = 0;
  player_exploding = 0;
  teeter = 0;
  wheel_frame = 0;
  
  init_monsters(TRUE);
  
  for (i = 0; i < NUM_EXPLOSION_BITS; i++)
    explosion[i].active = 0;
  
  
  /* MAIN GAME LOOP! */
  
  done = FALSE;
  quit = FALSE;
  frame = 0;
 

  /* Clear screen */
 
  clear_screen(level_colors[(level - 1) / 2][0],
	       level_colors[(level - 1) / 2][1],
    	       level_colors[(level - 1) / 2][2]);


  do
    {
      frame++;
      prev_time = SDL_GetTicks();
      
      num_rects = 0;


      /* Erase everything: */

      draw_left_detonator(FALSE);
      draw_right_detonator(FALSE);
      draw_score(FALSE);
      
      if (player_exploding == 0)
      {
        if (bullet_on)
        {
          dest.x = 160 + bullet_x - (images[IMG_BULLET]->w / 2);
          dest.y = bullet_y;
          dest.w = images[IMG_BULLET]->w;
          dest.h = images[IMG_BULLET]->h;

	  erase_rect(dest);

          add_rect(dest);
	}

	draw_cannon(FALSE);
      }
      
      draw_explosions(FALSE);
      draw_monsters(FALSE);
      draw_teetertotter(FALSE);

      
      /* Handle pending events: */
      
      want_shot = FALSE;
      
      while (SDL_PollEvent(&event))
	{
	  if (event.type == SDL_QUIT)
	    {
	      /* App Quit! (e.g., window close button) - Quit! */
	      
	      done = TRUE;
	      quit = TRUE;
	    }
	  else if (event.type == SDL_KEYDOWN)
	    {
	      /* Keypress! */
	      
	      key = event.key.keysym.sym;

	      if (key == SDLK_ESCAPE)
		{  /* ESCAPE - Go back to main menu! */
		  done = TRUE;
		}
	      else if (key == SDLK_F4 && (event.key.keysym.mod & KMOD_ALT))
		{  /* ALT+F4 - Quit! */
		  done = TRUE;
		  quit = TRUE;
		}
	      else if (key == SDLK_LEFT || key == SDLK_MINUS)
		{
		  /* Left is being held */
		  
		  key_left = TRUE;
		  key_right = FALSE;
		  follow_want = FALSE;
		}
	      else if (key == SDLK_RIGHT || key == SDLK_PLUS)
		{
		  /* Right is being held */
		  
		  key_right = TRUE;
		  key_left = FALSE;
		  follow_want = FALSE;
		}
	      else if (key == SDLK_SPACE || key == SDLK_RETURN)
		{
		  /* Space - Fire! */
		  
		  want_shot = TRUE;
		}
	    }
	  else if (event.type == SDL_KEYUP)
	    {
	      /* Key release! */
	      
	      key = event.key.keysym.sym;
	      
	      if (key == SDLK_LEFT || key == SDLK_MINUS)
		key_left = FALSE;
	      else if (key == SDLK_RIGHT || key == SDLK_PLUS)
		key_right = FALSE;
	    }
	  else if (event.type == SDL_MOUSEMOTION)
	    {
	      /* Mouse movement! */
	      
	      if (frame > 2)
		{
		  want_cannon_x = event.motion.x - 160;
		  follow_want = TRUE;
		}
	    }
	  else if (event.type == SDL_MOUSEBUTTONDOWN)
	    {
	      /* Mouse click: fire! */
	      
//	      want_shot = TRUE;
	    }
	}
      
      
      if (player_exploding == 0)
	{
	  /* Fire a bullet? */
	  
	  if (want_shot && bullet_on == FALSE)
	    {
	      bullet_on = TRUE;
	      bullet_x = cannon_x;
	      bullet_y = (teeter_height(cannon_x + 160) -
			  (images[IMG_WHEEL1]->h +
			   (images[IMG_CANNON]->h / 2)));
	    }
	  
	  
	  /* Move cannon: */
	  
	  cannon_x = cannon_x + cannon_xm;
	  if (cannon_x < -CANNON_LIMIT)
	    cannon_x = -CANNON_LIMIT;
	  if (cannon_x > CANNON_LIMIT)
	    cannon_x = CANNON_LIMIT;
	  
	  if (key_left)
	    cannon_xm--;
	  else if (key_right)
	    cannon_xm++;
	  else if (follow_want)
	    {
	      if (cannon_x > want_cannon_x)
		{
		  cannon_xm--;
		  
		  if (cannon_x + cannon_xm <= want_cannon_x)
		    {
		      cannon_x = want_cannon_x;
		      cannon_xm = 0;
		      follow_want = FALSE;
		    }
		}
	      else if (cannon_x < want_cannon_x)
		{
		  cannon_xm++;
		  
		  if (cannon_x + cannon_xm >= want_cannon_x)
		    {
		      cannon_x = want_cannon_x;
		      cannon_xm = 0;
		      follow_want = FALSE;
		    }
		}
	    }
	  
	  if (cannon_xm < -MAX_CANNON_SPEED)
	    cannon_xm = -MAX_CANNON_SPEED;
	  else if (cannon_xm > MAX_CANNON_SPEED)
	    cannon_xm = MAX_CANNON_SPEED;
	  
	  if ((frame % 2) == 0)
	    {
	      if (cannon_xm > 0)
		cannon_xm--;
	      else if (cannon_xm < 0)
		cannon_xm++;
	    }
	  
	  
	  /* Adjust position of teeter totter: */
	  
	  if (frame % (((MAX_LEVEL + 1) - level)) == 0)
	    {
	      /* (For cannon) */
	      
	      adjust_teeter(cannon_x, CANNON_WEIGHT);
	      
	      /* (For monsters) */
	      
	      for (i = 0; i < 18; i++)
		{
		  if (monsters[i].alive && monsters[i].attached)
		    adjust_teeter(monsters[i].x - 160 +
				  (images[IMG_BADDIE1]->w / 2),
				  MONSTER_WEIGHT);
		}
	    }
	  
	  
	  /* Blow up TNT if teeter totter goes too far! */
	  
	  if (teeter_height(0) > Sheight - images[IMG_DETONATOR]->h ||
	      teeter_height(319) > Sheight - images[IMG_DETONATOR]->h)
	    {
	      cmd = huge_explosion();
	      
	      if (cmd == CMD_MENU)
		done = TRUE;
	      else if (cmd == CMD_QUIT)
		{
		  done = TRUE;
		  quit = TRUE;
		}
	      
	      bullet_on = FALSE;
	      bullet_x = 0;
	      bullet_y = 0;
	      cannon_x = 0;
	      cannon_xm = 0; 
	      follow_want = FALSE;
	      want_cannon_x = 0;
	      player_exploding = 0;
	      teeter = 0;
	      key_left = FALSE;
	      key_right = FALSE;
	      
	      init_monsters(TRUE);
	    }
	  
	  
	  /* Move bullet: */
	  
	  if (bullet_on)
	    {
	      bullet_y = bullet_y - 12;
	      
	      if (bullet_y <= 0)
		bullet_on = FALSE;
	    }
	  
      
	  /* Make a random monster move now and then: */
	  
	  if (frame % (MAX_LEVEL + 1 - level) == 0)
	    {
	      i = rand() % 18;
	      
	      if (monsters[i].alive == TRUE &&
		  monsters[i].attached == FALSE &&	      
		  monsters[i].falling == FALSE)
		{
		  monsters[i].falling = TRUE;
		}
	    }
	  
	  
	  /* Deal with monsters: */
      
	  non_attached_monsters = 0;
	  attached_monsters = 0;
	  
	  for (i = 0; i < 18; i++)
	    {
	      if (monsters[i].alive == TRUE)
		{
		  if (monsters[i].attached == FALSE)
		    {
		      /* Count the non-attached monsters: */
		      
		      non_attached_monsters++;
		      
		      
		      /* Move the falling ones: */
		      
		      if (monsters[i].falling == TRUE)
			{
			  monsters[i].y = monsters[i].y + ((level / 3) + 1);
			  
			  if (monsters[i].y >= teeter_height(monsters[i].x))
			    {
			      /* Stick to the teeter totter! */
			      
			      monsters[i].falling = FALSE;
			      monsters[i].attached = TRUE;
			    }
			}
		      
		      
		      /* Bullets kill monsters: */
		      
		      sx = bullet_x + 160 - (images[IMG_BULLET]->w / 2);
		      
		      if (bullet_on &&
			  sx + images[IMG_BULLET]->w >= monsters[i].x &&
			  sx <= monsters[i].x + images[IMG_BADDIE1]->w &&
			  bullet_y + images[IMG_BULLET]->h >= monsters[i].y &&
			  bullet_y <= monsters[i].y + images[IMG_BADDIE1]->h)
			{
			  bullet_on = FALSE;
			  monsters[i].alive = FALSE;
			  
			  for (j = 0; j < 10; j++)
			    {
			      add_explosion(monsters[i].x,
					    monsters[i].y,
					    (monsters[i].x +
					     images[IMG_BADDIE1]->w),
					    (monsters[i].y +
					     images[IMG_BADDIE1]->h),
					    EXPL_MONSTER);
			    }
			  
			  score = score + level;
			}
		      
		      
		      /* Monsters kill player: */
		      
		      sx = cannon_x + 160 - (images[IMG_CANNON]->w / 2);
		      sy = teeter_height(sx) - images[IMG_WHEEL1]->h;
		      
		      sx = sx - images[IMG_WHEEL1]->w;
		      w = (images[IMG_WHEEL1]->w * 2) + images[IMG_CANNON]->w;
			    
		      if (sx + w >= monsters[i].x &&
			  sx <= monsters[i].x + images[IMG_BADDIE1]->w &&
			  sy + images[IMG_WHEEL1]->h >= monsters[i].y &&
			  sy <= monsters[i].y + images[IMG_BADDIE1]->h)
			{
			  explode_player(cannon_x);
			}
		    }
		  else
		    {
		      attached_monsters++;
		    }
		}
	    }
	  
	  
	  /* Advance to next level: */
	  
	  if (non_attached_monsters == 0 || attached_monsters == 18)
	    {
	      if (level < MAX_LEVEL)
		level++;
  
	      clear_screen(level_colors[(level - 1) / 2][0],
	      		   level_colors[(level - 1) / 2][1],
    	       		   level_colors[(level - 1) / 2][2]);

	      init_monsters((attached_monsters == 18));
	    }
	}
      
      
      /* Handle player explosion: */
      
      if (player_exploding)
	{
	  player_exploding--;
	  
	  if (player_exploding == 0)
	    init_monsters(FALSE);
	}
      
      
      /* Handle explosion bits: */
      
      for (i = 0; i < NUM_EXPLOSION_BITS; i++)
	{
	  if (explosion[i].active)
	    {
	      explosion[i].active--;
	      
	      explosion[i].x += explosion[i].xm;
	      explosion[i].y += explosion[i].ym;
	      
	      if ((frame % 3) == 0)
		explosion[i].ym++;
	      
	      if (explosion[i].y >= teeter_height(explosion[i].x))
		{
		  explosion[i].y = teeter_height(explosion[i].x);
		  explosion[i].ym = -explosion[i].ym;
		}
	    }
	}
   

      /* Score and level: */
     
      draw_score(TRUE);

      
      /* Draw everything: */
     
      draw_left_detonator(TRUE);
      draw_right_detonator(TRUE);
      
      
      if (player_exploding == 0)
	{
	  /* Draw bullet: */
	  
	  if (bullet_on)
	    {
	      dest.x = 160 + bullet_x - (images[IMG_BULLET]->w / 2);
	      dest.y = bullet_y;
	      dest.w = images[IMG_BULLET]->w;
	      dest.h = images[IMG_BULLET]->h;
	      
	      SDL_BlitSurface(images[IMG_BULLET], NULL, screen, &dest);

	      add_rect(dest);
	    }
	  
	 
	  draw_cannon(TRUE);
	}

      
      /* Explosion bits: */
     
      draw_explosions(TRUE);
      
      
      /* Teeter totter: */
     
      draw_teetertotter(TRUE);
      
      
      /* Monsters: */
    
      draw_monsters(TRUE);
      
      
      /* Show it all! */
      
      SDL_UpdateRects(screen, num_rects, rects);
      
      
      /* Throttle FPS: */
      
      cur_time = SDL_GetTicks();
      
      if (cur_time < prev_time + (1000 / FPS))
	SDL_Delay((prev_time + (1000 / FPS)) - cur_time);
    }
  while (!done);
  
  return(quit);
}


int title_loop(void)
{
  SDL_Event event;
  SDLKey key;
  int cmd, done;
  Uint32 prev_time, cur_time;
  int ticks, info_screen;

  done = FALSE;
  cmd = -1;
  ticks = 0;
  info_screen = SCREEN_TITLE;

  show_title();

  do
  {
    prev_time = SDL_GetTicks();

    while (SDL_PollEvent(&event))
    {
      if (event.type == SDL_QUIT)
      {
	/* App quit event - Quit! */

	done = TRUE;
	cmd = CMD_QUIT;
      }
      else if (event.type == SDL_KEYDOWN)
      {
	key = event.key.keysym.sym;

	if (key == SDLK_ESCAPE ||
	    (key == SDLK_F4 && (event.key.keysym.mod & KMOD_ALT)))
	{
	  /* ESCAPE or ALT+F4 - Quit! */
	  
	  done = TRUE;
	  cmd = CMD_QUIT;
	}
	else if (key == SDLK_SPACE || key == SDLK_RETURN)
	{
	  /* SPACE or ENTER - Start the game! */
	
	  done = TRUE;
	  cmd = CMD_GAME;
	}
	else if (key == SDLK_RIGHT)
	{
	  ticks = (FPS * 5) - 1;
	}
	else if (key == SDLK_LEFT)
	{
	  info_screen = info_screen - 2;
	  if (info_screen < 0);
	    info_screen = info_screen + NUM_SCREENS;

	  ticks = (FPS * 5) - 1;
	}
      }
      else if (event.type == SDL_MOUSEBUTTONDOWN) { done = TRUE; cmd = CMD_GAME; }
    }


    /* Every once in a while, show a different info/attract screen: */

    ticks++;

    if (ticks == FPS * 5)
    {
      /* Draw the next screen: */

      ticks = 0;

      info_screen = (info_screen + 1) % NUM_SCREENS;

      if (info_screen == SCREEN_TITLE)
	show_title();
      else if (info_screen == SCREEN_INSTRUCTIONS)
	show_instructions();
      else if (info_screen == SCREEN_HIGH_SCORE)
	show_high_score();
    }


    /* Throttle FPS: */
      
    cur_time = SDL_GetTicks();
      
    if (cur_time < prev_time + (1000 / FPS))
      SDL_Delay((prev_time + (1000 / FPS)) - cur_time);
  }
  while (!done);
  
  return(cmd);
}


void show_title(void)
{
  int ch;
  SDL_Rect rect;

  ch = images[IMG_LETTERS]->h;

  SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF));

  rect.x = (screen->w - images[IMG_TITLE]->w) / 2;
  rect.y = 0;
  SDL_BlitSurface(images[IMG_TITLE], NULL, screen, &rect);
  
  draw_text(TEXT_CENTERED, screen->h - (ch * 16), "BASED ON THE 1982", FALSE);
  draw_text(TEXT_CENTERED, screen->h - (ch * 14), "PROTOTYPE BY EXIDY", FALSE);
  
  draw_text(TEXT_CENTERED, screen->h - (ch * 11), "REIMPLEMENTED", FALSE);
  draw_text(TEXT_CENTERED, screen->h - (ch * 9), "BY BILL KENDRICK", FALSE);
  draw_text(TEXT_CENTERED, screen->h - (ch * 7), "COPYRIGHT 2005", FALSE);
  draw_text(TEXT_CENTERED, screen->h - (ch * 5), "NEW BREED SOFTWARE", FALSE);
  draw_text(TEXT_CENTERED, screen->h - (ch * 2), "EZX PORT BY BRUCELEE", FALSE);

  SDL_Flip(screen);
}


void show_instructions(void)
{
  int ch;
  
  SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));

  ch = images[IMG_LETTERS]->h;

  draw_text(TEXT_CENTERED, ch *  4, "TEETER TORTURE", FALSE);
  draw_text(TEXT_CENTERED, ch *  5, "INSTRUCTIONS", FALSE);

  draw_text(TEXT_CENTERED, ch *  8, "BALANCE THE TEETER TOTTER", TRUE);
  draw_text(TEXT_CENTERED, ch * 10, "BY MOVING THE CANNON", TRUE);
  draw_text(TEXT_CENTERED, ch * 12, "OR BY CATCHING THE WEIGHTS", TRUE);

  draw_text(TEXT_CENTERED, ch * 16, "BONUS FOR WEIGHTS CAUGHT", TRUE);
  draw_text(TEXT_CENTERED, ch * 20, "WEIGHTS DESTROY CANNON", TRUE);
  draw_text(TEXT_CENTERED, ch * 24, "CANNON BALLS", TRUE);
  draw_text(TEXT_CENTERED, ch * 26, "DESTROY WEIGHTS", TRUE);
  
  SDL_Flip(screen);
}

char * score_labels[5] = {
  "INCREDIBLE",
  "FABULOUS",
  "GREAT",
  "SUPER",
  "GOOD"
};


void show_high_score(void)
{
  int cw, ch;
  int i, y;
  char str[10];
  
  SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));

  cw = images[IMG_LETTERS]->w / 26;
  ch = images[IMG_LETTERS]->h;

  draw_text(TEXT_CENTERED, ch * 1, "TEETER TORTURE", FALSE);
  draw_text(TEXT_CENTERED, ch * 2, "HALL OF FAME", FALSE);

  for (i = 0; i < 5; i++)
  {
    y = (((ch * i) * 150) / 100) + (ch * 4);

    draw_text(cw * 4, y, high_score_names[i], TRUE);

    snprintf(str, sizeof(str), "%5d", high_score_scores[i]);
    draw_text(cw * 8, y, str, TRUE);

    draw_text(cw * 14, y, score_labels[i], TRUE);
  }

  draw_text(TEXT_CENTERED, ch * 16, "JOIN THE TEETER TORTURE", TRUE);
  draw_text(TEXT_CENTERED, ch * 18, "HALL OF FAME", TRUE);
  draw_text(TEXT_CENTERED, ch * 20, "BY ENTERING YOUR INITIALS", TRUE);
  draw_text(TEXT_CENTERED, ch * 22, "FOR A HIGH SCORE", TRUE);
  
  SDL_Flip(screen);
}


boolean setup(int argc, char * argv[])
{
  int i;
  char filename[FILENAME_MAX];
  char * home;
  char str[80];
  SDL_Surface * tmp_surf;
  FILE * fi;

  use_sound = TRUE;
  fullscreen = FALSE;
  
  for (i = 0; i < NUM_IMAGES; i++)
    images[i] = NULL;
  
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
      fprintf(stderr, "Error: Couldn't initialize video:\n%s\n",
	      SDL_GetError());
      return(FALSE);
    }
  
  if (use_sound)
    {
      if (SDL_Init(SDL_INIT_AUDIO) < 0)
	{
	  fprintf(stderr, "Warning: Couldn't initialize audio:\n%s\n",
		  SDL_GetError());
	  use_sound = FALSE;
	}
    }
  
  if (use_sound)
    {
      if (Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 1024) < 0)
	{
	  fprintf(stderr, "Warning: Could not set up 44100 Hz audio:\n%s\n",
		  SDL_GetError());
	  use_sound = FALSE;
	}
    }
  
  SDL_WM_SetCaption("Teeter Torture", "Teeter Torture");
  
  if (fullscreen)
    {
      screen = SDL_SetVideoMode(Swidth, Sheight, 16,
				SDL_FULLSCREEN | SDL_SWSURFACE |
				SDL_ANYFORMAT);
      
      if (screen == NULL)
	{
	  fprintf(stderr, "Warning: Could not switch to full screen:\n%s\n",
		  SDL_GetError());
	  fullscreen = FALSE;
	}
    }

  if (fullscreen == FALSE)
    {
      screen = SDL_SetVideoMode(Swidth, Sheight, 16, SDL_SWSURFACE | SDL_ANYFORMAT);
      
      if (screen == NULL)
	{
	  fprintf(stderr, "Warning: Could not open display:\n%s\n",
		  SDL_GetError());
	  return(FALSE);
	}
    }
  
  for (i = 0; i < NUM_IMAGES; i++)
    {
      snprintf(filename, sizeof(filename), "data/images/%s", img_filenames[i]);
      tmp_surf = IMG_Load(filename);
      
      if (tmp_surf == NULL)
	{
	  fprintf(stderr, "Error: Couldn't load image %s:\n%s\n",
		  filename, SDL_GetError());
	  return(FALSE);
	}
      
      images[i] = SDL_DisplayFormatAlpha(tmp_surf);
      SDL_FreeSurface(tmp_surf);
      
      if (images[i] == NULL)
	{
	  fprintf(stderr, "Error: Couldn't convert image %s:\n%s\n",
		  filename, SDL_GetError());
	  return(FALSE);
	}
    }


  /* Set default values: */

  strcpy(high_score_names[0], "SPT");
  high_score_scores[0] = 53940;

  strcpy(high_score_names[1], "LWH");
  high_score_scores[1] = 49720;

  strcpy(high_score_names[2], "DJS");
  high_score_scores[2] = 47340;

  strcpy(high_score_names[3], "VIC");
  high_score_scores[3] = 44130;

  strcpy(high_score_names[4], "MRK");
  high_score_scores[4] = 37840;


  /* Try to load prefs and data: */

  home = getenv("HOME");
  if (home != NULL)
  {
    snprintf(filename, sizeof(filename), "%s/.teetertorturerc", home);
    fi = fopen(filename, "r");
    if (fi != NULL)
    {
      do
      {
	fgets(str, sizeof(str), fi);
	if (!feof(fi))
	{
	  if (strstr(str, "name") == str &&
	      str[4] >= '1' && str[4] <= '5' &&
	      str[5] == '=' && strlen(str) >= 9)
	  {
	    high_score_names[str[4] - '1'][0] = str[6];
	    high_score_names[str[4] - '1'][1] = str[7];
	    high_score_names[str[4] - '1'][2] = str[8];
	    high_score_names[str[4] - '1'][3] = '\0';
	  }

	  if (strstr(str, "high") == str &&
	      str[4] >= '1' && str[4] <= '5' &&
	      str[5] == '=')
	  {
	    high_score_scores[str[4] - '1'] = atoi(str + 6);
	  }
	}
      }
      while (!feof(fi));
      
      fclose(fi);
    }
    free(home);
  }

  
  return(TRUE);
}


void do_shutdown(void)
{
  int i;
  FILE * fi;
  char * home;
  char filename[FILENAME_MAX];

  home = getenv("HOME");
  if (home != NULL)
  {
    snprintf(filename, sizeof(filename), "%s/.teetertorturerc", home);
    fi = fopen(filename, "w");
    if (fi != NULL)
    {
      for (i = 0; i < 5; i++)
      {
	fprintf(fi, "name%d=%s\n", i + 1, high_score_names[i]);
	fprintf(fi, "high%d=%d\n", i + 1, high_score_scores[i]);
      }
      fclose(fi);
    }
    free(home);
  }

  
  for (i = 0; i < NUM_IMAGES; i++)
    {
      if (images[i] != NULL)
	{
	  SDL_FreeSurface(images[i]);
	  images[i] = NULL;
	}
    }
  
  SDL_Quit();
}


int teeter_height(int x)
{
  int y;
  
  y = ((x - 160) * teeter) / (images[IMG_PANEL]->w) / 80;
  
  return(y + (Sheight - images[IMG_TNT]->h -
	      images[IMG_AXIS]->h - images[IMG_PANEL]->h));
}


void adjust_teeter(int position, int weight)
{
  teeter = teeter + ((position * weight) / 160);
}


void init_monsters(boolean complete_reset)
{
  int i;

  for (i = 0; i < 18; i++)
    {
      monsters[i].alive = TRUE;
      monsters[i].falling = FALSE;
      monsters[i].y = images[IMG_LETTERS]->h;
      
      if (complete_reset == TRUE)
	{
	  monsters[i].attached = FALSE;
	}
      
      if (i < 9)
	monsters[i].x = 54 + i * 24;
      else
	monsters[i].x = Swidth - 54 - (i - 8) * 24;      
    }
}


void draw_text(int x, int y, char * str, boolean yellow)
{
  int i;
  int cw;
  SDL_Rect src, dest;
  
  cw = images[IMG_LETTERS]->w / 26;

  if (x == TEXT_CENTERED)
  {
    x = (screen->w - (cw * strlen(str))) / 2;
  }
  
  for (i = 0; i < strlen(str); i++)
    {
      dest.x = x;
      dest.y = y;

      if (str[i] >= 'A' && str[i] <= 'Z')
	{
	  src.x = (str[i] - 'A') * cw;
	  src.y = 0;
	  src.w = cw;
	  src.h = images[IMG_LETTERS]->h;

	  if (yellow)
	    SDL_BlitSurface (images[IMG_LETTERS_YELLOW], &src, screen, &dest);
	  else
	    SDL_BlitSurface (images[IMG_LETTERS], &src, screen, &dest);
	}
      else if (str[i] >= '0' && str[i] <= '9')
	{
	  src.x = (str[i] - '0') * cw;
	  src.y = 0;
	  src.w = cw;
	  src.h = images[IMG_NUMBERS]->h;

	  if (yellow)
	    SDL_BlitSurface (images[IMG_NUMBERS_YELLOW], &src, screen, &dest);
	  else
	    SDL_BlitSurface (images[IMG_NUMBERS], &src, screen, &dest);
	}
      
      x = x + cw;
    }
}


void explode_player(int x)
{
  int i;
  int left_x, top_y, w, h;
  
  left_x = (x + 160 - (images[IMG_CANNON]->w / 2) - images[IMG_WHEEL1]->w);
  top_y = teeter_height(x + 160);
  
  w = images[IMG_CANNON]->w + (images[IMG_WHEEL1]->w * 2);
  h = images[IMG_WHEEL1]->h + (images[IMG_CANNON]->h / 2);
  
  for (i = 0; i < NUM_EXPLOSION_BITS; i++)
    {
      /* (Force at least half of them to come on, even if they're
	 being used for monsters!) */
      
      if ((i % 2) == 0)
	explosion[i].active = 0;
      
      add_explosion(left_x, top_y,
		    left_x + w, top_y + h,
		    EXPL_PLAYER);
    }
  
  player_exploding = 128;
}


void add_explosion(int x1, int y1, int x2, int y2, int expl_type)
{
  int i, found;
  
  found = -1;
  for (i = 0; i < NUM_EXPLOSION_BITS && found == -1; i++)
    {
      if (explosion[i].active == 0)
	found = i;
    }
  
  if (found != -1)
    {
      if (expl_type == EXPL_MONSTER)
	explosion[found].active = 32;
      else if (expl_type == EXPL_PLAYER)
	explosion[found].active = 128;

      explosion[found].expl_type = expl_type;
      
      explosion[found].x = x1 + (rand() % (x2 - x1));
      explosion[found].y = y1 + (rand() % (y2 - y1));
      
      explosion[found].xm = (rand() % 9) - 4;
      explosion[found].ym = (rand() % 15) - 10;
    }
}


int huge_explosion(void)
{
  int i, cmd=CMD_GAME, flash_wait=100, count_wait=1000, ch=images[IMG_LETTERS]->h;
  Uint32 prev_time, cur_time;
  SDL_Event event;
  SDLKey key;
  
  prev_time = SDL_GetTicks(); SDL_Flip(screen);
  
  while (SDL_PollEvent(&event))
  {
    if (event.type == SDL_QUIT) cmd = CMD_QUIT;
    else if (event.type == SDL_KEYDOWN)
    {
      key = event.key.keysym.sym;
           if (key == SDLK_ESCAPE) cmd = CMD_MENU;
      else if (key == SDLK_F4 && (event.key.keysym.mod & KMOD_ALT)) cmd = CMD_QUIT;
    }
  }
  
  for(i=0;i<4;i++) {
  	SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 255, 255, 64));	SDL_Flip(screen); SDL_Delay(flash_wait);
    SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));		SDL_Flip(screen); SDL_Delay(flash_wait);
  }
  

  SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
  draw_text(TEXT_CENTERED, screen->h/2 - ch/2, " 3 ", TRUE); SDL_Flip(screen); SDL_Delay(count_wait);
  SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
  draw_text(TEXT_CENTERED, screen->h/2 - ch/2, " 2 ", TRUE); SDL_Flip(screen); SDL_Delay(count_wait);
  SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
  draw_text(TEXT_CENTERED, screen->h/2 - ch/2, " 1 ", TRUE); SDL_Flip(screen); SDL_Delay(count_wait);
  clear_screen(level_colors[(level - 1) / 2][0], level_colors[(level - 1) / 2][1], level_colors[(level - 1) / 2][2]);
  
  while (SDL_PollEvent(&event))
  {
    if (event.type == SDL_QUIT) cmd = CMD_QUIT;
    else if (event.type == SDL_KEYDOWN)
    {
      key = event.key.keysym.sym;
           if (key == SDLK_ESCAPE) cmd = CMD_MENU;
      else if (key == SDLK_F4 && (event.key.keysym.mod & KMOD_ALT)) cmd = CMD_QUIT;
    }
  }
  /* Throttle FPS: */
  cur_time = SDL_GetTicks();
  if (cur_time < prev_time + (1000 / FPS))
  SDL_Delay((prev_time + (1000 / FPS)) - cur_time);

  return(cmd);
}


void clear_screen(Uint8 r, Uint8 g, Uint8 b)
{
  SDL_Rect dest;


  SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, r, g, b));

  bkgd_r = r;
  bkgd_g = g;
  bkgd_b = b;
  

  /* Fuse: */
  
  dest.x = 160 - (images[IMG_FUSE]->w / 2);
  dest.y = Sheight - images[IMG_FUSE]->h;
  
  SDL_BlitSurface(images[IMG_FUSE], NULL, screen, &dest);
  
  
  /* TNT: */
  
  dest.x = 160 - (images[IMG_TNT]->w / 2);
  dest.y = Sheight - (images[IMG_TNT]->h);
  
  SDL_BlitSurface(images[IMG_TNT], NULL, screen, &dest);
  
  
  /* Axis */
  
  dest.x = 160 - (images[IMG_AXIS]->w / 2);
  dest.y = Sheight - images[IMG_TNT]->h - images[IMG_AXIS]->h;
  
  SDL_BlitSurface(images[IMG_AXIS], NULL, screen, &dest);


  /* Flip screen and init rects: */

  SDL_Flip(screen);
  num_rects = 0;
}

void add_rect(SDL_Rect rect)
{
  if (num_rects < MAX_RECTS &&
      rect.w > 0 && rect.h > 0 &&
      rect.x < screen->w && rect.y < screen->h)
  {
    if (rect.x < 0)
    {
      rect.w = rect.w + rect.x;
      rect.x = 0;
    }

    if (rect.y < 0)
    {
      rect.h = rect.h + rect.y;
      rect.y = 0;
    }
    if (rect.x + rect.w >= screen->w)
      rect.w = screen->w - rect.x;

    if (rect.y + rect.h >= screen->h)
      rect.h = screen->h - rect.y;

    if (rect.w > 0 && rect.h > 0)
    {
      memcpy(&(rects[num_rects]), &rect, sizeof(SDL_Rect));
      num_rects++;
    }
  }
}


void draw_left_detonator(boolean draw)
{
  int min_y;
  int sx, sy;
  SDL_Rect dest;

  /* Plunger: */

  min_y = (Sheight - images[IMG_DETONATOR]->h - images[IMG_PLUNGER]->h);

  sx = (images[IMG_DETONATOR]->w / 2) - (images[IMG_PLUNGER]->w / 2);
  sy = teeter_height(sx) + images[IMG_PANEL]->h;
  if (sy < min_y)
    sy = min_y;
  
  dest.x = sx;
  dest.y = sy;
  dest.w = images[IMG_PLUNGER]->w;
  dest.h = images[IMG_PLUNGER]->h;
 
  if (draw)
    SDL_BlitSurface(images[IMG_PLUNGER], NULL, screen, &dest);
  else
    erase_rect(dest);

  add_rect(dest);

  
  /* Detonator: */

  if (draw)
  {
    dest.x = 0;
    dest.y = Sheight - (images[IMG_DETONATOR]->h);
    dest.w = images[IMG_DETONATOR]->w;
    dest.h = images[IMG_DETONATOR]->h;
  
    SDL_BlitSurface(images[IMG_DETONATOR], NULL, screen, &dest);

    add_rect(dest);
  }
}


void draw_right_detonator(boolean draw)
{
  int min_y;
  int sx, sy;
  SDL_Rect dest;

  /* Plunger: */

  min_y = (Sheight - images[IMG_DETONATOR]->h - images[IMG_PLUNGER]->h);

  sx = Swidth - (images[IMG_DETONATOR]->w / 2) - (images[IMG_PLUNGER]->w / 2);
  sy = teeter_height(sx) + images[IMG_PANEL]->h;
  if (sy < min_y)
    sy = min_y;
  
  dest.x = sx;
  dest.y = sy;
  dest.w = images[IMG_PLUNGER]->w;
  dest.h = images[IMG_PLUNGER]->h;
  
  if (draw)
    SDL_BlitSurface(images[IMG_PLUNGER], NULL, screen, &dest);
  else
    erase_rect(dest);

  add_rect(dest);

  
  /* Right detonator: */

  if (draw)
  {
    dest.x = Swidth - (images[IMG_DETONATOR]->w);
    dest.y = Sheight - (images[IMG_DETONATOR]->h);
    dest.w = images[IMG_DETONATOR]->w;
    dest.h = images[IMG_DETONATOR]->h;
  
    SDL_BlitSurface(images[IMG_DETONATOR], NULL, screen, &dest);

    add_rect(dest);
  }
}



void erase_rect(SDL_Rect rect)
{
  SDL_FillRect(screen, &rect,
	       SDL_MapRGB(screen->format, bkgd_r, bkgd_g, bkgd_b));
}


void draw_score(boolean draw)
{
  SDL_Rect dest;
  char str[32];

  sprintf(str, "SCORE %05d  LEVEL %2d", score, level);

  dest.x = 0;
  dest.y = 0;
  dest.w = (((images[IMG_LETTERS]->w) / 26) * strlen(str)) + 2;
  dest.h = images[IMG_LETTERS]->h + 2;

  if (draw)
  {
    draw_text(dest.x + 1, dest.y + 1, str, TRUE);
    draw_text(dest.x, dest.y, str, FALSE);
  }
  else
    erase_rect(dest);

  add_rect(dest);
}


void draw_cannon(boolean draw)
{
  int sx;
  SDL_Rect dest;

  /* Draw cannon: */
  
  sx = 160 + cannon_x - (images[IMG_CANNON]->w / 2);
  
  dest.x = sx;
  dest.y = teeter_height(sx) - (images[IMG_WHEEL1]->h +
				(images[IMG_CANNON]->h / 2));
  dest.w = images[IMG_CANNON]->w;
  dest.h = images[IMG_CANNON]->h;
 
  if (draw)
    SDL_BlitSurface(images[IMG_CANNON], NULL, screen, &dest);
  else
    erase_rect(dest);

  add_rect(dest);
  
  
  /* Wheels! */
  
  wheel_frame = ((cannon_x + 160) / 4) % 3;
  
  
  /* Left wheel: */
  
  dest.x = sx - (images[IMG_WHEEL1]->w);
  dest.y = teeter_height(dest.x) - (images[IMG_WHEEL1]->h);
  dest.w = images[IMG_WHEEL1]->w;
  dest.h = images[IMG_WHEEL2]->h;
 
  if (draw)
  {
    SDL_BlitSurface(images[IMG_WHEEL1 + (wheel_frame % 3)], NULL,
		    screen, &dest);
  }
  else
    erase_rect(dest);
  
  add_rect(dest);
 

  /* Right wheel: */
  
  dest.x = sx + (images[IMG_CANNON]->w);
  dest.y = teeter_height(dest.x) - (images[IMG_WHEEL1]->h);
  dest.w = images[IMG_WHEEL1]->w;
  dest.h = images[IMG_WHEEL2]->h;
  
  if (draw)
  {
    SDL_BlitSurface(images[IMG_WHEEL1 + ((wheel_frame + 1) % 3)], NULL,
		    screen, &dest);
  }
  else
    erase_rect(dest);

  add_rect(dest);
}


void draw_explosions(boolean draw)
{
  int i;
  SDL_Rect src, dest;

  for (i = 0; i < NUM_EXPLOSION_BITS; i++)
  {
    if (explosion[i].active)
    {
      dest.x = explosion[i].x;
      dest.y = explosion[i].y;
      
      if (explosion[i].expl_type == EXPL_PLAYER)
	{
	  dest.w = 3;
	  dest.h = 3;
	 
	  if (draw)
	  {
	    SDL_FillRect(screen, &dest,
		         SDL_MapRGB(screen->format,
				    (rand() % player_exploding),
				    (rand() % player_exploding) + 64,
				    (rand() % player_exploding) + 128));
	  }
	  else
	    erase_rect(dest);
	}
      else
	{
	  src.x = (rand() % (images[IMG_BADDIE1]->w - 1));
	  src.y = (rand() % (images[IMG_BADDIE1]->h - 1));
	  src.w = 2;
	  src.h = 2;
	  
	  dest.w = src.w;
	  dest.h = src.h;
	 
	  if (draw)
	    SDL_BlitSurface(images[IMG_BADDIE1], &src, screen, &dest);
	  else
	    erase_rect(dest);
	}

      add_rect(dest);
    }
  }
}

void draw_monsters(boolean draw)
{
  int i;
  SDL_Rect dest;


  for (i = 0; i < 18; i++)
  {
    if (monsters[i].alive)
    {
      dest.x = monsters[i].x;
    
      if (monsters[i].attached == FALSE)
	dest.y = monsters[i].y;
      else
	dest.y = teeter_height(dest.x);

      dest.w = images[IMG_BADDIE1]->w;
      dest.h = images[IMG_BADDIE1]->h;

      if (draw)
        SDL_BlitSurface(images[IMG_BADDIE1], NULL, screen, &dest);
      else
	erase_rect(dest);

      add_rect(dest);
    }
  }
}


void draw_teetertotter(boolean draw)
{
  int sx, sy;
  SDL_Rect dest;

  for (sx = 0; sx < Swidth; sx = sx + images[IMG_PANEL]->w)
  {
    sy = teeter_height(sx);
	  
    dest.x = sx;
    dest.y = sy;
    dest.w = images[IMG_PANEL]->w;
    dest.h = images[IMG_PANEL]->h;

    if (draw)
      SDL_BlitSurface(images[IMG_PANEL], NULL, screen, &dest);
    else
      erase_rect(dest);

    add_rect(dest);
  }
}
