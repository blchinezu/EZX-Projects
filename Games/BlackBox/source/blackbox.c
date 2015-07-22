/*  Black-box: guess where the crystals are !                                           
    Copyright (C) 2000 Karl Bartel                                              
                                                                                
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
                                                                                
    Karl Bartel                                                                 
    Cecilienstr. 14                                                             
    12307 Berlin                                                                
    GERMANY                                                                     
    karlb@gmx.net                                                               
*/                                                                              

// This ports the game to SFont...
#define Text( dest, d1, x, y, d2, text, d3) PutString( dest, x, y, text)

#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_timer.h>
#include <signal.h>
#include "SFont.h"

       int fullscreen=1,difficulty=4,trihigh=1,sound=0;   //settings

       SDL_Event event;
       SDL_Rect dstrect;
       SDL_Surface *screen,*box_pic,*edge1_pic,*edge2_pic,*think_pic;
       SDL_Surface *quit_pic,*real_pic,*box4_pic,*ok_pic,*new_pic,*edge3_pic;
       SDL_Surface *light_pic,*edge1red_pic,*edge2red_pic,*edge3red_pic;
       SDL_Surface *edge1rred_pic,*edge2rred_pic,*edge3rred_pic,*help_pic;
       SDL_Surface *next_pic,*prev_pic,*won_pic,*giveup_pic;
       SDL_Surface *demo_pic[8],*Font;
       int mouse_x, mouse_y, level_num, won_shown, this_turn_demo, LevelGiven=0;
       char text[80];
       char xline[9];
       char yline[11];
       char think[11][9];
       char light[11][9];
       char real[11][9];

#define PATHNUM 5
char DATAPATH[200]=DATADIR;
const char PATH[PATHNUM][200]={DATADIR,".","data","../data",DATADIR};

typedef struct {
       int x;
       int y;
       int dir;
} output;


void ComplainAndExit(void)
{
        fprintf(stderr, "Problem: %s\n", SDL_GetError());
        exit(1);
}

void setpix( Sint32 X, Sint32 Y, Uint8 red, Uint8 green, Uint8 blue)  //sets one Pixel to the screen 
{
        Uint32   pixel;
        Uint8   *bits, bpp;

        pixel = SDL_MapRGB(screen->format, red , green , blue);

        if ( SDL_MUSTLOCK(screen) ) {
                if ( SDL_LockSurface(screen) < 0 )
                        return;
        }
        bpp = screen->format->BytesPerPixel;
        bits = ((Uint8 *)screen->pixels)+Y*screen->pitch+X*bpp;

        switch(bpp) {
                case 1:
                        *((Uint8 *)(bits)) = (Uint8)pixel;
                        break;
                case 2:
                        *((Uint16 *)(bits)) = (Uint16)pixel;
                        break;
                case 3: { /* Format/endian independent */
                        Uint8 r, g, b;

                        r = (pixel>>screen->format->Rshift)&0xFF;
                        g = (pixel>>screen->format->Gshift)&0xFF;
                        b = (pixel>>screen->format->Bshift)&0xFF;
                        *((bits)+screen->format->Rshift/8) = r; 
                        *((bits)+screen->format->Gshift/8) = g;
                        *((bits)+screen->format->Bshift/8) = b;
                        }
                        break;
                case 4:
                        *((Uint32 *)(bits)) = (Uint32)pixel;
                        break;
        }
        /* Update the display */
        if ( SDL_MUSTLOCK(screen) ) {
                SDL_UnlockSurface(screen);
        }
        SDL_UpdateRect(screen, X, Y, 1, 1);
        return;
}

SDL_Surface *LoadImage(char *datafile, int transparent)   // reads one GIF into the memory
{
  SDL_Surface *pic,*pic2;
  char filename[200];
  int i=0;

  sprintf(filename,"%s/gfx/%s",DATAPATH,datafile);
  pic = IMG_Load(filename);
  while ( pic == NULL ) {
    strcpy(DATAPATH,PATH[i]);
    sprintf(filename,"%s/gfx/%s",DATAPATH,datafile);
    pic = IMG_Load(filename);
    i++;
    
    if (i>PATHNUM)
    {
      fprintf(stderr,"Couldn't load %s: %s\n", filename, SDL_GetError());
      exit(2);
    }
  }
  if (transparent==3)
      return pic;
  pic2 = SDL_DisplayFormat(pic);
  SDL_FreeSurface(pic);
  if (transparent==1)
    SDL_SetColorKey(pic2,SDL_SRCCOLORKEY|SDL_RLEACCEL,SDL_MapRGB(pic2->format,0xFF,0xFF,0xFF));
  if (transparent==2)
    SDL_SetColorKey(pic2,SDL_SRCCOLORKEY|SDL_RLEACCEL,0);
  return (pic2);
}

void blit(int Xpos,int Ypos,SDL_Surface *image)  //blits one GIF or BMP from the memory to the screen
{
  dstrect.x = Xpos;
  dstrect.y = Ypos;
  dstrect.w = image->w;
  dstrect.h = image->h;
  if ( SDL_BlitSurface(image, NULL, screen, &dstrect) < 0 ) 
  {
    SDL_FreeSurface(image);
    ComplainAndExit();
  }
}

void blit_part(int Xpos,int Ypos,SDL_Surface *image, SDL_Rect srcrect)  //blits a part of one GIF or BMP from the memory to the screen
{
  dstrect.x = srcrect.x;
  dstrect.y = srcrect.y;
  dstrect.w = srcrect.w;
  dstrect.h = srcrect.h;
  if ( SDL_BlitSurface(image, &srcrect , screen, &dstrect) < 0 ) 
  {
    SDL_FreeSurface(image);
    ComplainAndExit();
  }
}

void init_SDL()  // sets the video mode
{
  if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) {ComplainAndExit();}
  atexit(SDL_Quit);
  // Set the video mode (800x600 at 16-bit depth)  
  if (fullscreen)  
    { 
      screen = SDL_SetVideoMode(320, 240, 16, SDL_SWSURFACE| SDL_FULLSCREEN);
    }
  else { screen = SDL_SetVideoMode(320, 240, 16, SDL_SWSURFACE); }
  if ( screen == NULL ) {ComplainAndExit();}
  if ( SDL_Init(SDL_INIT_AUDIO) < 0 ) 
    {fprintf(stderr, "Couldn't initialize SDL: %s\n",SDL_GetError());exit(2);}
}

void DontReadKeys()
{
  SDL_EventState(SDL_KEYUP, SDL_IGNORE);                                  
  SDL_EventState(SDL_KEYDOWN, SDL_IGNORE);                                  
}

void load_images()
{
  Uint8 i;  

  for (i=0;i<=7;i++)
  {
    sprintf(text,"demo%d.gif",i+1);
    demo_pic[i]=LoadImage(text,0);
  }
  won_pic=LoadImage("won.png",3);
  giveup_pic=LoadImage("giveup.gif",0);
  next_pic=LoadImage("next.gif",0);
  prev_pic=LoadImage("prev.gif",0);
  box_pic=LoadImage("box.gif",0);
  edge1_pic=LoadImage("edge1.gif",1);
  edge2_pic=LoadImage("edge2.gif",1);
  edge3_pic=LoadImage("edge3.gif",1);
  edge1rred_pic=LoadImage("edge1red.gif",1);
  edge2rred_pic=LoadImage("edge2red.gif",1);
  edge3rred_pic=LoadImage("edge3red.gif",1);
  think_pic=LoadImage("think.gif",1);
  ok_pic=LoadImage("ok.gif",0);
  new_pic=LoadImage("new.gif",0);
  quit_pic=LoadImage("quit.gif",0);
  real_pic=LoadImage("real.gif",1);
  light_pic=LoadImage("light.gif",0);
  help_pic=LoadImage("help.gif",0);
  Font=LoadImage("font.png",3);

  SDL_SetAlpha(think_pic,(SDL_SRCALPHA),160);
  SDL_SetAlpha(real_pic,(SDL_SRCALPHA),210);
}

void init_blit()
{
  char text[100];
  SDL_Rect rect;
/*
  blit(695,10,giveup_pic);
  blit(695,530,quit_pic);
  blit(695,80,new_pic);
  blit(695,480,help_pic);
  
  sprintf(text,"Level %d",level_num);
  rect.x=800-TextWidth(text);
  rect.y=300;
  rect.w=TextWidth(text);
  rect.h=Font->h;
  SDL_FillRect(screen, &rect, SDL_MapRGB(screen->format,0,0,0));
  PutString(screen,800-TextWidth(text),300,text);
  SDL_UpdateRect(screen,0,0,0,0);
*/
  blit(278,4,giveup_pic);
  blit(278,212,quit_pic);
  blit(278,32,new_pic);
  blit(278,192,help_pic);
  
  sprintf(text,"%d",level_num);
  rect.x=320-TextWidth("Level: ");
  rect.y=50;
  rect.w=TextWidth("Level: ");
  rect.h=142;
  SDL_FillRect(screen, &rect, SDL_MapRGB(screen->format,0,0,0));
  PutString(screen,320-TextWidth("Level:"),110,"Level:");
  PutString(screen,320-TextWidth(text),130,text);
  SDL_UpdateRect(screen,0,0,0,0);
}

void ReadCommandLine(char *argv[])
{
  int i;
  for ( i=1;argv[i];i++ ) 
  {
    if ((strcmp(argv[i],"--nofullscreen")==0)||(strcmp(argv[i],"-f")==0)) {fullscreen=0;} else
    if ((strcmp(argv[i],"--nosound")==0)||(strcmp(argv[i],"-s")==0)) {sound=0;} else
    if (((strcmp(argv[i],"--levelnum")==0)&&(argv[i+1]))||((strcmp(argv[i],"-l")==0)&&(argv[i+1])))
      {i++;level_num=atoi(argv[i]);printf("starting with level #%d\n",level_num);LevelGiven=1;} else
    {
	if (!((strcmp(argv[i],"-h")==0)||(strcmp(argv[i],"--help")==0))) printf("Unknown parameter: \"%s\"\n", argv[i]);
	puts("\nCommand line options:\n");
	puts("  -f, --nofullscreen       start in windowed mode");
	puts("  -s, --nosound            start without sound");
	puts("  -l, --levelnum           start specified level (number has to follow)");
	puts("  -h, --help               this text\n");
	
	exit(0);
    }
  }
}

void free_memory()
{
  Uint8 i;  

  SDL_FreeSurface(box_pic);          
  SDL_FreeSurface(edge1_pic);     
  SDL_FreeSurface(edge2_pic);  
  SDL_FreeSurface(edge3_pic);     
  SDL_FreeSurface(edge1red_pic);     
  SDL_FreeSurface(edge2red_pic);  
  SDL_FreeSurface(edge3red_pic);     
  SDL_FreeSurface(think_pic);  
  SDL_FreeSurface(ok_pic);  
  SDL_FreeSurface(giveup_pic);  
  SDL_FreeSurface(new_pic);  
  SDL_FreeSurface(quit_pic);  
  SDL_FreeSurface(real_pic);  
  SDL_FreeSurface(light_pic);  
  SDL_FreeSurface(help_pic);  
}


void xshot(int y,int right,int erase) //x right
{
  int x,move;
  for (move=10;move<30;move++)
  {
    if (erase)
    {
      //setpix(move+right*600,y,0,0,0);
      //setpix(move+right*600,y+1,0,0,0);
      //setpix(move+right*600,y-1,0,0,0);
	  setpix(move+right*240,y,0,0,0);
      setpix(move+right*240,y+1,0,0,0);
      setpix(move+right*240,y-1,0,0,0);
    }
    else
    {
      //setpix(move+right*600,y,255,0,0);
      //setpix(move+right*600,y+1,255,0,0);
      //setpix(move+right*600,y-1,255,0,0);
	  setpix(move+right*240,y,255,0,0);
      setpix(move+right*240,y+1,255,0,0);
      setpix(move+right*240,y-1,255,0,0);
    }
    if (move % 2==0) SDL_Delay(1);
    SDL_PollEvent(&event);
  }
}

void xshot2(int y,int right,int erase) //x left
{
  int x,move;
  for (move=30;move>10;move--)
  {
    if (erase)
    {
      //setpix(move+right*600,y,0,0,0);
      //setpix(move+right*600,y+1,0,0,0);
      //setpix(move+right*600,y-1,0,0,0);
	  setpix(move+right*240,y,0,0,0);
      setpix(move+right*240,y+1,0,0,0);
      setpix(move+right*240,y-1,0,0,0);
    }
    else
    {
      //setpix(move+right*600,y,255,0,0);
      //setpix(move+right*600,y+1,255,0,0);
      //setpix(move+right*600,y-1,255,0,0);
	  setpix(move+right*240,y,255,0,0);
      setpix(move+right*240,y+1,255,0,0);
      setpix(move+right*240,y-1,255,0,0);
    }
    if (move % 2==0) SDL_Delay(1);
    SDL_PollEvent(&event);
  }
}

void yshot(int x,int down,int erase) // y down
{
  int y,move;
  for (move=10;move<30;move++)
  {
    if (erase)
    {
      //setpix(x,move+down*500,0,0,0);
      //setpix(x+1,move+down*500,0,0,0);
      //setpix(x-1,move+down*500,0,0,0);
	  setpix(x,move+down*200,0,0,0);
      setpix(x+1,move+down*200,0,0,0);
      setpix(x-1,move+down*200,0,0,0);
    }
    else 
    {
      //setpix(x,move+down*500,255,0,0);
      //setpix(x+1,move+down*500,255,0,0);
      //setpix(x-1,move+down*500,255,0,0);
	  setpix(x,move+down*200,255,0,0);
      setpix(x+1,move+down*200,255,0,0);
      setpix(x-1,move+down*200,255,0,0);
    }
    if (move % 2==0) SDL_Delay(1);
    SDL_PollEvent(&event);
  }
}

void yshot2(int x,int down,int erase)  //y up
{
  int y,move;
  for (move=30;move>10;move--)
  {
    if (erase)
    {
      //setpix(x,move+down*500,0,0,0);
      //setpix(x+1,move+down*500,0,0,0);
      //setpix(x-1,move+down*500,0,0,0);
	  setpix(x,move+down*200,0,0,0);
      setpix(x+1,move+down*200,0,0,0);
      setpix(x-1,move+down*200,0,0,0);
    }
    else 
    {
      //setpix(x,move+down*500,255,0,0);
      //setpix(x+1,move+down*500,255,0,0);
      //setpix(x-1,move+down*500,255,0,0);
	  setpix(x,move+down*200,255,0,0);
      setpix(x+1,move+down*200,255,0,0);
      setpix(x-1,move+down*200,255,0,0);
    }
    if (move % 2==0) SDL_Delay(1);
    SDL_PollEvent(&event);
  }
}

void select_shot(int x,int y,int dir,int erase)
{
  if ((dir==1)||(dir==3))
  {
    if (dir==1)
      xshot((y+2)*20,div(x,11).quot,erase);
    if (dir==3)
      xshot2((y+2)*20,div(x,11).quot,erase);
  }
  else
  {
    if (dir==0)
      yshot2((x+2)*20,div(y,9).quot,erase);
    if (dir==2)
      yshot((x+2)*20,div(y,9).quot,erase);
  }
}

output calc_real(int x,int y,int dir)
{
  int end=0;
  output out;
  
  while (!end)
  {
    if (dir==0) 
    {
      if (y==0) {end=1;} else
      if (real[x-1][y-1]==1) {dir=1;} else
      if (real[x][y-1]==1) {dir=3;}
      else {y--;}
    }
    if (dir==1)
    {
      if (x==12) {end=1;} else
      if (real[x][y]==1) {dir=0;} else
      if (real[x][y-1]==1) {dir=2;}
      else {x++;}
    }
    if (dir==2)
    {
      if (y==9) {end=1;} else
      if (real[x][y]==1) {dir=3;} else
      if (real[x-1][y]==1) {dir=1;}
      else {y++;}
    }
    if (dir==3)
    {
      if (x==0) {end=1;} else
      if (real[x-1][y]==1) {dir=0;} else
      if (real[x-1][y-1]==1) {dir=2;}
      else {x--;}
    }
    if ((x>30)||(y>20)||(y<0)||(y<0))
    {
      printf("ERROR: x or y out of range in function hidden, quitting now.\n");
      free_memory();
      exit(1);
    }
  }
  out.x=x;
  out.y=y;
  out.dir=dir;

  return(out);
}

output calc_think(int x,int y,int dir)
{
  int end=0;
  output out;
  
  while (!end)
  {
    if (dir==0) 
    {
      if (y==0) {end=1;} else
      if (think[x-1][y-1]==1) {dir=1;} else
      if (think[x][y-1]==1) {dir=3;}
      else {y--;}
    }
    if (dir==1)
    {
      if (x==12) {end=1;} else
      if (think[x][y]==1) {dir=0;} else
      if (think[x][y-1]==1) {dir=2;}
      else {x++;}
    }
    if (dir==2)
    {
      if (y==9) {end=1;} else
      if (think[x][y]==1) {dir=3;} else
      if (think[x-1][y]==1) {dir=1;}
      else {y++;}
    }
    if (dir==3)
    {
      if (x==0) {end=1;} else
      if (think[x-1][y]==1) {dir=0;} else
      if (think[x-1][y-1]==1) {dir=2;}
      else {x--;}
    }
    if ((x>30)||(y>20)||(y<0)||(y<0))
    {
      printf("ERROR: x or y out of range in function hidden, quitting now.\n");
      free_memory();
      exit(1);
    }
  }
  out.x=x;
  out.y=y;
  out.dir=dir;
  
  return(out);
}

void blit_screen()
{
  int x,y;
  SDL_Rect rect;
  
  if (trihigh)
  {
    edge1red_pic=edge1rred_pic;
    edge2red_pic=edge2rred_pic;
    edge3red_pic=edge3rred_pic;
  }else
  {
    edge1red_pic=edge1_pic;
    edge2red_pic=edge2_pic;
    edge3red_pic=edge3_pic;
  }
  
  for (x=0;x<=10;x++){
    for (y=0;y<=8;y++){
      if (light[x][y]==0)
        {blit(x*20+30,y*20+30,box_pic);}
      else {blit(x*20+30,y*20+30,light_pic);}
    }
  }
  for (x=0;x<=10;x++){
    for (y=0;y<=8;y++){
      if (think[x][y])
        {blit(x*20+40,y*20+40,think_pic);}
    }
  }
  for (x=0;x<=10;x++){
    if ((calc_real(x,0,2).x==calc_think(x,0,2).x)
       &&(calc_real(x,0,2).y==calc_think(x,0,2).y)
       &&(calc_real(x,0,2).dir==calc_think(x,0,2).dir))
    {blit(x*20+30,0,edge1_pic);}
    else {blit(x*20+30,0,edge1red_pic);}
  }
  for (y=0;y<=8;y++){
    if ((calc_real(0,y,1).x==calc_think(0,y,1).x)
       &&(calc_real(0,y,1).y==calc_think(0,y,1).y)
       &&(calc_real(0,y,1).dir==calc_think(0,y,1).dir))
    {blit(0,y*20+30,edge2_pic);}
    else {blit(0,y*20+30,edge2red_pic);}
  }  
  for (x=0;x<=10;x++){
    if ((calc_real(x,9,0).x==calc_think(x,9,0).x)
       &&(calc_real(x,9,0).y==calc_think(x,9,0).y)
       &&(calc_real(x,9,0).dir==calc_think(x,9,0).dir))
    {blit(x*50+75,562,edge3_pic);}
    else {blit(x*20+30,225,edge3red_pic);}
  }
  if (fullscreen)
  {/*
    rect.x=25;
    rect.y=25;
    rect.w=675;
    rect.h=50;
    SDL_FillRect(screen,&rect,0);
    rect.w=50;
    rect.h=550;
    SDL_FillRect(screen,&rect,0);
    rect.x=25;
    rect.y=525;
    rect.w=675;
    rect.h=50;
    SDL_FillRect(screen,&rect,0);
*/
	rect.x=10;
    rect.y=10;
    rect.w=270;
    rect.h=20;
    SDL_FillRect(screen,&rect,0);
    rect.w=20;
    rect.h=220;
    SDL_FillRect(screen,&rect,0);
    rect.x=10;
    rect.y=210;
    rect.w=270;
    rect.h=20;
    SDL_FillRect(screen,&rect,0);
  }
  SDL_UpdateRect(screen,0,0,0,0);
}

void hidden(int x,int y,int dir)  //0=up 1=right 2=down 3=left
{
  int end=0,x_in,y_in,dir_in;

  x_in=x;
  y_in=y;  
  dir_in=dir;
  
  select_shot(x,y,dir,0);
  while (!end)
  {
//    printf("x:%d  y:%d  dir=%d\n",x,y,dir);
    if (dir==0) 
    {
      if (y==0) {end=1;} else
      if (real[x-1][y-1]==1) {dir=1;} else
      if (real[x][y-1]==1) {dir=3;}
      else {y--;}
    }
    if (dir==1)
    {
      if (x==12) {end=1;} else
      if (real[x][y]==1) {dir=0;} else
      if (real[x][y-1]==1) {dir=2;}
      else {x++;}
    }
    if (dir==2)
    {
      if (y==9) {end=1;} else
      if (real[x][y]==1) {dir=3;} else
      if (real[x-1][y]==1) {dir=1;}
      else {y++;}
    }
    if (dir==3)
    {
      if (x==0) {end=1;} else
      if (real[x-1][y]==1) {dir=0;} else
      if (real[x-1][y-1]==1) {dir=2;}
      else {x--;}
    }
    if ((x>30)||(y>20)||(y<0)||(y<0))
    {
      printf("ERROR: x or y out of range in function hidden, quitting now.\n");
      free_memory();
      exit(1);
    }
  }
  if ((x==x_in)&&(y==y_in)&&((dir_in==dir+2)||(dir_in==dir-2)))
  {
    select_shot(x,y,dir_in,1);
    select_shot(x,y,dir,0);
    select_shot(x,y,dir,1);    
  }
  else
  {
    select_shot(x,y,dir,0);
    SDL_Delay(500);
    select_shot(x_in,y_in,dir_in,1);
    select_shot(x,y,dir,1);
  }
}

void show_real()
{
  int x,y;
  
  blit_screen();
  for (x=0;x<=10;x++){
    for (y=0;y<=8;y++){
      if (real[x][y])
        {blit(x*20+40,y*20+40,real_pic);}
    }
  }
  for (x=0;x<=10;x++){
    for (y=0;y<=8;y++){
      if (think[x][y])
        {blit(x*20+40,y*20+40,think_pic);}
    }
  }
  SDL_UpdateRect(screen,0,0,0,0);
}

void generate_field()
{
  int x,y,crystal_num;
  
  for (x=0;x<=10;x++){
    for (y=0;y<=8;y++){
      think[x][y]=0;
      light[x][y]=0;
    }
  }
  while ((crystal_num>difficulty*3)||(crystal_num<(difficulty-1)*3))
  {
    crystal_num=0;
    srand(level_num);
    for (x=0;x<=9;x++){
      for (y=0;y<=7;y++){
        if (abrand(0,10)==0) {real[x][y]=1;}
          else {real[x][y]=0;}
      }
    }
    for (x=0;x<=9;x++){
      for (y=0;y<=7;y++){
        if (real[x][y]==1) {crystal_num++;}
      }
    }  
    sprintf(text,"Black-Box: Level #%d",level_num);
    if (LevelGiven) {
	LevelGiven=0;
	break;
    }
    level_num=(((unsigned)time(NULL)+SDL_GetTicks())%100000);
//    printf("%d\n",crystal_num); 
  }
  for (x=0;x<=10;x++)
  {
    yline[x]=0;
  }
  for (y=0;y<=8;y++)
  {
    xline[y]=0;
  }
  SDL_WM_SetCaption(text,"Black-Box"); 
  won_shown=0;
}

void tex(int x, int y, char *text)
{
  Text(screen,&font,x,y,1150,text,-1);
}

void num(int x, int y, int numb)
{
  char text[30];  
                                                                     
  sprintf(text,"%d",numb);
  Text(screen,&font,x,y,1150,text,-1);
}

void demo()
{
  SDL_Rect rect;
  int end=0,end2=0,show=0;

  blit(278,4,ok_pic);
  while (!end)
  {
	  /*
  rect.x=0;
  rect.y=0;
  rect.w=700;
  rect.h=600;
*/
  rect.x=320-TextWidth("Level: ");
  rect.y=50;
  rect.w=TextWidth("Level: ");
  rect.h=142;
  SDL_FillRect(screen, &rect, SDL_MapRGB(screen->format,0,0,0));
  
  rect.x=0;
  rect.y=0;
  rect.w=280;
  rect.h=240;

  SDL_FillRect(screen,&rect,0);
  blit(96,40,demo_pic[show]);  
  blit(88,124,prev_pic);  
  blit(160,124,next_pic);  
  XCenteredString(screen,160,"info visit http://www.linux-games.com");
  //XCenteredString(screen,168,"or write a mail to Karl Bartel <karlb@gmx.net>");
  
  XCenteredString(screen,176,"A1200/E6 Port by BruceLee");
  
  XCenteredString(screen,5,"BLACK-BOX HELP & OPTIONS");
//  tex(60,200,"DIFFICULTY:");
  tex(20,200,"difficulty:");
  num(60,208,difficulty);
//  tex(110,200,"HIGHLIGHT:");
	tex(130,200,"highlight:");
  //if (trihigh) tex(124,208,"YES");
  //else tex(124,208,"No");
  //tex(160,200,"SOUND:");
  //if (sound) tex(164,208,"YES");
 // else tex(164,208,"No");

	  if (trihigh) tex(160,208,"yes");
  else tex(160,208,"no");
  tex(220,200,"sound:");
  if (sound) tex(230,208,"yes");
  else tex(230,208,"no");

  SDL_UpdateRect(screen,0,0,0,0);
  while (( SDL_WaitEvent(&event) >= 0 )&&(!end2)) {
    switch (event.type) {
      case SDL_MOUSEMOTION: {
        SDL_GetMouseState(&mouse_x, &mouse_y);
        }
        break;
      case SDL_MOUSEBUTTONDOWN:
        {end2=1;}
        break;
      case SDL_QUIT: {
        printf("Quit requested, quitting...\n");
        free_memory();
        exit(0);
      }
      break;
    }
  }end2=0;
//buttons
    if ((mouse_x>60)&&(mouse_y>200)&&(mouse_y<240)&&(mouse_x<100))
    {
      if (difficulty<6) difficulty++; else difficulty=1;
      level_num=(((unsigned)time(NULL)+SDL_GetTicks())%100000);
      generate_field();
    }
    if ((mouse_x>112)&&(mouse_y>200)&&(mouse_y<240)&&(mouse_x<160))
    {
      if (trihigh) trihigh=0; else trihigh=1;
    }
    if ((mouse_x>160)&&(mouse_y>200)&&(mouse_y<240)&&(mouse_x<200))
    {
    }
    if ((mouse_x>88)&&(mouse_y>124)&&(mouse_y<160)&&(mouse_x<156))
    {
      if (show) show--;
    }
    if ((mouse_x>166)&&(mouse_y>124)&&(mouse_y<160)&&(mouse_x<248))
    {
      if (show<7) show++;
    }
    if ((mouse_x>280)&&(mouse_y<48)&&(mouse_y>30))
    {
      level_num=(((unsigned)time(NULL)+SDL_GetTicks())%100000);
      generate_field();
      end=1;
    }
    if ((mouse_x>280)&&(mouse_y>212))
    {
      printf("Quit pressed, quitting...\n");
      free_memory();
      exit(0);
    } 
    if ((mouse_x>280)&&(mouse_y<28))
    {
      end=1;
      blit_screen();
    }
  }
  this_turn_demo=1;
  SDL_FillRect(screen,&rect,0);
  blit(278,4,giveup_pic);
  init_blit();
  SDL_Delay(100);
}

int IsItComplete()
{
  int x,y,won=1;
  for (x=0;x<=11;x++)
  {
    if (calc_real(x,0,2).x!=calc_think(x,0,2).x) {won=0;}
    if (calc_real(x,0,2).y!=calc_think(x,0,2).y) {won=0;}
    if (calc_real(x,0,2).dir!=calc_think(x,0,2).dir) {won=0;}
  }
  for (x=0;x<=11;x++)
  {
    if (calc_real(x,9,0).x!=calc_think(x,9,0).x) {won=0;}
    if (calc_real(x,9,0).y!=calc_think(x,9,0).y) {won=0;}
    if (calc_real(x,9,0).dir!=calc_think(x,9,0).dir) {won=0;}
  }
  for (y=0;y<=9;y++)
  {
    if (calc_real(0,y,1).x!=calc_think(0,y,1).x) {won=0;}
    if (calc_real(0,y,1).y!=calc_think(0,y,1).y) {won=0;}
    if (calc_real(0,y,1).dir!=calc_think(0,y,1).dir) {won=0;}
  }
  return(won);  
}

void click()
{
  int x,y;
  
  switch(SDL_GetMouseState(&mouse_x, &mouse_y))
  {
//block highlighting
  case SDL_BUTTON(3):
    if ((light[div(mouse_x+10,20).quot-2][div(mouse_y+10,20).quot-2]==0)
       &&(div(mouse_x+10,20).quot>1)&&(div(mouse_y+10,20).quot>1)
       &&(div(mouse_x+10,20).quot<13)&&(div(mouse_y+10,20).quot<11))
    {
      light[div(mouse_x+10,20).quot-2][div(mouse_y+10,20).quot-2]=1;
    }else
    if ((light[div(mouse_x+10,20).quot-2][div(mouse_y+10,20).quot-2]==1)
       &&(div(mouse_x+10,20).quot>1)&&(div(mouse_y+10,20).quot>1)
       &&(div(mouse_x+10,20).quot<13)&&(div(mouse_y+10,20).quot<11))
    {
      light[div(mouse_x+10,20).quot-2][div(mouse_y+10,20).quot-2]=0;
    }
//line highlighting    
    if ((div(mouse_x+10,20).quot<2)
       &&(div(mouse_y+10,20).quot>1)&&(div(mouse_y+10,20).quot<11))
    {
      if (xline[div(mouse_y+10,20).quot-2]==0)
      {
        for (x=0;x<11;x++)
        {
          light[x][div(mouse_y+10,20).quot-2]=1;
        }
	xline[div(mouse_y+10,20).quot-2]=1;
      }
      else{
        for (x=0;x<11;x++)
        {
          light[x][div(mouse_y+10,20).quot-2]=0;
        }
	xline[div(mouse_y+10,20).quot-2]=0;
      }      
    }
    if (((div(mouse_y+10,20).quot<2)&&(div(mouse_x+10,20).quot>1)
       &&(div(mouse_x+10,20).quot<13))||((div(mouse_y+10,20).quot>10)
       &&(div(mouse_x+10,20).quot>1)&&(div(mouse_x+10,20).quot<13)))
    {
      if (yline[div(mouse_x+10,20).quot-2]==0)
      {
        for (y=0;y<9;y++)
        {
          light[div(mouse_x+10,20).quot-2][y]=1;
        }
	yline[div(mouse_x+10,20).quot-2]=1;
      }
      else{
        for (y=0;y<9;y++)
        {
          light[div(mouse_x+10,20).quot-2][y]=0;
        }
	yline[div(mouse_x+10,20).quot-2]=0;
      }      
    }
  break;
  case SDL_BUTTON(1):
//think
    if ((think[div(mouse_x,20).quot-2][div(mouse_y,20).quot-2]==0)
       &&(div(mouse_x,20).quot>1)&&(div(mouse_y,20).quot>1)
       &&(div(mouse_x,20).quot<12)&&(div(mouse_y,20).quot<10))
    {
      think[div(mouse_x,20).quot-2][div(mouse_y,20).quot-2]=1;
    } else
    if ((think[div(mouse_x,20).quot-2][div(mouse_y,20).quot-2]==1)
       &&(div(mouse_x,20).quot>1)&&(div(mouse_y,20).quot>1)
       &&(div(mouse_x,20).quot<12)&&(div(mouse_y,20).quot<10))
    {
      think[div(mouse_x,20).quot-2][div(mouse_y,20).quot-2]=0;
    }
//shot  
    if ((div(mouse_x+10,20).quot<2)
       &&(div(mouse_y+10,20).quot>1)&&(div(mouse_y+10,20).quot<11))
    {
      hidden(0,div(mouse_y+10,20).quot-2,1);
    }
    if ((div(mouse_y+10,20).quot<2)
       &&(div(mouse_x+10,20).quot>1)&&(div(mouse_x+10,20).quot<13))
    {
      hidden(div(mouse_x+10,20).quot-2,0,2);
    }
    if ((div(mouse_y+10,20).quot>10)
       &&(div(mouse_x+10,20).quot>1)&&(div(mouse_x+10,20).quot<13))
    {
      hidden(div(mouse_x+10,20).quot-2,9,0);
    }
//buttons
    if ((mouse_x>280)&&(mouse_y<48)&&(mouse_y>30))
    {
      level_num=(((unsigned)time(NULL)+SDL_GetTicks())%100000);
      generate_field();
      init_blit();
      blit_screen();
    }
    if ((mouse_x>280)&&(mouse_y>212))
    {
      printf("Quit pressed, quitting...\n");
      free_memory();
      exit(1);
    } 
    if ((mouse_x>280)&&(mouse_y<212)&&(mouse_y>192))
    {
      demo();
    } 
  break;
  }
  blit_screen();
  if ((mouse_x>280)&&(mouse_y<28)&&(!this_turn_demo))
  {
    //Give up
    show_real();
  }
  this_turn_demo=0;
}

int abrand(int a,int b)  //random number between a and b (inclusive)
{
  return(a+(rand() % (b-a+1)));
}

void get_click()
{
  while ( SDL_WaitEvent(&event) >= 0 ) {
    switch (event.type) {
      case SDL_MOUSEMOTION:
        SDL_GetMouseState(&mouse_x, &mouse_y);
        break;
      case SDL_MOUSEBUTTONDOWN:
        click();
        break;
      case SDL_QUIT: {
        printf("Quit requested, quitting...\n");
        free_memory();
        exit(0);}
        break;
    }
    if ((IsItComplete())&&(!won_shown)) {
      won_shown=1;
      blit(60,80,won_pic);
      SDL_UpdateRect(screen,0,0,0,0);
    }
  }
}

void init_sound()
{
	// removed sound
}

main(int argc, char *argv[])
{
  printf("\nBlack-Box version %s, Copyright (C) 2000 Karl Bartel\n",VERSION);
  printf("Ported to A1200/E6 by BruceLee @ 2010\n");
  printf("Black-Box comes with ABSOLUTELY NO WARRANTY; for details see COPYING'.\n");
  printf("This is free software, and you are welcome to redistribute it\n");
  printf("under certain conditions.\n\n");
  level_num=((unsigned)time(NULL)%100000);
  //level_num=1;
  ReadCommandLine(argv);
  init_SDL();
  load_images();
  InitFont(Font);
  if (sound) init_sound();
  sprintf(text,"%s/font.scl",DATAPATH);
  generate_field();
  blit_screen();
  init_blit();
  blit_screen();
  get_click();
  free_memory();
  exit(0);
}
