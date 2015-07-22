class CStartingLevelState : public CCustomState
{
	private:
		
	public:		
		CStartingLevelState(CSnakeGameInfo *info) : CCustomState(info) {};
				
		void Render()
		{
			if (JustActivated()) {
				info->board->Draw(screen);
				info->snake->Draw(screen,true);
				info->scoreboard->Draw(screen,true);
				info->levelDialog->Draw(screen);
			}
		};
		
		void KeyDown(int iKeyEnum)
		{
			ChangeToState(ST_PLAYING);
		};
		
		void JoyButtonDown(int iJoyButtonEnum, SDL_Joystick *joystick)
		{
			if (iJoyButtonEnum==GP2X_BUTTON_START || iJoyButtonEnum==GP2X_BUTTON_CLICK)
			{
				ChangeToState(ST_PLAYING);
			}
		};
		
		
};
