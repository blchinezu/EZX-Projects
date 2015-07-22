//! A class to represent the Song Selection state

#ifndef SONGSELECTIONSTATE_H
#define SONGSELECTIONSTATE_H

#include "../StateModule/GameState.h"
#include "../ToolKit/Singleton.h"

namespace GfxModule
{
	class BGObject;
	class SurfaceWrapper;
}

namespace SoundModule
{
	class ChunkWrapper;
}

class SongSelectionState : public GameStateModule::GameState, public Singleton<SongSelectionState>
{
private:
	//! Surface Wrapper for the background
	GfxModule::SurfaceWrapper * _BGSurface;

	//! BGObject for the background
	GfxModule::BGObject * _BGObject;

	//! Pointers to store the forward and back sound effects
	SoundModule::ChunkWrapper *_BackSfx, *_ForwardSfx;

public:
	//! Standard constructor
	SongSelectionState(void);

	//! Standard desconstructor
	~SongSelectionState(void);

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

#endif // SONGSELECTIONSTATE_H

/*
History
=======
2006-07-16: Added forward and back sound effects
2006-07-14: Created file and base functionality.
*/
