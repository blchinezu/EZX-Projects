//! A class to represent the Rating state

#ifndef RATINGSTATE_H
#define RATINGSTATE_H

#include <vector>
#include <string>

#include "../StateModule/GameState.h"
#include "../ToolKit/Singleton.h"

namespace GfxModule
{
	class BGObject;
	class SurfaceWrapper;
	class FontWrapper;
}

namespace SoundModule
{
	class ChunkWrapper;
}

class RatingState : public GameStateModule::GameState, public Singleton<RatingState>
{
private:
	//! Surface Wrapper for the background
	GfxModule::SurfaceWrapper * _BackgroundSurface, * _ClearedSurface, * _FailedSurface;

	//! BGObject for the background
	GfxModule::BGObject * _BackgroundObject, * _ForegroundObject;

	//! Pointers to store the forward and back sound effects
	SoundModule::ChunkWrapper *_ForwardSfx;

	//! Font Wrapper for the text
	GfxModule::FontWrapper * _Font;

	//! Vector of strings to store the ratings to be shown on screen
	std::vector<std::string> _VectorOfRatings;

	//! Did the player fail
	bool _NotFailFlag;

public:
	//! Standard constructor
	RatingState(void);

	//! Standard desconstructor
	~RatingState(void);

	//! Pass the ratings to be shown on screen
	void SetRatings( 	unsigned int HighestCombo,
						unsigned int NumberOfMissSteps,
						unsigned int NumberOfBooSteps,
						unsigned int NumberOfGoodSteps,
						unsigned int NumberOfGreatSteps,
						unsigned int NumberOfPerfectSteps,
						unsigned int NumberOfMarvelousSteps,
						unsigned int CurrentScore,
						bool NotFailFlag
					);

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
2006-08-03: Created file and base functionality.
*/
