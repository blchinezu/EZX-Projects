//! A class to represent the CLASS_NAME state

#ifndef PLAYSTATE_H
#define PLAYSTATE_H

#include <string>

#include "../StateModule/GameState.h"
#include "../ToolKit/Singleton.h"

#include "../DanceMachine/DanceMachine.h"

namespace GfxModule
{
	class BGObject;
	class SurfaceWrapper;
	class Sprite;
	class FontWrapper;
}

namespace SoundModule
{
	class ChunkWrapper;
}

class PlayState : public GameStateModule::GameState, public Singleton<PlayState>
{
private:
	//! Instance of the Dance Machine
	DanceMachine _DanceMachine;

	//! Counter for the quit command. If select is held down more then X number of frames, then quit the state
	unsigned int _NumberOfFramesPassed;

	//! Pointers to store the back sound effects
	SoundModule::ChunkWrapper *_BackSfx, *_ForwardSfx;

	//! Framecounter timer
	unsigned int _FrameCounter;

public:
	//! Standard constructor
	PlayState(void);

	//! Standard desconstructor
	~PlayState(void);

	//! Sets the filepath to the files to be used in the DanceMachine object. Returns true if successful
	bool SetFilePath(const std::string &FilePath, const SongDifficulty Difficulty);

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

#endif // PLAYSTATE_H

/*
History
=======
2006-07-16: Created file and base functionality.
*/
