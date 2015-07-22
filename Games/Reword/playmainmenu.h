//PlayMainMenu.h
 
#ifndef _PlayMainMenu_H
#define _PlayMainMenu_H

#include "play.h"		//IPlay interface
#include "screen.h"
#include "input.h"
#include "gamedata.h"	//also holds constants and stuff
#include "roundels.h"


class PlayMainMenu : public IPlay	//##TODO derive from PlayMenu (generic menu base class)
{
public:
	PlayMainMenu(GameData& gd);
	virtual ~PlayMainMenu() {}

    virtual void init(Input *input);
    virtual void render(Screen* s);
    virtual void work(Input* input, float speedFactor);
    virtual void button(Input* input, Input::ButtonType b);

private:
	GameData &	_gd;			//shared data between screens (play classes)
	std::string _tmpstr;		//used in render()
	Roundels	_round;
	Waiting		_roundW;		//delay between jumbling
};

#endif //_PlayMainMenu_H
