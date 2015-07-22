////////////////////////////////////////////////////////////////////
/*

File:			playinst.cpp

Class impl:		PlayInst

Description:	A class derived from the IPlay interface to handle all screen
				events and drawing of the Instructions screen

Author:			Al McLuckie (al-at-purplepup-dot-org)

Date:			06 April 2007

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
#include "playinst.h"
#include "utils.h"
#include <sstream>

//max number of lines to display on the instructions screen at once without scrolling
#define INST_LINES		8
#define INST_MAX		38

PlayInst::PlayInst(GameData &gd)  : _gd(gd)
{
	_running = false;
	_init = false;
}

void PlayInst::init(Input *input)
{
	//once the class is initialised, init and running are set true

	_page = 1;

	_round.setWordCenterHoriz(std::string("INFO"), _gd._letters, 4, 2);
	_round.startMoveFrom(0, -(_gd._letters.tileH()*2), 15, 100, 0, 12);

	buildPage(_page);
	
	//set the repeat of the keys required
	input->setRepeat(Input::UP, 250, 250);		//button, rate, delay
	input->setRepeat(Input::DOWN, 250, 250);
	
	//set arrow (scroll positions)
	_gd._arrowUp.setPos(300, 68);		//positions dont change, just made visible or not if scroll available
	_gd._arrowUp.startAnim(0, -1, ImageAnim::ANI_REVERSE, 100, 0);
	_gd._arrowDown.setPos(300, 185);
	_gd._arrowDown.startAnim(0, -1, ImageAnim::ANI_REVERSE, 100, 0);

	//need to set the _init and _running flags
	_init = true;
	_running = true;
}

void PlayInst::render(Screen *s)
{
	if (!_init) return;

	_gd._menubg.blitTo( s );

	//draw screen title
	_round.draw(s);

//	int xx, yy;
	if (_page == 1)
	{
		//page 1
		_gd._fntMed.put_text(s, 50, "Controls", BLUE_COLOUR, true);
		_gd._fntClean.put_text(s, 215, "Press B for rules, Y to exit", GREY_COLOUR, true);
	}
	
	if (_page == 2)
	{
		//page 2
		_gd._fntMed.put_text(s, 50, "How to play", BLUE_COLOUR, true);
		_gd._fntClean.put_text(s, 215, "Press B for scoring, Y to exit", GREY_COLOUR, true);
	}
	else if (_page == 3)
	{
		//page 2
		_gd._fntMed.put_text(s, 50, "Scoring", BLUE_COLOUR, true);
		_gd._fntClean.put_text(s, 215, "Press B or Y to exit", GREY_COLOUR, true);
	}

	//draw the text here... use same code as drawing dictionary...
	int yy = 65 + ((_inst.size() > INST_LINES)?0:(((INST_LINES-_inst.size())/2)*16));
	std::vector<std::string>::const_iterator it = _inst.begin() + _instLine;	//add offset
	while (it != _inst.end())
	{
		_gd._fntClean.put_text(s, 20, yy, (*it).c_str(), _txtColour, false);
		yy+=16;	//fntClean font height
		if (yy > 65+(16*(INST_LINES-1))) break;	//max 8 lines (of 16pix) added to start line pos
		++it;
	}

	_gd._arrowUp.draw(s);		//only if set visible (more lines than screen shows)
	_gd._arrowDown.draw(s);		//..

}

void PlayInst::work(Input *input, float speedFactor)
{
	_round.work();

    if (input->repeat(Input::UP))	button(input, Input::UP);
    if (input->repeat(Input::DOWN)) button(input, Input::DOWN);

	_gd._arrowUp.setVisible(_instLine > 0);
	_gd._arrowUp.work();
	_gd._arrowDown.setVisible(_instLine < (int)_inst.size()-INST_LINES);
	_gd._arrowDown.work();
}

void PlayInst::button(Input *input, Input::ButtonType b)
{
	switch (b)
	{
	case Input::UP: 
		if (input->isPressed(b)) 
			//move the _instLine offset var up a line
			if (_inst.size()>INST_LINES && _instLine > 0) --_instLine;
		break;
	case Input::DOWN: 
		if (input->isPressed(b))
			//move the _instLine offset var down a line
			if (_inst.size()>INST_LINES && _instLine < (int)_inst.size()-INST_LINES) ++_instLine;
		break;
	case Input::Y: 
	case Input::START: 
		if (input->isPressed(b))	//exit back to menu
		{
			_gd._state = ST_MENU;
			_running = false;	//exit this class running state
		}
		break;
	case Input::CLICK: 
	case Input::B: 
		if (input->isPressed(b))
		{
			buildPage(++_page);
			if (_page>3)
			{
				_gd._state = ST_MENU;		//back to menu
				_running = false;
			}
		}
		break;
	default:break;
	}
}

void PlayInst::buildPage(int page)
{
	//now build the scrolling instructions strings to display (on seperate lines)
	_instLine = 0;	//init start of scrollable text, start on first line

	std::stringstream strstr;
	std::string str;
	switch (page)
	{
	case 1:strstr <<
			"During play:\n\n"
			"B  -  select a letter\n"
			"X  -  try the word against dictionary\n"
			"Y  -  clear the word\n"
			"A  -  jumble the available letters\n"
			"START -  pause game (easy mode only)\n"
			"SELECT -  in-game skip or quit options\n"
			"L or R  -  select last word\n"
			"\nAt end of level:\n\n"
			"B  -  continue to next level\n"
			"Y  -  show dictionary definition\n"
			"\nAdditional options are displayed on screen at the required time."
			;
			_txtColour = BLUE_COLOUR;
			break;
	case 2:	strstr <<	//instructions
			"There are three game modes to play, with three difficulty levels "
			"each. Time allowed and bonuses depend on the difficulty. \n\n"
			"1) Reword\n"
			"Make 3, 4, 5 and 6 letter words from the 6 letters given. "
			"You must make at least one 6 letter word in each round to be able "
			"to move on to the next round. If all words are found, "
			"a bonus is given for each second remaining."
			"\n\n"
			"2) Speed6\n"
			"You must get a single 6 letter word to continue to the next round.\n"
			"A bonus is given each time a fastest word is achieved."
			"\n\n"
			"3) TimeTrial\n"
			"You must get as many 6 letter words as you can in the time allowed.\n"
			"A bonus is given each time a fastest word is achieved.\n"
			"Note that the timer will continue to count down even after a word has been "
			"found. You must move on as quickly as possible for a good score!"
			"\n\n"
			"General Info:\n"
			"In Reword or Speed6 you can press SELECT for 'skip' and 'quit' options."
			;
			_txtColour = BLUE_COLOUR;
			break;
			
	case 3: strstr <<	//scoring
			"In Reword:\n" <<
			SCORE_BONUS <<" for each level passed\n" <<
			SCORE_SECONDS << " * seconds left, if ALL words found\n" <<
			SCORE_WORD6 << " for each 6 letter word found\n" <<
			SCORE_WORD << " for each 3, 4, 5 letter word\n" <<
			"\nIn Speed6:\n" <<
			SCORE_WORD6 << " * for each 6 letter word found\n" <<
			SCORE_FASTEST << " * remainder seconds for each fastest\n" <<
			"\nIn TimeTrial:\n" <<
			SCORE_WORD6 << " for each 6 letter word found\n" <<
			SCORE_FASTEST << " * remainder seconds for each fastest\n"
			;
			_txtColour = BLUE_COLOUR;
			break;
			
	default:return;	//not used
	}
	str = strstr.str();
	Utils::buildTextPage(str, INST_MAX, _inst);
}



