#ifndef _OnCleanup_
#define _OnCleanup_

#include "../main.h"

void OnCleanup() {
	SK::Quit();		// TODO: Free the SK surfaces
	Graphics::Cleanup();
	Mouse::Cleanup();
    if(TTF_WasInit()!=0) TTF_Quit();
    SDL_Quit();
}

#endif /* _OnCleanup_ */

