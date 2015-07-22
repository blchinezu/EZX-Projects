#include <string>

#include "./RatingState.h"
#include "../StateModule/GameStateManager.h"
#include "../GfxModule/GfxModule.h"
#include "../SoundModule/SoundModule.h"
#include "../InputModule/Input.h"

#include "../Timer/SdlTimer.h"

#include <sstream>

using namespace std;

using namespace GfxModule;
using namespace SoundModule;
using namespace GameStateModule;
using namespace InputModule;

using namespace TimerModule;

const ColourRGBA STANDARD_COLOUR(255, 255, 255, 255);
const unsigned int FONT_SIZE = 20;
const string FONT_FILENAME = "./Fonts/adlibn.ttf";

const unsigned int 	RATINGS_START_X_POSITION 	= 215,
					RATINGS_START_Y_POSITION 	= 8,
					RATINGS_Y_GAP				= 29;


const unsigned int	TOTAL_NUMBER_OF_RATINGS 	= 8,
					HIGHEST_COMBO 				= 6,
					NUMBER_OF_MISS_STEPS		= 5,
					NUMBER_OF_BOO_STEPS			= 4,
					NUMBER_OF_GOOD_STEPS		= 3,
					NUMBER_OF_GREAT_STEPS		= 2,
					NUMBER_OF_PERFECT_STEPS 	= 1,
					NUMBER_OF_MARVELOUS_STEPS 	= 0,
					CURRENT_SCORE				= 7;


RatingState::RatingState(void)
	:	_BackgroundSurface(0), _ClearedSurface(0), _FailedSurface(0), _BackgroundObject(0), _ForegroundObject(0), _ForwardSfx(0), _Font(0)
{
	_VectorOfRatings.resize(TOTAL_NUMBER_OF_RATINGS);
}

RatingState::~RatingState(void)
{

}

void RatingState::SetRatings(	unsigned int HighestCombo,
								unsigned int NumberOfMissSteps,
								unsigned int NumberOfBooSteps,
								unsigned int NumberOfGoodSteps,
								unsigned int NumberOfGreatSteps,
								unsigned int NumberOfPerfectSteps,
								unsigned int NumberOfMarvelousSteps,
								unsigned int CurrentScore,
								bool NotFailFlag
							)
{
	{ostringstream Text; Text << HighestCombo; 			_VectorOfRatings[HIGHEST_COMBO] 			= Text.str();}
	{ostringstream Text; Text << NumberOfMissSteps; 	_VectorOfRatings[NUMBER_OF_MISS_STEPS] 		= Text.str();}
	{ostringstream Text; Text << NumberOfBooSteps; 		_VectorOfRatings[NUMBER_OF_BOO_STEPS] 		= Text.str();}
	{ostringstream Text; Text << NumberOfGoodSteps; 	_VectorOfRatings[NUMBER_OF_GOOD_STEPS] 		= Text.str();}
	{ostringstream Text; Text << NumberOfGreatSteps; 	_VectorOfRatings[NUMBER_OF_GREAT_STEPS] 	= Text.str();}
	{ostringstream Text; Text << NumberOfPerfectSteps; 	_VectorOfRatings[NUMBER_OF_PERFECT_STEPS] 	= Text.str();}
	{ostringstream Text; Text << NumberOfMarvelousSteps;_VectorOfRatings[NUMBER_OF_MARVELOUS_STEPS] = Text.str();}
	{ostringstream Text; Text << CurrentScore; 			_VectorOfRatings[CURRENT_SCORE] 			= Text.str();}
	_NotFailFlag = NotFailFlag;
}

void RatingState::PushInitialise(
    GfxModuleWrapper* AGfxModule,
    SoundModuleWrapper* ASoundModule
)
{
	// Load the background image
	_BackgroundSurface = AGfxModule->GetSurfaceManager()->CreateSurface("./Backgrounds/Ranking.png");
	_ClearedSurface = AGfxModule->GetSurfaceManager()->CreateSurface("./Graphics/Cleared.png");
	_FailedSurface = AGfxModule->GetSurfaceManager()->CreateSurface("./Graphics/Failed.png");
	// Grab the background layer
	_BackgroundObject = AGfxModule->GetBGManager()->GetBGObject(0);
	_ForegroundObject = AGfxModule->GetBGManager()->GetBGObject(1);
	// Clear the BG Object and put the two together
	AGfxModule->GetBGManager()->ClearBGObject(_BackgroundObject);
	AGfxModule->GetBGManager()->ClearBGObject(_ForegroundObject);
	_BackgroundObject->SetSurfaceWrapper(_BackgroundSurface);

	// Set the position of the Cleared/Failed Background
	_ForegroundObject->SetXYPosition(10, 10);

	// Load the Sound effects
	_ForwardSfx = ASoundModule->GetSfxManager()->CreateSfx("./Sounds/Common start.ogg");

	// Load the text font
	_Font = AGfxModule->GetFontManager()->CreateFont(FONT_FILENAME.c_str(), FONT_SIZE);
}

void RatingState::PushCleanup(
    GfxModuleWrapper* AGfxModule,
    SoundModuleWrapper* ASoundModule
)
{
	// Free the Surface
	AGfxModule->GetBGManager()->ClearBGObject(_BackgroundObject);
	AGfxModule->GetBGManager()->ClearBGObject(_ForegroundObject);
	AGfxModule->GetSurfaceManager()->FreeSurface(_BackgroundSurface);
	AGfxModule->GetSurfaceManager()->FreeSurface(_FailedSurface);
	AGfxModule->GetSurfaceManager()->FreeSurface(_ClearedSurface);

	// Free the sounds
	ASoundModule->GetSfxManager()->FreeSfx(_ForwardSfx);

	// Free the fonts
	AGfxModule->GetFontManager()->FreeFont(_Font);
}

void RatingState::PopInitialise(
    GfxModuleWrapper* AGfxModule,
    SoundModuleWrapper* ASoundModule
)
{
	this->PushInitialise(AGfxModule, ASoundModule);
}

void RatingState::PopCleanup(
    GfxModuleWrapper* AGfxModule,
    SoundModuleWrapper* ASoundModule
)
{
	this->PushCleanup(AGfxModule, ASoundModule);
}

void RatingState::Update(
    GfxModuleWrapper* AGfxModule,
    SoundModuleWrapper* ASoundModule,
    Input* AInputModule,
    GameStateManager* AGameStateManager
)
{
	// Quit flag
	bool QuitFlag = false;
	// Get the current input and previous input states
    unsigned int CurrentInputState = AInputModule->GetCurrentInputState();
    unsigned int PreviousInputState = AInputModule->GetPreviousInputState();

	// Print the text on screen
	for(unsigned int i = 0; i < TOTAL_NUMBER_OF_RATINGS; ++i)
	{
		AGfxModule->GetFontManager()->WriteText( 	_Font,
													RATINGS_START_X_POSITION,
													RATINGS_START_Y_POSITION + (RATINGS_Y_GAP * i) ,
													_VectorOfRatings[i],
													STANDARD_COLOUR,
													ALIGN_RIGHT);
	}

	// Check if the players has failed or cleared
	if (_NotFailFlag)
	{
		// Cleared
		_ForegroundObject->SetSurfaceWrapper(_ClearedSurface);
	}
	else
	{
		// Failed
		_ForegroundObject->SetSurfaceWrapper(_FailedSurface);
	}

	if ( CurrentInputState & BUTTON_B )
    {
        // Go to the playing stage
        if ( !(PreviousInputState & BUTTON_B) )
        {
			ASoundModule->GetSfxManager()->PlaySfx(_ForwardSfx);
			QuitFlag = true;
			SdlTimer::Delay(800);
        }
    }

    // If the flag is true, then pop this state
    if( QuitFlag )
		AGameStateManager->Pop();
}

/*
History
=======
2006-08-03: Created file and base functionality.
*/
