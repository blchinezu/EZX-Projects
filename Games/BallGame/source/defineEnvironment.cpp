/* 
Code		->	RomanH, 2004/2005
License		->	GPL License (see http://www.gnu.org)
*/

#include "variables.h"

Position BallPosition[30][30];

void setPositions()
{
int i;
int j;
	for(i = 1; i<30; i++){
		for(j = 1; j<30; j++){
			BallPosition[i][j].occupied = false;
		}
	}

	BallPosition[1][1].x = 0;
	BallPosition[1][2].x = 16;
	BallPosition[1][3].x = 32;
	BallPosition[1][4].x = 48;
	BallPosition[1][5].x = 64;
	BallPosition[1][6].x = 80;

	BallPosition[3][1].x = 0;
	BallPosition[3][2].x = 16;
	BallPosition[3][3].x = 32;
	BallPosition[3][4].x = 48;
	BallPosition[3][5].x = 64;
	BallPosition[3][6].x = 80;

	BallPosition[5][1].x = 0;
	BallPosition[5][2].x = 16;
	BallPosition[5][3].x = 32;
	BallPosition[5][4].x = 48;
	BallPosition[5][5].x = 64;
	BallPosition[5][6].x = 80;

	BallPosition[7][1].x = 0;
	BallPosition[7][2].x = 16;
	BallPosition[7][3].x = 32;
	BallPosition[7][4].x = 48;
	BallPosition[7][5].x = 64;
	BallPosition[7][6].x = 80;

	BallPosition[9][1].x = 0;
	BallPosition[9][2].x = 16;
	BallPosition[9][3].x = 32;
	BallPosition[9][4].x = 48;
	BallPosition[9][5].x = 64;
	BallPosition[9][6].x = 80;

	BallPosition[11][1].x = 0;
	BallPosition[11][2].x = 16;
	BallPosition[11][3].x = 32;
	BallPosition[11][4].x = 48;
	BallPosition[11][5].x = 64;
	BallPosition[11][6].x = 80;

	BallPosition[13][1].x = 0;
	BallPosition[13][2].x = 16;
	BallPosition[13][3].x = 32;
	BallPosition[13][4].x = 48;
	BallPosition[13][5].x = 64;
	BallPosition[13][6].x = 80;

	BallPosition[15][1].x = 0;
	BallPosition[15][2].x = 16;
	BallPosition[15][3].x = 32;
	BallPosition[15][4].x = 48;
	BallPosition[15][5].x = 64;
	BallPosition[15][6].x = 80;


	BallPosition[2][1].x = 8;
	BallPosition[2][2].x = 24;
	BallPosition[2][3].x = 40;
	BallPosition[2][4].x = 56;
	BallPosition[2][5].x = 72;
	BallPosition[2][6].x = 88;

	BallPosition[4][1].x = 8;
	BallPosition[4][2].x = 24;
	BallPosition[4][3].x = 40;
	BallPosition[4][4].x = 56;
	BallPosition[4][5].x = 72;
	BallPosition[4][6].x = 88;

	BallPosition[6][1].x = 8;
	BallPosition[6][2].x = 24;
	BallPosition[6][3].x = 40;
	BallPosition[6][4].x = 56;
	BallPosition[6][5].x = 72;
	BallPosition[6][6].x = 88;

	BallPosition[8][1].x = 8;
	BallPosition[8][2].x = 24;
	BallPosition[8][3].x = 40;
	BallPosition[8][4].x = 56;
	BallPosition[8][5].x = 72;
	BallPosition[8][6].x = 88;

	BallPosition[10][1].x = 8;
	BallPosition[10][2].x = 24;
	BallPosition[10][3].x = 40;
	BallPosition[10][4].x = 56;
	BallPosition[10][5].x = 72;
	BallPosition[10][6].x = 88;

	BallPosition[12][1].x = 8;
	BallPosition[12][2].x = 24;
	BallPosition[12][3].x = 40;
	BallPosition[12][4].x = 56;
	BallPosition[12][5].x = 72;
	BallPosition[12][6].x = 88;

	BallPosition[14][1].x = 8;
	BallPosition[14][2].x = 24;
	BallPosition[14][3].x = 40;
	BallPosition[14][4].x = 56;
	BallPosition[14][5].x = 72;
	BallPosition[14][6].x = 88;

	BallPosition[16][1].x = 8;
	BallPosition[16][2].x = 24;
	BallPosition[16][3].x = 40;
	BallPosition[16][4].x = 56;
	BallPosition[16][5].x = 72;
	BallPosition[16][6].x = 88;

	SetLevelHeight(1);
}


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

void SetLevelHeight(int height)
{
if(LevelHeight>15){LevelHeight=15;height=15;}
if(LevelHeight<1) {LevelHeight=1;height=1;}
int i;
	for(i = 1; i<7; i++){
	BallPosition[1][i].y = 184-13*(height-1);
	BallPosition[2][i].y = 171-13*(height-1);
    BallPosition[3][i].y = 158-13*(height-1);
	BallPosition[4][i].y = 145-13*(height-1);
    BallPosition[5][i].y = 132-13*(height-1);
	BallPosition[6][i].y = 119-13*(height-1);
    BallPosition[7][i].y = 106-13*(height-1);
	BallPosition[8][i].y = 93-13*(height-1);
	BallPosition[9][i].y = 80-13*(height-1);
	BallPosition[10][i].y = 67-13*(height-1);
	BallPosition[11][i].y = 54-13*(height-1);
	BallPosition[12][i].y = 41-13*(height-1);
	BallPosition[13][i].y = 28-13*(height-1);
	BallPosition[14][i].y = 15-13*(height-1);
    BallPosition[15][i].y = 2-13*(height-1);
	BallPosition[16][i].y = -11-13*(height-1);
	} // end of for i
}


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

void SetBall(int row, int col, int colour)
{
     if(colour>0)
     {
     BallPosition[row][col].colour = colour;	
     BallPosition[row][col].occupied = true;
     }
     if(colour==0)
     {
     BallPosition[row][col].colour = 0;	
     BallPosition[row][col].occupied = false;
     }

}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

void SetLine(int row, int c1, int c2, int c3, int c4, int c5, int c6)
{
     SetBall(row,1,c1);
     SetBall(row,2,c2);
     SetBall(row,3,c3);
     SetBall(row,4,c4);
     SetBall(row,5,c5);
     SetBall(row,6,c6);          
}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////


void ClearLevel()
{
	for(int r = 1; r < 16;r++)
	{
		for(int c = 1; c < 7;c++)
		{
		BallPosition[r][c].occupied = false;
		BallPosition[r][c].colour = 0;
		} 
	}
}

