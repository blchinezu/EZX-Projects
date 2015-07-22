////////////////////////////////////////////////////////////////////
/*

File:			playgamepopup.cpp

Class impl:		PlayGamePopup

Description:	A class derived from the IPlay interface to handle the in-game popup 
				menu to allow "end of level", "next word" or "quit game" options

Author:			Al McLuckie (al-at-purplepup-dot-org)

Date:			02 Feb 2008

History:		Version	Date		Change
				-------	----------	--------------------------------
				0.4		02.02.2008	Created
						11.03.2008	Added Yes/No on quit
						
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
#include "playgamepopup.h"

#include <string>


const int HI_X = 58;	//highlight x pos
const int HI_Y = 65;	//highlight y pos start
const int HI_GAP = 30;	//gap between options for highlight to jump
const int HI_OFF = 7;	//added to HI_Y to get menu word placement


PlayGamePopup::PlayGamePopup(GameData &gd)  : _gd(gd)
{
	_running = false;
	_init = false;
}

void PlayGamePopup::init(Input *input)
{
	//once the class is initialised, init and running are set true

	//Do not init repeat keys so we dont have to reset game repeat on return
	_hasA6 = false;
	_menuoption = POP_CANCEL;	//always start on 0
	_selected = POP_CANCEL;		//POP_value of selected menu item (only valid if _bSelected=true)
	_bSelected = false;			//bool for caller to determine if selection made
	_bConfirm = false;			//true goes into yes/no mode if exit pressed
	
	memset(_xPos, 0, sizeof _xPos );
	_gd._star.setPos(0,0);	//modified once menu text X pos returned from put_text()
	_gd._star.startAnim( 0, 6, ImageAnim::ANI_LOOP, 35, 0);

	updateHelpStr();

	//need to set the _init and _running flags
	_init = true;
	_running = true;
}

void PlayGamePopup::render(Screen *s)
{
	_gd._popupmenu.blitTo(s, (s->_width - _gd._popupmenu.tileW()) / 2,
							 (s->_height - _gd._popupmenu.tileH()) / 2, 0);

	if (!_bConfirm)
	{
		if (POP_CANCEL==_menuoption)
			_xPos[POP_CANCEL] = _gd._fntMed.put_text(s, HI_Y+HI_OFF-2, "Resume game", GREEN_COLOUR, true);
		else
			_xPos[POP_CANCEL] = _gd._fntSmall.put_text(s, HI_Y+HI_OFF, "Resume Game", BLACK_COLOUR, false);
		
		if (POP_SKIP==_menuoption)
			_xPos[POP_SKIP] = _gd._fntMed.put_text(s, HI_Y+HI_OFF+HI_GAP-2, "Next word/Level", (_hasA6)?ORANGE_COLOUR:GREY_COLOUR, true);
		else
			_xPos[POP_SKIP] = _gd._fntSmall.put_text(s, HI_Y+HI_OFF+HI_GAP, "Next word/Level", (_hasA6)?BLACK_COLOUR:GREY_COLOUR, false);
		
		if (POP_QUIT==_menuoption)
			_xPos[POP_QUIT] = _gd._fntMed.put_text(s, HI_Y+HI_OFF+(HI_GAP*2)-2, "Quit Game !", RED_COLOUR, true);
		else
			_xPos[POP_QUIT] = _gd._fntSmall.put_text(s, HI_Y+HI_OFF+(HI_GAP*2), "Quit Game !", BLACK_COLOUR, false);
	}
	else
	{
		if (POP_NO==_menuoption)
			_xPos[POP_NO] = _gd._fntMed.put_text(s, HI_Y+HI_OFF-2, "No", GREEN_COLOUR, true);
		else
			_xPos[POP_NO] = _gd._fntSmall.put_text(s, HI_Y+HI_OFF, "No", BLACK_COLOUR, false);
		
		if (POP_YES==_menuoption)
			_xPos[POP_YES] = _gd._fntMed.put_text(s, HI_Y+HI_OFF+HI_GAP-2, "Yes", RED_COLOUR, true);
		else
			_xPos[POP_YES] = _gd._fntSmall.put_text(s, HI_Y+HI_OFF+HI_GAP, "Yes", BLACK_COLOUR, false);
	}
	
	_gd._fntClean.put_text(s, HI_Y+100, _helpStr.c_str(), GREY_COLOUR, true);
	
	_gd._star.setPos(_xPos[_menuoption]-30, HI_Y+(_menuoption*HI_GAP));
	_gd._star.draw(s);
}

void PlayGamePopup::updateHelpStr()
{
	if (!_bConfirm)
	{
		switch (_menuoption)
		{
		case POP_CANCEL:_helpStr = "Continue game";break;
		case POP_SKIP  :_helpStr = (_hasA6)?"Move on to next word/level":"You must have a 6 first!";break;
		case POP_QUIT  :_helpStr = "Exit (saves if highscore)";break;
		default:_helpStr = "";break;
		}
	}
	else
	{
		switch (_menuoption)
		{
		case POP_NO :_helpStr = "Back to menu";break;
		case POP_YES:_helpStr = "Quit the game";break;
		default:_helpStr = "";break;
		}
	}
}

void PlayGamePopup::work(Input *input, float speedFactor)
{
	_gd._star.work();

	//Do repeat keys...if a key is pressed and the interval
	//has expired process that button as if pressesd again
    if (input->repeat(Input::UP)) button(input, Input::UP);
    if (input->repeat(Input::DOWN))	button(input, Input::DOWN);
}

void PlayGamePopup::button(Input *input, Input::ButtonType b)
{
	switch (b)
	{
	case Input::UP: 
		if (input->isPressed(b)) 
			if (POP_CANCEL==_menuoption)
				_menuoption=(_bConfirm)?(int)POP_YES:(int)POP_QUIT;	//down to bottom
			else 
				_menuoption--;
		break;
	case Input::DOWN: 
		if (input->isPressed(b))
			if ((_bConfirm && POP_YES==_menuoption) || (!_bConfirm && POP_QUIT==_menuoption))
				_menuoption=(_bConfirm)?(int)POP_NO:(int)POP_CANCEL;	//back to top
			else 
				_menuoption++;
		break;
	case Input::CLICK: 
	case Input::B:
		if (input->isPressed(b))
		{
			if (_bConfirm)
			{
				//already showing YES/NO confirm options, so process...
				if (_menuoption == POP_NO)
				{
					_menuoption = POP_QUIT; 	//NO selected, back to POP_QUIT
					_bConfirm = false;			//back to POP_CANCEL/SKIP/QUIT menu
					break;
				}
				else
					_menuoption = POP_QUIT;	//YES selected, so exit
			}
			else
			{
				//if on exit option, go to yes/no options and wait for yes or no
				if (_menuoption == POP_SKIP && !_hasA6) break;
				if (_menuoption == POP_QUIT)
				{
					_menuoption = POP_NO;
					_bConfirm = true;
					break;
				}
			}
			_bSelected = true;
			_selected = _menuoption;
		}
		break;

	default:break;
	}

	//update on any key press as it could be up/down or exit/yes/no back and forth
	updateHelpStr();
	
}

