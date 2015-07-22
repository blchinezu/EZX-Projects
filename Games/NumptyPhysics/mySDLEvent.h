// mySDLEvent.hpp  writen by rock88
// 1.0

//    This file is part of OpenRedAlert.
//
//    OpenRedAlert is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, version 2 of the License.
//
//    OpenRedAlert is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with OpenRedAlert.  If not, see <http://www.gnu.org/licenses/>.

#ifndef MYSDLEVENT_HPP
#define MYSDLEVENT_HPP

#include <string>
#include <fstream>
#include "SDL/SDL.h"

int mySDL_PollEvent(SDL_Event *event);
void mySDL_GetMouseState(int *x, int *y);
int mySDL_WaitEvent(SDL_Event *event);

#endif //LOGGER_HPP
