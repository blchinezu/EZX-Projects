//! A class to represent the dance machine and handle animations, scores, etc in the main game

#ifndef DANCEMACHINE_H
#define DANCEMACHINE_H

#include <vector>
#include <string>

#include "../Timer/SdlTimer.h"
#include "../ToolKit/FixedPoint.h"
#include "./DwiParser.h"

// Forward declarations
namespace GfxModule
{
	class BGObject;
	class SurfaceWrapper;
	class Sprite;
	class FontWrapper;
	class GfxModuleWrapper;
}

namespace SoundModule
{
	class ChunkWrapper;
	class MusicManager;
	class SfxManager;
	class SoundModuleWrapper;
}

namespace InputModule
{
    class Input;
}

class DanceMachine
{
private:
	//! Vector to hold all the SurfaceWrappers
	std::vector<GfxModule::SurfaceWrapper *> _VectorOfSurfaceWrappers;

	//! BGObject for the background
	GfxModule::BGObject * _Background, * _Foreground;

	//! String variables to store the the music filename, dwi filename and background filename
	std::string _MusicFilename, _DwiFilename, _BackgroundFilename;

	//! The difficulty setting
	SongDifficulty _SongDifficulty;

	//! Vector of Sprites
	std::vector<GfxModule::Sprite *> _VectorOfSprites;

	//! Vector of all the steps generated from the parser
	std::vector<StepObject> _VectorOfSteps;

	//! Scroll speed: Number of pixels to move per MS
	FixedPoint _ScrollSpeedPerMs;

	//! Vector of the exact position for the scroll every nth interval
	std::vector<FixedPoint> _VectorForSyncPositions;

	//! Total time elapsed in ms, resest after every sync update
	unsigned int _SyncTimer;

	//! Sync counter
	unsigned int _SyncCounter;

	//! Beginning iterator when processing of the steps are made to decrease the number of loops done
	std::vector<StepObject>::iterator _StartItrForStepObjects;

	//! Current Scroll position
	FixedPoint _CurrentScrollPosition;

	//! Timer to time the steps
	TimerModule::SdlTimer _Timer;

	//! Font for the high score and combo system
	GfxModule::FontWrapper * _ScoreFont;

	//! Score for the game
	unsigned int _CurrentScore;

	//! Health bar value (0-300)
	int _Health;

	//! If health falls below 0, then Fail flag is set. No points are added if this is set
	bool _NotFailFlag;

	//! Combo score
	unsigned int _ComboCount;

	//! Members to keep track of the max combo, misses etc
	unsigned int 	_HighestCombo,
					_NumberOfMissSteps,
					_NumberOfBooSteps,
					_NumberOfGoodSteps,
					_NumberOfGreatSteps,
					_NumberOfPerfectSteps,
					_NumberOfMarvelousSteps;

	//! Process the steps every frame (move towards deadline, free and grab sprites from the manager etc)
	void ProcessSteps(GfxModule::GfxModuleWrapper* AGfxModule, SoundModule::SoundModuleWrapper* ASoundModule, int StepsPressed);

	//! Process and animate the health bar
	void ProcessHealthBar(GfxModule::GfxModuleWrapper* AGfxModule, SoundModule::SoundModuleWrapper* ASoundModule);

	//! Compares how far a step is from the deadline and rates the step based on the distance
	void RateStep(StepObject &AStep);

public:
	//! Standard constructor
	DanceMachine(void);

	//! Standard deconstructor
	~DanceMachine(void);

	//! Sets the filename for the Music, Dwi and Background filename;
	void SetFilenames(const std::string &MusicFilename, const std::string &DwiFilename, const std::string &BackgroundFilename);

	//! Sets the difficulty
	void SetSongDifficulty(const SongDifficulty Difficulty)
	{
		_SongDifficulty = Difficulty;
	}

	//! Initialise the dance machine
	void Initialise(GfxModule::GfxModuleWrapper* AGfxModule, SoundModule::SoundModuleWrapper* ASoundModule);

	//! Cleanup the dance machine
	void Cleanup(GfxModule::GfxModuleWrapper* AGfxModule, SoundModule::SoundModuleWrapper* ASoundModule);

	//! Get all the stats for the game. Note that all the parameters passed are modified
	void GetAllStats(	unsigned int &HighestCombo,
						unsigned int &NumberOfMissSteps,
						unsigned int &NumberOfBooSteps,
						unsigned int &NumberOfGoodSteps,
						unsigned int &NumberOfGreatSteps,
						unsigned int &NumberOfPerfectSteps,
						unsigned int &NumberOfMarvelousSteps,
						unsigned int &CurrentScore,
						bool &NotFailFlag
					)
	{
		HighestCombo 			= _HighestCombo;
		NumberOfMissSteps 		= _NumberOfMissSteps;
		NumberOfBooSteps 		= _NumberOfBooSteps;
		NumberOfGoodSteps		= _NumberOfGoodSteps;
		NumberOfGreatSteps		= _NumberOfGreatSteps;
		NumberOfPerfectSteps	= _NumberOfPerfectSteps;
		NumberOfMarvelousSteps	= _NumberOfMarvelousSteps;
		CurrentScore			= _CurrentScore;
		NotFailFlag				= _NotFailFlag;
	}

	//! Update function. Called once per loop
	void Update(
        GfxModule::GfxModuleWrapper* AGfxModule,
        SoundModule::SoundModuleWrapper* ASoundModule,
        InputModule::Input* AInputModule
    );
};

#endif // DANCEMACHINE_H

/*
History
=======
2006-08-03: Added methods and members to keep track of the players rank
2006-08-02: Adding syncing into the steps
2006-07-16: Created file and base functionality
*/
