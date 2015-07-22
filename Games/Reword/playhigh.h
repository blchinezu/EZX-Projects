//playhigh.h
 
#ifndef _PLAYHIGH_H
#define _PLAYHIGH_H

#include "play.h"		//IPlay interface
#include "screen.h"
#include "input.h"
#include "gamedata.h"	//also holds constants and stuff
#include "states.h"
#include "roundels.h"


class PlayHigh : public IPlay
{
public:
	PlayHigh(GameData& gd);
	virtual ~PlayHigh() {}

    virtual void init(Input *input);
    virtual void render(Screen* s);
    virtual void work(Input* input, float speedFactor);
    virtual void button(Input* input, Input::ButtonType b);

protected:
	void setDifficulty(eDifficulty diff);
	void setMode(eGameMode mode);
	void setDescription();
	void prepareBackground();
	
private:
	GameData &	_gd;		//shared data between screens (play classes)
	std::auto_ptr<Image> _menubg;
	
	int			_pos;
	int			_currPos;		//which of the 3 inits you are currently editing

	tHiScoreEntry _curr;	//temp inits during payer editing

	int			_diff;			//curr difficulty hiscore table to display
	SDL_Color	_diffColour;	//colour of text to denote hi score difficulty
	int			_mode;			//locally used game mode
	std::string	_description;	//"mode : difficulty"

	Roundels	_round;
};

#endif //_PLAYHIGH_H
