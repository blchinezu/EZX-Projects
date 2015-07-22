//! Class to handle input from SDL_Input from a GP2X point
/*!
PC Input:
Q: top left
W: top
E: top right
A: left
D: right
Z: down left
X: down
C: down right
S: joystick click

I: GP2X A
O: GP2X Y
K: GP2X X
L: GP2X B
U: GP2X L
P: GP2X R

T: select
Y: start
G: vol -
H: vol +
*/

#ifndef INPUT_H
#define INPUT_H

enum
{
	LEFT 			= (1 << 0),
	RIGHT 			= (1 << 1),
	DOWN			= (1 << 2),
	UP				= (1 << 3),
	UPRIGHT 		= (1 << 4),
	UPLEFT			= (1 << 5),
	DOWNRIGHT		= (1 << 6),
	DOWNLEFT		= (1 << 7),
	BUTTON_Y		= (1 << 8),
	BUTTON_A		= (1 << 9),
	BUTTON_B		= (1 << 10),
	BUTTON_X		= (1 << 11),
//	BUTTON_L		= (1 << 12),
//	BUTTON_R		= (1 << 13),
//	BUTTON_JOYCLICK = (1 << 14),
	BUTTON_SELECT 	= (1 << 15),
//	BUTTON_START	= (1 << 16),
	BUTTON_VOLDOWN	= (1 << 17),
	BUTTON_VOLUP	= (1 << 18),
	JOY_ORTHOGONAL	= LEFT | RIGHT | DOWN | UP,
	JOY_DIAGONALS	= UPRIGHT | UPLEFT | DOWNRIGHT | DOWNLEFT,
	JOY_MASK		= JOY_ORTHOGONAL | JOY_DIAGONALS
};

namespace InputModule
{
	class Input
	{
	private:
		//! The current state of the input
		unsigned int _CurrentInputState;
		unsigned int _PreviousInputState;

		//! Kill flag. If true, the app as been forced to quit by an outside process
		bool _KillFlag;

	public:
		//! Standard Constructor. Initialises the joystick if compiled was a GP2X app
		Input(void);

		//! Standard Destructor. Does nothing
		~Input(void);

		//! Returns the current state of the input
		const unsigned int GetCurrentInputState(void) const
		{
			return _CurrentInputState;
		}

		//! Returns the previous frame's input state
		const unsigned int GetPreviousInputState(void) const
		{
			return _PreviousInputState;
		}

		//! Return the status of the KillFlag
		bool GetKillFlag(void) const
		{
			return _KillFlag;
		}

		//! Update the the input state. Called once per frame
		void UpdateInputState(void);
	};
};

#endif // INPUT_H

/*
History
=======
2006-07-19: Adding support for case 5
2006-07-17: Added a kill app flag and function
2006-07-15: Changed #defines to consts
2006-06-11: Created file and started the Input Module
*/
