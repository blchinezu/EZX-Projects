#include "./PlayState.h"
#include "../StateModule/GameStateManager.h"
#include "../GfxModule/GfxModule.h"
#include "../SoundModule/SoundModule.h"
#include "../InputModule/Input.h"
#include "../Logger/Logger.h"

#include "../Timer/SdlTimer.h"

#include "../Game/RatingState.h"

using namespace std;

using namespace GfxModule;
using namespace SoundModule;
using namespace GameStateModule;
using namespace InputModule;
using namespace LogModule;

using namespace TimerModule;


PlayState::PlayState(void) : _NumberOfFramesPassed(0)
{

}

PlayState::~PlayState(void)
{

}

bool PlayState::SetFilePath(const string &FilePath,  const SongDifficulty Difficulty)
{
	// TODO : Find the images needed to pass to the DanceMachine
	_DanceMachine.SetFilenames(
		"./Songs/Dan (Old Skool Mix)/danoldskool.ogg",
		"./Songs/Dan (Old Skool Mix)/danoldskool.dwi",
		"./Songs/Dan (Old Skool Mix)/danoldskool-bg.jpg");
	_DanceMachine.SetSongDifficulty(BASIC);
	return true;
}

void PlayState::PushInitialise(
    GfxModuleWrapper* AGfxModule,
    SoundModuleWrapper* ASoundModule
)
{
	// Load the sounds
	_BackSfx = ASoundModule->GetSfxManager()->CreateSfx("./Sounds/Common back.ogg");
	_ForwardSfx = ASoundModule->GetSfxManager()->CreateSfx("./Sounds/Common start.ogg");
	_FrameCounter = 0;
	// Initialise the Dance Machine
	_DanceMachine.Initialise(AGfxModule, ASoundModule);
}

void PlayState::PushCleanup(
    GfxModuleWrapper* AGfxModule,
    SoundModuleWrapper* ASoundModule
)
{
	// Cleanup the dance machine
	_DanceMachine.Cleanup(AGfxModule, ASoundModule);
	// Free the sounds
	ASoundModule->GetSfxManager()->FreeSfx(_BackSfx);
}

void PlayState::PopInitialise(
    GfxModuleWrapper* AGfxModule,
    SoundModuleWrapper* ASoundModule
)
{
	this->PushInitialise(AGfxModule, ASoundModule);
}

void PlayState::PopCleanup(
    GfxModuleWrapper* AGfxModule,
    SoundModuleWrapper* ASoundModule
)
{
	this->PushCleanup(AGfxModule, ASoundModule);
}

void PlayState::Update(
    GfxModuleWrapper* AGfxModule,
    SoundModuleWrapper* ASoundModule,
    Input* AInputModule,
    GameStateManager* AGameStateManager
)
{
	// Number of Frames to pass while select is being held down before quitting the state
	const unsigned int TotalFramesToQuit = 15;

	// Get the current input and previous input states
    unsigned int CurrentInputState = AInputModule->GetCurrentInputState();

    // Call the dance machine update function
    _DanceMachine.Update(AGfxModule, ASoundModule, AInputModule);

	if ( CurrentInputState & BUTTON_SELECT )
    {
		++_NumberOfFramesPassed;
    }
    else
    {
		_NumberOfFramesPassed = 0;
	}

    // If the flag is true, then pop this state twice to get to the start menu
    if( _NumberOfFramesPassed > TotalFramesToQuit )
    {
		ASoundModule->GetSfxManager()->PlaySfx(_BackSfx);
		SdlTimer::Delay(800);
		AGameStateManager->Pop();
		AGameStateManager->Pop();
	}

	// Check if the music has stopped, if it has then quit
	if( ASoundModule->GetMusicManager()->HasMusicStopped() /*|| CurrentInputState & BUTTON_START*/)
	{
		++_FrameCounter;
		if(_FrameCounter > 30)
		{
			ASoundModule->GetSfxManager()->PlaySfx(_ForwardSfx);
			SdlTimer::Delay(800);

			// Variables to hold the stats
			unsigned int 	HighestCombo,
							NumberOfMissSteps,
							NumberOfBooSteps,
							NumberOfGoodSteps,
							NumberOfGreatSteps,
							NumberOfPerfectSteps,
							NumberOfMarvelousSteps,
							CurrentScore;
			bool 			NotFailFlag;

			// Get the stats from the dance machine
			_DanceMachine.GetAllStats(	HighestCombo,
										NumberOfMissSteps,
										NumberOfBooSteps,
										NumberOfGoodSteps,
										NumberOfGreatSteps,
										NumberOfPerfectSteps,
										NumberOfMarvelousSteps,
										CurrentScore,
										NotFailFlag
									);

			// Pass the stats to the Rating State
			RatingState::GetSingletonPtr()->SetRatings	(	HighestCombo,
															NumberOfMissSteps,
															NumberOfBooSteps,
															NumberOfGoodSteps,
															NumberOfGreatSteps,
															NumberOfPerfectSteps,
															NumberOfMarvelousSteps,
															CurrentScore,
															NotFailFlag
														);
			AGameStateManager->Pop();
		}
	}
}

/*
History
=======
2006-08-03: Added end game case when the music stops
2006-07-16: Created file and base functionality.
*/
