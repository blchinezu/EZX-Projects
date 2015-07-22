/* 
Code		->	RomanH, 2004/2005
License		->	GPL License (see http://www.gnu.org)
*/

#include "variables.h"

SDL_Rect Ball[300];
SDL_Rect BG_Single;
SDL_Rect FlipperTop;
SDL_Rect FlipperBottom;
SDL_Rect Lever[2];
SDL_Rect Bubble;
SDL_Rect GuidePoint;
SDL_Rect Bar;
int BallColour[2];
bool ColourPresent[15];
bool BallsLeft;

void defineSprites()
{
	for(int i = 1; i < 300;i++)
	{
		Ball[i].w = 16;	//all balls have the same width
		Ball[i].h = 16;	//all balls have the same height
	}
	Ball[99].x = 0;	//x-source-position of blue block
	Ball[99].y = 48;	//y-source-position of blue block

    //   1-19  = Balls
    //  20-39  = Balls Frosted
    // 101-XXX = Ice Blocks
    // 201-XXX = Bubbles


    //Balls
	Ball[1].x = 0;    Ball[1].y = 0;   //RED
	Ball[2].x = 16;	  Ball[2].y = 0;   //GREEN
	Ball[3].x = 32;	  Ball[3].y = 0;   //BLUE
 	Ball[4].x = 48;	  Ball[4].y = 0;   //PINK
 	Ball[5].x = 64;	  Ball[5].y = 0;   //BLACK
  	Ball[6].x = 80;   Ball[6].y = 0;   //LILAC
  	Ball[7].x = 96;   Ball[7].y = 0;   //YELLOW
	Ball[10].x = 144; Ball[10].y = 0;  //BLACK AGAIN

	Ball[21].x = 0;	 Ball[21].y = 16;   //RED ICED
	Ball[22].x = 16; Ball[22].y = 16;   //GREEN ICED
	Ball[23].x = 32; Ball[23].y = 16;   //BLUE ICED
 	Ball[24].x = 48; Ball[24].y = 16;   //PINK ICED
 	Ball[25].x = 64; Ball[25].y = 16;   //BLACK ICED
  	Ball[26].x = 80; Ball[26].y = 16;	//LILAC ICED
  	Ball[27].x = 96; Ball[27].y = 16;	//YELLOW ICED

    // Ice-Blocks
 	Ball[101].x = 16;  Ball[101].y = 48;
 	Ball[102].x = 32;  Ball[102].y = 48;
 	
 	//Gems
 	Ball[151].x = 144; Ball[151].y = 64;
 	
	//Magic Ball
 	Ball[161].x = 80;  Ball[151].y = 64;
 	
 	//Bombs
 	Ball[171].x = 112; Ball[171].y = 80;

    // Bubbles
 	Ball[201].x = 96;  Ball[201].y = 48;
  	Ball[202].x = 112; Ball[202].y = 48;
   	Ball[203].x = 128; Ball[203].y = 48;
   	Ball[204].x = 144; Ball[204].y = 48;

    //Guiding Points
    GuidePoint.x = 80; GuidePoint.y = 48;
    GuidePoint.w = 4;  GuidePoint.h = 4;

    //Bar
    Bar.x = 0; Bar.y = 168;
    Bar.w = 104;  Bar.h = 13;

	BG_Single.x = 160;
	BG_Single.y = 0;
	BG_Single.w = 160;
	BG_Single.h = 240;

    FlipperTop.x = 0;
    FlipperTop.y = 64;
    FlipperTop.w = 16;
    FlipperTop.h = 3;

    FlipperBottom.x = 0;
    FlipperBottom.y = 67;
    FlipperBottom.w = 16;
    FlipperBottom.h = 8;

    Lever[0].x = 16;
    Lever[0].y = 64;
    Lever[0].w = 23;
    Lever[0].h = 93;

    Lever[1].x = 48;
    Lever[1].y = 64;
    Lever[1].w = 23;
    Lever[1].h = 104;
}
