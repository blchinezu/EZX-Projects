#ifndef ENGINE_H
#define ENGINE_H

class SURFACE {
	public:
		static SDL_Surface* SCREEN;
};
SDL_Surface* SURFACE::SCREEN = NULL; 

class CGameEngine;

class CGameState
{

	private:	
		bool justActivated;
		
		void DoThink();
		void DoRender();
	
	protected:
		
		SDL_Surface *screen; 
		CGameEngine *game;
		
	public:

		CGameState();	
		~CGameState();

		void Init(SDL_Surface *_screen, CGameEngine *_game);		
		void ChangeToState(int st);
		void Activate();		
		bool JustActivated();

		// virtual functions to be overloaded
		virtual void AdditionalInit() {};
		virtual void Start() {};
	
		virtual void Think() {};
		virtual void Render() {};

		virtual void End() {};

		virtual void KeyUp  (int iKeyEnum) {};
		virtual void KeyDown(int iKeyEnum) {};

		virtual void JoyButtonUp  (int iJoyButtonEnum) {};
		virtual void JoyButtonDown(int iJoyButtonEnum, SDL_Joystick *joystick) = 0;

		virtual void MouseMoved(int iX, int iY, int iRelX, int iRelY) {};
		virtual void MouseButtonUp(int iButton) {};
		virtual void MouseButtonDown(int iButton) {};
	
};



//------------------------------------------------------------------------------------------//


class CGameEngine  
{

	public:
	CGameEngine(int _desiredFPS, bool _fullScreen, int _maxStates)
	{
		lastTick = 0;
		desiredFPS = _desiredFPS;
		fullScreen = _fullScreen;
		width	= 320;
		height	= 240;
		title   = 0;
		screen = NULL;
		maxStates = _maxStates;
		state = new CGameState*[maxStates];
		for (int i=0; i<maxStates; i++) {
			state[i] = NULL;
		}	
	}

	~CGameEngine()
	{
	}
	
	bool AddState(CGameState *st, int id)
	{
		bool r = true;
		if (id<maxStates) {
			state[id]=st;
			state[id]->Init(screen,this);
		} else {
			r = false;
		}
		return r;
	}

	void Init()
	{
		
		// Register SDL_Quit to be called at exit; makes sure things are cleaned up when we quit.
		//atexit(SDL_Quit); // Commented as it turns to a segmentation fault when quiting ( in EZX phone the memory is cleared anyway so it's no problem )
		
		// Initialize SDL's subsystems - in this case, only video.
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0) 
		{
			fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
			exit(1);
		}

		joystick = SDL_JoystickOpen(0);
		
		// Attempt to create a WIDTHxHEIGHT window with 32bit pixels.
		SetSize(width, height);

		AdditionalInit();

		// If we fail, return error.
		if (screen == NULL) 
		{
			fprintf(stderr, "Unable to set up video: %s\n", SDL_GetError());
			exit(1);
		}
	}	
	
	void Start(int _state)
	{
		SetState(_state);
		quit = false;

		// Main loop: loop forever.
		while (!quit)
		{
			DoThink();
			DoRender();
		
			SDL_Event event;
			while (SDL_PollEvent(&event)) 
			{
				switch (event.type) 
				{
				case SDL_KEYDOWN:
					state[activeState]->KeyDown(event.key.keysym.sym);
					break;
	
				case SDL_KEYUP:
					state[activeState]->KeyUp(event.key.keysym.sym);
					break;

				case SDL_QUIT:
					quit = true;
					break;
				
				/*
				case SDL_MOUSEMOTION:
					state[activeState]->MouseMoved(event.motion.x, event.motion.y, event.motion.xrel, event.motion.yrel);
					break;

				case SDL_MOUSEBUTTONUP:
					state[activeState]->MouseButtonUp(event.button.button);
					break;

				case SDL_MOUSEBUTTONDOWN:
					state[activeState]->MouseButtonDown(event.button.button);
					break;
				*/
				
				case SDL_JOYBUTTONDOWN:
					state[activeState]->JoyButtonDown(event.jbutton.button, joystick);
					break;
				case SDL_JOYBUTTONUP:
					state[activeState]->JoyButtonUp(event.jbutton.button);
					break;
				

				case SDL_ACTIVEEVENT:
					if ( event.active.state & SDL_APPACTIVE ) {
						if ( event.active.gain ) {
							WindowActive();
						} else {
							WindowInactive();
						}
					}
					break;
				}
			}
		}

		End();
	
	}
	
	void SetState(int _state)
	{
		activeState = _state;
		state[activeState]->Activate();
	}
	
	void Quit()
	{
		quit=true;	
	}

	void  SetTitle(char* _title)
	{
		title = _title;
		SDL_WM_SetCaption(title, "Icon Title");
	}
	char* GetTitle()
	{
		return title;	
	}
	
	
	virtual void AdditionalInit() {};
	virtual void End() {};

	virtual void WindowActive() {};
	virtual void WindowInactive() {};


	private:

		int lastTick;
		int width;
		int height;
		int desiredFPS;
		bool fullScreen;
		char* title;
		int maxStates;	

		CGameState **state;
		int activeState;

		SDL_Joystick *joystick;

	protected:
	
		SDL_Surface *screen; 
	
		void DoThink()
		{
			// Ask SDL for the time in milliseconds
			int tick = SDL_GetTicks();

			if (tick <= lastTick) 
			{
				SDL_Delay(1);
				return;
			}

			while (lastTick < tick)
			{
				state[activeState]->Think();
				lastTick += 1000 / desiredFPS;
			}
		}
		
		
		void DoRender()
		{
			// Lock surface if needed
			if (SDL_MUSTLOCK(screen))
				if (SDL_LockSurface(screen) < 0)
					return;

			state[activeState]->Render();

			// Unlock if needed
			if (SDL_MUSTLOCK(screen)) 
				SDL_UnlockSurface(screen);

			SDL_UpdateRect(screen, 0, 0, width, height);
		}

		void SetSize(int _width, int _height)
		{
			width  = _width;
			height = _height;
			if (fullScreen) {
				screen = SDL_SetVideoMode(width, height, 0, SDL_SWSURFACE | SDL_FULLSCREEN);
			} else {
				screen = SDL_SetVideoMode(width, height, 0, SDL_SWSURFACE);
			}		
			SURFACE::SCREEN=screen;
		}
	
		bool quit;


};

//-------------------------------------------------------------------------------------------//


CGameState::CGameState()
{
	screen = NULL;
	justActivated = false;
};
	
CGameState::~CGameState() {
			End();
		};

void CGameState::Init(SDL_Surface *_screen, CGameEngine *_game){
			screen = _screen;
			game = _game;
			AdditionalInit(); // call virtual function in inherited class
		}
		
void CGameState::ChangeToState(int st)
		{
			game->SetState(st);
		}
		
void CGameState::Activate()
		{
			justActivated = true;
		}
		
		
bool CGameState::JustActivated()
		{
			bool r = justActivated;
			if (r) justActivated = false;
			return r;
		}		



#endif // ENGINE_H
