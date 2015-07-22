/* 
Code		->	RomanH, 2004/2005
License		->	GPL License (see http://www.gnu.org)
*/

#include "SDL/SDL.h"				// include SDL stuff
#include <stdlib.h>				// include ability to exit program
#include <stdio.h>				// ***include ability to read/write files
#include <unistd.h>    //needed for graceful exit...

// FUNCTIONS
	void getInput();
	void intro();
	void initSDL();
	void setSurfaces();
	void setPositions();
	void defineSprites();
	void DrawBall(int left, int top, int colour);
	void game_singleplayer();
	void DrawBackground();
    void DrawFlipper(int ypos);
    void DrawLever(int state);
    void DrawGuidePoint(int x, int y);
    void DrawBar();
	void DropBall(int xPos, int yPos);
	void DropAllBalls();
	void GetPosition(int PosX, int PosY);
    void KillBalls();
    void KillBubbles();
    void KillIce(int row, int col1, int col2);
    void ClearLevel();
    void ReDrawScreen();
    void SetBall(int row, int col, int colour);
    void SetLine(int row, int c1, int c2, int c3, int c4, int c5, int c6);
    void defreezeBall(int row, int col);
    void LoadLevel(int Level);
    void SetLevelHeight(int height);
    void animateTiles();
    void GetNextBallColour();
    void ShootingAnimation();
// FUNCTIONS

// SURFACES
	extern SDL_Surface* DisplaySurface;			//display surface
	extern SDL_Surface* SpritesSurface;			//sprites surface
	extern SDL_Surface* FontSurface;			//font surface
// SURFACES

// EVENT STUFF
	extern SDL_Event sdlEvent;				//event structure
// EVENT STUFF

// INPUT STUFF
	extern SDL_Joystick* pStick;
	extern int nStickButtons;
	extern int StickAxis[2];
	extern int JoyButtonIndex;
	extern Uint8 *keys;

	extern int Key_UP;
	extern int Key_DOWN;
	extern int Key_LEFT;
	extern int Key_RIGHT;
	extern int Button_0;
	extern int Button_1;
	extern int Button_2;
	extern int Button_3;
	extern int Key_ESCAPE; extern int Key_ESCAPE_pressed_value; extern bool Key_ESCAPE_pressed;
	extern int Key_ENTER;  extern int Key_ENTER_pressed_value;  extern bool Key_ENTER_pressed;	
	extern int Key_1;      extern int Key_1_pressed_value;      extern bool Key_1_pressed;
	extern int Key_2;      extern int Key_2_pressed_value;      extern bool Key_2_pressed;
	extern int Key_3;      extern int Key_3_pressed_value;      extern bool Key_3_pressed;
	extern int Key_4;      extern int Key_4_pressed_value;      extern bool Key_4_pressed;
	extern int Key_5;      extern int Key_5_pressed_value;      extern bool Key_5_pressed;
	extern int Key_6;      extern int Key_6_pressed_value;      extern bool Key_6_pressed;
	extern int Key_7;      extern int Key_7_pressed_value;      extern bool Key_7_pressed;
	extern int Key_8;      extern int Key_8_pressed_value;      extern bool Key_8_pressed;
    extern int Key_9;      extern int Key_9_pressed_value;      extern bool Key_9_pressed;
	extern int Key_0;      extern int Key_0_pressed_value;      extern bool Key_0_pressed;            	
// INPUT STUFF

// TEXT ENGINE
	void DrawChar(int left, int top, char letter);
	void Print(int left, int top, char *str);
// TEXT ENGINE

// SPRITES
	extern SDL_Rect Ball[300];		//[Colour]
    extern SDL_Rect Bubble;
	extern SDL_Rect BG_Single;
    extern SDL_Rect FlipperTop;     //= top of "flipper"
    extern SDL_Rect FlipperBottom;  //= top of "flipper"
    extern SDL_Rect Lever[2];       //= Kette, die gezogen wird
    extern SDL_Rect GuidePoint;
    extern SDL_Rect Bar;
    extern bool ColourPresent[15];
// SPRITES

// SDL STUFF
	extern SDL_Rect SrcRect,DstRect;		//source and destination rectangles
// SDL STUFF

// SCREEN
	extern const int SCREEN_WIDTH;		//screen dimensions
	extern const int SCREEN_HEIGHT;  	//screen dimensions
	extern int intFullscreen;			//0 = windowed, 1 = fullscreen
// SCREEN

//BALLPOSITIONS
extern int FallingBallX;
extern int FallingBallY;
extern int RowType;  // is it gerade or ungerade
extern int ColType;  // is it gerade or ungerade
extern int FallingBallX;
extern int FallingBallY;
extern int FallingBallRow;
extern int FallingBallCol;
extern bool BallStopped;
extern int StartPosition;
//BALLPOSITIONS

// STRUCTS
typedef struct {
	Sint32 x, y;
	Sint32 colour;
	Sint32 state;
	bool occupied;
} Position;

extern Position BallPosition[30][30];
// STRUCTS

//Global Stuff
extern bool GameOver;
extern bool BallHasEnergy;
extern bool IceWasKilled;
extern int LevelNumber;
extern int LevelHeight;
extern int BallColour[2];
extern bool GetNextColour;
extern SDL_Rect BGDstRect;
extern SDL_Rect BGSrcRect;
//Global Stuff

extern bool DrawUnshotBall;
extern bool ShootingAnimationRunning;
extern int ShootingAnimationPosition;
extern int ShootingAnimationStop;
extern int ShootingCounter;
extern int LevelRaiseCounter;
extern bool RaiseLevel;
extern bool DrawGuidepoints;
