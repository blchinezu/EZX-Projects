//! A singleton abstract class to hold a game state
//! Uses the singleton pattern

#ifndef GAMESTATE_H
#define GAMESTATE_H

// Forward declarations
namespace GfxModule
{
    class GfxModuleWrapper;
}

namespace SoundModule
{
    class SoundModuleWrapper;
}

namespace InputModule
{
    class Input;
}

namespace GameStateModule
{
    // Forward declare classes
    class GameStateManager;

    class GameState
    {
    private:
        //! Singleton pattern. This state cannot be copied
        GameState(const GameState &AGameState);

    public:
        //! Singleton pattern. This state cannot be constructed outside the class hierarchy
        GameState(void)
        {
            // Intentionally left blank
        }

        //! Virtual destructor. Does nothing
        virtual ~GameState(void)
        {
            // Intentionally left blank
        }

        //! Virtual. Initialise the state when the state is pushed on top the stack
        virtual void PushInitialise(
            GfxModule::GfxModuleWrapper* AGfxModule,
            SoundModule::SoundModuleWrapper* ASoundModule
        )
        {
            // Intentionally left blank
        }

        //! Virtual. Cleanup up the state when it is pushed then further into the stack
        virtual void PushCleanup(
            GfxModule::GfxModuleWrapper* AGfxModule,
            SoundModule::SoundModuleWrapper* ASoundModule
        )
        {
            // Intentionally left blank
        }

        //! Virtual. When the state is popped up to the top of the stack
        virtual void PopInitialise(
            GfxModule::GfxModuleWrapper* AGfxModule,
            SoundModule::SoundModuleWrapper* ASoundModule
        )
        {
            // Intentionally left blank
        }

        //! Virtual. When the state is popped off the stack
        virtual void PopCleanup(
            GfxModule::GfxModuleWrapper* AGfxModule,
            SoundModule::SoundModuleWrapper* ASoundModule
        )
        {
            // Intentionally left blank
        }

        //! Virtual. Gets called once per frame when it is on top of the stack
        virtual void Update(
            GfxModule::GfxModuleWrapper* AGfxModule,
            SoundModule::SoundModuleWrapper* ASoundModule,
            InputModule::Input* AInputModule,
            GameStateModule::GameStateManager* AGameStateManager
        )
        {
            // Intentionally left blank
        }

    };
}

#endif // GAMESTATE_H

/*
History
=======
2006-06-23: Created the file and class
*/
