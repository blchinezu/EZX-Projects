/* 
Code		->	RomanH, 2004/2005
License		->	GPL License (see http://www.gnu.org)
*/

#include "variables.h"

SDL_Rect BallDstRect;
SDL_Rect BallSrcRect;

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

void DrawBall(int left, int top, int colour)
{
BallSrcRect.x = Ball[colour].x;	BallSrcRect.y = Ball[colour].y;
BallDstRect.x = left;			BallDstRect.y = top;
BallSrcRect.w = Ball[colour].w;	BallSrcRect.h = Ball[colour].h;
BallDstRect.w = Ball[colour].w;	BallDstRect.h = Ball[colour].h;

SDL_BlitSurface(SpritesSurface,&BallSrcRect,DisplaySurface,&BallDstRect);
}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

void DrawBackground()
{
	for(int i = 0; i < 20;i++)
	{
		for(int j = 0; j < 15;j++)
		{
			DrawBall(i*16, j*16, 99);
		} 
	}
}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

void DrawFlipper(int ypos)
{
if(ypos==0){ypos=1;}
SDL_Rect FlipperDstRect;
SDL_Rect FlipperSrcRect;
FlipperSrcRect.y = FlipperTop.y;
FlipperSrcRect.x = FlipperTop.x;
FlipperSrcRect.w = FlipperTop.w;
FlipperSrcRect.h = FlipperTop.h;
FlipperDstRect.x = 100;
FlipperDstRect.w = FlipperTop.w;
FlipperDstRect.h = FlipperTop.h;

SDL_Rect FlipperBottomDstRect;
SDL_Rect FlipperBottomSrcRect;
FlipperBottomSrcRect.y = FlipperBottom.y;
FlipperBottomSrcRect.x = FlipperBottom.x;
FlipperBottomSrcRect.w = FlipperBottom.w;
FlipperBottomSrcRect.h = FlipperBottom.h;
FlipperBottomDstRect.x = 100;
FlipperBottomDstRect.w = FlipperBottom.w;
FlipperBottomDstRect.h = FlipperBottom.h;

FlipperDstRect.y = ypos*8+117;
SDL_BlitSurface(SpritesSurface,&FlipperSrcRect,DisplaySurface,&FlipperDstRect);

//DRAW SPRING
while(ypos<13)
{
 FlipperBottomDstRect.y = 120+ypos*8;
 SDL_BlitSurface(SpritesSurface,&FlipperBottomSrcRect,DisplaySurface,&FlipperBottomDstRect);
 ypos++;
}
//DRAW SPRING
}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

void DrawLever(int state)
{
SDL_Rect LeverDstRect;
SDL_Rect LeverSrcRect;
LeverSrcRect.y = Lever[state].y;
LeverSrcRect.x = Lever[state].x;
LeverSrcRect.w = Lever[state].w;
LeverSrcRect.h = Lever[state].h;

LeverDstRect.y = 100;
LeverDstRect.x = 57;
LeverDstRect.w = Lever[state].w;
LeverDstRect.h = Lever[state].h;

SDL_BlitSurface(SpritesSurface,&LeverSrcRect,DisplaySurface,&LeverDstRect);
}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

void DrawGuidePoint(int x, int y)
{
SDL_Rect GuidePointDstRect;
SDL_Rect GuidePointSrcRect;

GuidePointSrcRect.y = GuidePoint.y;
GuidePointSrcRect.x = GuidePoint.x;
GuidePointSrcRect.w = GuidePoint.w;
GuidePointSrcRect.h = GuidePoint.h;

GuidePointDstRect.y = y;
GuidePointDstRect.x = x;
GuidePointDstRect.w = GuidePoint.w;
GuidePointDstRect.h = GuidePoint.h;

SDL_BlitSurface(SpritesSurface,&GuidePointSrcRect,DisplaySurface,&GuidePointDstRect);
}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

void ReDrawScreen()
{
////////////////////////////
// =blue background tiling...
DrawBackground();

////////////////////////////
// Drawing the playingfield
SDL_BlitSurface(SpritesSurface,&BG_Single,DisplaySurface,&BGDstRect);

////////////////////////////
// Drawing the flipper
DrawFlipper(StartPosition);

////////////////////////////
//Draw all existing balls
for(int i = 1; i<15; i++){
 for(int j = 1; j<7; j++){
  if(BallPosition[i][j].occupied == true)
   {DrawBall(BallPosition[i][j].x+117, BallPosition[i][j].y+24, BallPosition[i][j].colour);
   //game is over, if a ball is above the top line
  if(BallPosition[17-LevelHeight][j].colour > 0 && BallPosition[17-LevelHeight][j].colour < 30){GameOver=true;}
}}}



////////////////////////////
//Also draw the dropping ball
if(BallStopped==false)
{DrawBall(FallingBallX+117, FallingBallY+24, BallColour[0]);}

////////////////////////////
//Draw the bar (if balls are raised
DrawBar();

////////////////////////////
// Drawing the chain that's pulled
if(Key_DOWN == 0){DrawLever(0);}
if(Key_DOWN == 1){DrawLever(1);}

////////////////////////////
// = next colour ball
DrawBall(100, 4,  BallColour[1]);

////////////////////////////
Print (8,8,  "PLUS:");
Print (8,18, "  quit");
Print (8,42, "Down:");
Print (8,52, "  shoot");
Print (296,0, "EZX");
if(LevelNumber == 1){Print (8,200, "Level: 1");} else
if(LevelNumber == 2){Print (8,200, "Level: 2");} else
if(LevelNumber == 3){Print (8,200, "Level: 3");} else
if(LevelNumber == 4){Print (8,200, "Level: 4");} else
if(LevelNumber == 5){Print (8,200, "Level: 5");} else
if(LevelNumber == 6){Print (8,200, "Level: 6");} else
if(LevelNumber == 7){Print (8,200, "Level: 7");} else
if(LevelNumber == 8){Print (8,200, "Level: 8");} else
if(LevelNumber == 9){Print (8,200, "Level: 9");} else
if(LevelNumber == 10){Print (8,200, "Level: 10");} else
if(LevelNumber == 11){Print (8,200, "Level: 11");} else
if(LevelNumber == 12){Print (8,200, "Level: 12");} else
if(LevelNumber == 13){Print (8,200, "Level: 13");} else
if(LevelNumber == 14){Print (8,200, "Level: 14");} else
if(LevelNumber == 15){Print (8,200, "Level: 15");} else
if(LevelNumber == 16){Print (8,200, "Level: 16");} else
if(LevelNumber == 17){Print (8,200, "Level: 17");} else
if(LevelNumber == 18){Print (8,200, "Level: 18");} else
if(LevelNumber == 19){Print (8,200, "Level: 19");} else
if(LevelNumber == 20){Print (8,200, "Level: 20");}

if(ShootingCounter >3){Print (8,212,  "Timer: -");} else
if(ShootingCounter==3){Print (8,212,  "Timer: 3");} else
if(ShootingCounter==2){Print (8,212,  "Timer: 2");} else
if(ShootingCounter==1){Print (8,212,  "Timer: 1");} else
if(ShootingCounter==0){Print (8,212,  "Timer: 0");} 

// if(LevelRaiseCounter >3){Print (8,224,  "LRC: -");}
// if(LevelRaiseCounter==3){Print (8,224,  "LRC: 3");}
// if(LevelRaiseCounter==2){Print (8,224,  "LRC: 2");}
// if(LevelRaiseCounter==1){Print (8,224,  "LRC: 1");}
// if(LevelRaiseCounter==0){Print (8,224,  "LRC: 0");} 

////////////////////////////
// setting the Ball if it is not already falling
if(BallStopped == true && DrawUnshotBall==true && ShootingAnimationRunning == false){
    if(StartPosition==1 || StartPosition==0) {FallingBallX = BallPosition[15][1].x;} else
    if(StartPosition==2) {FallingBallX = BallPosition[14][1].x;} else
    if(StartPosition==3) {FallingBallX = BallPosition[15][2].x;} else
    if(StartPosition==4) {FallingBallX = BallPosition[14][2].x;} else
    if(StartPosition==5) {FallingBallX = BallPosition[15][3].x;} else
    if(StartPosition==6) {FallingBallX = BallPosition[14][3].x;} else
    if(StartPosition==7) {FallingBallX = BallPosition[15][4].x;} else
    if(StartPosition==8) {FallingBallX = BallPosition[14][4].x;} else
    if(StartPosition==9) {FallingBallX = BallPosition[15][5].x;} else
    if(StartPosition==10){FallingBallX = BallPosition[14][5].x;} else
    if(StartPosition==11){FallingBallX = BallPosition[15][6].x;} else
    if(StartPosition==12){FallingBallX = BallPosition[14][6].x;}

    ////////////////////////////
    // draw the Ball if it is not already falling (on Flipper)
    if(StartPosition==1 || StartPosition==0) {DrawBall(100, 109, BallColour[0]);} else
    if(StartPosition==2) {DrawBall(100, 117, BallColour[0]);} else
    if(StartPosition==3) {DrawBall(100, 125, BallColour[0]);} else
    if(StartPosition==4) {DrawBall(100, 133, BallColour[0]);} else
    if(StartPosition==5) {DrawBall(100, 141, BallColour[0]);} else
    if(StartPosition==6) {DrawBall(100, 149, BallColour[0]);} else
    if(StartPosition==7) {DrawBall(100, 157, BallColour[0]);} else
    if(StartPosition==8) {DrawBall(100, 165, BallColour[0]);} else
    if(StartPosition==9) {DrawBall(100, 173, BallColour[0]);} else
    if(StartPosition==10){DrawBall(100, 181, BallColour[0]);} else
    if(StartPosition==11){DrawBall(100, 189, BallColour[0]);} else
    if(StartPosition==12){DrawBall(100, 197, BallColour[0]);}

    ////////////////////////////
    // draw the guiding points if it is not already falling
    int GuidePosition;
    int GuidePoints, GuidePoints2;
    GuidePosition = StartPosition;
    if(GuidePosition==0){GuidePosition=1;}
    if(DrawGuidepoints==true)
    {
    //falling
    for(GuidePoints = 1;  GuidePoints < 10; GuidePoints++) {DrawGuidePoint(115+GuidePosition*8,GuidePoints*20-5);}
    //rising
    if(GuidePosition==1) {for(GuidePoints = 1; GuidePoints < 6; GuidePoints++){DrawGuidePoint(106,GuidePoints*20-5);}} else
    if(GuidePosition==2) {for(GuidePoints = 1; GuidePoints < 6; GuidePoints++){DrawGuidePoint(106,GuidePoints*22-7);}} else
    if(GuidePosition==3) {for(GuidePoints = 1; GuidePoints < 6; GuidePoints++){DrawGuidePoint(106,GuidePoints*24-9);}} else
    if(GuidePosition==4) {for(GuidePoints = 1; GuidePoints < 6; GuidePoints++){DrawGuidePoint(106,GuidePoints*26-11);}} else
    if(GuidePosition==5) {for(GuidePoints = 1; GuidePoints < 6; GuidePoints++){DrawGuidePoint(106,GuidePoints*28-13);}} else
    if(GuidePosition==6) {for(GuidePoints = 1; GuidePoints < 6; GuidePoints++){DrawGuidePoint(106,GuidePoints*30-15);}} else
    if(GuidePosition==7) {for(GuidePoints = 1; GuidePoints < 6; GuidePoints++){DrawGuidePoint(106,GuidePoints*32-17);}} else
    if(GuidePosition==8) {for(GuidePoints = 1; GuidePoints < 6; GuidePoints++){DrawGuidePoint(106,GuidePoints*34-19);}} else
    if(GuidePosition==9) {for(GuidePoints = 1; GuidePoints < 6; GuidePoints++){DrawGuidePoint(106,GuidePoints*36-21);}} else
    if(GuidePosition==10){for(GuidePoints = 1; GuidePoints < 6; GuidePoints++){DrawGuidePoint(106,GuidePoints*38-23);}} else
    if(GuidePosition==11){for(GuidePoints = 1; GuidePoints < 6; GuidePoints++){DrawGuidePoint(106,GuidePoints*40-25);}} else
    if(GuidePosition==12){for(GuidePoints = 1; GuidePoints < 6; GuidePoints++){DrawGuidePoint(106,GuidePoints*42-27);}}
    //horizontal
    if(GuidePosition==1) {for(GuidePoints = 1; GuidePoints < 4; GuidePoints++){DrawGuidePoint(106+GuidePoints*4,12);}} else
    if(GuidePosition==2) {for(GuidePoints = 1; GuidePoints < 4; GuidePoints++){DrawGuidePoint(106+GuidePoints*6,12);}} else
    if(GuidePosition==3) {for(GuidePoints = 1; GuidePoints < 4; GuidePoints++){DrawGuidePoint(106+GuidePoints*8,12);}} else
    if(GuidePosition==4) {for(GuidePoints = 1; GuidePoints < 4; GuidePoints++){DrawGuidePoint(106+GuidePoints*10,12);}} else
    if(GuidePosition==5) {for(GuidePoints = 1; GuidePoints < 4; GuidePoints++){DrawGuidePoint(106+GuidePoints*12,12);}} else
    if(GuidePosition==6) {for(GuidePoints = 1; GuidePoints < 4; GuidePoints++){DrawGuidePoint(106+GuidePoints*14,12);}} else
    if(GuidePosition==7) {for(GuidePoints = 1; GuidePoints < 4; GuidePoints++){DrawGuidePoint(106+GuidePoints*16,12);}} else
    if(GuidePosition==8) {for(GuidePoints = 1; GuidePoints < 4; GuidePoints++){DrawGuidePoint(106+GuidePoints*18,12);}} else
    if(GuidePosition==9) {for(GuidePoints = 1; GuidePoints < 4; GuidePoints++){DrawGuidePoint(106+GuidePoints*20,12);}} else
    if(GuidePosition==10){for(GuidePoints = 1; GuidePoints < 4; GuidePoints++){DrawGuidePoint(106+GuidePoints*22,12);}} else
    if(GuidePosition==11){for(GuidePoints = 1; GuidePoints < 4; GuidePoints++){DrawGuidePoint(106+GuidePoints*24,12);}} else
    if(GuidePosition==12){for(GuidePoints = 1; GuidePoints < 4; GuidePoints++){DrawGuidePoint(106+GuidePoints*26,12);}}
}
}

}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

void DrawBar()
{
if(LevelHeight>1)
{
SDL_Rect BarDstRect;
SDL_Rect BarSrcRect;

BarSrcRect.y = Bar.y;
BarSrcRect.x = Bar.x;
BarSrcRect.w = Bar.w;
BarSrcRect.h = Bar.h;

BarDstRect.y = 211+13-(LevelHeight-1)*13;
BarDstRect.x = 117;
BarDstRect.w = Bar.w;
BarDstRect.h = Bar.h;

SDL_BlitSurface(SpritesSurface,&BarSrcRect,DisplaySurface,&BarDstRect);
}
}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

