/*
The CEngine Framework was found on :

http://gpwiki.org/index.php/SDL:Tutorials:Simple_Engine_Framework

It was written by Anders "Sion" Nissen and last updated 14. August 2005.

Content of this file is available under GNU Free Documentation License 1.2. :
http://www.gnu.org/copyleft/fdl.html

We modified the file to make it feet our requirements.
*/

#include "Engine.h"
#include "hide_a1200.h"

/** Default constructor. **/
CEngine::CEngine()
{

	m_lLastRender   = 0;
	m_lLastThink    = 0;
	//m_iFrameLenght  = 15; //Durée en millisecondes d'une frame.
    m_iFrameLenght=0;
	m_iWidth		= 320;
	m_iHeight		= 240;
	m_czTitle		= NULL;

	m_pScreen		= NULL;

	m_bMinimized		= false;
	m_bFullscreen       = false;
}

/** Destructor. **/
CEngine::~CEngine()
{
	SDL_Quit();
}

/** Sets the height and width of the window.
	@param iWidth The width of the window
	@param iHeight The height of the window
**/
void CEngine::SetSize(const int& iWidth, const int& iHeight)
{
	m_iWidth  = iWidth;
	m_iHeight = iHeight;
	m_pScreen = SDL_SetVideoMode( iWidth, iHeight, 32, SDL_HWSURFACE | SDL_ANYFORMAT | SDL_DOUBLEBUF);
}

//Même chose que précédemment, mais en plein écran.
void CEngine::SetFullScreen(const int& iWidth, const int& iHeight)
{
    m_iWidth  = iWidth;
	m_iHeight = iHeight;
	//A vérifier : l'ancienne surface de m_pScreen est-elle supprimée ?
	m_pScreen = SDL_SetVideoMode( iWidth, iHeight, 32, SDL_HWSURFACE | SDL_ANYFORMAT | SDL_DOUBLEBUF |SDL_FULLSCREEN);
}

void CEngine::flipFullScreen(bool fullscreen)
{
    if (fullscreen==true)
    {
        SDL_ShowCursor(false);
        m_pScreen = SDL_SetVideoMode( m_iWidth, m_iHeight, 32, SDL_HWSURFACE | SDL_ANYFORMAT | SDL_DOUBLEBUF |SDL_FULLSCREEN);
    }
    else
    {
        SDL_ShowCursor(true);
        m_pScreen = SDL_SetVideoMode( m_iWidth, m_iHeight, 32, SDL_HWSURFACE | SDL_ANYFORMAT | SDL_DOUBLEBUF);
    }
}

/** Initialize SDL, the window and the additional data. **/
void CEngine::Init()
{
    //+ éventuellement d'autres.
	#ifdef NO_SOUND_2H4U
		if ( SDL_Init( SDL_INIT_VIDEO |  SDL_INIT_TIMER ) < 0 )
	#else
	if ( SDL_Init( SDL_INIT_VIDEO |  SDL_INIT_TIMER  | SDL_INIT_AUDIO ) < 0 )
	#endif
	{
		fprintf( stderr, "Impossible d'initialiser SDL: %s\n", SDL_GetError() );
		exit( 1 );
	}

    //Ligne rajouté, au cas ou le programme est quitté violement (kill).
//    atexit(SDL_Quit);

    putenv("SDL_VIDEO_WINDOW_POS=center"); //pour centrer la fenêtre

    //Chargement des paramètres, afin de choisir si fenêtre ou plein écran
    ParamInit(m_iWidth,m_iHeight);

	// If we fail, return error.
	if ( m_pScreen == NULL )
	{
		fprintf( stderr, "Impossible de définir la vidéo: %s\n", SDL_GetError() );
		exit( 1 );
	}

	AdditionalInit();
}

//Le jeu sera en fait appellé par le menu.
//Pour revenir au menu, on fera un return.
/** The main loop. **/
void CEngine::Start()
{
	m_lLastRender = st_get_ticks();
	m_lLastThink = st_get_ticks();
	m_bQuit = false;

	// Main loop: loop forever.
	while ( !m_bQuit )
	{
		// Handle mouse and keyboard input
		HandleInput();

		if ( m_bMinimized ) {
			// Release some system resources if the app. is minimized.
		    SDL_Delay(100);
		} else {
			// Do some thinking
			DoThink();

			// Render stuff
			DoRender();

		}
        //On se passe du Delay, car l'affichage est limité (60 images par secondes)
	}

	AdditionalEnd();
	End(); //Lorsque l'on quitte le programme.

}

void CEngine::Quit()
{
    m_bQuit = true;
}

void CEngine::End()
{
     SDL_Quit();
     exit(0);
}

/** Handles all controller inputs.
	@remark This function is called once per frame.
**/
void CEngine::HandleInput()
{
	// Poll for events, and handle the ones we care about.
	SDL_Event event;
	while ( mySDL_PollEvent( &event ) )
	{
		switch ( event.type )
		{
		case SDL_KEYDOWN:

			KeyDown( event.key.keysym.sym );
			break;

		case SDL_KEYUP:
			KeyUp( event.key.keysym.sym );
			break;
			
		case SDL_MOUSEMOTION:
			touched_x=event.motion.x;
			touched_y=event.motion.y;
			touched=true;
			break;
			

        //Rajouter la gestion de 4 ou 5 touches supplémentaires.

		case SDL_QUIT:
			m_bQuit = true;
			break;

        //Si on minimize la fenêtre, le jeu passera en pause.
		case SDL_ACTIVEEVENT:
			if ( (event.active.state & SDL_APPACTIVE) || (event.active.state & SDL_APPINPUTFOCUS)) {
				if ( event.active.gain ) {
					m_bMinimized = false;
					WindowActive();
				} else {
					m_bMinimized = true;
					WindowInactive();
				}
			}
			break;
		} // switch
	} // while (handling input)
}

/** Handles the updating routine. **/
void CEngine::DoThink()
{
    long iElapsedThink = st_get_ticks() - m_lLastThink;
	m_lLastThink = st_get_ticks();

	Think( iElapsedThink );

}


//Permet de limiter l'affichage a 60 images par seconde.
/** Handles the rendering and FPS calculations. **/
void CEngine::DoRender()
{
    long iElapsedRender = st_get_ticks() - m_lLastRender;

    if (iElapsedRender>=m_iFrameLenght)
    {
        m_lLastRender = st_get_ticks();
        Render(m_pScreen);

        SDL_Flip( m_pScreen );
    }
}

/** Sets the title of the window
	@param czTitle A character array that contains the text that the window title should be set to.
**/

void CEngine::SetTitle(const char* czTitle)
{
	m_czTitle = czTitle;
	SDL_WM_SetCaption( czTitle, NULL );
}

/** Retrieve the title of the application window.
	@return The last set windows title as a character array.
	@remark Only the last set title is returned. If another application has changed the window title, then that title won't be returned.
**/
const char* CEngine::GetTitle()
{
	return m_czTitle;
}

//Nécessite SDL_Image. Recoit le chemin d'accès de l'icone en paramètre.
void CEngine::SetIcon(const char* czChemin)
{
    SDL_WM_SetIcon(IMG_Load(czChemin), NULL);
}

/** Retrieve the main screen surface.
	@return A pointer to the SDL_Surface surface
	@remark The surface is not validated internally.
**/
SDL_Surface* CEngine::GetSurface()
{
	return m_pScreen;
}

