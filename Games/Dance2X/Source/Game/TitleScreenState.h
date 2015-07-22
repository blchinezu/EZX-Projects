//! A class to represent the title screen state

#ifndef TITLESCREENSTATE_H
#define TITLESCREENSTATE_H

#include "../StateModule/GameState.h"
#include "../ToolKit/Singleton.h"

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

class TitleScreenState : public GameStateModule::GameState, public Singleton<TitleScreenState>
{
private:
	//! Font Wrappers for the title and menu
	GfxModule::FontWrapper * _TitleFont, * _MenuFont;

	//! Surface Wrapper for the title screen
	GfxModule::SurfaceWrapper * _BGSurface, * _SelectorSurface;

	//! BGObject for the back ground
	GfxModule::BGObject * _BGObjectMain;

	//! Sprite for the selector bar
	GfxModule::Sprite * _SelectorBar;

	//! Current menu selection
	unsigned int _CurrentMenuSelection;

	//! Pointers to the change and selected sound effect
	SoundModule::ChunkWrapper * _ChangeSfx, * _SelectedSfx;

public:
	//! Standard constructor
	TitleScreenState(void);

	//! Standard desconstructor
	~TitleScreenState(void);

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

#endif // TITLESCREENSTATE_H

/*
History
=======
2006-07-15: Added sound effects
2006-07-13: Created file and base functionality.
*/
