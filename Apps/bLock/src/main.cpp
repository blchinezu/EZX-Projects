#include "main.h"


// Initializing the classes
ZConfig		cfg(Path_Config, true);
CEZX		ezx;
APP			app;
GRAPHICS	gfx;
INFO_PANEL	infop;
iCLOCK		iclock;
unLOCK		unLock;

// Starting the application
int main(int argc, char* argv[]) {
	#ifdef DEBUG
	if (0 == 1) printf("%d%s", argc, argv[0]);
	// Only for removing build warnings... Totally useless
	#endif
	return app.Run();
}

