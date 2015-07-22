/* 
Code		->	RomanH, 2004/2005
License		->	GPL License (see http://www.gnu.org)
*/

#include "variables.h"

void defreezeBall(int row, int col)
{
if(col<6){if(BallPosition[row][col+1].colour > 20 && BallPosition[row][col+1].colour < 30){BallPosition[row][col+1].colour -= 20;}}
if(col>1){if(BallPosition[row][col-1].colour > 20 && BallPosition[row][col-1].colour < 30){BallPosition[row][col-1].colour -= 20;}}

if(row==1 || row==3 || row==5 || row==7 || row==9 || row==11 || row==13)
{
if(BallPosition[row+1][col].colour > 20 && BallPosition[row+1][col].colour < 30){BallPosition[row+1][col].colour -= 20;}
if(col>1){if(BallPosition[row+1][col-1].colour > 20 && BallPosition[row+1][col-1].colour < 30){BallPosition[row+1][col-1].colour -= 20;}}
if(row>1)
  {
    if(BallPosition[row-1][col].colour > 20 && BallPosition[row-1][col].colour < 30){BallPosition[row-1][col].colour -= 20;}
    if(col>1){if(BallPosition[row-1][col-1].colour > 20 && BallPosition[row-1][col-1].colour < 30){BallPosition[row-1][col-1].colour -= 20;}}
  }
}

if(row==2 || row==4 || row==6 || row==8 || row==10 || row==12 || row==14)
{
if(BallPosition[row-1][col].colour > 20 && BallPosition[row-1][col].colour < 30){BallPosition[row-1][col].colour -= 20;}
if(col<6){if(BallPosition[row-1][col+1].colour > 20 && BallPosition[row-1][col+1].colour < 30){BallPosition[row-1][col+1].colour -= 20;}}
}

}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

void KillIce(int row, int col1, int col2)
{
if(BallHasEnergy == true)
{
if(BallPosition[row][col1].colour>100 && BallPosition[row][col1].colour<150)
{
  BallPosition[row][col1].colour = 0;
  BallPosition[row][col1].occupied = false;
  IceWasKilled = true;
  BallHasEnergy=false;
}
if(BallPosition[row][col2].colour>100 && BallPosition[row][col2].colour<150)
{
  BallPosition[row][col2].colour = 0;
  BallPosition[row][col2].occupied = false;
  IceWasKilled = true;
  BallHasEnergy=false; 
}
}
}

