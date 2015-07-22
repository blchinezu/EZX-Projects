class CLostLifeState : public CCustomState
{
	private:

	public:		
		CLostLifeState(CSnakeGameInfo *info) : CCustomState(info) {};
				
		void Render()
		{
			if (JustActivated()) {
				info->snake->Draw(screen,false);
				info->lostLifeDialog->Draw(screen);
			}
		};
		
		void KeyDown(int iKeyEnum)
		{
			info->PrepareLevel();
			ChangeToState(ST_STARTING_LEVEL);
		};
		
		void JoyButtonDown(int iJoyButtonEnum, SDL_Joystick *joystick)
		{
			if (iJoyButtonEnum==GP2X_BUTTON_START || iJoyButtonEnum==GP2X_BUTTON_CLICK)
			{
				info->PrepareLevel();
				ChangeToState(ST_STARTING_LEVEL);
			}
		};		
		
};
