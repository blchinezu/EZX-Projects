////////////////////////////////////////////////////////////////////
/*

File:			playdiff.cpp

Class impl:		PlayDiff

Description:	A class derived from the IPlay interface to handle all screen
				events and drawing of the Difficulty setting screen

Author:			Al McLuckie (al-at-purplepup-dot-org)

Date:			06 April 2007

History:		Version	Date		Change
				-------	----------	--------------------------------
				0.3.1	07.06.2007	Speed up menu movement a little
				0.4		17.03.2008	Changed description to use score consts

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
#include "playdiff.h"
#include "states.h"
#include <sstream>

const int HI_X = 58;	//highlight x pos
const int HI_Y = 55;	//highlight y pos start
const int HI_GAP = 25;	//gap between options for highlight to jump
const int HI_OFF = 7;	//added to HI_Y to get menu word placement

PlayDiff::PlayDiff(GameData &gd)  : _gd(gd)
{
	_running = false;
	_init = false;
}

void PlayDiff::init(Input *input)
{
	//once the class is initialised, init and running are set true

	//set the repeat of the keys required
	input->setRepeat(Input::UP, 150, 300);		//button, rate, delay
	input->setRepeat(Input::DOWN, 150, 300);

	_newDiff = _gd._diffLevel;	//set curr difficulty
	updateStrTimes();	//helper fn to format time available str for display

	_gd._star.setPos(HI_X,HI_Y+(((int)_newDiff)*HI_GAP));	//modified once menu text X pos returned from put_text()
	_gd._star.startAnim( 0, 6, ImageAnim::ANI_LOOP, 35);

	_round.setWordCenterHoriz(std::string("LEVEL"), _gd._letters, 4, 2);
	_round.startMoveFrom(0, -(_gd._letters.tileH()*2), 15, 100, 0, 12);

	//need to set the _init and _running flags
	_init = true;
	_running = true;
}

void PlayDiff::render(Screen *s)
{
	if (!_init) return;

	_gd._menubg.blitTo( s );

	//draw screen title
	_round.draw(s);

	int xPos[3];
	if (_newDiff == DIF_EASY)
		xPos[0] = _gd._fntMed.put_text(s, HI_Y+HI_OFF-2, "Easy", GREEN_COLOUR, true);
	else
		xPos[0] = _gd._fntSmall.put_text(s, HI_Y+HI_OFF, "Easy", GREEN_COLOUR);

	if (_newDiff == DIF_MED)
		xPos[1] = _gd._fntMed.put_text(s, HI_Y+HI_OFF+HI_GAP-2, "Medium", ORANGE_COLOUR, true);
	else
		xPos[1] = _gd._fntSmall.put_text(s, HI_Y+HI_OFF+HI_GAP, "Medium", ORANGE_COLOUR);

	if (_newDiff == DIF_HARD)
		xPos[2] = _gd._fntMed.put_text(s, HI_Y+HI_OFF+(HI_GAP*2)-2, "Hard", RED_COLOUR, true);
	else
		xPos[2] = _gd._fntSmall.put_text(s, HI_Y+HI_OFF+(HI_GAP*2), "Hard", RED_COLOUR);

	int yy(150), _starPos(0);
	switch (_newDiff)
	{
	case DIF_EASY:
		_gd._fntClean.put_text(s, yy, _strTimes.c_str(), GREEN_COLOUR, true);
		_gd._fntClean.put_text(s, yy+20, "Only easy words used. Can pause game.", GREEN_COLOUR, true);
		_starPos = 0;
		break;
	case DIF_MED:
		_gd._fntClean.put_text(s, yy, _strTimes.c_str(), ORANGE_COLOUR, true);
		_gd._fntClean.put_text(s, yy+20, "Also includes easy words. No pause.", ORANGE_COLOUR, true);
		_starPos = 1;
		break;
	case DIF_HARD:
		_gd._fntClean.put_text(s, yy, _strTimes.c_str(), RED_COLOUR, true);
		_gd._fntClean.put_text(s, yy+20, "Also easy and medium words. No Pause", RED_COLOUR, true);
		_starPos = 2;
		break;
	default:break;
	}

	_gd._fntClean.put_text(s, 215, "Press B to select difficulty", GREY_COLOUR, true);
	
	_gd._star.setPos(xPos[_starPos]-30, HI_Y+(_starPos*HI_GAP));
	_gd._star.draw(s);
}

void PlayDiff::work(Input *input, float speedFactor)
{
	_gd._star.work();

	_round.work();

	//Do repeat keys...
	//if a key is pressed and the interval has expired process 
	//that button as if pressesd again

    if (input->repeat(Input::UP)) button(input, Input::UP);
    if (input->repeat(Input::DOWN))	button(input, Input::DOWN);
}

void PlayDiff::button(Input *input, Input::ButtonType b)
{
	switch (b)
	{
	case Input::UP: 
		if (input->isPressed(b))
		{
	    	if (_newDiff > DIF_EASY) _newDiff = (eDifficulty)(_newDiff-1);
		}
		break;
	case Input::DOWN: 
		if (input->isPressed(b))
		{
			if (_newDiff < DIF_HARD) _newDiff = (eDifficulty)(_newDiff+1);
		}
		break;
	case Input::CLICK: 
	case Input::START: 
	case Input::B: 
		if (input->isPressed(b))
		{
			_gd.setDiffLevel(_newDiff);

			_gd._state = ST_MENU;
			_running = false;	//exit this class running state
		}
		break;
	default:break;
	}
	
	updateStrTimes();
}

void PlayDiff::	updateStrTimes()
{
	std::stringstream strstr;
	strstr << "Reword " << ((int)DIF_MAX - _newDiff)*COUNTDOWN_REWORD << " sec, " <<
			  "Speed6 " << ((int)DIF_MAX - _newDiff)*COUNTDOWN_SPEED6 << ", " <<
			  "TimeTrial " << ((int)DIF_MAX - _newDiff)*COUNTDOWN_TIMETRIAL;

	_strTimes = strstr.str();
}

