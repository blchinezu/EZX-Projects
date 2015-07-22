////////////////////////////////////////////////////////////////////
/*

File:			gamedata.cpp

Class impl:		GameData

Description:	A wrapper class for all shared game data passed about in-game

Author:			Al McLuckie (al-at-purplepup-dot-org)

Date:			06 April 2007

History:		Version	Date		Change
				-------	----------	--------------------------------
				0.4.0	25.02.2008	Added game modes - "reword" "speed6" and "time trial"

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
#include "gamedata.h"
#include <iostream>

GameData::GameData() : _init(false)
{
	//initialise, load everything needed...
	bool bErr = false;

	std::string base = "data/";	//for linux builds, reword bin is peer level to data/ dir
#ifdef WIN32
	base = "../" + base;	//up to source dir as data/ is off that, not the debug or release dir
#endif
	//FONTS
	bErr |= !_fntTiny.load(base+"fonts/BD_Cartoon_Shout.ttf", 10);
	bErr |= !_fntSmall.load(base+"fonts/BD_Cartoon_Shout.ttf", 14);
	bErr |= !_fntMed.load(base+"fonts/BD_Cartoon_Shout.ttf", 16);
	bErr |= !_fntBig.load(base+"fonts/BD_Cartoon_Shout.ttf", 20);
	bErr |= !_fntClean.load(base+"fonts/FreeSansBold.ttf", 14);
	bErr |= !_fntTiny.load(base+"fonts/FreeSansBold.ttf", 11);

	//BACKGROUNDS & IMAGES
	bErr |= !_menubg.load(base+"images/menubg16.png", 255);		//solid background (no alpha)
	bErr |= !_menubg_plain.load(base+"images/menubg_plain16.png", 255);
	bErr |= !_menu_reword.load(base+"images/menu_reword16.png", 255);
	bErr |= !_menu_speed6.load(base+"images/menu_speed616.png", 255);
	bErr |= !_menu_timetrial.load(base+"images/menu_timetrial16.png", 255);
	bErr |= !_gamebg.load(base+"images/gamebg_plain16.png", 255);		//ditto
	bErr |= !_game_reword.load(base+"images/game_reword16.png", 255);
	bErr |= !_game_speed6.load(base+"images/game_speed616.png", 255);
	bErr |= !_game_timetrial.load(base+"images/game_timetrial16.png", 255);
	bErr |= !_scratch.load(base+"images/scratch16.png", 255);
	bErr |= !_cursor.load(base+"images/cursor_a16.png", 255);
	bErr |= !_cursor.setTileSize(CURSORW,CURSORH);
	bErr |= !_letters.load(base+"images/roundel_letters16.png", 255);
	bErr |= !_letters.setTileSize(LETTERW,LETTERH);
	bErr |= !_boxes.load(base+"images/boxes16.png", 255);
	bErr |= !_boxes.setTileSize(BOXW, BOXH);	//tile 0=3, 1=4, 2=5, 3=6 letter words
	bErr |= !_popupmenu.load(base+"images/popup_menu16.png", 215);

	//SPRITES
	bErr |= !_arrowUp.load(base+"images/arrowup16.png", 255, 5);		//5 frames
	bErr |= !_arrowDown.load(base+"images/arrowdown16.png", 255, 5);	//..
	bErr |= !_star.load(base+"images/star.png", 255, 7);

//	bErr |= !_moreWords.load(base+"images/morewords.png",255, 2);	//frame 1=more_to_get, 2=more_all_got

	//SOUNDS - no wrapper class so need to free on exit
	_fxCountdown = Mix_LoadWAV(std::string(base+"sounds/ping.wav").c_str());	//<10 seconds remaining
	_fxBadword = Mix_LoadWAV(std::string(base+"sounds/boing.wav").c_str());		//word not in dict
	_fxOldword = Mix_LoadWAV(std::string(base+"sounds/beepold.wav").c_str());	//word already picked 
	_fx6notfound = Mix_LoadWAV(std::string(base+"sounds/honk.wav").c_str());	//not found a 6 letter word
	_fx6found = Mix_LoadWAV(std::string(base+"sounds/binkbink.wav").c_str());	//found a/the 6 letter word
	_fxFound = Mix_LoadWAV(std::string(base+"sounds/blipper.wav").c_str());		//found a non 6 letter word
	_fxBonus = Mix_LoadWAV(std::string(base+"sounds/fanfare.wav").c_str());		//all words done before countdown
	_fxWoosh = Mix_LoadWAV(std::string(base+"sounds/woosh2.wav").c_str());		//jumble letters sound

//	_musicMenu = Mix_LoadMUS(std::string(base+"sounds/Andy_Wilman_Defection.wav").c_str());
//	_musicMenu = Mix_LoadMUS(std::string(base+"sounds/cascade.mod").c_str());
	std::string err = Mix_GetError();

	//SCORES ETC
	Uint32 hash = _score.load();

	//LOAD WORDS - 	//pass score hash + ticks as random seed
	bErr |= !_words.load(base+"words/rewordlist.txt", hash + SDL_GetTicks());

	//GAME STATES
	//start in main menu
	_state = ST_MENU;
	_menuoption = 0;	//start on [play] menu option
	//default to medium diff
	setDiffLevel(DIF_MED);
	//default to "classic" game mode
	_mode = GM_REWORD;

	//finally, are we done? If bErr was set true anywhere above then don't set _init true
	_init = !bErr;
}

GameData::~GameData()
{
	//other specific resources to clear up

	Mix_FreeChunk(_fxBadword);
	Mix_FreeChunk(_fxOldword);
	Mix_FreeChunk(_fxCountdown);
	Mix_FreeChunk(_fx6notfound);
	Mix_FreeChunk(_fx6found);
	Mix_FreeChunk(_fxFound);
	Mix_FreeChunk(_fxBonus);

//	Mix_FreeMusic(_musicMenu);
}




//set the relevant vars (value, name, colour) for the difficulty level
void GameData::setDiffLevel(eDifficulty newDiff)
{
	_diffLevel = newDiff;
	_diffName = (_diffLevel==DIF_EASY)?"   Easy":(_diffLevel==DIF_MED)?"Medium":"   Hard";
	_diffColour = (_diffLevel==DIF_EASY)?GREEN_COLOUR:(_diffLevel==DIF_MED)?ORANGE_COLOUR:RED_COLOUR;
}

