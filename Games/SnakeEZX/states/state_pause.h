class CPauseState : public CCustomState
{
	public:		
		CPauseState(CSnakeGameInfo *info) : CCustomState(info) {};
		
		void Render()
		{
			if (JustActivated()) {
				info->pauseDialog->Draw(screen);
			}
		};
		
		
		void KeyDown(int iKeyEnum)
		{
			if (iKeyEnum==SDLK_RETURN)
			{
				ChangeToState(ST_PLAYING);
			}
		};
		
		void JoyButtonDown(int iJoyButtonEnum, SDL_Joystick *joystick)
		{
			if (iJoyButtonEnum==GP2X_BUTTON_SELECT)
			{
				ChangeToState(ST_PLAYING);
			}
		};
				
		
};
