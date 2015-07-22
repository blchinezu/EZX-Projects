#define LANG_ES 0
#define LANG_EN 1


#define TXT_PLAY       0
#define TXT_OPTIONS    1
#define TXT_HELP       2
#define TXT_QUIT       3
#define TXT_PAUSE      4
#define TXT_PAUSEINFO  5
#define TXT_QUITINFO   6
#define TXT_LANGUAGE   7
#define TXT_RESTART    8
#define TXT_BACK       10
#define IMG_HELP       11
#define TXT_LEVEL      12
#define TXT_SCORE      13
#define TXT_START      14
#define TXT_LOSTLIFE   15
#define TXT_LOSTGAME   16


class CTranslation
{
	public:
		CTranslation()
		{
			lang = 0;
		}
		
		void nextLanguage()
		{
			lang++;
			if (lang>1) lang=0;
		}
		
		void setLanguage(int l)
		{
			lang = l;
		}
		
		int getLang()
		{
			return lang;
		}
		
		char *get(int id)
		{
			if (lang==LANG_ES)
			{
				switch(id)
				{
					case TXT_PLAY:      return "Jugar";
					case TXT_OPTIONS:   return "Opciones";
					case TXT_HELP:      return "Instrucciones";
					case TXT_QUIT:      return "Salir";
					case TXT_PAUSE:     return "Pausa";
					case TXT_PAUSEINFO: return "SELECT: Continuar";
					case TXT_QUITINFO:  return "SELECT: Continuar  START: Salir";
					case TXT_LANGUAGE:  return "Cambiar idioma";
					case TXT_RESTART:   return "Es necesario reiniciar para aplicar los cambios";
					case TXT_BACK:      return "Volver";
					case IMG_HELP:      return "pixmaps/help_ES.bmp";
					case TXT_LEVEL:     return "Nivel";
					case TXT_SCORE:     return "Puntos";
					case TXT_START:     return "Pulsa START para continuar";
					case TXT_LOSTLIFE:  return "Has perdido una vida";
					case TXT_LOSTGAME:  return "Has perdido";
				}
			}
			else if (lang==LANG_EN)
			{
				switch(id)
				{
					case TXT_PLAY:      return "Play";
					case TXT_OPTIONS:   return "Setup";
					case TXT_HELP:      return "Help";
					case TXT_QUIT:      return "Quit";
					case TXT_PAUSE:     return "Pause";
					case TXT_PAUSEINFO: return "SELECT: Resume";
					case TXT_QUITINFO:  return "SELECT: Resume  HANGUP: Quit";
					case TXT_LANGUAGE:  return "Change language";
					case TXT_RESTART:   return "Changes will apply on restart";
					case TXT_BACK:      return "Back";
					case IMG_HELP:      return "pixmaps/help_EN.bmp";
					case TXT_LEVEL:     return "Level";
					case TXT_SCORE:     return "Score";
					case TXT_START:     return "Press ANY KEY to continue";
					case TXT_LOSTLIFE:  return "You have lost a life";
					case TXT_LOSTGAME:  return "You have lost";
					
				}
			}
		}
		
	private:
	
		int lang;
};

