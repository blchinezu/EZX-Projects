//! A class to represent the splash screen state

#ifndef SPLASHPAGESTATE_H
#define SPLASHPAGESTATE_H

#include "../StateModule/GameState.h"
#include "../ToolKit/Singleton.h"

namespace GfxModule
{
	class BGObject;
	class SurfaceWrapper;
}

class SplashPageState : public GameStateModule::GameState, public Singleton<SplashPageState>
{
private:
	//! BGObject to hold the background objects
	GfxModule::BGObject * _BGObjectA, * _BGObjectB;

	//! Surface Wrapper to hold graphics for the background object
	GfxModule::SurfaceWrapper * _SurfaceWrapperA, * _SurfaceWrapperB;

	//! Frame count
	unsigned int _FrameCount;

public:
	//! Standard constructor
	SplashPageState(void);

	//! Standard desconstructor
	~SplashPageState(void);

	//! Initialise the state when the state is pushed on top the stack
    void PushInitialise(
        GfxModule::GfxModuleWrapper* AGfxModule,
        SoundModule::SoundModuleWrapper* ASoundModule
    );

    //! Cleanup up the state when it is pushed then further into the stack
    void PushCleanup(
        GfxModule::GfxModuleWrapper* AGfxModule,
        SoundModule::SoundModuleWrapper* ASoundModule
    );

    //! When the state is popped up to the top of the stack
    void PopInitialise(
        GfxModule::GfxModuleWrapper* AGfxModule,
        SoundModule::SoundModuleWrapper* ASoundModule
    );

    //! When the state is popped off the stack
    void PopCleanup(
        GfxModule::GfxModuleWrapper* AGfxModule,
        SoundModule::SoundModuleWrapper* ASoundModule
    );

    //! Gets called once per frame when it is on top of the stack
    void Update(
        GfxModule::GfxModuleWrapper* AGfxModule,
        SoundModule::SoundModuleWrapper* ASoundModule,
        InputModule::Input* AInputModule,
        GameStateModule::GameStateManager* AGameStateManager
    );
};

#endif // SPLASHPAGESTATE_H

/*
History
=======
2006-07-13: Created file and base functionality.
*/
