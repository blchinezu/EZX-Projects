#include <iostream>
#include <SDL/SDL.h>

#include "../Kernel/Kernel.h"
#include "../StateModule/GameStateManager.h"

#include "../Game/SplashPageState.h"
#include "../Game/TitleScreenState.h"
#include "../Game/SongSelectionState.h"
#include "../Game/RatingState.h"
#include "../Game/PlayState.h"

using namespace KernelModule;
using namespace GameStateModule;

int main ( int argc, char** argv )
{
	// Create a new Kernel
	Kernel *TheKernel = new Kernel(
        320,                    // Screen Width
        240,                    // Screen Height
        16,     					// Colour depth
        DEFAULT_VIDEO_MODE,     // Default video mode/SDL flags
        128,                    // Number of Surfaces
        5,                      // Number of Layers
        128,                    // Number of Sprites
        30,                     // FPS Cap
        16,						// Number of Sound effects
        4,						// Number of Sound channels
        4						// Number of Fonts
    );

    // Check the Kernel Status
    if( !TheKernel->GetStatus() )
    {
		// If bad, Bail
		return -1;
	}

	// Create a new State Manager [Singleton]
    GameStateManager * TheGameStateManager = new GameStateManager(TheKernel);

    // Create our states
    new TitleScreenState();
	new SplashPageState();
	new RatingState();
	new PlayState();

    // Add a new state
    TheGameStateManager->Push( TitleScreenState::GetSingletonPtr() );
    TheGameStateManager->Push( SplashPageState::GetSingletonPtr() );

	while( !TheGameStateManager->Empty() )
	{
        TheGameStateManager->Update();
	}

    // Free the memory
    delete PlayState::GetSingletonPtr();
    delete RatingState::GetSingletonPtr();
	delete SplashPageState::GetSingletonPtr();
	delete TitleScreenState::GetSingletonPtr();
	delete TheGameStateManager;
	delete TheKernel;

    // Return to the menu
//	#ifdef GP2X
//   chdir("/usr/gp2x");
//	execl("/usr/gp2x/gp2xmenu", "/usr/gp2x/gp2xmenu", NULL);
//	#endif // GP2X

	return 0;
}

/*
History
=======
2006-07-14: Added the StateManager to handle the states
*/
