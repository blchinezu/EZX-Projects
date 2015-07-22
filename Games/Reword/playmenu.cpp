////////////////////////////////////////////////////////////////////
/*

File:			PlayMenu.cpp

Class impl:		PlayMenu

Description:	A class derived from the IPlay interface to handle all screen
				events and drawing of the Menu screen

Author:			Al McLuckie (al-at-purplepup-dot-org)

Date:			06 April 2007

History:		Version	Date		Change
				-------	----------	--------------------------------
				0.3.1	07.06.2007	Speed up menu movement a little

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
#include "playmenu.h"

#include <string>


PlayMenu::PlayMenu(GameData &gd)  : _gd(gd)
{
}

void PlayMenu::init(Input *input)
{
}

void PlayMenu::render(Screen *s)
{
}

void PlayMenu::work(Input *input, float speedFactor)
{
}

void PlayMenu::button(Input *input, Input::ButtonType b)
{
	switch (b)
	{
	case Input::UP: 
		if (input->isPressed(b) && _itemList.size()) 
			if (0==_item)
				_item=_itemList.size()-1;	//down to bottom
			else 
				_item--;
		break;
	case Input::DOWN: 
		if (input->isPressed(b) && _itemList.size())
			if (_itemList.size()-1==_item)
				_item=0;	//back to top
			else 
				_item++;
		break;
	case Input::CLICK: 
	case Input::B:
		if (input->isPressed(b))
		{
			if (!_itemList[_item]._enabled) break;
			_selected = _item+1;	//return [1..n] to caller
		}
		break;

	default:break;
	}
}

Uint32 PlayMenu::addItem(MenuItem &mi)
{
	_itemList.push_back(mi);
	return _itemList.size();	//return new no. of items in menu
}

void PlayMenu::setItem(Uint32 item)
{
	if (item && item <= _itemList.size())
		_item = item-1;
}

//item asked for is [1..n]
//internal _item is [0..n]
MenuItem PlayMenu::getItem(Uint32 item)
{
	if (!item || item > _itemList.size())
	{
		MenuItem mi;
		return mi;	//blank
	}
	return _itemList[item-1];
}

//return menu option selected 1..n, 0="menu cancelled"
MenuItem PlayMenu::getSelected()
{
	return getItem(_selected);
}


