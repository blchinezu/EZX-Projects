#include <string>

#include "./SongSelectionState.h"
#include "../StateModule/GameStateManager.h"
#include "../GfxModule/GfxModule.h"
#include "../SoundModule/SoundModule.h"
#include "../InputModule/Input.h"

#include "../Timer/SdlTimer.h"

#include "../Game/PlayState.h"

#include "../DanceMachine/DwiParser.h"

using namespace std;

using namespace GfxModule;
using namespace SoundModule;
using namespace GameStateModule;
using namespace InputModule;

using namespace TimerModule;

SongSelectionState::SongSelectionState(void)
	:	_BGSurface(0), _BGObject(0), _BackSfx(0), _ForwardSfx(0)
{

}

SongSelectionState::~SongSelectionState(void)
{

}

void SongSelectionState::PushInitialise(
    GfxModuleWrapper* AGfxModule,
    SoundModuleWrapper* ASoundModule
)
{
	// Load the background image
	_BGSurface = AGfxModule->GetSurfaceManager()->CreateSurface("./Backgrounds/SongSelect.png");
	// Grab the background layer
	_BGObject = AGfxModule->GetBGManager()->GetBGObject(0);
	// Clear the BG Object and put the two together
	AGfxModule->GetBGManager()->ClearBGObject(_BGObject);
	_BGObject->SetSurfaceWrapper(_BGSurface);

	// Load the Sound effects
	_BackSfx = ASoundModule->GetSfxManager()->CreateSfx("./Sounds/Common back.ogg");
	_ForwardSfx = ASoundModule->GetSfxManager()->CreateSfx("./Sounds/Common start.ogg");
}

void SongSelectionState::PushCleanup(
    GfxModuleWrapper* AGfxModule,
    SoundModuleWrapper* ASoundModule
)
{
	// Free the Surface
	AGfxModule->GetBGManager()->ClearBGObject(_BGObject);
	AGfxModule->GetSurfaceManager()->FreeSurface(_BGSurface);

	// Free the sounds
	ASoundModule->GetSfxManager()->FreeSfx(_BackSfx);
	ASoundModule->GetSfxManager()->FreeSfx(_ForwardSfx);
}

void SongSelectionState::PopInitialise(
    GfxModuleWrapper* AGfxModule,
    SoundModuleWrapper* ASoundModule
)
{
	this->PushInitialise(AGfxModule, ASoundModule);
}

void SongSelectionState::PopCleanup(
    GfxModuleWrapper* AGfxModule,
    SoundModuleWrapper* ASoundModule
)
{
	this->PushCleanup(AGfxModule, ASoundModule);
}

void SongSelectionState::Update(
    GfxModuleWrapper* AGfxModule,
    SoundModuleWrapper* ASoundModule,
    Input* AInputModule,
    GameStateManager* AGameStateManager
)
{
	// Quit flag
	bool QuitFlag = false;
	// Get the current input and previous input states
    unsigned int CurrentInputState = AInputModule->GetCurrentInputState();
    unsigned int PreviousInputState = AInputModule->GetPreviousInputState();

	if ( CurrentInputState & BUTTON_B )
    {
        // Go to the playing stage
        if ( !(PreviousInputState & BUTTON_B) )
        {
			// TODO : Send initialising messages to the Playing state (song title etc)
			if(PlayState::GetSingletonPtr()->SetFilePath("", BEGINNER) )
			{
				ASoundModule->GetSfxManager()->PlaySfx(_ForwardSfx);
				SdlTimer::Delay(800);
				AGameStateManager->Push(PlayState::GetSingletonPtr() );
			}
        }
    }


	if ( CurrentInputState & BUTTON_X )
    {
        ASoundModule->GetSfxManager()->PlaySfx(_BackSfx);
		SdlTimer::Delay(800);
        // Quit the game if B is pressed when Quit is selected
        if ( !(PreviousInputState & BUTTON_X) )
           QuitFlag = true;
    }

    // If the flag is true, then pop this state
    if( QuitFlag )
		AGameStateManager->Pop();
}

/*
History
=======
2006-07-16: Added forward and back sound effects
2006-07-14: Created file and base functionality.
*/
