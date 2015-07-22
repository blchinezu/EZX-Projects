class CLostLastLifeState : public CCustomState
{
	private:

	public:		
		CLostLastLifeState(CSnakeGameInfo *info) : CCustomState(info) {};
				
		void Render()
		{
			if (JustActivated()) {
				info->snake->Draw(screen,false);
				info->lostLastLifeDialog->Draw(screen);
			}
		};
		
		void KeyDown(int iKeyEnum)
		{
			info->NewGame();
			ChangeToState(ST_MENU);
		};
		
		void JoyButtonDown(int iJoyButtonEnum, SDL_Joystick *joystick)
		{
			if (iJoyButtonEnum==GP2X_BUTTON_START || iJoyButtonEnum==GP2X_BUTTON_CLICK)
			{
				info->NewGame();
				ChangeToState(ST_MENU);
			}
		};		
		
};
