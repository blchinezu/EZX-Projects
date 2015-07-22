/* 
Code		->	RomanH, 2004/2005
License		->	GPL License (see http://www.gnu.org)
*/

#include "variables.h"
int RowType;  // is it gerade or ungerade
int ColType;  // is it gerade or ungerade
bool GetNextColour = false;
bool IceWasKilled;

void DropBall(int xPos, int yPos)
{
IceWasKilled = false;     
GetPosition(xPos, yPos);
if(FallingBallRow == 1){BallStopped = true;}

if(RowType==1 && ColType == 1)
{
if(FallingBallCol == 1){KillIce(FallingBallRow-1,1,1);}
if(FallingBallCol == 2){KillIce(FallingBallRow-1,1,2);}
if(FallingBallCol == 3){KillIce(FallingBallRow-1,2,3);}
if(FallingBallCol == 4){KillIce(FallingBallRow-1,3,4);}
if(FallingBallCol == 5){KillIce(FallingBallRow-1,4,5);}
if(FallingBallCol == 6){KillIce(FallingBallRow-1,5,6);}
}

if(RowType==1 && ColType == 2)
{
if(FallingBallCol == 1){KillIce(FallingBallRow-1,1,1);}
if(FallingBallCol == 2){KillIce(FallingBallRow-1,2,2);}
if(FallingBallCol == 3){KillIce(FallingBallRow-1,3,3);}
if(FallingBallCol == 4){KillIce(FallingBallRow-1,4,4);}
if(FallingBallCol == 5){KillIce(FallingBallRow-1,5,5);}
if(FallingBallCol == 6){KillIce(FallingBallRow-1,6,6);}
}

if(RowType==2 && ColType == 1)
{
if(FallingBallCol == 1){KillIce(FallingBallRow-1,1,1);}
if(FallingBallCol == 2){KillIce(FallingBallRow-1,2,2);}
if(FallingBallCol == 3){KillIce(FallingBallRow-1,3,3);}
if(FallingBallCol == 4){KillIce(FallingBallRow-1,4,4);}
if(FallingBallCol == 5){KillIce(FallingBallRow-1,5,5);}
if(FallingBallCol == 6){KillIce(FallingBallRow-1,6,6);}
}

if(RowType==2 && ColType == 2)
{
if(FallingBallCol == 1){KillIce(FallingBallRow-1,1,2);}
if(FallingBallCol == 2){KillIce(FallingBallRow-1,2,3);}
if(FallingBallCol == 3){KillIce(FallingBallRow-1,3,4);}
if(FallingBallCol == 4){KillIce(FallingBallRow-1,4,5);}
if(FallingBallCol == 5){KillIce(FallingBallRow-1,5,6);}
if(FallingBallCol == 6){KillIce(FallingBallRow-1,6,6);}
}
//################################
//################################
//################################
//################################
//################################
//################################
//################################
//################################
//################################
//################################
//################################
//################################
//################################
//################################
//################################
//################################
//################################
//################################
//################################
//################################
//################################
//################################
//################################
//################################
//################################

// all uneven positions 
// all uneven lines
// col > 1
if(RowType==1 && FallingBallRow != 1 && FallingBallCol > 1 && ColType == 1)
{
	if(BallPosition[FallingBallRow-1][FallingBallCol-1].occupied == 0)
	{
		if(BallPosition[FallingBallRow-1][FallingBallCol].occupied == 1)
		{
		//Fall down to the left!
		FallingBallX = BallPosition[FallingBallRow-1][FallingBallCol-1].x;
		/////  
        BallHasEnergy=false;
        /////
		}
	}
	if(BallPosition[FallingBallRow-1][FallingBallCol-1].occupied == 1)
	{
		if(BallPosition[FallingBallRow-1][FallingBallCol].occupied == 0)
		{
		//Fall down to the right!
		FallingBallX = BallPosition[FallingBallRow-1][FallingBallCol].x;
		/////  
        BallHasEnergy=false;
        /////		
		}
		if(BallPosition[FallingBallRow-1][FallingBallCol].occupied == 1)
		{
		//Stop!!!
		BallStopped = true;
		/////  
        BallHasEnergy=false;
        /////		
		}
	}
}

// all even positions 
// all uneven lines
// col > 1
if(RowType==1 && FallingBallRow != 1 && FallingBallCol > 1 && ColType == 2)
{
	if(BallPosition[FallingBallRow-1][FallingBallCol].occupied == 1)
	{
		FallingBallX = BallPosition[FallingBallRow][FallingBallCol].x; //???
		/////  
        BallHasEnergy=false;
        /////		
		if(BallPosition[FallingBallRow-1][FallingBallCol-1].occupied == 1)
		{
		//Stop!!!
		BallStopped = true;
		/////  
        BallHasEnergy=false;
        /////		
		}
	}
}

// all even positions 
// all uneven lines
// col == 1
if(RowType==1 && FallingBallRow != 1 && FallingBallCol == 1 && ColType == 2)
{
	if(BallPosition[FallingBallRow-1][FallingBallCol].occupied == 1)
	{
		FallingBallX = BallPosition[FallingBallRow][FallingBallCol-1].x;
		BallStopped = true;
		/////  
        BallHasEnergy=false;
        /////		
	}
}

// uneven position 1
// all uneven lines
// col == 1
if(RowType==1 && FallingBallRow != 1 && FallingBallCol == 1 && ColType == 1)
{
	if(BallPosition[FallingBallRow-1][FallingBallCol].occupied == 1)
	{
		BallStopped = true;
		/////  
        BallHasEnergy=false;
        /////		
	}
}

// even positions number 6
// all uneven lines
// col == 6
if(RowType==1 && FallingBallRow != 1 && FallingBallCol == 6 && ColType==2)
{
	if(BallPosition[FallingBallRow-1][FallingBallCol].occupied == 1)
	{
		FallingBallX = BallPosition[FallingBallRow][FallingBallCol].x;
		/////  
        BallHasEnergy=false;
        /////		
		if(BallPosition[FallingBallRow-1][FallingBallCol-1].occupied == 1)
		{
			BallStopped = true;
		/////  
        BallHasEnergy=false;
        /////			
		}
	}
}
//################################
//################################
//################################
//################################
//################################

// all even positions 
// all even lines
// col < 6
if(RowType==2 && FallingBallRow != 1 && FallingBallCol < 6 && ColType == 2)
{
	if(BallPosition[FallingBallRow-1][FallingBallCol].occupied == 0)
	{
		if(BallPosition[FallingBallRow-1][FallingBallCol+1].occupied == 1)
		{
			FallingBallX = BallPosition[FallingBallRow-1][FallingBallCol].x;
		/////  
        BallHasEnergy=false;
        /////			
		}
	}
	if(BallPosition[FallingBallRow-1][FallingBallCol].occupied == 1)
	{
		if(BallPosition[FallingBallRow-1][FallingBallCol+1].occupied == 0)
		{
			FallingBallX = BallPosition[FallingBallRow-1][FallingBallCol+1].x;
		/////  
        BallHasEnergy=false;
        /////			
		}
		if(BallPosition[FallingBallRow-1][FallingBallCol+1].occupied == 1)
		{
			BallStopped = true;
		/////  
        BallHasEnergy=false;
        /////			
		}
	}
}


// all uneven positions 
// all even lines
// col > 1
if(RowType==2 && FallingBallRow != 1 && FallingBallCol > 1 && ColType == 1)
{
	if(BallPosition[FallingBallRow-1][FallingBallCol].occupied == 1)
	{
		FallingBallX = BallPosition[FallingBallRow][FallingBallCol-1].x;
		/////  
        BallHasEnergy=false;
        /////		
		if(BallPosition[FallingBallRow-1][FallingBallCol-1].occupied == 1)
		{
			BallStopped = true;
		/////  
        BallHasEnergy=false;
        /////			
		}
	}
}

// all uneven position 1 
// all even lines
// col == 1
if(RowType==2 && FallingBallRow != 1 && FallingBallCol == 1 && ColType == 1)
{
	if(BallPosition[FallingBallRow-1][FallingBallCol].occupied == 1)
	{
		FallingBallX = BallPosition[FallingBallRow][FallingBallCol].x;
		/////  
        BallHasEnergy=false;
        /////		
		if(BallPosition[FallingBallRow-1][FallingBallCol+1].occupied == 1)
		{
			BallStopped = true;
		/////  
        BallHasEnergy=false;
        /////			
		}
	}
}

// all even position 6 
// all even lines
// col == 6
if(RowType==2 && FallingBallRow != 1 && FallingBallCol == 6 && ColType == 2)
{
	if(BallPosition[FallingBallRow-1][FallingBallCol].occupied == 1)
	{
		BallStopped = true;
		/////  
        BallHasEnergy=false;
        /////		
	}
}


//################################
//################################
//################################
//################################
//################################
if(BallStopped == true)
	{
		GetPosition(FallingBallX, FallingBallY);
		BallPosition[FallingBallRow][FallingBallCol].occupied = true;
		BallPosition[FallingBallRow][FallingBallCol].colour = BallColour[0];
		FallingBallX = BallPosition[FallingBallRow][FallingBallCol].x;
		FallingBallY = BallPosition[FallingBallRow][FallingBallCol].y;
        //if(FallingBallRow == 16-LevelHeight){GameOver=true;}  //stops game if ball stops on row 15
        GetNextColour=true;
        KillBalls();
        KillBubbles();
//        DropAllBalls();
	}
if(BallStopped == false){FallingBallY = yPos + 1;}
}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

void GetPosition(int PosX, int PosY)

{
	if(PosY == BallPosition[1][1].y){FallingBallRow = 1; RowType = 1;}
	if(PosY == BallPosition[2][1].y){FallingBallRow = 2; RowType = 2;}
	if(PosY == BallPosition[3][1].y){FallingBallRow = 3; RowType = 1;}
	if(PosY == BallPosition[4][1].y){FallingBallRow = 4; RowType = 2;}
	if(PosY == BallPosition[5][1].y){FallingBallRow = 5; RowType = 1;}
	if(PosY == BallPosition[6][1].y){FallingBallRow = 6; RowType = 2;}
	if(PosY == BallPosition[7][1].y){FallingBallRow = 7; RowType = 1;}
	if(PosY == BallPosition[8][1].y){FallingBallRow = 8; RowType = 2;}
	if(PosY == BallPosition[9][1].y){FallingBallRow = 9; RowType = 1;}
	if(PosY == BallPosition[10][1].y){FallingBallRow = 10; RowType = 2;}
	if(PosY == BallPosition[11][1].y){FallingBallRow = 11; RowType = 1;}
	if(PosY == BallPosition[12][1].y){FallingBallRow = 12; RowType = 2;}
	if(PosY == BallPosition[13][1].y){FallingBallRow = 13; RowType = 1;}
	if(PosY == BallPosition[14][1].y){FallingBallRow = 14; RowType = 2;}
	if(PosY == BallPosition[15][1].y){FallingBallRow = 15; RowType = 1;}

	if(PosX == BallPosition[1][1].x){FallingBallCol = 1; ColType = 1;}
	if(PosX == BallPosition[2][1].x){FallingBallCol = 1; ColType = 2;}
	if(PosX == BallPosition[1][2].x){FallingBallCol = 2; ColType = 1;}
	if(PosX == BallPosition[2][2].x){FallingBallCol = 2; ColType = 2;}
	if(PosX == BallPosition[1][3].x){FallingBallCol = 3; ColType = 1;}
	if(PosX == BallPosition[2][3].x){FallingBallCol = 3; ColType = 2;}
	if(PosX == BallPosition[1][4].x){FallingBallCol = 4; ColType = 1;}
	if(PosX == BallPosition[2][4].x){FallingBallCol = 4; ColType = 2;}
	if(PosX == BallPosition[1][5].x){FallingBallCol = 5; ColType = 1;}
	if(PosX == BallPosition[2][5].x){FallingBallCol = 5; ColType = 2;}
	if(PosX == BallPosition[1][6].x){FallingBallCol = 6; ColType = 1;}
	if(PosX == BallPosition[2][6].x){FallingBallCol = 6; ColType = 2;}
}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

void GetNextBallColour()
{
             int CountTheRounds, i;
             BallColour[0] = BallColour[1];
             for(i=1;i<14;i++){ColourPresent[i]=false;}

			 for(int row = 1; row<15; row++){
              for(int col = 1; col<30; col++){
		       if(BallPosition[row][col].colour > 0 && BallPosition[row][col].colour < 11)
				{ColourPresent[BallPosition[row][col].colour]=true;}
		       if(BallPosition[row][col].colour-20 > 0 && BallPosition[row][col].colour-20 < 11)
				{ColourPresent[BallPosition[row][col].colour-20]=true;}
              }}

             BallColour[1]++;
             if(BallColour[1]>7){BallColour[1]=1;}

             CountTheRounds = 0;

             while(ColourPresent[BallColour[1]]==false && CountTheRounds < 20)
             {
             CountTheRounds++;
             BallColour[1]++;
             if(BallColour[1]>7){BallColour[1]=1;}
             }
             if(CountTheRounds>15){LevelNumber++; LoadLevel(LevelNumber); DrawGuidepoints=false;}//GameOver = true;} //keine Kugeln übrig -> naechstes Level laden!!!
}

