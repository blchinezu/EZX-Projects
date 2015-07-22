#include "main.h"

void APP::Render() {
	if (status == ACTIVE) {
		// This is killing me
		// I still don't understand why I get more FPS with FullScreen
		// updates rather than using minimal screen updates.
		gfx.FullUpdate(gfx.Screen);
		//iclock.updateTime();
	}
}

