//! A class to represent the CLASS_NAME state

#ifndef CLASS_NAME_H
#define CLASS_NAME_H

#include "../StateModule/GameState.h"
#include "../ToolKit/Singleton.h"

namespace GfxModule
{
	class BGObject;
	class SurfaceWrapper;
	class Sprite;
	class FontWrapper;
}

class CLASS_NAME : public GameStateModule::GameState, public Singleton<CLASS_NAME>
{
private:

public:
	//! Standard constructor
	CLASS_NAME(void);

	//! Standard desconstructor
	~CLASS_NAME(void);

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

#endif // CLASS_NAME_H

/*
History
=======
2006-07-13: Created file and base functionality.
*/
