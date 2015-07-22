/* 
Code		->	RomanH, 2004/2005
License		->	GPL License (see http://www.gnu.org)
*/

#include "variables.h"

void animateTiles()
{
int i, j;
 for(i = 1; i<15; i++){
  for(j = 1; j<7; j++){
   if(BallPosition[i][j].colour > 200){BallPosition[i][j].colour++;}
   if(BallPosition[i][j].colour > 204){BallPosition[i][j].colour=201;}
  }
 }
}

void ShootingAnimation()
{
int ShootingAniPos = 0;
if(ShootingAnimationPosition > 0  && ShootingAnimationPosition < 3)  {DrawBall(100, 109, BallColour[0]);}
if(ShootingAnimationPosition > 2  && ShootingAnimationPosition < 5)  {DrawBall(100, 89,  BallColour[0]);}
if(ShootingAnimationPosition > 4  && ShootingAnimationPosition < 7)  {DrawBall(100, 79,  BallColour[0]);}
if(ShootingAnimationPosition > 6  && ShootingAnimationPosition < 9)  {DrawBall(100, 59,  BallColour[0]);}
if(ShootingAnimationPosition > 8  && ShootingAnimationPosition < 11) {DrawBall(100, 39,  BallColour[0]);}
if(ShootingAnimationPosition > 10 && ShootingAnimationPosition < 12) {DrawBall(100, 19,  BallColour[0]);}
if(ShootingAnimationPosition == 12) {DrawBall(BallPosition[1][1].x+117, 7,  BallColour[0]); ShootingAniPos = 1;}
if(ShootingAnimationPosition == 13) {DrawBall(BallPosition[2][1].x+117, 7,  BallColour[0]); ShootingAniPos = 2;}
if(ShootingAnimationPosition == 14) {DrawBall(BallPosition[1][2].x+117, 7,  BallColour[0]); ShootingAniPos = 3;}
if(ShootingAnimationPosition == 15) {DrawBall(BallPosition[2][2].x+117, 7,  BallColour[0]); ShootingAniPos = 4;}
if(ShootingAnimationPosition == 16) {DrawBall(BallPosition[1][3].x+117, 7,  BallColour[0]); ShootingAniPos = 5;}
if(ShootingAnimationPosition == 17) {DrawBall(BallPosition[2][3].x+117, 7,  BallColour[0]); ShootingAniPos = 6;}
if(ShootingAnimationPosition == 18) {DrawBall(BallPosition[1][4].x+117, 7,  BallColour[0]); ShootingAniPos = 7;}
if(ShootingAnimationPosition == 19) {DrawBall(BallPosition[2][4].x+117, 7,  BallColour[0]); ShootingAniPos = 8;}
if(ShootingAnimationPosition == 20) {DrawBall(BallPosition[1][5].x+117, 7,  BallColour[0]); ShootingAniPos = 9;}
if(ShootingAnimationPosition == 21) {DrawBall(BallPosition[2][5].x+117, 7,  BallColour[0]); ShootingAniPos = 10;}
if(ShootingAnimationPosition == 22) {DrawBall(BallPosition[1][6].x+117, 7,  BallColour[0]); ShootingAniPos = 11;}
if(ShootingAnimationPosition == 23) {DrawBall(BallPosition[2][6].x+117, 7,  BallColour[0]); ShootingAniPos = 12;}
ShootingAnimationPosition++;
if(ShootingAniPos == ShootingAnimationStop){BallStopped = false; ShootingAnimationRunning=false;}

}
