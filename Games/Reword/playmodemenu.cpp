////////////////////////////////////////////////////////////////////
/*

File:			PlayModeMenu.cpp

Class impl:  	PlayModeMenu

Description:	A class derived from the IPlay interface to handle game mode
				selection

Author:			Al McLuckie (al-at-purplepup-dot-org)

Date:			25 Feb 2008

History:		Version	Date		Change
				-------	----------	--------------------------------
				0.4.0	25.02.2008	implemented

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
#include "playmodemenu.h"

const int HI_X = 58;	//highlight x pos
const int HI_Y = 65;	//highlight y pos start
const int HI_GAP = 30;	//gap between options for highlight to jump
const int HI_OFF = 7;	//added to HI_Y to get menu word placement


PlayModeMenu::PlayModeMenu(GameData &gd)  : _gd(gd)
{
	_running = false;
	_init = false;
}

void PlayModeMenu::init(Input *input)
{
	//once the class is initialised, init and running are set true

	//play menu music - if not already playing
//	if (!Mix_PlayingMusic())
//		Mix_PlayMusic(_gd._musicMenu, -1);	//play 'forever'

	//set the repeat of the keys required
	input->setRepeat(Input::UP, 150, 300);		//button, rate, delay
	input->setRepeat(Input::DOWN, 150, 300);

	_round.setWordCenterHoriz(std::string("MODE"), _gd._letters, 4, 2);
	_round.startMoveFrom(0, -(_gd._letters.tileH()*2), 15, 100, 0, 12);
	_roundW.start(3000, 2000);

	_gd._star.setPos(HI_X,HI_Y+(_gd._menuoption*HI_GAP));	//modified once menu text X pos returned from put_text()
	_gd._star.startAnim( 0, 6, ImageAnim::ANI_LOOP, 35, 0);
	
	_menuoption = (int)_gd._mode;
	updateHelpStr();
	
	//need to set the _init and _running flags
	_init = true;
	_running = true;
}

void PlayModeMenu::render(Screen *s)
{
	if (!_init) return;

	_gd._menubg.blitTo( s );
	_round.draw(s);		//draw game screen title "REWORD"

	int xPos[4];
	if (0==_menuoption)
		xPos[0] = _gd._fntMed.put_text(s, HI_Y+HI_OFF-2, "Reword", PURPLE_COLOUR, true);
	else
		xPos[0] = _gd._fntSmall.put_text(s, HI_Y+HI_OFF, "Reword", BLACK_COLOUR, false);
	
	if (1==_menuoption)
		xPos[1] = _gd._fntMed.put_text(s, HI_Y+HI_OFF+HI_GAP-2, "Speed 6", GOLD_COLOUR, true);
	else
		xPos[1] = _gd._fntSmall.put_text(s, HI_Y+HI_OFF+HI_GAP, "Speed 6", BLACK_COLOUR, false);
	
	if (2==_menuoption)
		xPos[2] = _gd._fntMed.put_text(s, HI_Y+HI_OFF+(HI_GAP*2)-2, "Time Trial", BLUE_COLOUR, true);
	else
		xPos[2] = _gd._fntSmall.put_text(s, HI_Y+HI_OFF+(HI_GAP*2), "Time Trial", BLACK_COLOUR, false);
	
	if (3==_menuoption)
		xPos[3] = _gd._fntMed.put_text(s, HI_Y+HI_OFF+(HI_GAP*3)-2, "Back", RED_COLOUR, true);
	else
		xPos[3] = _gd._fntSmall.put_text(s, HI_Y+HI_OFF+(HI_GAP*3), "Back", BLACK_COLOUR, false);
	
	_gd._fntClean.put_text(s, HI_Y+120, _helpStr.c_str(), BLACK_COLOUR, false);
	
	_gd._star.setPos(xPos[_menuoption]-30, HI_Y+(_menuoption*HI_GAP));
	_gd._star.draw(s);
	
	_gd._fntClean.put_text(s, 215, "Press B to select option", GREY_COLOUR, true);

}

void PlayModeMenu::updateHelpStr()
{
	switch (_menuoption)
	{
	case 0:_helpStr = "Get 3, 4, 5 and 6 letter words";break;
	case 1:_helpStr = "Just get a 6 letter word to continue";break;
	case 2:_helpStr = "Get the most 6 letter words in the time limit";break;
	case 3:_helpStr = "Back to main menu";break;
	default:_helpStr = "";break;
	}
}

void PlayModeMenu::work(Input *input, float speedFactor)
{
	_round.work();
	_gd._star.work();

	//Do repeat keys...
	//if a key is pressed and the interval has expired process 
	//that button as if pressesd again

    if (input->repeat(Input::UP)) button(input, Input::UP);
    if (input->repeat(Input::DOWN))	button(input, Input::DOWN);
}

void PlayModeMenu::button(Input *input, Input::ButtonType b)
{

	switch (b)
	{
	case Input::UP:
		if (input->isPressed(b))
		{
			if (0==_menuoption)
				_menuoption=3;	//down to bottom
			else 
				_menuoption--;
			updateHelpStr();
		}
		break;
	case Input::DOWN: 
		if (input->isPressed(b))
		{
			if (3==_menuoption)
				_menuoption=0;	//back to top
			else 
				_menuoption++;
			updateHelpStr();
		}
		break;
	case Input::CLICK:
	case Input::START: 
	case Input::B: 
		if (input->isPressed(b))
		{
			if (_menuoption < 3)			// 0..2, so play
			{
				_gd._mode = (eGameMode)_menuoption;
				_gd._state = ST_GAME;
			}
			else 							// 3, so exit & leave mode as-is
				_gd._state = ST_MENU;
				
			_running = false;	//exit this class running state
		}
		break;
	default:break;
	}


}


