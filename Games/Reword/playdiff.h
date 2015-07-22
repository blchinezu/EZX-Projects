//playdiff.h

#ifndef _PLAYDIFF_H
#define _PLAYDIFF_H

#include "play.h"		//IPlay interface
#include "screen.h"
#include "input.h"
#include "gamedata.h"	//also holds constants and stuff
#include "states.h"
#include "roundels.h"


class PlayDiff : public IPlay
{
public:
	PlayDiff(GameData& gd);
	virtual ~PlayDiff() {}

    virtual void init(Input *input);
    virtual void render(Screen* s);
    virtual void work(Input* input, float speedFactor);
    virtual void button(Input* input, Input::ButtonType b);
    
protected:
	void updateStrTimes();
	
private:
	GameData &	_gd;			//shared data between screens (play classes)

	eDifficulty	_newDiff;

	Roundels	_round;

	std::string _strTimes;
};

#endif //_PLAYDIFF_H
