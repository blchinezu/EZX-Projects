#include <sstream>

using namespace std;

#include "./DanceMachine.h"

#include "../StateModule/GameStateManager.h"
#include "../GfxModule/GfxModule.h"
#include "../SoundModule/SoundModule.h"
#include "../InputModule/Input.h"
#include "../Logger/Logger.h"

#include "../Timer/SdlTimer.h"

#include "./DwiParser.h"

using namespace std;

using namespace GfxModule;
using namespace SoundModule;
using namespace GameStateModule;
using namespace InputModule;
using namespace LogModule;

// Constants for the Various surfaces
const unsigned int 	BACKGROUND 			= 0,
					LEFT_PRESS			= 1,
					DOWN_PRESS			= 2,
					UP_PRESS			= 3,
					RIGHT_PRESS			= 4,
					LEFT_SINGLE_STEP	= 5,
					DOWN_SINGLE_STEP	= 6,
					UP_SINGLE_STEP		= 7,
					RIGHT_SINGLE_STEP	= 8,
					LEFT_SINGLE_HEAD	= 9,
					LEFT_SINGLE_BODY	= 10,
					LEFT_SINGLE_END		= 11,
					DOWN_SINGLE_HEAD	= 12,
					DOWN_SINGLE_BODY	= 13,
					DOWN_SINGLE_END		= 14,
					UP_SINGLE_HEAD		= 15,
					UP_SINGLE_BODY		= 16,
					UP_SINGLE_END		= 17,
					RIGHT_SINGLE_HEAD	= 18,
					RIGHT_SINGLE_BODY	= 19,
					RIGHT_SINGLE_END	= 20,
					FOREGROUND			= 21,
					BLACK_HEALTH_BAR	= 22,
					COLOURED_HEALTH_BAR	= 23,
					RATING_MARVELOUS	= 24,
					RATING_PERFECT		= 25,
					RATING_GREAT		= 26,
					RATING_GOOD			= 27,
					RATING_BOO			= 28,
					RATING_MISS			= 29,
					RATING_BLANK		= 30;

// Constants for identifier of each sprite
const unsigned int 	LEFT_ARROW_DEADLINE		= 0,
					DOWN_ARROW_DEADLINE		= 1,
					UP_ARROW_DEADLINE		= 2,
					RIGHT_ARROW_DEADLINE	= 3,
					COLOURED_HEALTH_COVER	= 4,
					BLACK_HEALTH_COVER		= 5,
					RATING_SPRITE			= 6;

// Constants for the step pressed by character. Follows the DWI format
const unsigned int 	STEP_PRESSED_LEFT		= (1 << 0),
					STEP_PRESSED_DOWN		= (1 << 1),
					STEP_PRESSED_UP			= (1 << 2),
					STEP_PRESSED_RIGHT		= (1 << 3);

const unsigned int NUMBER_OF_DEADLINE_ARROWS = 4;
const unsigned int NUMBER_OF_SURFACES 	= 40;
const unsigned int NUMBER_OF_SPRITES	= 20;
const int 	DEADLINE_ARROWS_X_POSITION 	= 90,
			DEADLINE_ARROWS_Y_POSITION 	= 35,
			DEADLINE_ARROWS_X_GAP		= 35,
			TOP_OF_SCREEN_RELATIVE_TO_DEADLINE 		= 0 - 24, //(0 - DEADLINE_ARROWS_Y_POSITION - 32), // 32 = Arrow height
			BOTTOM_OF_SCREEN_RELATIVE_TO_DEADLINE 	= (240 - DEADLINE_ARROWS_Y_POSITION);

const int 	SCORE_X_POSITION = 315,
			SCORE_Y_POSITION = 212,
			RATING_X_POSITION = 18,
			RATING_Y_POSITION = 198,
			COMBO_X_POSITION = 115,
			COMBO_Y_POSITION = SCORE_Y_POSITION;

const int 	HEALTH_COVER_OFFSET 		= 10,
			HEALTH_MAX					= 300,
			HEALTH_UNIT					= 3,
			HEALTH_DEFAULT				= 50 * HEALTH_UNIT,
			HEALTH_DIFFERENCE_MARVELOUS = 3 * HEALTH_UNIT,
			HEALTH_DIFFERENCE_PERFECT 	= 2 * HEALTH_UNIT,
			HEALTH_DIFFERENCE_GREAT 	= 1 * HEALTH_UNIT,
			HEALTH_DIFFERENCE_GOOD 		= 0 * HEALTH_UNIT,
			HEALTH_DIFFERENCE_BOO 		= -1 * HEALTH_UNIT,
			HEALTH_DIFFERENCE_MISS 		= -2 * HEALTH_UNIT;

// Deadline distances
const int	DISTANCE_MARVELOUS	= 2 * 2,
			DISTANCE_PERFECT	= 4 * 4,
			DISTANCE_GREAT		= 6 * 6,
			DISTANCE_GOOD		= 8 * 8,
			DISTANCE_BOO		= 10 * 10,
 			DISTANCE_MISS		= -10;

// Points for each rating
const unsigned int 	SCORE_MARVELOUS = 5,
					SCORE_PERFECT	= 4,
					SCORE_GREAT		= 3,
					SCORE_GOOD		= 2,
					SCORE_BOO		= 1;

// Constants for syncing
const unsigned int 	SYNC_INTERVAL_IN_MS 	= 2 * 1000,
					SYNC_MAX_DURATION_IN_MS = 180 * 1000,
					SYNC_SIZE_OF_BUFFER		= SYNC_MAX_DURATION_IN_MS / SYNC_INTERVAL_IN_MS;

DanceMachine::DanceMachine(void)
	:	_Background(0), _Foreground(0), _MusicFilename(""), _DwiFilename(""), _BackgroundFilename(""), _ScrollSpeedPerMs(0),
		_StartItrForStepObjects(0), _CurrentScrollPosition(0), _ScoreFont(0)
{
	// Resize the sync buffer
	_VectorForSyncPositions.resize(SYNC_SIZE_OF_BUFFER);
}

DanceMachine::~DanceMachine(void)
{

}

void DanceMachine::SetFilenames(const string &MusicFilename, const string &DwiFilename, const string &BackgroundFilename)
{
	// Set the filenames
	_MusicFilename = MusicFilename;
	_DwiFilename = DwiFilename;
	_BackgroundFilename = BackgroundFilename;
}

void DanceMachine::Initialise(GfxModuleWrapper* AGfxModule, SoundModuleWrapper* ASoundModule)
{
	// Clear and resize the vector
	_VectorOfSurfaceWrappers.clear();
	_VectorOfSurfaceWrappers.resize(NUMBER_OF_SURFACES, 0);

	// Load the font used
	_ScoreFont = AGfxModule->GetFontManager()->CreateFont("./Fonts/adlibn.ttf", 18);

	// Load the background
	_VectorOfSurfaceWrappers[BACKGROUND] = AGfxModule->GetSurfaceManager()->CreateSurface( _BackgroundFilename.c_str() );
	// Check for errors
	if( !_VectorOfSurfaceWrappers[BACKGROUND] )
	{
		// TODO : Load the default bg image
	}

	// Load the foreground
	_VectorOfSurfaceWrappers[FOREGROUND] = AGfxModule->GetSurfaceManager()->CreateSurface("./Backgrounds/InGameForeground.png"  );

	_Background = AGfxModule->GetBGManager()->GetBGObject(0);
	AGfxModule->GetBGManager()->ClearBGObject(_Background);
	_Background->SetSurfaceWrapper( _VectorOfSurfaceWrappers[BACKGROUND] );
	_Background->ScaleBG(320, 240);

	_Foreground = AGfxModule->GetBGManager()->GetBGObject(4);
	AGfxModule->GetBGManager()->ClearBGObject(_Foreground);
	_Foreground->SetSurfaceWrapper( _VectorOfSurfaceWrappers[FOREGROUND] );

	// Load the Arrow graphics for the deadline
	_VectorOfSurfaceWrappers[LEFT_PRESS] = AGfxModule->GetSurfaceManager()->CreateSurface( "./Graphics/LeftArrowPress.png" );
	_VectorOfSurfaceWrappers[DOWN_PRESS] = AGfxModule->GetSurfaceManager()->CreateSurface( "./Graphics/DownArrowPress.png" );
	_VectorOfSurfaceWrappers[UP_PRESS] 	 = AGfxModule->GetSurfaceManager()->CreateSurface( "./Graphics/UpArrowPress.png" );
	_VectorOfSurfaceWrappers[RIGHT_PRESS]= AGfxModule->GetSurfaceManager()->CreateSurface( "./Graphics/RightArrowPress.png" );

	// Load the Step graphics
	_VectorOfSurfaceWrappers[LEFT_SINGLE_STEP] = AGfxModule->GetSurfaceManager()->CreateSurface( "./Graphics/LeftStep.png" );
	_VectorOfSurfaceWrappers[DOWN_SINGLE_STEP] = AGfxModule->GetSurfaceManager()->CreateSurface( "./Graphics/DownStep.png" );
	_VectorOfSurfaceWrappers[UP_SINGLE_STEP]   = AGfxModule->GetSurfaceManager()->CreateSurface( "./Graphics/UpStep.png" );
	_VectorOfSurfaceWrappers[RIGHT_SINGLE_STEP]= AGfxModule->GetSurfaceManager()->CreateSurface( "./Graphics/RightStep.png" );

	_VectorOfSurfaceWrappers[LEFT_SINGLE_HEAD] 	= AGfxModule->GetSurfaceManager()->CreateSurface( "./Graphics/LeftHoldHead.png" );
	_VectorOfSurfaceWrappers[LEFT_SINGLE_BODY] 	= AGfxModule->GetSurfaceManager()->CreateSurface( "./Graphics/LeftHoldBody.png" );
	_VectorOfSurfaceWrappers[LEFT_SINGLE_END]  	= AGfxModule->GetSurfaceManager()->CreateSurface( "./Graphics/LeftHoldEnd.png" );
	_VectorOfSurfaceWrappers[DOWN_SINGLE_HEAD]	= AGfxModule->GetSurfaceManager()->CreateSurface( "./Graphics/DownHoldHead.png" );
	_VectorOfSurfaceWrappers[DOWN_SINGLE_BODY] 	= AGfxModule->GetSurfaceManager()->CreateSurface( "./Graphics/DownHoldBody.png" );
	_VectorOfSurfaceWrappers[DOWN_SINGLE_END] 	= AGfxModule->GetSurfaceManager()->CreateSurface( "./Graphics/DownHoldEnd.png" );
	_VectorOfSurfaceWrappers[UP_SINGLE_HEAD]  	= AGfxModule->GetSurfaceManager()->CreateSurface( "./Graphics/UpHoldHead.png" );
	_VectorOfSurfaceWrappers[UP_SINGLE_BODY]	= AGfxModule->GetSurfaceManager()->CreateSurface( "./Graphics/UpHoldBody.png" );
	_VectorOfSurfaceWrappers[UP_SINGLE_END] 	= AGfxModule->GetSurfaceManager()->CreateSurface( "./Graphics/UpHoldEnd.png" );
	_VectorOfSurfaceWrappers[RIGHT_SINGLE_HEAD] = AGfxModule->GetSurfaceManager()->CreateSurface( "./Graphics/RightHoldHead.png" );
	_VectorOfSurfaceWrappers[RIGHT_SINGLE_BODY]	= AGfxModule->GetSurfaceManager()->CreateSurface( "./Graphics/RightHoldBody.png" );
	_VectorOfSurfaceWrappers[RIGHT_SINGLE_END]	= AGfxModule->GetSurfaceManager()->CreateSurface( "./Graphics/RightHoldEnd.png" );

	// Load the Rating graphics
	_VectorOfSurfaceWrappers[RATING_MARVELOUS] 	= AGfxModule->GetSurfaceManager()->CreateSurface( "./Graphics/Rating_Marvelous.png" );
	_VectorOfSurfaceWrappers[RATING_PERFECT] 	= AGfxModule->GetSurfaceManager()->CreateSurface( "./Graphics/Rating_Perfect.png" );
	_VectorOfSurfaceWrappers[RATING_GREAT] 		= AGfxModule->GetSurfaceManager()->CreateSurface( "./Graphics/Rating_Great.png" );
	_VectorOfSurfaceWrappers[RATING_GOOD] 		= AGfxModule->GetSurfaceManager()->CreateSurface( "./Graphics/Rating_Good.png" );
	_VectorOfSurfaceWrappers[RATING_BOO] 		= AGfxModule->GetSurfaceManager()->CreateSurface( "./Graphics/Rating_Boo.png" );
	_VectorOfSurfaceWrappers[RATING_MISS] 		= AGfxModule->GetSurfaceManager()->CreateSurface( "./Graphics/Rating_Missing.png" );
	_VectorOfSurfaceWrappers[RATING_BLANK] 		= AGfxModule->GetSurfaceManager()->CreateSurface( "./Graphics/Rating_Blank.png" );

	// Load the health bar graphics
	_VectorOfSurfaceWrappers[BLACK_HEALTH_BAR]		= AGfxModule->GetSurfaceManager()->CreateSurface( "./Graphics/BlackHealthBar.png" );
	_VectorOfSurfaceWrappers[COLOURED_HEALTH_BAR]	= AGfxModule->GetSurfaceManager()->CreateSurface( "./Graphics/ColouredHealthBar.png" );

	// Setup up the sprite properties and grab a bunch of free sprites
	_VectorOfSprites.clear();
	_VectorOfSprites.resize(NUMBER_OF_SPRITES, 0);
	vector<Sprite *>::iterator SpriteItr = _VectorOfSprites.begin();
	while( SpriteItr != _VectorOfSprites.end() )
	{
		(*SpriteItr) = AGfxModule->GetSpriteManager()->GetFreeSprite();
		++SpriteItr;
	}

	// Setup the health bar
	_Health = HEALTH_DEFAULT;
	_VectorOfSprites[BLACK_HEALTH_COVER]->SetSpriteDimensions(320, 28);
	_VectorOfSprites[BLACK_HEALTH_COVER]->SetSpriteAnimation(1, 1, false);
	_VectorOfSprites[BLACK_HEALTH_COVER]->SetPriority(3);
	_VectorOfSprites[BLACK_HEALTH_COVER]->SetXYPosition(_Health + HEALTH_COVER_OFFSET, 0);
	_VectorOfSprites[BLACK_HEALTH_COVER]->SetSurfaceWrapper(_VectorOfSurfaceWrappers[BLACK_HEALTH_BAR]);

	_VectorOfSprites[COLOURED_HEALTH_COVER]->SetSpriteDimensions(320, 28);
	_VectorOfSprites[COLOURED_HEALTH_COVER]->SetSpriteAnimation(1, 1, false);
	_VectorOfSprites[COLOURED_HEALTH_COVER]->SetPriority(2);
	_VectorOfSprites[COLOURED_HEALTH_COVER]->SetXYPosition(0, 0);
	_VectorOfSprites[COLOURED_HEALTH_COVER]->SetSurfaceWrapper(_VectorOfSurfaceWrappers[COLOURED_HEALTH_BAR]);

	// Get some sprites and set the base properites for the deadline
	for(unsigned int i = LEFT_ARROW_DEADLINE; i <= RIGHT_ARROW_DEADLINE; ++i )
	{
		_VectorOfSprites[i]->SetSpriteDimensions(31, 32);
		_VectorOfSprites[i]->SetSpriteAnimation(4, 1, false);
		_VectorOfSprites[i]->SetPriority(0);
	}

	// Set the deadline and position
	_VectorOfSprites[LEFT_ARROW_DEADLINE]->SetSurfaceWrapper(_VectorOfSurfaceWrappers[LEFT_PRESS]);
	_VectorOfSprites[DOWN_ARROW_DEADLINE]->SetSurfaceWrapper(_VectorOfSurfaceWrappers[DOWN_PRESS]);
	_VectorOfSprites[UP_ARROW_DEADLINE]->SetSurfaceWrapper(_VectorOfSurfaceWrappers[UP_PRESS]);
	_VectorOfSprites[RIGHT_ARROW_DEADLINE]->SetSurfaceWrapper(_VectorOfSurfaceWrappers[RIGHT_PRESS]);

	_VectorOfSprites[LEFT_ARROW_DEADLINE]->SetXYPosition(DEADLINE_ARROWS_X_POSITION + DEADLINE_ARROWS_X_GAP * LEFT_ARROW_DEADLINE, DEADLINE_ARROWS_Y_POSITION);
	_VectorOfSprites[DOWN_ARROW_DEADLINE]->SetXYPosition(DEADLINE_ARROWS_X_POSITION + DEADLINE_ARROWS_X_GAP * DOWN_ARROW_DEADLINE, DEADLINE_ARROWS_Y_POSITION);
	_VectorOfSprites[UP_ARROW_DEADLINE]->SetXYPosition(DEADLINE_ARROWS_X_POSITION + DEADLINE_ARROWS_X_GAP * UP_ARROW_DEADLINE, DEADLINE_ARROWS_Y_POSITION);
	_VectorOfSprites[RIGHT_ARROW_DEADLINE]->SetXYPosition(DEADLINE_ARROWS_X_POSITION + DEADLINE_ARROWS_X_GAP * RIGHT_ARROW_DEADLINE, DEADLINE_ARROWS_Y_POSITION);

	// Setup the ranking sprite
	_VectorOfSprites[RATING_SPRITE]->SetSurfaceWrapper(_VectorOfSurfaceWrappers[RATING_BLANK]);
	_VectorOfSprites[RATING_SPRITE]->SetSpriteDimensions(84, 12);
	_VectorOfSprites[RATING_SPRITE]->SetSpriteAnimation(1, 1, false);
	_VectorOfSprites[RATING_SPRITE]->SetPriority(4);
	_VectorOfSprites[RATING_SPRITE]->SetXYPosition(RATING_X_POSITION, RATING_Y_POSITION);

	// Load the Dwi header Information
	DwiHeaderInformation DwiHeaderObject;
	ParseDwiFileHeader(DwiHeaderObject, _DwiFilename);

	// Calculate the scroll speed from BPM and GAP
	double ScrollSpeedInMs = 0.0;
	_ScrollSpeedPerMs = ScrollSpeedInMs = static_cast<double>(16.0 * 8.0 * DwiHeaderObject._Bpm.ToDouble() / 4.0 / 60.0 / 1000.0);
	ErrLog << "Scroll Speed in Ms: " << _ScrollSpeedPerMs.ToFloat() << "\n";
	_CurrentScrollPosition = 0.0 - (_ScrollSpeedPerMs.ToDouble() * static_cast<double>(DwiHeaderObject._Gap) );

	// Calculate the sync positions
	vector<FixedPoint>::iterator FixedPointItr = _VectorForSyncPositions.begin();
	vector<FixedPoint>::iterator FixedPointItrEnd = _VectorForSyncPositions.end();
	unsigned int Counter = 1;
	const double DistancePerInterval = ScrollSpeedInMs * SYNC_INTERVAL_IN_MS;
	ErrLog << "Size of the Sync Buffer: " << SYNC_SIZE_OF_BUFFER << "\n";
	ErrLog << "DistancePerInterval: " << DistancePerInterval << " every " << SYNC_INTERVAL_IN_MS << " ms\n";
	while(FixedPointItr != FixedPointItrEnd)
	{
		(*FixedPointItr) = DistancePerInterval * Counter;
		++Counter;
		++FixedPointItr;
	}
	_SyncTimer = 0;
	_SyncCounter = 0;

	// Parse the steps
	_VectorOfSteps.clear();
	ParseDwiFileSteps(_VectorOfSteps, _DwiFilename, _SongDifficulty);

	// Reset the variables needed for scrolling
	_StartItrForStepObjects = _VectorOfSteps.begin();

	// Reset the score
	_CurrentScore = 0;

	// Reset the combo count
	_ComboCount = 0;

	// Reset the fail flag
	_NotFailFlag = true;

	// Reset all the ranking variables
	_HighestCombo 			= 0;
	_NumberOfMissSteps 		= 0;
	_NumberOfBooSteps 		= 0;
	_NumberOfGoodSteps 		= 0;
	_NumberOfGreatSteps 	= 0;
	_NumberOfPerfectSteps 	= 0;
	_NumberOfMarvelousSteps = 0;

	// Start playing the music
	ASoundModule->GetMusicManager()->LoadMusic( _MusicFilename );
	ASoundModule->GetMusicManager()->Play( 1 );

	// Update the timer
	_Timer.GetDeltatime();
}

void DanceMachine::Cleanup(GfxModuleWrapper* AGfxModule, SoundModuleWrapper* ASoundModule)
{
	// Stop and free the music
	ASoundModule->GetMusicManager()->Stop();
	ASoundModule->GetMusicManager()->FreeMusic();

	// Free all the surface wrappers
	vector<SurfaceWrapper *>::iterator SurfaceItr = _VectorOfSurfaceWrappers.begin();
	while( SurfaceItr != _VectorOfSurfaceWrappers.end() )
	{
		AGfxModule->GetSurfaceManager()->FreeSurface(*SurfaceItr);
		++SurfaceItr;
	}
	_VectorOfSurfaceWrappers.clear();

	// Free the sprites
	vector<Sprite *>::iterator SpriteItr = _VectorOfSprites.begin();
	while( SpriteItr != _VectorOfSprites.end() )
	{
		AGfxModule->GetSpriteManager()->FreeSprite(*SpriteItr);
		++SpriteItr;
	}
	_VectorOfSprites.clear();

	// Clear the background
	AGfxModule->GetBGManager()->ClearBGObject(_Background);

	// Clear the font
	AGfxModule->GetFontManager()->FreeFont(_ScoreFont);

	// Clear the steps
	_VectorOfSteps.clear();
}

void DanceMachine::Update( GfxModuleWrapper* AGfxModule, SoundModuleWrapper* ASoundModule, Input* AInputModule)
{
	// Draw the high score
	ostringstream Score, Combo;
	Score << _CurrentScore;
	AGfxModule->GetFontManager()->WriteText(_ScoreFont, SCORE_X_POSITION, SCORE_Y_POSITION, Score.str(), 255, 255, 255, ALIGN_RIGHT);
	Combo << _ComboCount;
	AGfxModule->GetFontManager()->WriteText(_ScoreFont, COMBO_X_POSITION, COMBO_Y_POSITION, Combo.str(), 255, 255, 255, ALIGN_RIGHT);

	// Check the pad input
	unsigned int CurrentInputState = AInputModule->GetCurrentInputState();
	unsigned int PreviousInputState = AInputModule->GetPreviousInputState();

	// Which steps have been pressed
	unsigned int StepsPressed = 0;

	// Check the Joystick state
	if( (CurrentInputState & JOY_MASK) != (PreviousInputState & JOY_MASK) )
	{
		switch( CurrentInputState & JOY_MASK )
		{
		case LEFT:
			{
				_VectorOfSprites[LEFT_ARROW_DEADLINE]->ResetAnimation();
				StepsPressed |= STEP_PRESSED_LEFT;
			} break;
		case RIGHT:
			{
				_VectorOfSprites[RIGHT_ARROW_DEADLINE]->ResetAnimation();
				StepsPressed |= STEP_PRESSED_RIGHT;
			} break;
		case DOWN:
			{
				_VectorOfSprites[DOWN_ARROW_DEADLINE]->ResetAnimation();
				StepsPressed |= STEP_PRESSED_DOWN;
			} break;
		case UP:
			{
				_VectorOfSprites[UP_ARROW_DEADLINE]->ResetAnimation();
				StepsPressed |= STEP_PRESSED_UP;
			} break;
		case UPRIGHT:
			{
				_VectorOfSprites[RIGHT_ARROW_DEADLINE]->ResetAnimation();
				_VectorOfSprites[UP_ARROW_DEADLINE]->ResetAnimation();
				StepsPressed |= (STEP_PRESSED_RIGHT | STEP_PRESSED_UP);
			} break;
		case UPLEFT:
			{
				_VectorOfSprites[LEFT_ARROW_DEADLINE]->ResetAnimation();
				_VectorOfSprites[UP_ARROW_DEADLINE]->ResetAnimation();
				StepsPressed |= (STEP_PRESSED_LEFT | STEP_PRESSED_UP);
			} break;
		case DOWNRIGHT:
			{
				_VectorOfSprites[RIGHT_ARROW_DEADLINE]->ResetAnimation();
				_VectorOfSprites[DOWN_ARROW_DEADLINE]->ResetAnimation();
				StepsPressed |= (STEP_PRESSED_DOWN | STEP_PRESSED_RIGHT);
			} break;
		case DOWNLEFT:
			{
				_VectorOfSprites[LEFT_ARROW_DEADLINE]->ResetAnimation();
				_VectorOfSprites[DOWN_ARROW_DEADLINE]->ResetAnimation();
				StepsPressed |= (STEP_PRESSED_DOWN | STEP_PRESSED_LEFT);
			} break;
		default:
			{
			} break;
		} // end case statement
	}

	// LeftArrow
	if( (CurrentInputState & BUTTON_A) && !(PreviousInputState & BUTTON_A) )
	{
		// Reset the animation
		_VectorOfSprites[LEFT_ARROW_DEADLINE]->ResetAnimation();
		StepsPressed |= STEP_PRESSED_LEFT;
	}
	// Down Arrow
	if( (CurrentInputState & BUTTON_X) && !(PreviousInputState & BUTTON_X) )
	{
		// Reset the animation
		_VectorOfSprites[DOWN_ARROW_DEADLINE]->ResetAnimation();
		StepsPressed |= STEP_PRESSED_DOWN;
	}
	// Up Arrow
	if( (CurrentInputState & BUTTON_Y) && !(PreviousInputState & BUTTON_Y) )
	{
		// Reset the animation
		_VectorOfSprites[UP_ARROW_DEADLINE]->ResetAnimation();
		StepsPressed |= STEP_PRESSED_UP;
	}
	// Right Arrow
	if( (CurrentInputState & BUTTON_B) && !(PreviousInputState & BUTTON_B) )
	{
		// Reset the animation
		_VectorOfSprites[RIGHT_ARROW_DEADLINE]->ResetAnimation();
		StepsPressed |= STEP_PRESSED_RIGHT;
	}

	// Check if the Highest combo has been breached
	if( _ComboCount > _HighestCombo)
		_HighestCombo = _ComboCount;

	// Process the steps
	this->ProcessSteps(AGfxModule, ASoundModule, StepsPressed);
	this->ProcessHealthBar(AGfxModule, ASoundModule);
}

void DanceMachine::ProcessHealthBar( GfxModuleWrapper* AGfxModule, SoundModuleWrapper* ASoundModule )
{
	// Animate the bar
	int XPosition = _VectorOfSprites[BLACK_HEALTH_COVER]->GetXPosition();
	++XPosition;

	// Check if health is above 300 or 30
	if (_Health > HEALTH_MAX)
		_Health = HEALTH_MAX;
	if (_Health < 0)
	{
		_Health = 0;
		// Set the Notfailflag to false
		_NotFailFlag = false;
	}

	if(XPosition > (_Health + HEALTH_COVER_OFFSET) )
		XPosition = _Health;

	_VectorOfSprites[BLACK_HEALTH_COVER]->SetXPosition(XPosition);
}


void DanceMachine::RateStep(StepObject &AStepObject)
{
	// Checks the distance from the dad line for ratings between BOO and MARVELOUS
	// Get the distance squared from the deadline
	int DistanceFromDeadlineSqaured = AStepObject._DistanceFromDeadline.ToInt() - _CurrentScrollPosition.ToInt();
	DistanceFromDeadlineSqaured *= DistanceFromDeadlineSqaured;

	if (DistanceFromDeadlineSqaured <= DISTANCE_BOO)
	{
		if (DistanceFromDeadlineSqaured <= DISTANCE_GOOD)
		{
			if (DistanceFromDeadlineSqaured <= DISTANCE_GREAT)
			{
				if (DistanceFromDeadlineSqaured <= DISTANCE_PERFECT)
				{
					if (DistanceFromDeadlineSqaured <= DISTANCE_MARVELOUS)
					{
						// MARVELOUS
						++_ComboCount;
						_VectorOfSprites[RATING_SPRITE]->SetSurfaceWrapper(_VectorOfSurfaceWrappers[RATING_MARVELOUS] );
						// Add to score
						_CurrentScore += SCORE_MARVELOUS * _ComboCount * _NotFailFlag;
						// Modify health
						_Health += HEALTH_DIFFERENCE_MARVELOUS;
						++_NumberOfMarvelousSteps;
					}
					else
					{
						// PERFECT
						++_ComboCount;
						_VectorOfSprites[RATING_SPRITE]->SetSurfaceWrapper(_VectorOfSurfaceWrappers[RATING_PERFECT] );
						// Add to score
						_CurrentScore += SCORE_GREAT * _ComboCount * _NotFailFlag;
						// Modify health
						_Health += HEALTH_DIFFERENCE_PERFECT;
						++_NumberOfPerfectSteps;
					}
				}
				else
				{
					// GREAT
					++_ComboCount;
					_VectorOfSprites[RATING_SPRITE]->SetSurfaceWrapper(_VectorOfSurfaceWrappers[RATING_GREAT] );
					// Add to score
					_CurrentScore += SCORE_GOOD * _ComboCount * _NotFailFlag;
					// Modify health
					_Health += HEALTH_DIFFERENCE_GREAT;
					++_NumberOfGreatSteps;
				}
			}
			else
			{
				// GOOD
				_ComboCount = 0;
				_VectorOfSprites[RATING_SPRITE]->SetSurfaceWrapper(_VectorOfSurfaceWrappers[RATING_GOOD] );
				// Add to score
				_CurrentScore += SCORE_GOOD * _NotFailFlag;
				// Modify health
				_Health += HEALTH_DIFFERENCE_GOOD;
				++_NumberOfGoodSteps;
			}
		}
		else
		{
			// BOO
			_ComboCount = 0;
			_VectorOfSprites[RATING_SPRITE]->SetSurfaceWrapper(_VectorOfSurfaceWrappers[RATING_BOO] );
			// Add to score
			_CurrentScore += SCORE_BOO * _NotFailFlag;
			// Modify health
			_Health += HEALTH_DIFFERENCE_BOO;
			++_NumberOfBooSteps;
		}

		// Clear the step status
		AStepObject._StepType = NONE;
	}
}

void DanceMachine::ProcessSteps( GfxModuleWrapper* AGfxModule, SoundModuleWrapper* ASoundModule, int StepsPressed)
{
	// Get the deltatime
	FixedPoint Deltatime = _Timer.GetDeltatime();
	_SyncTimer += Deltatime.ToInt();
	// Check if the SyncCounter is higher then the SyncRate
	if(_SyncTimer > SYNC_INTERVAL_IN_MS)
	{
		// Check if the counter isn't more then the size of the sync buffer
		if(_SyncCounter < _VectorForSyncPositions.size() )
		{
			// Find the time difference
			FixedPoint TimerDifference = _SyncTimer - SYNC_INTERVAL_IN_MS;
			// Adjust the scroll position
			_CurrentScrollPosition = _VectorForSyncPositions[_SyncCounter];
			_CurrentScrollPosition += (_ScrollSpeedPerMs * TimerDifference);
			// Reset the timer
			_SyncTimer = TimerDifference.ToInt();
			// Increment the counter
			++_SyncCounter;
		}
	}
	else
	{
		// Scroll the bar
		_CurrentScrollPosition += (Deltatime * _ScrollSpeedPerMs);
	}
	// Process all the steps starting from the last 'dead' object
	vector<StepObject>::iterator StepObjectItr = _StartItrForStepObjects;
	while(StepObjectItr != _VectorOfSteps.end() )
	{
		// Caclulate the distance from the deadline
		int DistanceFromDeadline = (*StepObjectItr)._DistanceFromDeadline.ToInt() - _CurrentScrollPosition.ToInt();

		// Check if the step has fallen off the screen (top)
		if( DistanceFromDeadline < TOP_OF_SCREEN_RELATIVE_TO_DEADLINE )
		{
			// Update the StartItr for next time
			_StartItrForStepObjects = StepObjectItr;
			// If there is a sprite (which also means that the step has been missed)
			if( (*StepObjectItr)._Sprite )
			{
				// Free the sprite
				AGfxModule->GetSpriteManager()->FreeSprite( (*StepObjectItr)._Sprite );
				(*StepObjectItr)._Sprite = 0;

				if( (*StepObjectItr)._StepType != NONE )
				{
					// Reset the combo
					_ComboCount = 0;
					_VectorOfSprites[RATING_SPRITE]->SetSurfaceWrapper(_VectorOfSurfaceWrappers[RATING_MISS] );
					// Modify health
					_Health += HEALTH_DIFFERENCE_MISS;
					++_NumberOfMissSteps;
				}
			}
		}
		else
		// else check if the step is on the screen from the (bottom)
		if( DistanceFromDeadline < BOTTOM_OF_SCREEN_RELATIVE_TO_DEADLINE )
		{
			// Check if the step object has a sprite
			if( !(*StepObjectItr)._Sprite )
			{
				// If not, get a free one
				Sprite * FreeSprite = AGfxModule->GetSpriteManager()->GetFreeSprite();

				// Set the properties which are the same for all steps
				FreeSprite->SetSpriteDimensions(31, 32);
				FreeSprite->SetPriority(1);
				// Check which type of arrow is used and assign the correct properties
				switch( (*StepObjectItr)._StepType )
				{
				case LEFT_SINGLE:
					{
						FreeSprite->SetSpriteAnimation(16, 4, true);
						FreeSprite->SetSurfaceWrapper(_VectorOfSurfaceWrappers[LEFT_SINGLE_STEP] );
						FreeSprite->SetXPosition( DEADLINE_ARROWS_X_POSITION + ( DEADLINE_ARROWS_X_GAP * LEFT_ARROW_DEADLINE ) );
					} break;
				case DOWN_SINGLE:
					{
						FreeSprite->SetSpriteAnimation(16, 4, true);
						FreeSprite->SetSurfaceWrapper(_VectorOfSurfaceWrappers[DOWN_SINGLE_STEP] );
						FreeSprite->SetXPosition( DEADLINE_ARROWS_X_POSITION + ( DEADLINE_ARROWS_X_GAP * DOWN_ARROW_DEADLINE ) );

						// If this direction was pressed then check the distance from the deadline
						if(StepsPressed & STEP_PRESSED_DOWN)
							this->RateStep( (*StepObjectItr) );
					} break;
				case UP_SINGLE:
					{
						FreeSprite->SetSpriteAnimation(16, 4, true);
						FreeSprite->SetSurfaceWrapper(_VectorOfSurfaceWrappers[UP_SINGLE_STEP] );
						FreeSprite->SetXPosition( DEADLINE_ARROWS_X_POSITION + ( DEADLINE_ARROWS_X_GAP * UP_ARROW_DEADLINE ) );
					} break;
				case RIGHT_SINGLE:
					{
						FreeSprite->SetSpriteAnimation(16, 4, true);
						FreeSprite->SetSurfaceWrapper(_VectorOfSurfaceWrappers[RIGHT_SINGLE_STEP] );
						FreeSprite->SetXPosition( DEADLINE_ARROWS_X_POSITION + ( DEADLINE_ARROWS_X_GAP * RIGHT_ARROW_DEADLINE ) );
					} break;
				// FREEZE ARROWS BROKEN!!!
				/*case LEFT_HOLD_HEAD:
					{
						FreeSprite->SetSpriteAnimation(1, 1, false);
						FreeSprite->SetSurfaceWrapper(_VectorOfSurfaceWrappers[LEFT_SINGLE_HEAD] );
						FreeSprite->SetXPosition( DEADLINE_ARROWS_X_POSITION + ( DEADLINE_ARROWS_X_GAP * LEFT_ARROW_DEADLINE ) );
					} break;
				case DOWN_HOLD_HEAD:
					{
						FreeSprite->SetSpriteAnimation(1, 1, false);
						FreeSprite->SetSurfaceWrapper(_VectorOfSurfaceWrappers[DOWN_SINGLE_HEAD] );
						FreeSprite->SetXPosition( DEADLINE_ARROWS_X_POSITION + ( DEADLINE_ARROWS_X_GAP * DOWN_ARROW_DEADLINE ) );
					} break;
				case UP_HOLD_HEAD:
					{
						FreeSprite->SetSpriteAnimation(1, 1, false);
						FreeSprite->SetSurfaceWrapper(_VectorOfSurfaceWrappers[UP_SINGLE_HEAD] );
						FreeSprite->SetXPosition( DEADLINE_ARROWS_X_POSITION + ( DEADLINE_ARROWS_X_GAP * UP_ARROW_DEADLINE ) );
					} break;
				case RIGHT_HOLD_HEAD:
					{
						FreeSprite->SetSpriteAnimation(1, 1, false);
						FreeSprite->SetSurfaceWrapper(_VectorOfSurfaceWrappers[RIGHT_SINGLE_HEAD] );
						FreeSprite->SetXPosition( DEADLINE_ARROWS_X_POSITION + ( DEADLINE_ARROWS_X_GAP * RIGHT_ARROW_DEADLINE ) );
					} break;
				case LEFT_HOLD_BODY:
					{
						FreeSprite->SetSpriteAnimation(1, 1, false);
						FreeSprite->SetSurfaceWrapper(_VectorOfSurfaceWrappers[LEFT_SINGLE_BODY] );
						FreeSprite->SetXPosition( DEADLINE_ARROWS_X_POSITION + ( DEADLINE_ARROWS_X_GAP * LEFT_ARROW_DEADLINE ) );
					} break;
				case DOWN_HOLD_BODY:
					{
						FreeSprite->SetSpriteAnimation(1, 1, false);
						FreeSprite->SetSurfaceWrapper(_VectorOfSurfaceWrappers[DOWN_SINGLE_BODY] );
						FreeSprite->SetXPosition( DEADLINE_ARROWS_X_POSITION + ( DEADLINE_ARROWS_X_GAP * DOWN_ARROW_DEADLINE ) );
					} break;
				case UP_HOLD_BODY:
					{
						FreeSprite->SetSpriteAnimation(1, 1, false);
						FreeSprite->SetSurfaceWrapper(_VectorOfSurfaceWrappers[UP_SINGLE_BODY] );
						FreeSprite->SetXPosition( DEADLINE_ARROWS_X_POSITION + ( DEADLINE_ARROWS_X_GAP * UP_ARROW_DEADLINE ) );
					} break;
				case RIGHT_HOLD_BODY:
					{
						FreeSprite->SetSpriteAnimation(1, 1, false);
						FreeSprite->SetSurfaceWrapper(_VectorOfSurfaceWrappers[RIGHT_SINGLE_BODY] );
						FreeSprite->SetXPosition( DEADLINE_ARROWS_X_POSITION + ( DEADLINE_ARROWS_X_GAP * RIGHT_ARROW_DEADLINE ) );
					} break;
				case LEFT_HOLD_END:
					{
						FreeSprite->SetSpriteAnimation(1, 1, false);
						FreeSprite->SetSurfaceWrapper(_VectorOfSurfaceWrappers[LEFT_SINGLE_END] );
						FreeSprite->SetXPosition( DEADLINE_ARROWS_X_POSITION + ( DEADLINE_ARROWS_X_GAP * LEFT_ARROW_DEADLINE ) );
					} break;
				case DOWN_HOLD_END:
					{
						FreeSprite->SetSpriteAnimation(1, 1, false);
						FreeSprite->SetSurfaceWrapper(_VectorOfSurfaceWrappers[DOWN_SINGLE_END] );
						FreeSprite->SetXPosition( DEADLINE_ARROWS_X_POSITION + ( DEADLINE_ARROWS_X_GAP * DOWN_ARROW_DEADLINE ) );
					} break;
				case UP_HOLD_END:
					{
						FreeSprite->SetSpriteAnimation(1, 1, false);
						FreeSprite->SetSurfaceWrapper(_VectorOfSurfaceWrappers[UP_SINGLE_END] );
						FreeSprite->SetXPosition( DEADLINE_ARROWS_X_POSITION + ( DEADLINE_ARROWS_X_GAP * UP_ARROW_DEADLINE ) );
					} break;
				case RIGHT_HOLD_END:
					{
						FreeSprite->SetSpriteAnimation(1, 1, false);
						FreeSprite->SetSurfaceWrapper(_VectorOfSurfaceWrappers[RIGHT_SINGLE_END] );
						FreeSprite->SetXPosition( DEADLINE_ARROWS_X_POSITION + ( DEADLINE_ARROWS_X_GAP * RIGHT_ARROW_DEADLINE ) );
					} break;*/
				default: break;
				} // end switch

				// Assign the sprite
				(*StepObjectItr)._Sprite = FreeSprite;
			} // end if the step has no sprite

			// Compare the arrows to the dead line
			switch( (*StepObjectItr)._StepType )
			{
			case LEFT_SINGLE:
				{
					if(StepsPressed & STEP_PRESSED_LEFT)
					{
						this->RateStep( (*StepObjectItr) );
						StepsPressed &= ~STEP_PRESSED_LEFT;
					}
				} break;
			case DOWN_SINGLE:
				{
					if(StepsPressed & STEP_PRESSED_DOWN)
					{
						this->RateStep( (*StepObjectItr) );
						StepsPressed &= ~STEP_PRESSED_DOWN;
					}
				} break;
			case UP_SINGLE:
				{
					if(StepsPressed & STEP_PRESSED_UP)
					{
						this->RateStep( (*StepObjectItr) );
						StepsPressed &= ~STEP_PRESSED_UP;
					}
				} break;
			case RIGHT_SINGLE:
				{
					if(StepsPressed & STEP_PRESSED_RIGHT)
					{
						this->RateStep( (*StepObjectItr) );
						StepsPressed &= ~STEP_PRESSED_RIGHT;
					}
				} break;
			case NONE:
				{
					// Free the sprite if it has one
					(*StepObjectItr)._Sprite->SetSurfaceWrapper(BLANK_SURFACEWRAPPER);
				}
			default: break;
			}

			// Move the sprite to the correct position on the Y axis
			(*StepObjectItr)._Sprite->SetYPosition(DEADLINE_ARROWS_Y_POSITION + DistanceFromDeadline);
		}
		else
		{
			// If the current step is off the bottom of the screen break
			break;
		}

		++StepObjectItr;
	} // End while loop
}

/*
History
=======
2006-08-03: Added methods and members to keep track of the players rank
2006-08-02: Adding syncing into the steps
2006-08-01: Adding button presses, combos and score. I really should have put more design thought into this .. :/
2006-07-27: Adding the gameplay functionality of pressing the steps [On hold - currently optimising the GfxModule]
2006-07-22: Added score font
2006-07-20: Adding the parsing of the Dwi file
2006-07-16: Created file and base functionality
*/
