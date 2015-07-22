/* 
Code		->	RomanH, 2004/2005
License		->	GPL License (see http://www.gnu.org)
*/

#include "variables.h"

void KillBubbles();
void DropAllBalls();


void KillBalls()
{
int r, c;
int goTwice;
int neighbours[16][7];
bool BubblesCreated = false;

//reset all
for(r = 1; r < 16;r++){for(c = 1; c < 7;c++){neighbours[r][c] = 0;}}
//reset all


for(goTwice = 0; goTwice < 2; goTwice++){
 for(r = 1; r < 16;r++)
  {
   for(c = 1; c < 7;c++)
		{

         if(r == 1 || r == 3 || r == 5 || r == 7 || r == 9 || r == 11 || r == 13 || r == 15)
         {
      	  if(BallPosition[r+1][c].occupied == true && BallPosition[r][c].colour == BallPosition[r+1][c].colour && BallPosition[r][c].colour < 20)
            {//right up...
            if(neighbours[r][c] > neighbours[r+1][c]) {neighbours[r][c]++;   neighbours[r+1][c] = neighbours[r][c]  +1;}
            if(neighbours[r][c] < neighbours[r+1][c]) {neighbours[r+1][c]++; neighbours[r][c]   = neighbours[r+1][c]+1;}
            if(neighbours[r][c] == neighbours[r+1][c]){neighbours[r+1][c]++; neighbours[r][c]++;}
            }

 		if(BallPosition[r][c].occupied == true && c < 6)
 		 {if(BallPosition[r][c+1].occupied == true && BallPosition[r][c].colour == BallPosition[r][c+1].colour && BallPosition[r][c].colour < 20)
            {//right...
            if(neighbours[r][c] > neighbours[r][c+1]) {neighbours[r][c]++;   neighbours[r][c+1] = neighbours[r][c]+1;}
            if(neighbours[r][c] < neighbours[r][c+1]) {neighbours[r][c+1]++; neighbours[r][c]   = neighbours[r][c+1]+1;}
            if(neighbours[r][c] == neighbours[r][c+1]){neighbours[r][c+1]++; neighbours[r][c]++;}
            }
          }

  
 		if(BallPosition[r][c].occupied == true && c > 1)
 		 {if(BallPosition[r+1][c-1].occupied == true && BallPosition[r][c].colour == BallPosition[r+1][c-1].colour && BallPosition[r][c].colour < 20)
            {//left up...
            if(neighbours[r][c] > neighbours[r+1][c-1]) {neighbours[r][c]++;     neighbours[r+1][c-1] = neighbours[r][c]+1;}
            if(neighbours[r][c] < neighbours[r+1][c-1]) {neighbours[r+1][c-1]++; neighbours[r][c]     = neighbours[r+1][c-1]+1;}
            if(neighbours[r][c] == neighbours[r+1][c-1]){neighbours[r+1][c-1]++; neighbours[r][c]++;}
            }
          }
         }//end of r==1...


		
         if(r == 2 || r == 4 || r == 6 || r == 8 || r == 10 || r == 12 || r == 14)
         {
      	 if(BallPosition[r+1][c].occupied == true && BallPosition[r][c].colour == BallPosition[r+1][c].colour && BallPosition[r][c].colour < 20)
          {//left up...
            if(neighbours[r][c] > neighbours[r+1][c]) {neighbours[r][c]++;   neighbours[r+1][c] = neighbours[r][c]  +1;}
            if(neighbours[r][c] < neighbours[r+1][c]) {neighbours[r+1][c]++; neighbours[r][c]   = neighbours[r+1][c]+1;}
            if(neighbours[r][c] == neighbours[r+1][c]){neighbours[r+1][c]++; neighbours[r][c]++;}
          }

 		if(BallPosition[r][c].occupied == true && c < 6)
 		 {if(BallPosition[r][c+1].occupied == true && BallPosition[r][c].colour == BallPosition[r][c+1].colour && BallPosition[r][c].colour < 20)
            {//right...
            if(neighbours[r][c] > neighbours[r][c+1]) {neighbours[r][c]++;   neighbours[r][c+1] = neighbours[r][c]+1;}
            if(neighbours[r][c] < neighbours[r][c+1]) {neighbours[r][c+1]++; neighbours[r][c]   = neighbours[r][c+1]+1;}
            if(neighbours[r][c] == neighbours[r][c+1]){neighbours[r][c+1]++; neighbours[r][c]++;}
            }
          }

 		if(BallPosition[r][c].occupied == true)
 		 {if(BallPosition[r+1][c+1].occupied == true && BallPosition[r][c].colour == BallPosition[r+1][c+1].colour && BallPosition[r][c].colour < 20)
            {//right up...
            if(neighbours[r][c] > neighbours[r+1][c+1]) {neighbours[r][c]++;     neighbours[r+1][c+1] = neighbours[r][c]+1;}
            if(neighbours[r][c] < neighbours[r+1][c+1]) {neighbours[r+1][c+1]++; neighbours[r][c]     = neighbours[r+1][c+1]+1;}
            if(neighbours[r][c] == neighbours[r+1][c+1]){neighbours[r+1][c+1]++; neighbours[r][c]++;}
            }
          }
        }//end of r==2...
}//end for c
}//end for r
}//end go twice
bool thisLinegotBubbles;
thisLinegotBubbles=false;
  for(r = 1; r < 16;r++)
	{for(c = 1; c < 7;c++)
        {if(neighbours[r][c] > 2)
           {
	          BallPosition[r][c].colour = 201;
	          BallPosition[r][c].occupied = true;
              BubblesCreated = true;
              thisLinegotBubbles = true;
              defreezeBall(r,c);
           }
        }
   if(thisLinegotBubbles == true){SDL_Delay(50); ReDrawScreen(); SDL_UpdateRect(DisplaySurface,0,0,0,0);thisLinegotBubbles = false;}
   }
   if(BubblesCreated == true){ReDrawScreen(); SDL_UpdateRect(DisplaySurface,0,0,0,0); SDL_Delay(100); KillBubbles();}
   if(BubblesCreated == true){RaiseLevel=false;}
}//end function





///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////





void KillBubbles()
{
     
int r,c;
                                
 for(r = 14; r > 0;r--)
  {
   for(c = 6; c > 0;c--)
    {
     if(BallPosition[r][c].colour > 200)
      {
       if(r == 1 || r == 3 || r == 5 || r == 7 || r == 9 || r == 11 || r == 13 || r == 15)
        {
      	 if(BallPosition[r+1][c].occupied == false)                //right up...
          {SetBall(r,c,0); ReDrawScreen();SDL_Delay(15); SDL_UpdateRect(DisplaySurface,0,0,0,0);}
 		 if(BallPosition[r][c+1].occupied == false && c < 6)          //right...
          {SetBall(r,c,0); ReDrawScreen();SDL_Delay(15); SDL_UpdateRect(DisplaySurface,0,0,0,0);}
         if(BallPosition[r+1][c-1].occupied == false && c > 1)      //left up...
          {SetBall(r,c,0); ReDrawScreen();SDL_Delay(15); SDL_UpdateRect(DisplaySurface,0,0,0,0);}
         if(BallPosition[r][c-1].occupied == false && c > 1)           //left...
          {SetBall(r,c,0); ReDrawScreen();SDL_Delay(15); SDL_UpdateRect(DisplaySurface,0,0,0,0);}
         }//end of r==1...

       if(r == 2 || r == 4 || r == 6 || r == 8 || r == 10 || r == 12 || r == 14)
        {
      	 if(BallPosition[r+1][c+1].occupied == false && c < 6)     //right up...
          {SetBall(r,c,0); ReDrawScreen();SDL_Delay(15); SDL_UpdateRect(DisplaySurface,0,0,0,0);}
 		 if(BallPosition[r][c+1].occupied == false && c < 6)          //right...
          {SetBall(r,c,0); ReDrawScreen();SDL_Delay(15); SDL_UpdateRect(DisplaySurface,0,0,0,0);}
         if(BallPosition[r+1][c].occupied == false)                 //left up...
          {SetBall(r,c,0); ReDrawScreen();SDL_Delay(15); SDL_UpdateRect(DisplaySurface,0,0,0,0);}
         if(BallPosition[r][c-1].occupied == false && c > 1)           //left...
          {SetBall(r,c,0); ReDrawScreen();SDL_Delay(15); SDL_UpdateRect(DisplaySurface,0,0,0,0);}
        }//end of r=2...
        
        if(r == 3 || r == 5 || r == 7 || r == 9 || r == 11 || r == 13 || r == 15)
        {
      	 if(BallPosition[r-1][c].occupied == false)                 //down right
          {SetBall(r,c,0); ReDrawScreen();SDL_Delay(15); SDL_UpdateRect(DisplaySurface,0,0,0,0);}
         if(BallPosition[r-1][c-1].occupied == false && c > 1)      //down left
          {SetBall(r,c,0); ReDrawScreen();SDL_Delay(15); SDL_UpdateRect(DisplaySurface,0,0,0,0);}
         }//end of r==1...

       if(r == 2 || r == 4 || r == 6 || r == 8 || r == 10 || r == 12 || r == 14)
        {
      	 if(BallPosition[r-1][c+1].occupied == false && c < 6)      //down right
          {SetBall(r,c,0); ReDrawScreen();SDL_Delay(15); SDL_UpdateRect(DisplaySurface,0,0,0,0);}
         if(BallPosition[r-1][c].occupied == false)                 //down left
          {SetBall(r,c,0); ReDrawScreen();SDL_Delay(15); SDL_UpdateRect(DisplaySurface,0,0,0,0);}
        }//end of r=2...
        
      }//end if colour > 200...
    }//end for c...
  }//end for r...

 for(r = 0; r < 15;r++)
  {
   for(c = 0; c < 7;c++)
    {
     if(BallPosition[r][c].colour > 200)
      {
       if(r == 1 || r == 3 || r == 5 || r == 7 || r == 9 || r == 11 || r == 13 || r == 15)
        {
      	 if(BallPosition[r+1][c].occupied == false)                //right up...
          {SetBall(r,c,0); ReDrawScreen();SDL_Delay(15); SDL_UpdateRect(DisplaySurface,0,0,0,0);}
 		 if(BallPosition[r][c+1].occupied == false && c < 6)          //right...
          {SetBall(r,c,0); ReDrawScreen();SDL_Delay(15); SDL_UpdateRect(DisplaySurface,0,0,0,0);}
         if(BallPosition[r+1][c-1].occupied == false && c > 1)      //left up...
          {SetBall(r,c,0); ReDrawScreen();SDL_Delay(15); SDL_UpdateRect(DisplaySurface,0,0,0,0);}
         if(BallPosition[r][c-1].occupied == false && c > 1)           //left...
          {SetBall(r,c,0); ReDrawScreen();SDL_Delay(15); SDL_UpdateRect(DisplaySurface,0,0,0,0);}
         }//end of r==1...

       if(r == 2 || r == 4 || r == 6 || r == 8 || r == 10 || r == 12 || r == 14)
        {
      	 if(BallPosition[r+1][c+1].occupied == false && c < 6)     //right up...
          {SetBall(r,c,0); ReDrawScreen();SDL_Delay(15); SDL_UpdateRect(DisplaySurface,0,0,0,0);}
 		 if(BallPosition[r][c+1].occupied == false && c < 6)          //right...
          {SetBall(r,c,0); ReDrawScreen();SDL_Delay(15); SDL_UpdateRect(DisplaySurface,0,0,0,0);}
         if(BallPosition[r+1][c].occupied == false)                 //left up...
          {SetBall(r,c,0); ReDrawScreen();SDL_Delay(15); SDL_UpdateRect(DisplaySurface,0,0,0,0);}        
         if(BallPosition[r][c-1].occupied == false && c > 1)           //left...
          {SetBall(r,c,0); ReDrawScreen();SDL_Delay(15); SDL_UpdateRect(DisplaySurface,0,0,0,0);}          
        }//end of r=2...
        
        if(r == 3 || r == 5 || r == 7 || r == 9 || r == 11 || r == 13 || r == 15)
        {
      	 if(BallPosition[r-1][c].occupied == false)           	 //down right...
          {SetBall(r,c,0); ReDrawScreen();SDL_Delay(15); SDL_UpdateRect(DisplaySurface,0,0,0,0);}
         if(BallPosition[r-1][c-1].occupied == false && c > 1)    //down left...
          {SetBall(r,c,0); ReDrawScreen();SDL_Delay(15); SDL_UpdateRect(DisplaySurface,0,0,0,0);}
         }//end of r==1...

       if(r == 2 || r == 4 || r == 6 || r == 8 || r == 10 || r == 12 || r == 14)
        {
      	 if(BallPosition[r-1][c+1].occupied == false && c < 6)   //down right...
          {SetBall(r,c,0); ReDrawScreen();SDL_Delay(15); SDL_UpdateRect(DisplaySurface,0,0,0,0);}
         if(BallPosition[r-1][c].occupied == false)               //down left...
          {SetBall(r,c,0); ReDrawScreen();SDL_Delay(15); SDL_UpdateRect(DisplaySurface,0,0,0,0);}
        }//end of r=2...
        
      }//end if colour > 200...
    }//end for c...
  }//end for r...


  DropAllBalls();
}















///////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////

void DropAllBalls()
{
int c, r;
bool IDroppedSomething;
IDroppedSomething=false;

for(int RepeatMe=0; RepeatMe < 15; RepeatMe++)
{

for(r=2;r<15;r++)
{
//IDroppedSomething=false;

if(r==2 || r==4 || r==6 || r==8 || r==10 || r==12 || r==14)
 {
 for(c=1; c<7; c++)
  {
  if(BallPosition[r][c].colour > 0 && BallPosition[r][c].colour < 200 && BallPosition[r-1][c].occupied == 0)
   {
    BallPosition[r-1][c].colour = BallPosition[r][c].colour;
    BallPosition[r][c].colour = 0;
    BallPosition[r][c].occupied = false;
    BallPosition[r-1][c].occupied = true;
    IDroppedSomething=true;
   }
  if(BallPosition[r][c].colour > 0 && BallPosition[r][c].colour < 200  && BallPosition[r-1][c+1].occupied == 0 && c < 6)
   {
    BallPosition[r-1][c+1].colour = BallPosition[r][c].colour;
    BallPosition[r][c].colour = 0;
    BallPosition[r][c].occupied = false;
    BallPosition[r-1][c+1].occupied = true;
    IDroppedSomething=true;
   }
  }//end for c...
 }//end if r...


if(r==3 || r==5 || r==7 || r==9 || r==11 || r==13)
 {
 for(c=1; c<7; c++)
 {
 if(BallPosition[r][c].colour > 0 && BallPosition[r][c].colour < 200  && BallPosition[r-1][c-1].occupied == 0 && c > 1)
   {
    BallPosition[r-1][c-1].colour = BallPosition[r][c].colour;
    BallPosition[r-1][c-1].occupied = true;
    BallPosition[r][c].colour = 0;
    BallPosition[r][c].occupied = false;
    IDroppedSomething=true;
   }

  if(BallPosition[r][c].colour > 0 && BallPosition[r][c].colour < 200 && BallPosition[r-1][c].occupied == 0)
   {
    BallPosition[r-1][c].colour = BallPosition[r][c].colour;
    BallPosition[r-1][c].occupied = true;
    BallPosition[r][c].colour = 0;
    BallPosition[r][c].occupied = false;
    IDroppedSomething=true;
   }
  }//end for c...
 }//end if r...
if(IDroppedSomething==true){IDroppedSomething=false;ReDrawScreen();SDL_UpdateRect(DisplaySurface,0,0,0,0);SDL_Delay(10);}
}//end for r...
}//end for RepeatMe...
KillBalls();
} // end DropAllBalls()...
