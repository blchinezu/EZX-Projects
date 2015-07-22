/* 
Code		->	RomanH, 2004/2005
License		->	GPL License (see http://www.gnu.org)
*/

#include "variables.h"

Uint32 timecounter_singleplayer;
Uint32 timecounter_singleplayer_input;
Uint32 timecounter_animation;
Uint32 timecounter_shootingcountdown;
SDL_Rect BGDstRect;
SDL_Rect BGSrcRect;
int FallingBallX;
int FallingBallY;
int FallingBallRow;
int FallingBallCol;
bool BallStopped;
bool BallActivated;
bool BallHasEnergy;
int StartPosition;
bool ReallyGetNextColour;
bool DrawUnshotBall;
bool ExitGame;
bool ShootingAnimationRunning;
int ShootingAnimationPosition;
int ShootingAnimationStop;
int ShootingCounter;
bool ShootBall;
bool RaiseLevel;
bool DrawGuidepoints;
void ContinueHere();

void game_singleplayer()
{
    int i, j;
    RaiseLevel=false;
    DrawGuidepoints=true;
	FallingBallY = BallPosition[15][1].y;
	BallStopped = true;
    GetNextColour = false;
    BallHasEnergy = false;
    ReallyGetNextColour = false;
    ExitGame = false;
    ShootingAnimationRunning = false;
	StartPosition = 1;
	ShootingCounter = 5;
    BGDstRect.x = 80;		BGDstRect.y = 0;
	BGDstRect.w = 160;		BGDstRect.h = 240;
    timecounter_shootingcountdown = SDL_GetTicks();
    GameOver = false;
    
	while(ExitGame == 0 && GameOver == false)
	{
        ////////////////////////////
        //get keyboard and joystick input
 		getInput();
        ////////////////////////////

        ////////////////////////////
        //escape get's you back to the intro screen
        if(Key_ESCAPE_pressed == true){ExitGame=true;}
        ////////////////////////////

        ////////////////////////////
        //change level height
        if(Key_9_pressed == true){LevelHeight--; SetLevelHeight(LevelHeight);}
        if(Key_0_pressed == true){LevelHeight++; SetLevelHeight(LevelHeight);}                     
        ////////////////////////////

		if(SDL_GetTicks() - timecounter_singleplayer > 20)
		{
            // reset counter...
			timecounter_singleplayer = SDL_GetTicks();

            //Animation
			if(SDL_GetTicks() - timecounter_animation > 100) //change here to change speed...
			 {
              timecounter_animation = SDL_GetTicks();
              animateTiles();
             }

            //Shooting Countdown
			if(SDL_GetTicks() - timecounter_shootingcountdown > 1000) //change here to change speed...
			 {
              timecounter_shootingcountdown = SDL_GetTicks();
              ShootingCounter--;
              if(ShootingCounter==0)
              {
               ShootingCounter=5;
               ShootBall=true;
              }
             }
             
            //reset ball to starting row...
            if(BallStopped == true){FallingBallY = BallPosition[16-LevelHeight][1].y;}

            //reset Spannung auf dem Schlaeger...
   			if(BallStopped == true && BallActivated == false){StartPosition = 0;}

            //Schlaeger spannen...
			if(SDL_GetTicks() - timecounter_singleplayer_input > 60) //change here to change speed...
			{
				timecounter_singleplayer_input = SDL_GetTicks(); 	// reset counter...
                if(Key_DOWN == 0 && BallStopped == true && BallActivated == true && ShootingAnimationRunning == false) // Schlaeger zuruecksetzen
                   {
                    ShootBall=true;
                   }
				if(Key_DOWN == 1 && StartPosition<12 && BallStopped == true && ShootingAnimationRunning == false) // Schlaeger spannen
                   {
                      BallActivated = true;
                      StartPosition++;
                   }
			}


              if(ShootBall==true){
                      ShootingCounter=5;
                      ShootBall=false;
                      ShootingAnimationStop = StartPosition;
                      ShootingAnimationRunning = true;
                      ShootingAnimationPosition = 1;
                      StartPosition = 1;
                      BallActivated = false;
                      BallHasEnergy = true;
                      ReallyGetNextColour = true;
                      if(LevelRaiseCounter>0){LevelRaiseCounter--;}
                      if(LevelRaiseCounter==0){RaiseLevel=true;}
                      }

            if(ShootingAnimationRunning == false && RaiseLevel==true && BallStopped == true){RaiseLevel=false;LevelHeight++; SetLevelHeight(LevelHeight);}

            // Change here to increase droppingspeed
            for(int GameSpeed=0; GameSpeed < 8;GameSpeed++)
             {
			  if(BallStopped == false){ShootingCounter=5; timecounter_shootingcountdown = SDL_GetTicks(); DropBall(FallingBallX, FallingBallY);}
             }
            
            // getting the next ballcolour
            if(GetNextColour == true && ReallyGetNextColour == true)
            {
             GetNextBallColour();
             GetNextColour = false;
             ReallyGetNextColour = false;
            }
          
            DrawUnshotBall=true; ReDrawScreen(); DrawUnshotBall=false;   
            if(ShootingAnimationRunning == true){ShootingAnimation();}
			SDL_UpdateRect(DisplaySurface,0,0,0,0);

		}
	}
  if(GameOver==false){ClearLevel();intro();} // escape goes straight to intro
  if(GameOver==true) {ContinueHere();} // GameOver goes to Continue-Screen
}


void ContinueHere()
{
bool GotoIntro = false;
bool ContinueWithThisLevel = false;
LevelHeight=1; SetLevelHeight(LevelHeight);
ClearLevel();
ReDrawScreen();
{Print (120,80,   " PLUS:      ");}
{Print (120,90,   "   Exit     ");}
{Print (120,108,  " OK:        ");}
{Print (120,118,  "   Continue ");}
SDL_UpdateRect(DisplaySurface,0,0,0,0);


    getInput();
    while(Key_ESCAPE_pressed == false && Key_ENTER_pressed == false)
	{
     getInput();
     if(Key_ESCAPE_pressed == true){GotoIntro = true;}
     if(Key_ENTER_pressed == true) {DrawGuidepoints=true;ContinueWithThisLevel = true;}
     }
if(GotoIntro == true)             {ClearLevel();intro();}
if(ContinueWithThisLevel == true) {LoadLevel(LevelNumber); game_singleplayer();}
}
