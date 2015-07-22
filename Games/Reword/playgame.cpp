////////////////////////////////////////////////////////////////////
/*

File:			playgame.cpp

Class impl:		PlayGame

Description:	A class derived from the IPlay interface to handle all screen
				events and drawing of the main game screen. This is the main
				class for the actual playing screen.

Author:			Al McLuckie (al-at-purplepup-dot-org)

Date:			06 April 2007

History:		Version	Date		Change
				-------	----------	--------------------------------
				0.3.1	03.06.2007	Change Stick+L+R exit to allow player to set hiscore if needed
									Fix doDictionary, check if in PG_END
				0.4		13.06.07	Added _dictLine offset into onscreen dict display to scroll lines
										also added scroll arrows to indicate more text to see
									Added popup menu for quick exit/save/move-on
									Changed to stack for various major states in-play and added
										seperate functions for each render/work state.
									Added pinger when in popup menu, to warn user


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
#include "playgame.h"
#include "spritemgr.h"
#include "utils.h"

//max number of lines to display on the dictionary screen at once without scrolling
#define DICT_LINES		8
//max number of words to display in a 3, 4, 5 or 6 word column
#define MAX_WORD_COL	8

static	int _countdown;				//seconds remaining

//function called when timer reaches interval set
Uint32 countdown_callback(Uint32 interval, void *param)
{
	if (_countdown <= 0) 
	{
		PlayGame::pushEvent(USER_EV_END_COUNTDOWN); //pushEndOfLevel();
		return 0;
	}

	PlayGame::pushEvent(USER_EV_PING_COUNTDOWN);

	return(interval);
}


PlayGame::PlayGame(GameData& gd) : _gd(gd), _pPopup(0)
{
	_inputL  = _inputR = false;	//keys pressed + stick click to exit game
	_running = false;	//not init yet
	_init = false;		//ditto
	_bAbort = false;	//until L+R+CLICK which exits game to hiscore if needed or to menu

	statePush(PG_PLAY);		//also sets default to PG_PLAY
}

void PlayGame::init(Input *input)
{
	//fade out any music (from the menu)
	Mix_FadeOutMusic(3000);

	//once the class is initialised, init and running are set true
	newGame();	//reset scores etc
	newLevel();	//preparebackground, get next word etc

	//set the repeat of the keys required
	input->setRepeat(Input::UP, 250, 250);		//button, rate, delay
	input->setRepeat(Input::DOWN, 250, 250);
	input->setRepeat(Input::LEFT, 250, 250);
	input->setRepeat(Input::RIGHT, 250, 250);

	//need to set the _init and _running flags
	_init = true;
	_running = true;
}

//exit the game play screen with a specific state, eg. back to ST_MENU
void PlayGame::exit(eGameState toState)
{
	stopCountdown();
	clearEventBuffer();
	_gd._state = toState;	//ST_MENU, ST_PLAY etc
	_running = false;
}

//do not call directly. Use statePush() & statePop()
void PlayGame::stateFn(eState state)
{
	switch (state)
	{
	//note PG_PLAY: is default:

	case PG_WAIT:	pRenderFn = &PlayGame::render_wait; 
					pWorkFn = &PlayGame::work_wait; 
					pButtonFn = &PlayGame::button_wait;
					break;	//same renderer as current
	case PG_END:	pRenderFn = &PlayGame::render_end; 
					pWorkFn = &PlayGame::work_end; 
					pButtonFn = &PlayGame::button_end;
					break;
	case PG_DICT:	pRenderFn = &PlayGame::render_dict; 
					pWorkFn = &PlayGame::work_dict; 
					pButtonFn = &PlayGame::button_dict;
					break;
	case PG_PAUSE:	pRenderFn = &PlayGame::render_pause; 
					pWorkFn = &PlayGame::work_pause; 
					pButtonFn = &PlayGame::button_pause;
					break;
	case PG_PLAY:
	default:		pRenderFn = &PlayGame::render_play;
					pWorkFn = &PlayGame::work_play; 
					pButtonFn = &PlayGame::button_play;
					break;
	}
}
void PlayGame::statePush(eState state)
{
	_state = state;	//for quick reference
	_states.push(state);
	stateFn(state);
}
PlayGame::eState PlayGame::statePop()
{
	eState state = _states.pop();
	stateFn(state);
	return _state = state;
}

//main render fn called by Game class
void PlayGame::render(Screen* s)
{
	//depending on state, call the function pointer of the 
	//correctly mapped render function

	(*this.*pRenderFn)(s);

	//render popup menu on top of curr screen
	if (_pPopup) render_popup(s);
}

//display the game screen PG_PLAY
void PlayGame::render_play(Screen* s)
{
	int xx, yy;

	//draw background
	_gamebg->blitTo(s);

	//draw scores and coloured seconds countdown
	_gd._fntClean.put_number(s, 74,3, _gd._score.currScore(), "%08d", BLACK_COLOUR);	//SCORE:
	_gd._fntClean.put_number(s, 224,3, _gd._score.currWords(), "%04d", BLACK_COLOUR);	//WORDS:
	//(>10) normal countdown in "plenty of time" colour
	//(<=10) countdown in "oh crap" colour (red to denote time running out)
	//		A warning "ping" is also sounded in the countdown callback fn each second...
	_gd._fntBig.put_number(s, 272,3, _countdown, "%03d", 
		(_countdown > 10)?YELLOW_COLOUR:RED_COLOUR, true);	//TIME:

	if (PG_PLAY == _state)
	{
		for (xx=0; xx<6; xx++)
		{
			//draw the tile background before any lower letters placed
			_gd._scratch.blitTo(s, SCRATCHX+(xx*(CURSORW+2)), SCRATCHY2);
		}

		//draw game letters		
		_round.draw(s);

		_gd._cursor.blitTo(s, SCRATCHX+(_round.currentX()*(CURSORW+2)),
						(_round.cursorIsTop()?SCRATCHY1:SCRATCHY2), (int)_gd._diffLevel-1);
	}

#if defined _DEBUG
	_gd._fntClean.put_text(s, 0, 224, _gd._words.getWord6().c_str(), BLUE_COLOUR);	//##DEBUG## shows first target word
#endif

	//draw word boxes 1 length at a time downwards (easier)
	int yo = SCRATCHY2+CURSORH+5;	//start y offset
	tWordsFoundList::const_iterator it;

	//[3..], [4...], [5....] and [6.....] letter boxes
	for (xx=0; xx<4; ++xx)	//accross the screen
	{
		it = _wordsFound[xx].begin();
		int nWords = _gd._words.wordsOfLength(xx);

		if (nWords)	//column has words
		{
			for (yy=0; yy<nWords; ++yy)	//down the screen
			{

				//As the old limit of 7 on screen word boxes has now been extended to 8, we can't use
				//this fuction. If it ever goes back to 7 and includes more words than can fit in
				//a 7 word column, we might use this again. But probably not!
				if (yy >= MAX_WORD_COL)
				{
					//? there are more words than can fit on screen (7 down)
					//  so show remaining count which turns green when all words are found
					//NOTE: this is currently not used as the dictionary file only has words
					//		with up to seven 3, 4, 5 and 6 letter words that make up those available
					//		Just use a dictionary with > 7 words in any 3, 4, 5 or 6 letter words
					//		for this functionality to work
//#ifdef _DEBUG
					if (PG_PLAY == _state)
						_gd._fntTiny.put_number(s, _boxOffset[xx]-8, 218, nWords-_wordsFound[xx].size(), "%d",
							(nWords==(int)_wordsFound[xx].size())?GREEN_COLOUR:BLUE_COLOUR);

					//render the "more" arrow under column
//					_gd._moreWords
//#endif
					break;	//out of this n letters loop
				}

				//draw the box for the word to be displayed in - using a highlighted box if
				//not in play state and this is the curr word the selection is on.
				_gd._boxes.blitTo( s, _boxOffset[xx], yo+(yy*(BOXH-1))+4,
					((PG_PLAY != _state) && xx == _xxWordHi && yy == _yyWordHi)?xx+4:xx );	//tile 0=3, 1=4, 2=5, 3=6 letter words

				if (it != _wordsFound[xx].end())
				{
					//display any dictionary def
					if (PG_END == _state && xx == _xxWordHi && yy == _yyWordHi)
					{
						//level complete, all, some or no words found, check for dictionary

						_tmpStr2 = (*it)._word;
						_tmpDefStr = _gd._words.getDictForWord(_tmpStr2)._description;
						if (_tmpDefStr.length())
						{
							if (_tmpDefStr.length() > 35)	//make "definition..."
							{
								_tmpDefStr.erase(35);	//no more than 42 chars
								_tmpDefStr += "...";	//add ellipsis to make 38
							}
							//display the word description
							_gd._fntClean.put_text(s, 100, _tmpDefStr.c_str(), BLACK_COLOUR);
						}
						else	//blank word - no dictionary entry
							_gd._fntClean.put_text(s, 100, "This word has no definition", GREY_COLOUR);
					}

					//display the word (in red or blue)
					_gd._fntClean.put_text(s, _boxOffset[xx]+6, yo+(yy*(BOXH-1)),
							(*it)._word.c_str(), ((*it)._found)?BLUE_COLOUR:RED_COLOUR);

					++it;
				}

			}
		}
//		else
//		{
//			//draw "none" in column with no matching sub words
//			_gd._boxes.blitTo ...  ( _boxOffset[xx], yo+4, boxes, screen, &r);
//			_gd._fntClean.put_text ... (fntClean, _boxOffset[xx]+6, yo, "none", GREY_COLOUR, screen);
//		}
	}	//for
}

void PlayGame::render_wait(Screen* s)
{
	//##TODO - need to call some animation or something here before
	//			going to PG_END and user can move on to next level
	render_end(s);

/*
	render_play(s);	//same as PG_PLAY... just waiting
*/
}

void PlayGame::render_end(Screen* s)
{
	//first call main game render as our popup just overlays
	render_play(s);

	//finished level so show success type and bonus etc

	switch (_success)
	{
	case SU_GOT6:		//WELL DONE! - You got the 6
		switch (_randomTitle)
		{
		case 0:_gd._fntBig.put_text(s, 40, "GREAT!", PURPLE_COLOUR, true);break;
		case 1:_gd._fntBig.put_text(s, 40, "GOOD!", PURPLE_COLOUR, true);break;
		case 2:_gd._fntBig.put_text(s, 40, "COOL!", PURPLE_COLOUR, true);break;
		default:_gd._fntBig.put_text(s, 40, "WELL DONE!", PURPLE_COLOUR, true);break;
		}
		_gd._fntMed.put_number(s, 65, _bonusScore, "You got a 6, add %d points", PURPLE_COLOUR, true);
		break;
	case SU_BONUS:		//BONUS!! - You got all words
		switch (_randomTitle)
		{
		case 0:_gd._fntBig.put_text(s, 40, "PERFECT!", PURPLE_COLOUR, true);break;
		case 1:_gd._fntBig.put_text(s, 40, "EXCELLENT!", PURPLE_COLOUR, true);break;
		case 2:_gd._fntBig.put_text(s, 40, "AMAZING!!", PURPLE_COLOUR, true);break;
		default:_gd._fntBig.put_text(s, 40, "AWESOME!!", PURPLE_COLOUR, true);break;
		}
		_gd._fntMed.put_number(s, 65, _bonusScore, "All words, add %d points", PURPLE_COLOUR, true);
		break;
	case SU_BADLUCK:	//Bad Luck! - Out of time
		switch (_randomTitle)
		{
		case 0:_gd._fntBig.put_text(s, 40, "BUMMER!", RED_COLOUR, true);break;
		case 1:_gd._fntBig.put_text(s, 40, "WHAT A SHAME!", RED_COLOUR, true);break;
		case 2:_gd._fntBig.put_text(s, 40, "OH NO!!", RED_COLOUR, true);break;
		default:_gd._fntBig.put_text(s, 40, "BAD LUCK!", RED_COLOUR, true);break;
		}
		_gd._fntMed.put_text(s, 65, "Out of time", RED_COLOUR, false);
		break;
	
	case SU_SPEED6:		//Speed 6 mode - a single 6 letter word achieved
		if (_fastest)
		{
			_gd._fntBig.put_text(s, 40, "FASTEST YET!", GOLD_COLOUR, true);
			_gd._fntMed.put_number(s, 65, _fastest, "You got it in %d seconds!", GOLD_COLOUR, false);
			_gd._fntMed.put_number(s, 180, (maxCountdown()-_fastest)*SCORE_FASTEST, "Bonus: %d !", GOLD_COLOUR, false);
		}
		else
		{
			switch (_randomTitle)
			{
			case 0:_gd._fntBig.put_text(s, 40, "GREAT!", GOLD_COLOUR, true);break;
			case 1:_gd._fntBig.put_text(s, 40, "EXCELLENT!", GOLD_COLOUR, true);break;
			case 2:_gd._fntBig.put_text(s, 40, "GOOD!", GOLD_COLOUR, true);break;
			default:_gd._fntBig.put_text(s, 40, "WELL DONE!", GOLD_COLOUR, true);break;
			}
			_gd._fntMed.put_text(s, 65, "Now try another", GOLD_COLOUR, false);
		}
		break;
	case SU_TIMETRIAL:	//Time trial - single count down to 0 - no timer reset
		if (_fastest)
		{
			_gd._fntBig.put_text(s, 40, "FASTEST YET!", BLUE_COLOUR, true);
			_gd._fntMed.put_number(s, 65, _fastest, "You got it in %d seconds!", BLUE_COLOUR, false);
			_gd._fntMed.put_number(s, 180, (maxCountdown()-_fastest)*SCORE_FASTEST, "Bonus: %d !", BLUE_COLOUR, false);
		}
		else
		{
			switch (_randomTitle)
			{
			case 0:_gd._fntBig.put_text(s, 40, "QUICKLY!", BLUE_COLOUR, true);break;
			case 1:_gd._fntBig.put_text(s, 40, "HURRY!", BLUE_COLOUR, true);break;
			case 2:_gd._fntBig.put_text(s, 40, "GREAT!", BLUE_COLOUR, true);break;
			default:_gd._fntBig.put_text(s, 40, "WELL DONE!", BLUE_COLOUR, true);break;
			}
			_gd._fntMed.put_text(s, 65, "get another one...", BLUE_COLOUR, false);
		}
		break;
	
	default:
	case SU_GAMEOVER:	//Game Over - You're a high scorer!
		_gd._fntBig.put_text(s, 40, "Game Over", RED_COLOUR, true);
		_gd._fntMed.put_text(s, 70, "But you're a high scorer!", RED_COLOUR, false);
		break;
	}

	//helpful message - if a dict definition available tell player
	if (PG_END == _state)	//BODGE - only show if in end state(as render_wait() calls this), otherwise "Press B..." shows for a second
		_gd._fntClean.put_text(s, 85,
			(_tmpDefStr.length())?"Y for more detail, or B to continue":"Press B to continue", GREY_COLOUR, false);

}

void PlayGame::render_dict(Screen* s)
{
	_gd._menubg.blitTo( s );

	_roundDict->draw(s);

	_gd._fntClean.put_text(s, 46, "Definition:", GREY_COLOUR, true);

	//draw the dictionary text here... previously split into vector string "lines"
	int yy = 65 + ((_dictDef.size() > DICT_LINES)?0:(((DICT_LINES-_dictDef.size())/2)*16));
	std::vector<std::string>::const_iterator it = _dictDef.begin() + _dictLine;	//add offset
	while (it != _dictDef.end())
	{
		_gd._fntClean.put_text(s, yy, (*it).c_str(), BLACK_COLOUR, false);
		yy+=16;	//fntClean font height
		if (yy > 65+(16*(DICT_LINES-1))) break;	//max 8 lines (of 16pix) added to start line pos
		++it;
	}

	_gd._arrowUp.draw(s);		//only if set visible (more lines than screen shows)
	_gd._arrowDown.draw(s);		//..

	_gd._fntClean.put_text(s, 215, "Press Y or CLICK to continue", PURPLE_COLOUR, true);
}

void PlayGame::render_pause(Screen* s)
{
	SDL_Colour c = BLACK_COLOUR;
	s->drawSolidRect(0,0,s->width(), s->height(), c);
	_roundPaused->draw(s);
	return;
}

//render fn used by PG_POPUP
void PlayGame::render_popup(Screen* s)
{
	//this fn called after any other 'curr' screen so it sits "above"
	_pPopup->render(s);

	return;
}

void PlayGame::work(Input* input, float speedFactor)
{
	//depending on state, call the function pointer of the 
	//correctly mapped work function

	(*this.*pWorkFn)(input, speedFactor);

	//handle popup menu on top of curr screen
	if (_pPopup) work_popup(input, speedFactor);
}


void PlayGame::work_play(Input* input, float speedFactor)
{
	//Do repeat keys...
	//if a key is pressed and the interval has expired process 
	//that button as if pressesd again

    if (input->repeat(Input::UP))	button(input, Input::UP);
    if (input->repeat(Input::DOWN)) button(input, Input::DOWN);
    if (input->repeat(Input::LEFT))	button(input, Input::LEFT);
    if (input->repeat(Input::RIGHT)) button(input, Input::RIGHT);

	_round.work();
}
void PlayGame::work_wait(Input* input, float speedFactor)
{
	//wait.... a while
	if (_waiting.done())
	{
		//_state = PG_END;	//move into end of level mode
		statePop();	//out of waiting state
		statePush(PG_END);	//into end state
	}
}
void PlayGame::work_end(Input* input, float speedFactor)
{
}
void PlayGame::work_dict(Input* input, float speedFactor)
{
	//Do repeat keys...
	//if a key is pressed and the interval has expired process 
	//that button as if pressesd again

    if (input->repeat(Input::UP))	button(input, Input::UP);
    if (input->repeat(Input::DOWN)) button(input, Input::DOWN);
    if (input->repeat(Input::LEFT))	button(input, Input::LEFT);
    if (input->repeat(Input::RIGHT)) button(input, Input::RIGHT);

	_roundDict->work();
	_gd._arrowUp.setVisible(_dictLine > 0);
	_gd._arrowUp.work();
	_gd._arrowDown.setVisible(_dictLine < (int)_dictDef.size()-DICT_LINES);
	_gd._arrowDown.work();
}
void PlayGame::work_pause(Input* input, float speedFactor)
{
	_roundPaused->work();
}
void PlayGame::work_popup(Input* input, float speedFactor)
{
	_pPopup->work(input, speedFactor);
}

void PlayGame::startPopup(Input *input)
{
	_pPopup = new PlayGamePopup(_gd);
	if (_pPopup)
	{
		_pPopup->init(input);
		_pPopup->setHasA6(6 == _maxwordlen);
	}
}

void PlayGame::stopPopup()
{
	delete _pPopup;
	_pPopup = NULL;
}

void PlayGame::button(Input* input, Input::ButtonType b)
{
	//first handle any global keys
	switch (b)
	{
	case Input::L:
		_inputL = input->isPressed(b);
		break;
	case Input::R:
		_inputR = input->isPressed(b);
		break;
	case Input::SELECT:
		if (input->isPressed(b) && (_state != PG_PAUSE))
		{
			if (_pPopup)
				stopPopup();
			else
				startPopup(input);
		}
		break;
	case Input::CLICK:
		if (input->isPressed(b))
		{
			if (_inputL && _inputR) 
			{
				_bAbort = true;
				pushEvent(USER_EV_END_COUNTDOWN); //try to exit more gracefully, pushes end of level
			}
		}
		break;
	default:
		break;
	}

	//now handle popup menu on top of curr screen or depending on state,
	//call the function pointer of the correctly mapped button function
	if (_pPopup)
		button_popup(input, b);
	else
		(*this.*pButtonFn)(input, b);

}

void PlayGame::button_play(Input* input, Input::ButtonType b)
{
	if (!_waiting.done()) return;	//no user input until finished waiting

	//not waiting so allow button use...
	switch (b)
	{
	case Input::LEFT:
		if (input->isPressed(b)) _round.cursorPrev();
		break;
	case Input::RIGHT: 
		if (input->isPressed(b)) _round.cursorNext();
		break;
	case Input::UP: 
		if (input->isPressed(b))
		{
			_round.cursorUp();
		}
		break;
	case Input::DOWN: 
		if (input->isPressed(b))
		{
			_round.cursorDown(); //will only go down if letters exist
		}
		break;
	case Input::L:
		if (input->isPressed(b)) _round.setWordToLast();
		break;
	case Input::R:
		if (input->isPressed(b)) _round.setWordToLast();
		break;
	case Input::A:
		if (input->isPressed(b))
		{
			if (_round.jumbleWord())
				Mix_PlayChannel(-1,_gd._fxWoosh,0);	//sound only if not already moving etc
		}
		break;
	case Input::B: 
		if (input->isPressed(b))
		{
			//user still in play so select curr letter
			if (_round.cursorIsTop())
	  	 		_round.moveLetterDown();
			else
			{
	    		_round.moveLetterUp();
				//if no letters left on bottom row, go back to top
				if (!_round.cursorPrev()) _round.cursorUp();	
			}
//			_round.cursorNext();	//dont use as it confuses player (well me anyway)
		}
		break;
	case Input::START:
		if (input->isPressed(b)) doPauseGame();
		break;
	case Input::Y:
		if (input->isPressed(b)) _round.clearAllToTop();
		break;
	case Input::X:
		if (input->isPressed(b))
		{
			int wordlen = tryWordAgainstDict();

			 //speed6 and timetrial only need a 6 to continue
			if (GM_TIMETRIAL == _gd._mode || GM_SPEED6 == _gd._mode)
			{
				if (6 == wordlen)
				{
					pushEvent(USER_EV_END_COUNTDOWN);	//pushes end of level
					break;
				}
				else wordlen = -1;	//speed6 or timetrial badWord sound as < 6 letters
			}
			
			//must be GM_REWORD or bad word
			if (wordlen > 0)	//is 3,4,5, or 6
			{
				_gd._score.addCurrScore( (6 == wordlen)?SCORE_WORD6:SCORE_WORD );	//more for a 6 letter word
				_round.clearAllToTop(true);	//remove hit word
				
				if (allWordsFound())
				{
					pushEvent(USER_EV_END_COUNTDOWN); //pushes end of level
					break;	//exit before sound, as success() plays fanfare sound
				}
				//it's a simple word find
				Mix_PlayChannel(-1,(6 == wordlen)?_gd._fx6found:_gd._fxFound,0);
			}
			else	//0=already found, -1=not a 6 or in sub word list
			{
				Mix_PlayChannel(-1,(0 == wordlen)?_gd._fxOldword:_gd._fxBadword,0);
				_round.clearAllToTop(false);	//remove bad word - dont move cursor
			}
		}
		break;

	default:break;
	}
}
void PlayGame::button_wait(Input* input, Input::ButtonType b)
{
	//same as play state
	button_play(input, b);
}
void PlayGame::button_end(Input* input, Input::ButtonType b)
{
	//in end state, not waiting so allow button use...
	switch (b)
	{
	case Input::B: 
		if (input->isPressed(b))
		{
			//play over so do we go to next level or back to menu...?
			switch (_success)
			{
			case SU_GOT6:					//WELL DONE! - You got the 6
			case SU_SPEED6:					//or if in Speed6 mode, a 6 entered so move onto next level
				statePop(); 				//out of PG_END state
				newLevel();
				break;
			case SU_TIMETRIAL:				//in time trial mode quickly move on to next level immediately
				if (_countdown==0)
				{
					_bAbort = true;
					pushEvent(USER_EV_END_COUNTDOWN); //try to exit more gracefully, pushes end of level
				}
				else
				{
					statePop(); 				//out of PG_END state
					newLevel();
				}
				break;
			case SU_BONUS:					//BONUS!! - You got all words
				statePop(); 				//out of PG_END state
				newLevel();
				break;
			case SU_BADLUCK:				//Bad Luck! - Out of time
				exit(ST_MENU);
				break;

			default:
			case SU_GAMEOVER:				//Game Over - You're a high scorer!
				exit(ST_HIGHEDIT);
				break;
			}
		}
		break;
	case Input::LEFT:
		if (input->isPressed(b))
		{
			do {	//repeat jump left if finds a gap (missing 3, 4, 5 letter word)
				if (_xxWordHi-1 < 0) _xxWordHi = 3; else --_xxWordHi;
			} while (_gd._words.wordsOfLength(_xxWordHi) == 0);
			if (_yyWordHi > _gd._words.wordsOfLength(_xxWordHi)-1) _yyWordHi = _gd._words.wordsOfLength(_xxWordHi)-1;
		}
		break;
	case Input::RIGHT:
		if (input->isPressed(b))
		{
			do {	//repeat jump right if finds a gap (missing 3, 4, 5 letter word)
				if (_xxWordHi+1 > 3) _xxWordHi = 0; else ++_xxWordHi;
			} while (_gd._words.wordsOfLength(_xxWordHi) == 0);
			if (_yyWordHi > _gd._words.wordsOfLength(_xxWordHi)-1) _yyWordHi = _gd._words.wordsOfLength(_xxWordHi)-1;
		}
		break;
	case Input::UP:
		if (input->isPressed(b))
			if (_yyWordHi-1 < 0) _yyWordHi = _gd._words.wordsOfLength(_xxWordHi)-1; else --_yyWordHi;
		break;
	case Input::DOWN:
		if (input->isPressed(b))
			if (_yyWordHi+1 > _gd._words.wordsOfLength(_xxWordHi)-1) _yyWordHi = 0; else ++_yyWordHi;
		break;

	case Input::Y:
	case Input::CLICK:
//		if (!_bAbort && //make sure global L+R+Click not pressed
		if (input->isPressed(b))
		{
			doDictionary();		//toggle dictionary mode
		}
		break;

	default:break;
	}

}
void PlayGame::button_dict(Input* input, Input::ButtonType b)
{
	switch (b)
	{
	case Input::UP: 
		if (input->isPressed(b)) 
			//move the _dictLine offset var up a line
			if (_dictDef.size()>DICT_LINES && _dictLine > 0) --_dictLine;
		break;
	case Input::DOWN: 
		if (input->isPressed(b))
			//move the _dictLine offset var down a line
			if (_dictDef.size()>DICT_LINES && _dictLine < (int)_dictDef.size()-DICT_LINES) ++_dictLine;
		break;
	case Input::Y:
	case Input::CLICK:
		if (input->isPressed(b)) doDictionary();
		break;

	default:break;
	}
}
void PlayGame::button_pause(Input* input, Input::ButtonType b)
{
	switch (b)
	{
	case Input::START:
	case Input::B: 
		if (input->isPressed(b)) doPauseGame();	//will un-pause as it's in pause mode
		break;

	default:break;
	}
}
void PlayGame::button_popup(Input* input, Input::ButtonType b)
{
	_pPopup->button(input, b);
	if (!_pPopup->isSelected()) return;
	
	switch (_pPopup->selected())
	{
	case PlayGamePopup::POP_CANCEL:
		//continue with game level
		break;
	case PlayGamePopup::POP_SKIP:
		if (_state != PG_END)	//not already at end
			pushEvent(USER_EV_END_COUNTDOWN);	//next level - if not got a 6, will end game
		break;
	case PlayGamePopup::POP_QUIT:
		//back to main menu - like L+R+Click (or ESC on PC)
		_bAbort = true;
		pushEvent(USER_EV_END_COUNTDOWN); //try to exit more gracefully, pushes end of level
		break;
	
	default:break;	//do nothing
	}
	
	stopPopup();	//deletes _pPopup after user selects, if SELECT pressed, button() fn stops popup
}

//process any events not handled by the main input function
void PlayGame::handleEvent(SDL_Event &sdlevent)
{
	if (sdlevent.type == SDL_USEREVENT)
	{
		if (USER_EV_PING_COUNTDOWN == sdlevent.user.code)
		{
			if ( --_countdown < 11 || _pPopup)	//added pinger when in popup to warn user that timer still going...
			{
				Mix_PlayChannel(-1,_gd._fxCountdown,0);
			}
		}
		else
		if (USER_EV_END_COUNTDOWN == sdlevent.user.code)
		{
			if ((6 == _maxwordlen && !_bAbort) 					//must be at least one 6 letter word found to continue
				&& !(_countdown==0 && GM_TIMETRIAL==_gd._mode))	//and not timetrial that has reahed 0 (timetrial still going is ok though)
			{
				//if fastest show a flash of the bonus, maybe below the wordlist
				//just to tell user they got the fastest time... and a bonus...?
				_fastest = _fastestCountStart - _countdown;
				if (!_gd._score.setCurrFastest(_fastest)) _fastest = 0;	//reset to 0 if not the fastest (so we can display)
				if (_gd._score.currWords()==0) _fastest = 0;	//never bonus if first word, but has recorded it as fastest for scoreboard
				
				if (_gd._mode == GM_REWORD && allWordsFound())
				{
					//wow! all words got so bonus and go to next level
					Mix_PlayChannel(-1,_gd._fxBonus,0);
					int bonus = SCORE_BONUS + (_countdown*SCORE_SECONDS);	//say: 100 + remaining seconds * 10
					_gd._score.addCurrScore(bonus);
					showSuccess(SU_BONUS, bonus);
				}
				else
				{
					//great! at least one 6 letter found so go to next level
					//If in SPEED6 or TIMETRIAL mode, only one 6 letter word needed to move on...

					//fill rest of found words list with words to be found and
					//display them in diff colour to show player what they missed
					fillRemainingWords();

					Mix_PlayChannel(-1,_gd._fx6found,0);

					if (GM_REWORD == _gd._mode)
					{
						_gd._score.addCurrScore(SCORE_BONUS);
						showSuccess(SU_GOT6, SCORE_BONUS);
					}
					else
					{
						//it's GM_SPEED6 or GM_TIMETRIAL so add score and any fastest bonus, 
						//then move to next word
						_gd._score.addCurrScore(SCORE_WORD6);
						if (GM_SPEED6 == _gd._mode)
						{
							if (_fastest) _gd._score.addCurrScore((maxCountdown()-_fastest)*SCORE_FASTEST);
							showSuccess(SU_SPEED6);
						}
						else //GM_TIMETRIAL
						{
							if (_fastest) _gd._score.addCurrScore((maxCountdown()-_fastest)*SCORE_FASTEST);
							showSuccess(SU_TIMETRIAL, _fastest);
						}
					}
				}
				_gd._score.addCurrWords(1);
			}
			else
			{
				//oh dear, ran out of time (or player forced exit)

				//fill rest of found words with words to be found and 
				//display them in diff colour to show player what they missed
				fillRemainingWords();

				if (_gd._score.isHiScore(_gd._mode, _gd._diffLevel) != -1)
				{
					//excellent! player got on scoreboard
					Mix_PlayChannel(-1,_gd._fx6notfound,1);	//play twice if got on hiscore
					showSuccess(SU_GAMEOVER);
				}
				else
				{
					//oh dear, ran out of time without a high enough score
					Mix_PlayChannel(-1,_gd._fx6notfound,2);	//play 3 times if no hiscore
					showSuccess(SU_BADLUCK);
				}
			}
		}
	}
}

//toggle dictionary mode at end of level to allow player 
//to see the full definition of a word
void PlayGame::doDictionary()
{
	if (PG_DICT == _state)
	{
		//put state back to end of level state as thats the 
		//only state we can get to dictionary mode from
		statePop(); //_state = PG_END;
	}
	else if (PG_END == _state) 	//fix, was doing dict in PAUSE
	{
		//get the word currently highlighted
		std::string dictWord, dictDefinition, dictDefLine;
		tWordsFoundList::const_iterator it;
		it = _wordsFound[_xxWordHi].begin();
		for (int yy=0; yy<(int)_wordsFound[_xxWordHi].size(); ++yy)	//find the matching word in the list
		{
			if (_yyWordHi == yy && it != _wordsFound[_xxWordHi].end())
			{
				dictWord = (*it)._word;
				break;
			}
			++it;
		}

		//now build definition strings to display (on seperate lines)
		_dictDef.clear();
		_dictLine = 0;	//start on first line
		//set arrow (scroll positions)
		_gd._arrowUp.setPos(300, 68);		//positions dont change, just made visible or not if scroll available
		_gd._arrowUp.startAnim(0, -1, ImageAnim::ANI_REVERSE, 100, 0);
		_gd._arrowDown.setPos(300, 185);
		_gd._arrowDown.startAnim(0, -1, ImageAnim::ANI_REVERSE, 100, 0);
		//get the dictionary definition into one long string
		dictDefinition = _gd._words.getDictForWord(dictWord)._description;
		Utils::trimLeft(dictDefinition, " \t\n\r");	//NOTE need \n\r on GP2X
		Utils::trimRight(dictDefinition, " \t\n\r");//ditto
		if (!dictDefinition.length()) dictDefinition = "** sorry, not defined **";	//blank word - no dictionary entry
		//split definition up into lines displayable on screen...
		int start = 0, lastBreak = 0, curr = 0;
		const int MAX = 41;
		while (1) 
		{
			if (curr-start > MAX || curr == (int)dictDefinition.length()-1)
			{
				//if no break found, just copy up to the MAX chars and cut off
				if (lastBreak == start)	lastBreak = start+MAX;
				//if we are at end of definitioin, make sure its all added
				if (curr == (int)dictDefinition.length()-1) 
					lastBreak = (int)dictDefinition.length();

				//copy from start to lastBreak as one line
				dictDefLine = dictDefinition.substr(start, lastBreak-start);
				_dictDef.push_back(dictDefLine);	//add to list of lines
				start = lastBreak+1;
			}
			//find next break
			if (' ' == dictDefinition[curr]) lastBreak = curr;
			//are we completely done?
			if (++curr >= (int)dictDefinition.length()) break;
		}
		
		//prepare roundel class ready for a dictionary display
		_roundDict= std::auto_ptr<Roundels>(new Roundels());
		_roundDict->setWordCenterHoriz(dictWord, _gd._letters, 4, 4);
		_roundDict->startMoveFrom(Screen::width(), 0, 10, 50, 18, 0);

		//display current highlighted word on seperate 'screen'
		statePush(PG_DICT); //_state = PG_DICT;
	}
}

void PlayGame::doPauseGame()
{
	//check if already paused
	if (PG_PAUSE == _state)
	{
		//is paused so unpause
		statePop(); //_state = _oldState;

		//penalty for pause, at least 10 seconds used
		if (_countdown > 10) _countdown-=10; 

		//release the countdown timer
		startCountdown();
		return;
	}
	//else is not paused, so check if we can pause

	//if not in play mode dont need to pause the game
	if (PG_PLAY != _state) return;

	//only on easy mode, or at least one 6 letter word found on this level
	if (DIF_EASY == _gd._diffLevel || _wordsFound[3].size())
	{
		//pause the countdown timer
		stopCountdown();

//		_oldState = _state;
		statePush(PG_PAUSE); //_state = PG_PAUSE;

		//prepare roundel class ready for a pause
		_roundPaused = std::auto_ptr<Roundels>(new Roundels());
		_roundPaused->setWordCenterHoriz(std::string("PAUSED"), _gd._letters, 100, 4);
		_roundPaused->startMoveFrom(Screen::width(), 0, 10, 50, 18, 0);

		Mix_FadeOutChannel(-1, 1000);
	}
}

void PlayGame::newGame()
{
	if (_gd._mode == GM_REWORD)	//classic mode
	{
		_boxOffset[0] = 10;
		_boxOffset[1] = 65;
		_boxOffset[2] = 135;
		_boxOffset[3] = 220;
	}
	else	//GM_SPEED6 or GM_TIMETRIAL
	{
		//current_w not supported in my gp2x SDL build, so saved from video setup
//		const SDL_VideoInfo *pVI = SDL_GetVideoInfo();	//for pVI->current_w
	
		_boxOffset[0] = 0;
		_boxOffset[1] = 0;
		_boxOffset[2] = 0;
		_boxOffset[3] = (_gd._current_w / 2) - (_gd._boxes.tileW() / 2);  	//120;	//middle of screen as only a single 6 letter word column
	
		//set initial timetrial counter as each level does not reset the timer
		_countdown = maxCountdown();	
	}
	
	_gd._score.resetCurr();
	_gd._unmatchedWords.clear();
	
	Mix_FadeOutChannel(-1, 1000);	//fade out menu music sound effects etc
////	Mix_FadeOutMusic(1000);	//and any music	##TODO##

}

//push an event into the SDL event queue
void PlayGame::pushEvent(int code, void *data1, void *data2)
{
	//push event "end of level"
    SDL_Event event;
    SDL_UserEvent userevent;

    userevent.type = SDL_USEREVENT;
    userevent.code = code;

	userevent.data1 = data1;
    userevent.data2 = data2;

    event.type = SDL_USEREVENT;
    event.user = userevent;

    SDL_PushEvent(&event);
}

//timer specific to the countdown timer
//no need for anything else in this game
void PlayGame::startCountdown()
{
	//(xthou / 10) * 10;  /* To round it down to the nearest 10 ms */

	Uint32 rate = 1000; //1 second
	stopCountdown();	//make sure its stopped first
	_countdownID = SDL_AddTimer(rate, countdown_callback, NULL);
}

//stop the countdown timer redrawing the screen
void PlayGame::stopCountdown()
{
	if (_countdownID)
	{
		if (SDL_RemoveTimer(_countdownID)) _countdownID = 0;
		clearEventBuffer();

	  // Fade out all the audio channels in 1 second
//	  Mix_FadeOutChannel(-1, 1000);

		// and the music in 1 second
	  //Mix_FadeOutMusic(1000);	//dont have any right now

	  // wait one second for fade to finish
//	  SDL_Delay(1000);

	}
}

//clear any unused events from the event queue
//used to prevent timer going off after being closed etc
void PlayGame::clearEventBuffer()
{
	SDL_Event sdlevent;
	while (SDL_PollEvent(&sdlevent));	//empty event buffer
}

//define the number of seconds each difficulty level gets (in global.h)
int PlayGame::maxCountdown()
{
#ifndef _DEBUG
	if (_gd._mode == GM_TIMETRIAL)
		return ((int)DIF_MAX - _gd._diffLevel)*COUNTDOWN_TIMETRIAL;
	if (_gd._mode == GM_SPEED6)
		return ((int)DIF_MAX - _gd._diffLevel)*COUNTDOWN_SPEED6;
	//else its REWORD
	return ((int)DIF_MAX - _gd._diffLevel)*COUNTDOWN_REWORD;
#else
	return 30;	//##DEBUG##
#endif
}

void PlayGame::newLevel()
{
	stopCountdown();
	
	//maybe show a filler screen here..?
	//##TODO## ??

	int xx;
	for (xx=0; xx<4; ++xx)
		_wordsFound[xx].clear();

	std::string newword;
	//nextWord() returns false if bad dictionary entry (XXXXXX corrupted or hacked) !
	bool bWord = _gd._words.nextWord(newword, _gd._diffLevel, _gd._mode);	//return next word found.

	//setup the top and bottom rows
//	_round.setWord(_gd._words.getWord6(), _gd._letters, SCRATCHX+2, SCRATCHY1+2, 6, true);
	_round.setWord(newword, _gd._letters, SCRATCHX+2, SCRATCHY1+2, 6, true);
	if (bWord) 
		_round.jumbleWord(false);		//randomize the letters
	_round.startMoveFrom(Screen::width(), 0, 15, 100, 18, 0);//animate roundels into screen pos

	//paint this levels word boxes onto the background for quick blit display rather
	//than having to loop through and blit each box every frame as in prev versions
	prepareBackground();

	//if in REWORD or SPEED6 then reset the timer. TimeTrial keeps counting down...
	if (_gd._mode != GM_TIMETRIAL)
		_countdown = maxCountdown();	//60 seconds for hard, 120 for med, 180 for easy

	_fastestCountStart = _countdown;	//either continuing in TIMETRIAL, or else restarted
	_success = SU_NONE;	
	_bonusScore = 0;
	_fastest = 0;
	_xxWordHi = 3;			//highlight the 6 letter word column (for dict definition on end of level)
	_yyWordHi = 0;			//highlight first 6 letter word in column
	_maxwordlen = 0;		//always reset the fact that a 6 letter word not got yet

	//prepare random 'sucess' title here ready for end of level
	_randomTitle = g_randInt.random(4);		//0..3

	startCountdown();
	clearEventBuffer();	//start fresh each level
}

//show a SUCCESS! screen over play area before continuing with next level
//passes in one of the eSuccess types: SU_GOT6, SU_BONUS, SU_BADLUCK, SU_GAMEOVER
void PlayGame::showSuccess(eSuccess newSuccess, int newBonus)
{
	if (SU_TIMETRIAL != newSuccess)
		stopCountdown();
	
	statePush(PG_WAIT); //_state = PG_WAIT;
	_success = newSuccess;
	_bonusScore = newBonus;

	//make sure player doesn't clear the screen before its seen
	clearEventBuffer();
	_waiting.start(500);	//half second before user can continue after level done
}

//ok, match whatever's in the bottom row with our list of words
//that are made up from the top row letters/word
//Returns the number of letters matched in a word (ie 3,4,5 or 6 letter word)
//Return 0 if a word already used, or -1 if its not in the list at all
//Also saves curr bottom word in "last" array so we can repopulate it if player asks 
int PlayGame::tryWordAgainstDict()
{
	std::string newword = _round.getBottomWord();

	int ret = _gd._words.checkWordsInTarget(newword);
	if (ret == -1 && newword.length() > 2) _gd._unmatchedWords.insert(newword);	//save for add to personal dict
	if (ret < 1) return ret; //0=already found, -1=not found
	//else found in target and is set to 'found' in list

	int w = newword.length()-3;

	//add found word to (front of) list of words of that length already found
	//(and so be displayed in reverse found order in the boxes under the main letters)
	tDictWord dictWord;
	dictWord._word = newword;
	dictWord._found = true;
	_wordsFound[w].push_front(dictWord);

	std::sort(_wordsFound[w].begin(), _wordsFound[w].end(), tDictWord());

	//save max len word so far
	if ((int)newword.length() > _maxwordlen) _maxwordlen = newword.length();

	return newword.length();	
}

//return true if all words available for the 6 letter word have been found
bool PlayGame::allWordsFound()
{
	bool bAllWords = true;
	int xx;
	for (xx=0; xx<4; ++xx)
		//if max words found, or all words on screen (if some are off screen) found
		if ((_gd._words.wordsOfLength(xx) <= MAX_WORD_COL && _gd._words.wordsOfLength(xx)!=(int)_wordsFound[xx].size())
			|| (_gd._words.wordsOfLength(xx) > MAX_WORD_COL && _wordsFound[xx].size() < MAX_WORD_COL))
			bAllWords = false;
	return bAllWords;
}

//now at end of level so fill the remaining spaces with the words not yet found
void PlayGame::fillRemainingWords()
{
	const tWordsInTarget wit = _gd._words.getWordsInTarget();
	tWordsInTarget::const_iterator it;
	tWordsFoundList::iterator foundit;
	int w;
	std::string wrd;
	bool bFound;
	tDictWord dictWord;
	
	for (it=wit.begin(); it != wit.end(); ++it)
	{
		wrd = (*it).first.c_str();
		w = wrd.length()-3;
		bFound = false;
		for (foundit = _wordsFound[w].begin(); foundit != _wordsFound[w].end(); ++foundit)
		{
			if ((*foundit)._word == wrd)
			{
				bFound = true;
				break;
			}
		}
		if (!bFound)
		{
			//we only need the word and "not found"
			dictWord._word = wrd;
			dictWord._found = false;
			_wordsFound[w].push_front(dictWord);
		}

	}

	for (w=0; w<4; ++w)	//do each 3, 4, 5 & 6 word group
		std::sort(_wordsFound[w].begin(), _wordsFound[w].end(), tDictWord());
}

void PlayGame::prepareBackground()
{
	//create the background to be used for this level, 
	//pre drawing so we dont need to do it each frame.
	//...
	_gamebg = std::auto_ptr<Image>(new Image());
	_gamebg->createThisFromImage(_gd._gamebg);	//copy of basic gamebg image

	//prerender the score and words titles 
	_gd._fntSmall.put_text(_gamebg.get(), 11, 7, "SCORE:", WHITE_COLOUR);
	_gd._fntSmall.put_text(_gamebg.get(), 154, 7, "WORDS:", WHITE_COLOUR);

	//draw mode in bot right corner (before/under word boxes so if 8 6-letter words, it doesnt cover anything up)
	switch (_gd._mode)
	{
	case GM_REWORD:		_gamebg->blitFrom(&_gd._game_reword, -1, 195, 115);
						break;
	case GM_SPEED6:		_gamebg->blitFrom(&_gd._game_speed6, -1, 195, 115);
						break;
	case GM_TIMETRIAL:	_gamebg->blitFrom(&_gd._game_timetrial, -1, 195, 115);
						break;
	default:break;
	}
	//draw difficulty level in bot right corner (before/under word boxes so if 8 6-letter words, it doesnt cover anything up)
	_gd._fntSmall.put_text_right(_gamebg.get(), 224, _gd._diffName.c_str(), _gd._diffColour);
}


