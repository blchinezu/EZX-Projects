class CPlayingState : public CCustomState
{
	public:
		CPlayingState(CSnakeGameInfo *info) : CCustomState(info) {};
		
		void Render()
		{
			if (JustActivated()) {
				info->board->Draw(screen);
				info->snake->Draw(screen,true);
				info->scoreboard->Draw(screen,true);
			} else {
				info->snake->Draw(screen,false);
				info->scoreboard->Draw(screen,false);
			}
		};
		
		void Think()
		{
			info->snake->Think(info->scoreboard);
			if (info->snake->HasCollided()) {
				info->scoreboard->LostLife();
				if (info->scoreboard->GetLifes()>=0) {
					ChangeToState(ST_LOST_LIFE);
				} else {
					ChangeToState(ST_LOST_LAST_LIFE);
				}
			}
			if (info->board->Finished()) {
				info->NextLevel();
				ChangeToState(ST_STARTING_LEVEL);
			}

			info->scoreboard->Think();

		};
		
		void KeyDown(int iKeyEnum)
		{
			switch (iKeyEnum) {
				case SDLK_RETURN:
					ChangeToState(ST_PAUSE);
					break;
				case SDLK_LEFT:
					info->snake->ChangeDirection(DIR_LEFT);
					break;
				case SDLK_RIGHT:
					info->snake->ChangeDirection(DIR_RIGHT);
					break;
				case SDLK_UP:
					info->snake->ChangeDirection(DIR_UP);
					break;
				case SDLK_DOWN:
					info->snake->ChangeDirection(DIR_DOWN);
					break;
				case SDLK_ESCAPE:
					ChangeToState(ST_EXIT_REQUESTED);
					break;
			}
		};
		
		/* NOTE: Modified to match http://wiki.gp2x.org/wiki/Suggested_Joystick_Configurations (Case 5) */
		void JoyButtonDown(int iJoyButtonEnum, SDL_Joystick *joystick)
		{
			if (iJoyButtonEnum==GP2X_BUTTON_SELECT)
			{
				ChangeToState(ST_PAUSE);
			}		
			if (iJoyButtonEnum==GP2X_BUTTON_START)
			{
				ChangeToState(ST_EXIT_REQUESTED);
			}

			joy = joystick;
			if (dirUp())
			{
				info->snake->ChangeDirection(DIR_UP);
			}		
			if (dirDown())
			{
				info->snake->ChangeDirection(DIR_DOWN);
			}		
			if (dirLeft())
			{
				info->snake->ChangeDirection(DIR_LEFT);
			}		
			if (dirRight())
			{
				info->snake->ChangeDirection(DIR_RIGHT);
			}
		};

		bool dirUp()
		{
			return GetState(GP2X_BUTTON_Y) ||
				GetState(GP2X_BUTTON_UP) || /* sw1, sw1+sw2, sw8+sw1 */ 
				(GetState(GP2X_BUTTON_UPLEFT) && !GetState(GP2X_BUTTON_LEFT)); /* sw8, sw8+sw1 */
		};
		bool dirDown()
		{
			return GetState(GP2X_BUTTON_X) ||
				GetState(GP2X_BUTTON_DOWN) || /* sw5, sw5+sw6, sw4+sw5 */
				(GetState(GP2X_BUTTON_DOWNRIGHT) && !GetState(GP2X_BUTTON_RIGHT)); /* sw4, sw4+sw5 */
		};
		bool dirLeft()
		{
			return GetState(GP2X_BUTTON_A) ||
				GetState(GP2X_BUTTON_LEFT) || /* sw7, sw7+sw8, sw6+sw7 */
				(GetState(GP2X_BUTTON_DOWNLEFT) && !GetState(GP2X_BUTTON_DOWN)); /* sw6, sw6+sw7 */
		};
		bool dirRight()
		{
			return GetState(GP2X_BUTTON_B) ||
				GetState(GP2X_BUTTON_RIGHT) || /* sw3, sw2+sw3, sw3+sw4 */
				(GetState(GP2X_BUTTON_UPRIGHT) && !GetState(GP2X_BUTTON_UP)); /* sw2, sw2+sw3 */
		};

		bool GetState(int key)
		{
			return SDL_JoystickGetButton(joy, key) == 1;
		};

	private:
		SDL_Joystick *joy;
		
};
