#include "./GameStateManager.h"
#include "./GameState.h"
#include "../Kernel/Kernel.h"
#include "../InputModule/Input.h"
#include "../SoundModule/SoundModuleWrapper.h"
#include "../SoundModule/SfxManager.h"
#include "../SoundModule/MusicManager.h"
#include "../Logger/Logger.h"

using namespace GameStateModule;
using namespace LogModule;
using namespace KernelModule;

GameStateManager::GameStateManager(Kernel * const AKernel)
{
	ErrLog << "<< Creating Stack Manager >>\n";
	_Kernel = AKernel;
	ErrLog << "<< Created Stack Manager >>\n";
}

GameStateManager::~GameStateManager(void)
{
	ErrLog << "<< Destroying State Manager >>\n";
	ErrLog << "<< Clearing State Manager Stack >>\n";
	// Clear the stack
	while(!_GameStateStack.empty() )
	{
		_GameStateStack.pop();
	}

	ErrLog << "<< Destroyed State Manager >>\n";
}

void GameStateManager::EmptyTheStack(void)
{
	ErrLog << "Empting the entire stack\n";
	// While the stack is not empty, pop the stack
	while(!_GameStateStack.empty() )
	{
		_GameStateStack.pop();
	}

	ErrLog << "Emptied the entire stack\n";
}

void GameStateManager::Push(GameState * const AGameState)
{
	// Check if the stack is empty
    if( !_GameStateStack.empty() )
    {
        // If not, Cleanup the state that is on top
        ErrLog << "Cleaning up the Game State that is at the top of the Stack\n";
        _GameStateStack.top()->PushCleanup(
            _Kernel->GetGfxModuleWrapper(),
            _Kernel->GetSoundModuleWrapper()
        );
    }

	// Pushing the new state onto the top
	ErrLog << "Pushing a new Game State onto the Stack\n";
	_GameStateStack.push(AGameState);

	// Initialise the new game state
	ErrLog << "Initialising the new Game State\n";
	_GameStateStack.top()->PushInitialise(
        _Kernel->GetGfxModuleWrapper(),
        _Kernel->GetSoundModuleWrapper()
    );
}

void GameStateManager::Pop(void)
{
    // Cleanup the state that is on top
    ErrLog << "Cleaning up the Game State that is at the top of the Stack\n";
    _GameStateStack.top()->PopCleanup(
		_Kernel->GetGfxModuleWrapper(),
        _Kernel->GetSoundModuleWrapper()
    );
    // Pop it off the stack
    ErrLog << "Popping off a Game State off the Stack\n";
	_GameStateStack.pop();

    // Check if the stack is empty
    if( !_GameStateStack.empty() )
    {
        // If not then intialise the games state that is on top
        ErrLog << "Initialising the current Game State on top of the Stack\n";
        _GameStateStack.top()->PopInitialise(
            _Kernel->GetGfxModuleWrapper(),
            _Kernel->GetSoundModuleWrapper()
        );
    }
}

void GameStateManager::Update(void)
{
    // Run the State's update loop
	_GameStateStack.top()->Update(
        _Kernel->GetGfxModuleWrapper(),
        _Kernel->GetSoundModuleWrapper(),
        _Kernel->GetInputManager(),
        this    // Pass itself to the State
    );
    // Render the frame
    _Kernel->RenderScreen();
    // Regulate the framerate
    _Kernel->RegulateFramerate();
    // Process input
    _Kernel->GetInputManager()->UpdateInputState();
    // Check the status of the kill flag. If true, quit the app
    if(_Kernel->GetInputManager()->GetKillFlag() )
    {
		this->EmptyTheStack();
	}
	// Change the volume
	unsigned int CurrentInputState = _Kernel->GetInputManager()->GetCurrentInputState();
	if( CurrentInputState & BUTTON_VOLUP )
	{
		_Kernel->GetSoundModuleWrapper()->GetMusicManager()->VolumeUp();
		_Kernel->GetSoundModuleWrapper()->GetSfxManager()->VolumeUp();
	}
	if( CurrentInputState & BUTTON_VOLDOWN )
	{
		_Kernel->GetSoundModuleWrapper()->GetMusicManager()->VolumeDown();
		_Kernel->GetSoundModuleWrapper()->GetSfxManager()->VolumeDown();
	}
}

/*
History
=======
2006-07-17: Added the kill process to the update loop
2006-07-06: Forget to cleanup on pop
2006-07-01: Added support for obtain the sound module wrapper. Fixed an error with the destructor as
it was checking against the wrong condition.
2006-06-23: Renamed class and file from State to GameState.
2006-06-02: Created file
*/
