#include <string>

#include "./TitleScreenState.h"
#include "../StateModule/GameStateManager.h"
#include "../GfxModule/GfxModule.h"
#include "../SoundModule/SoundModule.h"
#include "../InputModule/Input.h"
#include "../Timer/SdlTimer.h"

#include "../Game/PlayState.h"
#include "../Game/RatingState.h"

using namespace std;

using namespace GfxModule;
using namespace SoundModule;
using namespace GameStateModule;
using namespace InputModule;
using namespace TimerModule;

// Some globals to provide easier modifications
const unsigned int TITLE_FONT_SIZE = 32;
const unsigned int MENU_FONT_SIZE = 12;
const string FONT_FILENAME = "./Fonts/drvinyl.ttf";

const ColourRGBA STANDARD_COLOUR(255, 255, 255, 255);
const int 	MENU_X_START_POSITION 	= 100,
			MENU_Y_START_POSITION 	= 119,
			MENU_SPACE_GAP 			= 20,
			LOADING_TEXT_X_POSITION	= 315,
			LOADING_TEXT_Y_POSITION = 223;

const unsigned int MENU_START 	= 0;
const unsigned int MENU_QUIT 	= 1;

TitleScreenState::TitleScreenState(void)
	: 	_TitleFont(0), _MenuFont(0), _BGSurface(0), _SelectorSurface(0), _BGObjectMain(0),
		_SelectorBar(0), _CurrentMenuSelection(MENU_START), _ChangeSfx(0), _SelectedSfx(0)
{

}

TitleScreenState::~TitleScreenState(void)
{

}

void TitleScreenState::PushInitialise(
    GfxModuleWrapper* AGfxModule,
    SoundModuleWrapper* ASoundModule
)
{
	// Load the main graphic and fonts
	_BGSurface = AGfxModule->GetSurfaceManager()->CreateSurface("./Backgrounds/TitleScreen.png");
	_BGObjectMain = AGfxModule->GetBGManager()->GetBGObject(0);

	// Put the two together
	_BGObjectMain->SetSurfaceWrapper(_BGSurface);

	// Load the selector bar and graphics
	_SelectorSurface = AGfxModule->GetSurfaceManager()->CreateSurface("./Graphics/SelectorBar.png");
	_SelectorBar = AGfxModule->GetSpriteManager()->GetFreeSprite();
	_SelectorBar->SetSurfaceWrapper(_SelectorSurface);
	_SelectorBar->SetSpriteDimensions(128, 21);
	_SelectorBar->SetSpriteAnimation(1, 0, false);
	_SelectorBar->SetPriority(0);
	_SelectorBar->SetXYPosition(MENU_X_START_POSITION, MENU_Y_START_POSITION);

	// Load the fonts
	_TitleFont = AGfxModule->GetFontManager()->CreateFont(FONT_FILENAME.c_str(), TITLE_FONT_SIZE);
	_MenuFont  = AGfxModule->GetFontManager()->CreateFont(FONT_FILENAME.c_str(), MENU_FONT_SIZE);

	// Load the sound effects
	_ChangeSfx = ASoundModule->GetSfxManager()->CreateSfx("./Sounds/_common change.ogg");
	_SelectedSfx = ASoundModule->GetSfxManager()->CreateSfx("./Sounds/Common start.ogg");

	// Initialise the Menu
	_CurrentMenuSelection = MENU_START;
}

void TitleScreenState::PushCleanup(
    GfxModuleWrapper* AGfxModule,
    SoundModuleWrapper* ASoundModule
)
{
	// Free the font
	AGfxModule->GetFontManager()->FreeFont(_TitleFont);
	AGfxModule->GetFontManager()->FreeFont(_MenuFont);

	// Free the selector bar
	AGfxModule->GetSurfaceManager()->FreeSurface(_SelectorSurface);
	AGfxModule->GetSpriteManager()->FreeSprite(_SelectorBar);

	// Free the sounds
	ASoundModule->GetSfxManager()->FreeSfx(_ChangeSfx);
	ASoundModule->GetSfxManager()->FreeSfx(_SelectedSfx);
}

void TitleScreenState::PopInitialise(
    GfxModuleWrapper* AGfxModule,
    SoundModuleWrapper* ASoundModule
)
{
	// Set BGObject to the correct place just in case
	AGfxModule->GetBGManager()->ClearBGObject(_BGObjectMain);
	_BGObjectMain->SetSurfaceWrapper(_BGSurface);

	// Load the selector bar and graphics
	_SelectorSurface = AGfxModule->GetSurfaceManager()->CreateSurface("./Graphics/SelectorBar.png");
	_SelectorBar = AGfxModule->GetSpriteManager()->GetFreeSprite();
	_SelectorBar->SetSurfaceWrapper(_SelectorSurface);
	_SelectorBar->SetSpriteDimensions(128, 21);
	_SelectorBar->SetSpriteAnimation(1, 0, false);
	_SelectorBar->SetPriority(0);
	_SelectorBar->SetXPosition(MENU_X_START_POSITION);
	_SelectorBar->SetYPosition(MENU_Y_START_POSITION);

	// Load the fonts
	_TitleFont = AGfxModule->GetFontManager()->CreateFont(FONT_FILENAME.c_str(), TITLE_FONT_SIZE);
	_MenuFont  = AGfxModule->GetFontManager()->CreateFont(FONT_FILENAME.c_str(), MENU_FONT_SIZE);

	// Initialise the Menu
	_CurrentMenuSelection = MENU_START;

	// Load the sound effects
	_ChangeSfx = ASoundModule->GetSfxManager()->CreateSfx("./Sounds/_common change.ogg");
	_SelectedSfx = ASoundModule->GetSfxManager()->CreateSfx("./Sounds/Common start.ogg");
}

void TitleScreenState::PopCleanup(
    GfxModuleWrapper* AGfxModule,
    SoundModuleWrapper* ASoundModule
)
{
	// Free the memory
	AGfxModule->GetBGManager()->ClearBGObject(_BGObjectMain);
	AGfxModule->GetSurfaceManager()->FreeSurface(_BGSurface);
	AGfxModule->GetFontManager()->FreeFont(_TitleFont);
	AGfxModule->GetFontManager()->FreeFont(_MenuFont);
	AGfxModule->GetSurfaceManager()->FreeSurface(_SelectorSurface);
	AGfxModule->GetSpriteManager()->FreeSprite(_SelectorBar);

	// Free the sounds
	ASoundModule->GetSfxManager()->FreeSfx(_ChangeSfx);
	ASoundModule->GetSfxManager()->FreeSfx(_SelectedSfx);
}

void TitleScreenState::Update(
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

    // Draw the menu and title text
    AGfxModule->GetFontManager()->WriteText( _TitleFont, 20, 40, "Dance2x", STANDARD_COLOUR);
    AGfxModule->GetFontManager()->WriteText( _MenuFont, 130, 120, "start", 	STANDARD_COLOUR);
    AGfxModule->GetFontManager()->WriteText( _MenuFont, 139, 140, "quit", 	STANDARD_COLOUR);

    // Change the state of the menu
    _SelectorBar->SetYPosition( MENU_Y_START_POSITION + (MENU_SPACE_GAP * _CurrentMenuSelection) );

	if ( CurrentInputState & UP )
    {
        // Move up the menu
        if ( !(PreviousInputState & UP) )
			if(_CurrentMenuSelection > MENU_START )
			{
				--_CurrentMenuSelection;
				ASoundModule->GetSfxManager()->PlaySfx(_ChangeSfx);
			}
    }

    if ( CurrentInputState & DOWN )
    {
        // Move down the menu
        if ( !(PreviousInputState & DOWN) )
			if(_CurrentMenuSelection < MENU_QUIT )
			{
				++_CurrentMenuSelection;
				ASoundModule->GetSfxManager()->PlaySfx(_ChangeSfx);
			}
    }

	if ( CurrentInputState & BUTTON_B )
    {

		ASoundModule->GetSfxManager()->PlaySfx(_SelectedSfx);
		SdlTimer::Delay(800);
        // Quit the game if B is pressed when Quit is selected
        if ( !(PreviousInputState & BUTTON_B) )
        {
			switch(_CurrentMenuSelection )
			{
			case MENU_START:
				{
					// Add the song selection state to the top of the stack
					PlayState::GetSingletonPtr()->SetFilePath("", BEGINNER);
					AGameStateManager->Push(RatingState::GetSingletonPtr() );
					AGameStateManager->Push(PlayState::GetSingletonPtr() );
				} break;
			case MENU_QUIT:
				{
					// Quit the game
					QuitFlag = true;
				} break;
			default: break;

			}
		}
    }

    // If the flag is true, then pop this state
    if( QuitFlag )
		AGameStateManager->Pop();
}

/*
History
=======
2006-07-15: Added sound effects
2006-07-13: Created file and base functionality.
*/
