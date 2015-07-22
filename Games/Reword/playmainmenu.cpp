////////////////////////////////////////////////////////////////////
/*

File:			PlayMainMenu.cpp

Class impl:		PlayMainMenu

Description:	A class derived from the IPlay interface to handle all screen
				events and drawing of the main menu screen

Author:			Al McLuckie (al-at-purplepup-dot-org)

Date:			06 April 2007

History:		Version	Date		Change
				-------	----------	--------------------------------
				0.4.0	11.02.2008	implemented

Licence:		This program is free software; you can redistribute it and/or modify
				it under the terms of the GNU General Public License as published by
				the Free Software Foundation; either version 2 of the License, or
				(at your option) any later version.

				This software is distributed in the hope that it will be useful,
				but WITHOUT ANY WARRANTY; without even the implied warranty of
				MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
				GNU General Public License for more details.

				You should have received a copy of the GNU General Public License
				along with this program; if not, write to the Free Software
				Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

*/
////////////////////////////////////////////////////////////////////


#include "global.h"
#include "playmainmenu.h"

#include <string>


const int HI_X = 58;	//highlight x pos
const int HI_Y = 55;	//highlight y pos start
const int HI_GAP = 30;	//gap between options for highlight to jump
const int HI_OFF = 7;	//added to HI_Y to get menu word placement


PlayMainMenu::PlayMainMenu(GameData &gd)  : _gd(gd)
{
	_running = false;
	_init = false;
}

void PlayMainMenu::init(Input *input)
{
	//once the class is initialised, init and running are set true

	//play menu music - if not already playing
//	if (!Mix_PlayingMusic())
//		Mix_PlayMusic(_gd._musicMenu, -1);	//play 'forever'

	//set the repeat of the keys required
	input->setRepeat(Input::UP, 150, 300);		//button, rate, delay
	input->setRepeat(Input::DOWN, 150, 300);

	_round.setWordCenterHoriz(std::string("REWORD"), _gd._letters, 4, 2);
	_round.startMoveFrom(0, -(_gd._letters.tileH()*2), 15, 100, 0, 12);
	_roundW.start(3000, 2000);

	_gd._star.setPos(HI_X,HI_Y+(_gd._menuoption*HI_GAP));	//modified once menu text X pos returned from put_text()
	_gd._star.startAnim( 0, 6, ImageAnim::ANI_LOOP, 35, 0);
	
	//need to set the _init and _running flags
	_init = true;
	_running = true;
}

void PlayMainMenu::render(Screen *s)
{
	if (!_init) return;

	_gd._menubg.blitTo( s );

	//draw game screen title "REWORD"
	_round.draw(s);

	_gd._fntClean.put_text(s, 280, 2, VERSION_STRING, BLACK_COLOUR); //display vN.N at top right

	int xPos[5];
	if (0==_gd._menuoption)
		xPos[0] = _gd._fntMed.put_text(s, HI_Y+HI_OFF-2, "Play", GREEN_COLOUR, true);
	else
		xPos[0] = _gd._fntSmall.put_text(s, HI_Y+HI_OFF, "Play", BLACK_COLOUR, false);
	_tmpstr = "Difficulty ";
	_tmpstr += (_gd._diffLevel==DIF_EASY)?"(easy)":(_gd._diffLevel==DIF_MED)?"(med)":"(hard)";
	if (1==_gd._menuoption)
		xPos[1] = _gd._fntMed.put_text(s, HI_Y+HI_OFF+HI_GAP-2, _tmpstr.c_str(), ORANGE_COLOUR, true);
	else
		xPos[1] = _gd._fntSmall.put_text(s, HI_Y+HI_OFF+HI_GAP, _tmpstr.c_str(), BLACK_COLOUR, false);
	
	if (2==_gd._menuoption)
		xPos[2] = _gd._fntMed.put_text(s, HI_Y+HI_OFF+(HI_GAP*2)-2, "Highscores", BLUE_COLOUR, true);
	else
		xPos[2] = _gd._fntSmall.put_text(s, HI_Y+HI_OFF+(HI_GAP*2), "Highscores", BLACK_COLOUR, false);
	
	if (3==_gd._menuoption)
		xPos[3] = _gd._fntMed.put_text(s, HI_Y+HI_OFF+(HI_GAP*3)-2, "Instructions", PURPLE_COLOUR, true);
	else
		xPos[3] = _gd._fntSmall.put_text(s, HI_Y+HI_OFF+(HI_GAP*3), "Instructions", BLACK_COLOUR, false);
	
	if (4==_gd._menuoption)
		xPos[4] = _gd._fntMed.put_text(s, HI_Y+HI_OFF+(HI_GAP*4)-2, "Exit", RED_COLOUR, true);
	else
		xPos[4] = _gd._fntSmall.put_text(s, HI_Y+HI_OFF+(HI_GAP*4), "Exit", BLACK_COLOUR, false);
	
	_gd._star.setPos(xPos[_gd._menuoption]-30, HI_Y+(_gd._menuoption*HI_GAP));
	_gd._star.draw(s);
	
	_gd._fntClean.put_text(s, 215, "Press B to select option", GREY_COLOUR, true);

}

void PlayMainMenu::work(Input *input, float speedFactor)
{
	_round.work();
	_gd._star.work();

	//animate the roundel title if its not moving and 
	//we have waited long enough since it animated last
	if (!_round.isMoving() && _roundW.done(true))
	{
		if (_round.isInOrder())
			_round.jumbleWord(true); 
		else
			_round.unJumbleWord(true);
	}

	//Do repeat keys...
	//if a key is pressed and the interval has expired process 
	//that button as if pressesd again

    if (input->repeat(Input::UP)) button(input, Input::UP);
    if (input->repeat(Input::DOWN))	button(input, Input::DOWN);
}

void PlayMainMenu::button(Input *input, Input::ButtonType b)
{
	switch (b)
	{
	case Input::LEFT:
		if (input->isPressed(b))
		{
			if (1==_gd._menuoption && _gd._diffLevel > DIF_EASY) 
				_gd.setDiffLevel((eDifficulty)(_gd._diffLevel-1));
		}
		break;
	case Input::RIGHT: 
		if (input->isPressed(b))
		{
	    	if (1==_gd._menuoption && _gd._diffLevel < DIF_HARD) 
				_gd.setDiffLevel((eDifficulty)(_gd._diffLevel+1));
		}
		break;
	case Input::UP: 
		if (input->isPressed(b))
		{
			if (0==_gd._menuoption) 
				_gd._menuoption=4;	//down to bottom
			else 
				_gd._menuoption--;
		}
		break;
	case Input::DOWN: 
		if (input->isPressed(b))
		{
			if (4==_gd._menuoption)
				_gd._menuoption=0;	//back to top
			else 
				_gd._menuoption++;
		}
		break;
	case Input::CLICK: 
	case Input::START: 
	case Input::B: 
		if (input->isPressed(b))
		{
			switch (_gd._menuoption)
			{
			case 0:_gd._state = ST_MODE;break;	//play (select mode first)
			case 1:_gd._state = ST_DIFF;break;	//difficulty level
			case 2:_gd._state = ST_HIGH;break;	//high score
			case 3:_gd._state = ST_INST;break;	//instructions
			case 4:
			default:_gd._state = ST_EXIT;break;	//quit
			}
			_running = false;	//exit this class running state
		}
		break;
	default:break;
	}
}


