//PlayMenu.h
 
#ifndef _PlayMenu_H
#define _PlayMenu_H

#include "play.h"		//IPlay interface
#include "screen.h"
#include "input.h"
#include "gamedata.h"	//also holds constants and stuff
#include "roundels.h"


struct MenuItem
{
	int			_id;		//item id
	SDL_Color	_hoverOff;	//title colour
	SDL_Color	_hoverOn;	//title colour when highlighted
	std::string	_title;		//menu text
	std::string _comment;	//tooltip type help text
	bool		_enabled;	//greyed out?
	bool		_highlight;	//set to highlight even when not hover over (eg diff setting prev selected in diff menu)

};

typedef std::vector<MenuItem> tMenuItem;

class PlayMenu : public IPlay
{
public:
	PlayMenu(GameData& gd);
	virtual ~PlayMenu() {}

    virtual void init(Input *input);
    virtual void render(Screen* s);
    virtual void work(Input* input, float speedFactor);
    virtual void button(Input* input, Input::ButtonType b);

	Uint32		addItem(MenuItem &i);	//returns item number
	void		setItem(Uint32 item);	//sets the current highlighted item
	MenuItem	getItem(Uint32 item);	//gets the ...
	MenuItem	getSelected();

private:
	GameData &	_gd;			//shared data between screens (play classes)

	tMenuItem	_itemList;		//list of menu items
	Uint32		_item;			//current item highlighted
	Uint32		_selected;		//item actually pressed/selected
};

#endif //_PlayMenu_H
