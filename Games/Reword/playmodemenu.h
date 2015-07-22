//PlayModeMenu.h
 
#ifndef _PlayModeMenu_H
#define _PlayModeMenu_H

#include "play.h"		//IPlay interface
#include "screen.h"
#include "input.h"
#include "gamedata.h"	//also holds constants and stuff
#include "roundels.h"

#include <string>


class PlayModeMenu : public IPlay	//##TODO derive from PlayMenu (generic menu base class)
{
public:
	PlayModeMenu(GameData& gd);
	virtual ~PlayModeMenu() {}

    virtual void init(Input *input);
    virtual void render(Screen* s);
    virtual void work(Input* input, float speedFactor);
    virtual void button(Input* input, Input::ButtonType b);

    void updateHelpStr();

private:
	GameData &	_gd;			//shared data between screens (play classes)
//	std::string _tmpstr;		//used in render()
	Roundels	_round;
	Waiting		_roundW;		//delay between jumbling
	int			_menuoption;
	std::string _helpStr;
};

#endif //_PlayModeMenu_H
