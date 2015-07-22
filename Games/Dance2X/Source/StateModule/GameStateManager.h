//! Class to hold and manage a stack of Games States

#ifndef GAMESTATEMANAGER_H
#define GAMESTATEMANAGER_H

#include <stack>

// Forward declare some classes
namespace KernelModule
{
	class Kernel;
};

namespace GameStateModule
{
    // Forward declare classes
    class GameState;

	class GameStateManager
	{
	private:
		//! A stack of Game State references. Manager always deals with the last item added
		std::stack<GameState *> _GameStateStack;

		//! Pointer to the Kernel
		KernelModule::Kernel *_Kernel;

	public:
		//! Standard Constructor. Does nothing
		GameStateManager(KernelModule::Kernel * const AKernel);

		//! Standard Destructor. Frees all states currently on the stack
		~GameStateManager(void);

		//! Check if the stac kis empty
		const bool Empty(void) const
		{
            return _GameStateStack.empty();
        }

        //! Empty the whole stack. Effectively the 'quit function'
        void EmptyTheStack(void);

		//! Push wraparound of the stack.push function
		void Push(GameState * const AGameState);

		//! Pop wraparound of the stack.pop function.
		void Pop(void);

		//! Updates the state. Usually called once per game loop
		void Update(void);
	};
};

#endif // GAMESTATEMANAGER_H

/*
History
=======
2006-07-01: Empty the entire stack
2006-06-23: Renamed class and file from State to GameState. Added IsEmpty() Function
2006-06-02: States will now be singletons. Changes reflect this fact. All functions now moved to a
separate source file
2006-06-01: Created the header file
*/
