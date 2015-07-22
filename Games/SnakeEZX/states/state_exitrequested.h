class CExitRequestedState : public CCustomState
{
	private:

	public:		
		CExitRequestedState(CSnakeGameInfo *info) : CCustomState(info) {};
				
		void Render()
		{
			if (JustActivated()) {
				info->exitDialog->Draw(screen);
			}
		};
		
		void KeyDown(int iKeyEnum)
		{
			if (iKeyEnum==SDLK_RETURN)
			{
				ChangeToState(ST_PLAYING);
			}
			else if (iKeyEnum==SDLK_ESCAPE)
			{
				ChangeToState(ST_MENU);
			}
		};
		
		void JoyButtonDown(int iJoyButtonEnum, SDL_Joystick *joystick)
		{
			if (iJoyButtonEnum==GP2X_BUTTON_SELECT)
			{
				ChangeToState(ST_PLAYING);
			}		
			else if (iJoyButtonEnum==GP2X_BUTTON_START)
			{
				ChangeToState(ST_MENU);
			}		
		};				
		
};
