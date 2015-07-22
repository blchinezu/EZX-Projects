////////////////////////////////////////////////////////////////////
/*

File:			playhigh.cpp

Class impl:		PlayHigh

Description:	A class derived from the IPlay interface to handle all screen
				events and drawing of the Hi-Score screen

Author:			Al McLuckie (al-at-purplepup-dot-org)

Date:			06 April 2007

History:		Version	Date		Change
				-------	----------	--------------------------------
				0.4.0	06.03.2008	Added speed6 and timetrial displays and mode logos

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
#include "playhigh.h"


PlayHigh::PlayHigh(GameData &gd)  : _gd(gd)
{
	_running = false;
	_init = false;
}

void PlayHigh::init(Input *input)
{
	//once the class is initialised, init and running are set true

	//play menu music - if not already playing - as we could come 
	//here from the end of the game rather than from the menu
//	if (!Mix_PlayingMusic())
//		Mix_PlayMusic(_gd._musicMenu, -1);	//play 'forever'

	//set up local difficulty settings
	setDifficulty(_gd._diffLevel);
	setMode(_gd._mode);	//calls prepareBackground();
	
	_currPos = 0;
	//set pos to -1 if not a high score, ie -1=editing high score
	_pos = (ST_HIGHEDIT==_gd._state)?_gd._score.isHiScore(_mode, _diff):-1;

	_curr = _gd._score.curr();

	_round.setWordCenterHoriz(std::string("HISCORE"), _gd._letters, 4, 2);
	_round.startMoveFrom(0, -(_gd._letters.tileH()*2), 15, 100, 0, 12);

	//set the repeat of the keys required
	input->setRepeat(Input::UP, 100, 300);		//button, rate, delay
	input->setRepeat(Input::DOWN, 100, 300);
	input->setRepeat(Input::LEFT, 100, 300);
	input->setRepeat(Input::RIGHT, 100, 300);
	
	//need to set the _init and _running flags
	_init = true;
	_running = true;
}

void PlayHigh::render(Screen *s)
{
	if (!_init) return;

	//display:

	//           HIGHSCORES :
	//
	//   ABC ...... 2000000 (#words)
	//   GFG ...... 1232322 (#words)
	//   TTT ......  109999 (#words)
	//   AAA ......    4344 (#words)
	//
	//

	_menubg->blitTo( s );

	//draw screen title
	_round.draw(s);

	int line = 0;
	int yy = 0;
	char sLetter[2] = {0x00, 0x00};	//"string" to hold editable letters
	
	_gd._fntClean.put_text(s, 0, 43, _description.c_str(), _diffColour, false);

	while (yy < 10)
	{
		if (yy == _pos)	//if it's -1 (no hiscore) then this wont display
		{
			//draw current score in the gap for player to enter inits
			sLetter[0] = _curr.inits[0];
			_gd._fntClean.put_text(s, 70, 43+(16*yy), sLetter, (0==_currPos)?BLACK_COLOUR:_diffColour);
			sLetter[0] = _curr.inits[1];
			_gd._fntClean.put_text(s, 80, 43+(16*yy), sLetter, (1==_currPos)?BLACK_COLOUR:_diffColour);
			sLetter[0] = _curr.inits[2];
			_gd._fntClean.put_text(s, 90, 43+(16*yy), sLetter, (2==_currPos)?BLACK_COLOUR:_diffColour);

			_gd._fntClean.put_number(s, 110, 43+(16*yy), _curr.score, "%08d", _diffColour);
			_gd._fntClean.put_number(s, 192, 43+(16*yy), _curr.words, "%04dw", _diffColour);
			if (_mode != GM_REWORD)
				_gd._fntClean.put_number(s, 255, 43+(16*yy), _curr.fastest, "%03ds", _diffColour);
		}
		else
		{
			//draw scores in order as normal
			_gd._fntClean.put_text(s, 70, 43+(16*yy), _gd._score.inits(_mode, _diff, line).c_str(), _diffColour);
			_gd._fntClean.put_number(s, 110, 43+(16*yy), _gd._score.score(_mode, _diff, line), "%08d", BLUE_COLOUR);
			_gd._fntClean.put_number(s, 192, 43+(16*yy), _gd._score.words(_mode, _diff, line), "%04dw", BLUE_COLOUR);
			if (_mode != GM_REWORD)
				_gd._fntClean.put_number(s, 255, 43+(16*yy), _gd._score.fastest(_mode, _diff, line), "%03ds", BLUE_COLOUR);

			++line;	//dont jump a score (ie line = 0->9, but yy will jump+1 if yy==pos
		}
		++yy;
	}

	if (-1 == _pos)
		_gd._fntClean.put_text(s, 215, "Up/down:mode, left/right:difficulty, B to exit", GREY_COLOUR, true);
	else
		_gd._fntClean.put_text(s, 215, "Enter initials then B to save", GREY_COLOUR, true);

}

void PlayHigh::work(Input *input, float speedFactor)
{
	_round.work();

	//Do repeat keys...
	//if a key is pressed and the interval has expired process 
	//that button as if pressesd again

    if (input->repeat(Input::UP))	button(input, Input::UP);
    if (input->repeat(Input::DOWN)) button(input, Input::DOWN);
    if (input->repeat(Input::LEFT))	button(input, Input::LEFT);
    if (input->repeat(Input::RIGHT)) button(input, Input::RIGHT);
}

void PlayHigh::button(Input *input, Input::ButtonType b)
{
	switch (b)
	{
	case Input::UP: 
		if (input->isPressed(b))
		{
			//move from C to B to A
			if (_pos != -1)
			{
				if (_curr.inits[_currPos] > ' ') --_curr.inits[_currPos];
			}
			else
				setMode((eGameMode)--_mode);
		}
		break;
	case Input::DOWN: 
		if (input->isPressed(b))
		{
			//move from A to B to C
			if (_pos != -1)
			{
				if (_curr.inits[_currPos] < 'Z') ++_curr.inits[_currPos];
			}
			else
				setMode((eGameMode)++_mode);
		}
		break;
	case Input::LEFT: 
		if (input->isPressed(b))
		{
			if (_pos != -1)
			{
				if (_currPos > 0) _currPos--;
			}
			else
				setDifficulty((eDifficulty)(_diff-1));
		}
		break;
	case Input::RIGHT: 
		if (input->isPressed(b))
		{
			if (_pos != -1)
			{
				if (_currPos < 2) _currPos++;
			}
			else
				setDifficulty((eDifficulty)(_diff+1));
		}
		break;
	case Input::CLICK: 
	case Input::B: 
		if (input->isPressed(b))
		{
			if (_pos != -1 && _currPos < 2) 
			{	
				_currPos++;	//treat as button right unless on 3rd char
				break;
			}
			if (_pos != -1)	//is editing and position must be 2 (3rd char)
			{
				//so player has pressed B on last char to save to entered inits
				_gd._score.insert(_mode, _diff, _pos, _curr);
				_gd._score.save();	//save now so player can switch off or return to menu if wishes

				_pos = -1;		//set to not-editing 
				break;
			}
			//else pos is -1 (not editing) so follow on to exit...		
		}
		//follow on to exit to ST_MENU ... 
		//not break
	case Input::X: 
		if (input->isPressed(b))
		{
			_gd._state = ST_MENU;
			_running = false;	//exit this class running state
		}
		break;
	default:break;
	}
}

//set difficulty locally
void PlayHigh::setDifficulty(eDifficulty diff)
{
	if (diff < DIF_EASY) diff = DIF_EASY;
	if (diff > DIF_HARD) diff = DIF_HARD;
	_diff = diff;
	_diffColour = (DIF_EASY==_diff)?GREEN_COLOUR:(DIF_MED==_diff)?ORANGE_COLOUR:RED_COLOUR;
	setDescription();
}
//set mode locally
void PlayHigh::setMode(eGameMode mode)
{
	if (mode < GM_REWORD) mode = GM_REWORD;
	if (mode > GM_TIMETRIAL) mode = GM_TIMETRIAL;
	_mode = mode;
	setDescription();
	prepareBackground();
}

void PlayHigh::setDescription()
{
	switch (_diff)
	{
	case DIF_EASY:		_description = "EASY";break;
	case DIF_MED:		_description = "MEDIUM";break;
	case DIF_HARD:		_description = "HARD";break;
	default:break;
	}
}

void PlayHigh::prepareBackground()
{
	//create the background to be used for this level, 
	//pre drawing so we dont need to do it each frame.
	//...
	_menubg = std::auto_ptr<Image>(new Image());
	_menubg->createThisFromImage(_gd._menubg_plain);	//copy of basic menubg without roundel

	switch (_mode)
	{
	case GM_REWORD:		_menubg->blitFrom(&_gd._menu_reword, -1, -32, 54);
						break;
	case GM_SPEED6:		_menubg->blitFrom(&_gd._menu_speed6, -1, -31, 54);
						break;
	case GM_TIMETRIAL:	_menubg->blitFrom(&_gd._menu_timetrial, -1, -30, 55);
						break;
	default:break;
	}
}

