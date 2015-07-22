/* 
Code		->	RomanH, 2004/2005
License		->	GPL License (see http://www.gnu.org)
*/

#include "variables.h"
int LevelRaiseCounter;

void LoadLevel(int Level)
{
if(Level==1){Level=1;} //cheat here to test higher levels...
LevelHeight=1;
SetLevelHeight(LevelHeight);
RaiseLevel=false;

int i;
int j;
	for(i = 1; i<30; i++){
		for(j = 1; j<30; j++){
			BallPosition[i][j].occupied = false;
		}
	}

if(Level==1)
{
    //= Level 1
    LevelRaiseCounter = 3;
    BallColour[0] = 1; BallColour[1] = 2;
    SetLine(1,2,2,3,3,1,1);
    SetLine(2,1,1,2,2,3,3);    
    SetLine(3,201,201,201,201,201,201);
    SetLine(4,3,1,2,3,1,2);    
    SetLine(5,201,201,201,201,201,201);
    SetLine(6,1,2,3,1,2,3);
}

if(Level==2)
{
//= Level 2
    LevelRaiseCounter = 3;
    BallColour[0] = 1; BallColour[1] = 2;
    SetLine(1,201,201,201,201,201,201);
    SetLine(2,22,22,22,22,22,22);
    SetLine(3,21,21,1,21,21,21);
    SetLine(4,201,201,201,201,201,201);
    SetLine(5,24,4,24,24,4,24);
    SetLine(6,201,201,201,201,201,201);
    SetLine(7,2,22,2,22,2,22);
    SetLine(8,0,1,1,0,4,4);
}


if(Level==3)
{
//= Level 3
    LevelRaiseCounter = 3;
    BallColour[0] = 2; BallColour[1] = 4;
    SetLine(1,2,2,4,4,7,7);
    SetLine(2,201,201,201,201,201,201);
    SetLine(3,201,201,201,201,201,201);
    SetLine(4,101,101,101,201,201,201);
    SetLine(5,201,201,201,2,4,7);
    SetLine(6,201,201,201,201,201,201);
    SetLine(7,201,201,201,101,101,101);
    SetLine(8,101,101,101,101,101,101);
}

if(Level==4)
{
    //=Level 6
    LevelRaiseCounter = 3;
    BallColour[0] = 1; BallColour[1] = 1;
    SetBall(1,1,1);   SetBall(1,2,1);   SetBall(1,3,201);
    SetBall(1,4,201); SetBall(1,5,201); SetBall(1,6,1);
    SetBall(2,1,201); SetBall(2,2,201); SetBall(2,3,201);    
    SetBall(2,4,201); SetBall(2,5,201); SetBall(2,6,1);
    SetBall(3,1,1);   SetBall(3,2,201); SetBall(3,3,1);  
    SetBall(3,4,201); SetBall(3,5,1);   SetBall(3,6,201);
    SetBall(4,1,1);   SetBall(4,2,201); SetBall(4,3,201);    
    SetBall(4,4,1);   SetBall(4,5,201); SetBall(4,6,201);
    SetBall(5,1,201); SetBall(5,2,201); SetBall(5,3,1);  
    SetBall(5,4,201); SetBall(5,5,201); SetBall(5,6,1);
    SetBall(6,1,201); SetBall(6,2,1);   SetBall(6,3,201);    
    SetBall(6,4,1);   SetBall(6,5,201); SetBall(6,6,1);
    SetBall(7,1,1);   SetBall(7,2,201); SetBall(7,3,201);  
    SetBall(7,4,201); SetBall(7,5,201); SetBall(7,6,201);
    SetBall(8,1,1);   SetBall(8,2,201); SetBall(8,3,101);    
    SetBall(8,4,101); SetBall(8,5,201); SetBall(8,6,201);
    SetBall(9,1,151); SetBall(9,2,151); SetBall(9,3,151);  
                      SetBall(9,5,151); SetBall(9,6,151);
}

if(Level==5)
{
    //=Level 7
    LevelRaiseCounter = 8;
    BallColour[0] = 1; BallColour[1] = 2;
    SetLine(1,3,26,1,2,26,1);
    SetLine(2,1,2,26,1,5,26);
    SetLine(3,5,26,2,3,3,2);
    SetLine(4,5,22,1,1,2,3);
    SetLine(5,2,25,5,5,3,5);
    SetLine(6,3,2,2,1,5,6);
    SetLine(7,5,26,1,5,26,26);
    SetLine(8,3,3,2,2,3,5);
}
if(Level==6)
{
//= Level 8
    LevelRaiseCounter = 3;
    BallColour[0] = 1; BallColour[1] = 2;
    SetLine(1,201,201,201,201,201,201);
    SetLine(2,201,201,201,201,201,201);
    SetLine(3,4,6,2,3,1,5);
    SetLine(4,4,6,2,3,1,5);
    SetLine(5,201,201,201,201,201,201);
    SetLine(6,201,201,201,201,201,201);
    SetLine(7,4,6,2,3,1,5);
    SetLine(8,5,2,6,1,4,3);
    SetLine(9,1,5,4,6,2,3);
}

if(Level==7)
{
//= Level 9
    LevelRaiseCounter = 3;
    BallColour[0] = 1; BallColour[1] = 2;
    SetBall(1,1,151);   SetBall(1,2,151);   SetBall(1,3,151);
    SetBall(1,4,151);   SetBall(1,5,151);   SetBall(1,6,151);
    SetBall(2,1,151);   SetBall(2,2,151);   SetBall(2,3,151);
    SetBall(2,4,151);   SetBall(2,5,151);   SetBall(2,6,151);
    SetBall(3,1,151);   SetBall(3,2,151);   SetBall(3,3,151);
    SetBall(3,4,151);   SetBall(3,5,151);   SetBall(3,6,151);
    SetBall(4,1,7);     SetBall(4,2,7);     SetBall(4,3,151);
    SetBall(4,4,151);   SetBall(4,5,151);   SetBall(4,6,151);
                        SetBall(5,2,151);   SetBall(5,3,151);
    SetBall(5,4,151);   SetBall(5,5,4);     SetBall(5,6,4);
                        SetBall(6,2,1);     SetBall(6,3,151);
    SetBall(6,4,151);   SetBall(6,5,151);
                                            SetBall(7,3,1);
    SetBall(7,4,151);   SetBall(7,5,2);
                                            SetBall(8,3,151);
    SetBall(8,4,2);
                       SetBall(9,4,151);

}

if(Level==8)
{
//= Level 10
    LevelRaiseCounter = 8;
    BallColour[0] = 1; BallColour[1] = 2;
    SetLine(1,1,4,25,1,4,25);
    SetLine(2,25,1,4,25,1,4);
    SetLine(3,1,4,25,1,4,25);
    SetLine(4,25,1,4,25,1,4);
    SetLine(5,201,201,201,201,201,201);
    SetLine(6,201,7,7,6,6,201);
    SetLine(7,201,3,3,1,2,2);
    SetLine(8,7,1,7,6,1,6);
    SetLine(9,0,7,0,1,0,6);
}

if(Level==9)
{
//= Level 11
    LevelRaiseCounter = 8;
    BallColour[0] = 1; BallColour[1] = 2;
    SetLine(1,7,7,27,27,27,27);
    SetLine(2,27,27,27,27,27,27);
    SetLine(3,27,7,201,7,27,27);
    SetLine(4,27,27,7,201,27,27);
    SetLine(5,7,27,27,27,27,7);
    SetLine(6,151,151,151,151,151,201);
    SetLine(7,101,1,1,2,2,101);
    SetLine(8,2,2,4,4,101,0);
    SetLine(9,0,101,1,1,101,0);
    SetLine(10,0,3,3,101,0,0);    
    SetLine(11,0,0,101,101,0,0);     
    SetLine(12,0,0,101,0,0,0);      
}

if(Level==10)
{
//= Level 12
    LevelRaiseCounter = 12;
    BallColour[0] = 1; BallColour[1] = 2;
    SetLine(1,2,2,1,1,6,6);
    SetLine(2,201,201,201,201,201,201);
    SetLine(3,201,201,201,201,201,201);
    SetLine(4,201,201,201,201,201,201);
    SetLine(5,2,2,1,1,6,6);
    SetLine(6,5,5,4,5,5,4);
    SetLine(7,2,2,4,6,6,1);
    SetLine(8,4,6,2,1,5,4);
    SetLine(9,1,5,4,6,2,1);
    SetLine(10,151,0,0,0,0,151);     
}

if(Level==11)
{
    LevelRaiseCounter = 3;
    BallColour[0] = 1; BallColour[1] = 2;           
    SetBall(1,1,22);   SetBall(1,2,22);   SetBall(1,3,23);
    SetBall(1,4,23);   SetBall(1,5,21);   SetBall(1,6,21);
    SetBall(2,1,21);   SetBall(2,2,21);   SetBall(2,3,22);
    SetBall(2,4,22);   SetBall(2,5,23);   SetBall(2,6,23);
    SetBall(3,1,201); SetBall(3,2,201); SetBall(3,3,201);
    SetBall(3,4,201); SetBall(3,5,201); SetBall(3,6,201);
    SetBall(4,1,23);   SetBall(4,2,21);   SetBall(4,3,22);
    SetBall(4,4,23);   SetBall(4,5,21);   SetBall(4,6,22);
    SetBall(5,1,201); SetBall(5,2,201); SetBall(5,3,201);
    SetBall(5,4,201); SetBall(5,5,201); SetBall(5,6,201);
    SetBall(6,1,21);  SetBall(6,2,22);  SetBall(6,3,23);
    SetBall(6,4,21);  SetBall(6,5,22);  SetBall(6,6,23);    
    SetBall(7,1,21);                    SetBall(7,3,21);
}

if(Level==12)
{
    LevelRaiseCounter = 3;
    BallColour[0] = 1; BallColour[1] = 1;
    SetBall(1,1,2);   SetBall(1,2,2);   SetBall(1,3,3);
    SetBall(1,4,3);   SetBall(1,5,1);   SetBall(1,6,1);
    SetBall(2,1,1);   SetBall(2,2,1);   SetBall(2,3,2);
    SetBall(2,4,2);   SetBall(2,5,3);   SetBall(2,6,3);
    SetBall(3,1,201); SetBall(3,2,201); SetBall(3,3,201);
    SetBall(3,4,201); SetBall(3,5,201); SetBall(3,6,201);
    SetBall(4,1,3);   SetBall(4,2,1);   SetBall(4,3,2);
    SetBall(4,4,3);   SetBall(4,5,1);   SetBall(4,6,2);
    SetBall(5,1,101); SetBall(5,2,101); SetBall(5,3,101);
    SetBall(5,4,101); SetBall(5,5,101); SetBall(5,6,101);
    SetBall(6,1,101);   SetBall(6,2,102);   SetBall(6,3,101);
    SetBall(6,4,101);   SetBall(6,5,102);   SetBall(6,6,101); 
}

if(Level>12){GameOver=true;}
}
