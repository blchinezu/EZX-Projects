//playinst.h
 
#ifndef _PLAYINST_H
#define _PLAYINST_H

#include "play.h"		//IPlay interface
#include "screen.h"
#include "input.h"
#include "gamedata.h"	//also holds constants and stuff
#include "states.h"
#include "roundels.h"

#include <string>
#include <vector>

class PlayInst : public IPlay
{
public:
	PlayInst(GameData& gd);
	virtual ~PlayInst() {}

    virtual void init(Input *input);
    virtual void render(Screen* s);
    virtual void work(Input* input, float speedFactor);
    virtual void button(Input* input, Input::ButtonType b);

protected:
	void buildPage(int page);
	
private:
	
	GameData &	_gd;			//shared data between screens (play classes)
	int			_page;
	Roundels	_round;

	std::vector<std::string> _inst;
	int	_instLine;				//offset into _dictDef (ie start at _dictDef.begin+_dictLine)
	SDL_Color	_txtColour;

};

#endif //_PLAYINST_H
