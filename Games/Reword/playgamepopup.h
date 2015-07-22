//playgamepopup.h
 
#ifndef _PLAYGAMEPOPUP_H
#define _PLAYGAMEPOPUP_H

#include "play.h"		//IPlay interface
#include "screen.h"
#include "input.h"
#include "gamedata.h"	//also holds constants and stuff


class PlayGamePopup : public IPlay
{
public:
	PlayGamePopup(GameData& gd);
	virtual ~PlayGamePopup() {}

	enum eOptions { POP_CANCEL, POP_SKIP, POP_QUIT, POP_MAXVAL };
	enum eYNOptions { POP_NO, POP_YES, POP_YN_MAXVAL };	//same as POP_CANCEL etc as same positions in menu
//	void select(Uint32 i) { if (i >= POP_MAXVAL) _menuoption = 0; else _menuoption = i; }
	int selected() { return _selected; }
	bool isSelected() { return _bSelected; }
	void updateHelpStr();
	void setHasA6(bool b) { _hasA6 = b; }

    virtual void init(Input *input);
    virtual void render(Screen* s);
    virtual void work(Input* input, float speedFactor);
    virtual void button(Input* input, Input::ButtonType b);

private:
	GameData &	_gd;			//shared data between screens (play classes)
	std::string _helpStr;
	int			_menuoption;	//highlight position
	bool		_bSelected;
	int			_selected;
	bool 		_hasA6;			//must have a 6 letter word before some menu items available
	int 		_xPos[POP_MAXVAL];

	bool 		_bConfirm;		//user must select yes tpo exit game
};

#endif //_PLAYGAMEPOPUP_H
