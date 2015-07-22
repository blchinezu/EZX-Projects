class CSnakeGameInfo {
	public:
		CTranslation *trans;
		CBoard *board;
		CScoreboard *scoreboard;
		
		CSnake *snake;
		
		TTF_Font *smallFont, *bigFont;
		CMessageBox *pauseDialog;
		CMessageBox *levelDialog;
		CMessageBox *exitDialog;
		CMessageBox *lostLifeDialog;
		CMessageBox *lostLastLifeDialog;

		CSnakeGameInfo(int lang)
		{
			trans = new CTranslation();
			trans->setLanguage(lang);
			
			// create fonts
			smallFont = TTF_OpenFont("fonts/pachyderm.ttf", 12);
				if(!smallFont) {
				printf("TTF_OpenFont: %s\n", TTF_GetError());
				exit(1);
			}
			bigFont = TTF_OpenFont("fonts/pachyderm.ttf", 16);
				if(!bigFont) {
				printf("TTF_OpenFont: %s\n", TTF_GetError());
				exit(1);
			}
			
			// create dialogs
			levelDialog = new CMessageBox(bigFont,smallFont,
				"",trans->get(TXT_START));
			pauseDialog = new CMessageBox(bigFont,smallFont,
				trans->get(TXT_PAUSE),trans->get(TXT_PAUSEINFO));
			exitDialog = new CMessageBox(bigFont,smallFont,
				trans->get(TXT_QUIT),trans->get(TXT_QUITINFO));
			lostLifeDialog = new CMessageBox(bigFont,smallFont,
				trans->get(TXT_LOSTLIFE),trans->get(TXT_START));
			lostLastLifeDialog = new CMessageBox(bigFont,smallFont,
				trans->get(TXT_LOSTGAME),trans->get(TXT_START));

			// create board
			board = new CBoard(30,19,10,10,0,30);
			
			// create snake. need board created
			snake = new CSnake(board,3);
			
			// create scoreboard
			scoreboard = new CScoreboard(bigFont,smallFont);

			NewGame();

	
		}						
		
		~CSnakeGameInfo()
		{
			delete snake;
			delete scoreboard;
			delete board;
			delete pauseDialog;
			delete exitDialog;
			delete levelDialog;
			delete lostLifeDialog;
			delete lostLastLifeDialog;
			TTF_CloseFont(smallFont);
			TTF_CloseFont(bigFont);
			
			delete trans;
		}
		
		void NextLevel()
		{
			scoreboard->IncLevel();
			PrepareLevel();
		}
		
		void PrepareLevel()
		{
			snake->Init();
			board->PrepareLevel(scoreboard->GetLevel());
			
			char *text = new char[20];
			sprintf(text, "%s %d", trans->get(TXT_LEVEL), scoreboard->GetLevel()); 
			levelDialog->SetMainText(text);
			delete text;
		}
		
		void NewGame()
		{
			// prepare level. need snake, board, fonts, dialogs and scoreboard
			scoreboard->Init();
			PrepareLevel();
		}
};
