#include "./SplashPageState.h"

#include "../StateModule/GameStateManager.h"

#include "../GfxModule/GfxModuleWrapper.h"
#include "../GfxModule/BGManager.h"
#include "../GfxModule/BGObject.h"
#include "../GfxModule/SurfaceManager.h"
#include "../GfxModule/SurfaceWrapper.h"

#include "../SoundModule/SoundModuleWrapper.h"
#include "../SoundModule/MusicManager.h"

#include "../InputModule/Input.h"

using namespace GfxModule;
using namespace SoundModule;
using namespace GameStateModule;
using namespace InputModule;

SplashPageState::SplashPageState(void)
	: _BGObjectA(0), _BGObjectB(0), _SurfaceWrapperA(0), _SurfaceWrapperB(0), _FrameCount(0)
{

}

SplashPageState::~SplashPageState(void)
{

}

void SplashPageState::PushInitialise(
    GfxModuleWrapper* AGfxModule,
    SoundModuleWrapper* ASoundModule
)
{
	// Load the textures needed
	_SurfaceWrapperA = AGfxModule->GetSurfaceManager()->CreateSurface("./Backgrounds/Splash1.png");
	_SurfaceWrapperB = AGfxModule->GetSurfaceManager()->CreateSurface("./Backgrounds/Splash2.png");

	// Get the background layers needed
	_BGObjectA = AGfxModule->GetBGManager()->GetBGObject(3);
	_BGObjectB = AGfxModule->GetBGManager()->GetBGObject(2);

	// Put the two together
	_BGObjectA->SetSurfaceWrapper(_SurfaceWrapperA);
	_BGObjectB->SetSurfaceWrapper(_SurfaceWrapperB);

	// Load and play the music file
	ASoundModule->GetMusicManager()->LoadMusic("./Sounds/ScreenCompany music.ogg");
	ASoundModule->GetMusicManager()->Play();
}

void SplashPageState::PushCleanup(
    GfxModuleWrapper* AGfxModule,
    SoundModuleWrapper* ASoundModule
)
{
	this->PopCleanup(AGfxModule, ASoundModule);
}

void SplashPageState::PopInitialise(
    GfxModuleWrapper* AGfxModule,
    SoundModuleWrapper* ASoundModule
)
{
	this->PushInitialise(AGfxModule, ASoundModule);
}

void SplashPageState::PopCleanup(
    GfxModuleWrapper* AGfxModule,
    SoundModuleWrapper* ASoundModule
)
{
	// Free the BGObjects and SurfaceWrappers
	AGfxModule->GetSurfaceManager()->FreeSurface(_SurfaceWrapperA);
	AGfxModule->GetSurfaceManager()->FreeSurface(_SurfaceWrapperB);
	AGfxModule->GetBGManager()->ClearBGObject(_BGObjectA);
	AGfxModule->GetBGManager()->ClearBGObject(_BGObjectB);

	// Stop and free the music
	ASoundModule->GetMusicManager()->Stop();
	ASoundModule->GetMusicManager()->FreeMusic();
}

void SplashPageState::Update(
    GfxModuleWrapper* AGfxModule,
    SoundModuleWrapper* ASoundModule,
    Input* AInputModule,
    GameStateManager* AGameStateManager
)
{
	// Quit flag
	bool QuitFlag = false;

    // Pull off the sliding animation
	if( _FrameCount > 60 )
	{
		// Move A left and B right
		_BGObjectA->MoveBG(-4,  0);
		_BGObjectB->MoveBG( 4,  0);
	}

	// Check if the Animation has finished
	if( _BGObjectB->GetXPosition() > 330 )
	{
		QuitFlag = true;
	}

    // Increment the framecount
    ++_FrameCount;

    // If the flag is true, then pop this state
    if( QuitFlag )
		AGameStateManager->Pop();
}

/*
History
=======
2006-07-14: Added music
2006-07-13: Created file and base functionality.
*/
