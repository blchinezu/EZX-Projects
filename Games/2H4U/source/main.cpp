/*
    Copyright 2006 Pierre Lagouge, Pierre-Yves Ricau

    This file is part of 2H4U.

    2H4U is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    2H4U is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with 2H4U; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

//entrée sortie etc standard.
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

//Le moteur global de jeu dont on hérite
#include "Engine.h"
//Le moteur du menu
#include "menu.h"
//Pour la gestion de paramètres.
#include "Param.h"
//include pour l'objet jeu.
#include "jeu.h"
//Include pour la gestion des highscores.
#include "highscore.h"
//Permet de gérer l'intro.
#include "intro.h"
//Launcher pour choisir la langue et le clavier
#include "launcher.h"
//Les includes de SDL et quelques constantes.
#include "sdl_includes.h"
#include "hide_a1200.h"

// Define the EZX Keyboard
#define EZX_GAME_BRICKS_LEFT	SDLK_MINUS
#define EZX_GAME_BRICKS_RIGHT	SDLK_PLUS
#define EZX_GAME_TETRIS_LEFT	SDLK_LEFT
#define EZX_GAME_TETRIS_RIGHT	SDLK_RIGHT
#define EZX_GAME_TETRIS_DROP	SDLK_DOWN
#define EZX_GAME_TETRIS_ROTATE	SDLK_UP
#define EZX_GAME_TETRIS_DROP2	SDLK_SPACE
#define EZX_GAME_PAUSE			SDLK_PAUSE
#define EZX_GAME_ESCAPE			SDLK_ESCAPE
#define EZX_GAME_RETURN			SDLK_RETURN

#define EZX_MENU_UP				SDLK_UP
#define EZX_MENU_DOWN			SDLK_DOWN
#define EZX_MENU_LEFT			SDLK_LEFT
#define EZX_MENU_RIGHT			SDLK_RIGHT
#define EZX_MENU_RETURN			SDLK_RETURN
#define EZX_MENU_ESCAPE			SDLK_ESCAPE



//Pour la gestion du clavier, vue qu'on a besoin que de quelques touches.
//Plutot que d'accéder aux cases 0, 1 etc... on utilise des define.
#define K_NBKEYS  12 //Nombre de touches définies.
#define K_ENTER   0
#define K_LEFT    1
#define K_RIGHT   2
#define K_UP      3
#define K_DOWN    4
#define K_Q       5
#define K_D       6
#define K_Z       7
#define K_S       8
#define K_SPACE   9
#define K_P       10
#define K_ESCAPE  11

//Constantes représentant les différentes phases du programme.
#define P_MENU 0 //Pendant le menu
#define P_NEW_GAME  1 //Nouvelle partie
#define P_GAME  2 //Pendant le jeu.
#define P_END_GAME  3
#define P_PAUSE_GAME  4
#define P_SCORE  5
#define P_INTRO  6
#define P_LAUNCHER  7

//Permet la gestion de la raquette, pour lui donner une direction.
#define R_GAUCHE -1
#define R_DROITE 1

int touched_x=0;
int touched_y=0;
bool touched=false;


//***********************************************************************
class CMyEngine: public CEngine
{
private:

    //Indique dans qu'elle phase du programme on se trouve (menu, pause, jeu...)
    int m_iPhase;

    //Pour la gestion du clavier
    //Attention : Pas la même gestion pour le jeu et pour le menu.
    bool m_iKey[K_NBKEYS];

    CMenu m_Menu;
    CJeu m_Jeu;
    CParam m_Param;
    CHighscore m_Highscore;
    CIntro m_Intro;
    CLauncher m_Launcher;

    char langTxt[MAX_LANG][256];

    //Pour tester les temps de calculs dans le jeu.
    long tempTest1;
    long tempTest2;

public:
	void ParamInit(int,int);
	void AdditionalInit ();
    void AdditionalEnd();
	void Think	    ( const int& iElapsedTime );
	void Render	    ( SDL_Surface* pDestSurface );

	void ProcessTouch   ();
	
	void KeyUp  	    (const int& iKeyEnum);
	void KeyDown	    (const int& iKeyEnum);

	void WindowActive() {}
    void WindowInactive();

};
//*********************************************************************** : le main.
int main(int argc, char* argv[])
{

    CMyEngine Engine;
	Engine.Init();
	Engine.SetTitle( "2H4U : Too Hard For You" );
	Engine.Start();

	return 0;
}
//***********************************************************************
//Fonctions de CMyEngine.
//Appellé par Init.
void CMyEngine::ParamInit(int iWidth,int iHeight)
{

    SetIcon("data/images/icone.png");

    //Dans un premier temps, on charge le fichier de paramètres.
    m_Param.initParam("data/config.dat");

    m_Highscore.loadHighscore("data/HighScores.dat");

        //Puis on fait plein écran ou fenetre suivant le cas.
        if (m_Param.getFullscreen()==0)
        {
            SDL_ShowCursor(true);
            SetSize( iWidth, iHeight );
        }
        else
        {
            SDL_ShowCursor(false);
            SetFullScreen( iWidth, iHeight );
        }

}
//Appellé par Init.
void CMyEngine::AdditionalInit()
{
    srand(time(NULL));

    //Librairie d'écriture.
    TTF_Init();

    if (m_Param.getNew()==true)
    {
        m_iPhase=P_LAUNCHER;
        m_Launcher.initLauncher();
    }
    else
    {
        //On prépare l'entrée vers l'intro.
        m_iPhase=P_INTRO;
        m_Param.getTxtLang(langTxt);
        m_Param.getTxtLang(langTxt);
        m_Intro.initIntro(langTxt);
    }

    for (int i=0;i<K_NBKEYS;i++) {m_iKey[i]=false;}

}
//Appellé par End.
void CMyEngine::AdditionalEnd()
{

    //Si l'on est dans le menu lorsque le programme quitte.
    switch(m_iPhase)
    {
        case P_MENU:
            m_Menu.deleteMenu();
        break;
        case P_GAME:
            m_Jeu.deleteJeu();
        break;
    }

    TTF_Quit();

}

//Agissements du programme.
void CMyEngine::Think( const int& iElapsedTime )
{
    //tempTest1=st_get_ticks();
    int moveStrafe;

	ProcessTouch(); // EZX: If touched and got coresponding action for it then react
	
    switch(m_iPhase)
    {
        //Ce case est placé en premier car il est celui qui nécessite le plus de rapidité d'execution.
        case P_GAME:

            moveStrafe=0;
            //Dans un premier temps, on réagit par rapport aux inputs.

            //*********************************************************
            //1)Gestion de la raquette
            if (m_iKey[K_LEFT]==true)
            {
                m_Jeu.deplacerRaquette(iElapsedTime, R_GAUCHE);
            }
            else if(m_iKey[K_RIGHT]==true)
            {
                m_Jeu.deplacerRaquette(iElapsedTime, R_DROITE);
            }
            else
                m_Jeu.deplacerRaquette(iElapsedTime, 0);
            //*********************************************************
            //2)Ajouter ici la gestion des inputs, pour le tetris.
            //On vide la figure des briques.
            m_Jeu.flipFigure();
            if (m_iKey[K_Q]==true)
            {
                moveStrafe=-1;
            }
            else if(m_iKey[K_D]==true)
            {
                moveStrafe=1;
            }
            //Va gérer aussi la descente automatique, et la destruction d'éventuelles lignes.
            if (m_Jeu.allMoveFigure(moveStrafe,m_iKey[K_S],m_iKey[K_Z],m_iKey[K_SPACE])==true)
            {
                m_iKey[K_S]=false;
            }
            //On remet la figure :
            m_Jeu.flipFigure();
            //*********************************************************

            //Gestion du mouvement de la balle ET en meme temps des briques cassées, via la meme fonction.
            m_Jeu.deplacerBalle(iElapsedTime);

            if (m_iKey[K_P]==true)
            {
                m_iPhase=P_PAUSE_GAME;
                m_Jeu.pauseJeu(langTxt);
            }

            if (m_iKey[K_ESCAPE]==true)
            {
                m_Jeu.deleteJeu();
                m_iPhase=P_MENU;
                m_Menu.initMenu(langTxt);
                for (int i=0;i<K_NBKEYS;i++) {m_iKey[i]=false;}
            }

            //Rémise a 0 des touches nécessaires (Celle qui sert au changement d'angle).
            m_iKey[K_Z]=false;
            m_iKey[K_SPACE]=false;
            m_iKey[K_P]=false;
            m_iKey[K_ESCAPE]=false;
            //Test si l'on a perdu.
            if (m_Jeu.gameOver()==true)
            {
                m_Jeu.saveLastDisplay(langTxt);
                m_iPhase=P_END_GAME;
                for (int i=0;i<K_NBKEYS;i++) {m_iKey[i]=false;}
                m_Highscore.AjouterScore(m_Jeu.getScore());
                m_Highscore.creerFichierHighscore();
            }

        break;
        case P_INTRO:
            if (m_Intro.calculateFrame()==false || m_iKey[K_ENTER]==true || m_iKey[K_ESCAPE]==true)
            {
                m_Intro.deleteIntro();
                m_iPhase=P_MENU;
                m_Menu.initMenu(langTxt);
                for (int i=0;i<K_NBKEYS;i++) {m_iKey[i]=false;}
            }
        break;
        case P_MENU:
            if (m_iKey[K_UP]==true)
            {
                m_Menu.selectPreviousItem();
            }
            else if (m_iKey[K_DOWN]==true)
            {
                m_Menu.selectNextItem();
            }
            else if (m_iKey[K_RIGHT]==true || m_iKey[K_ENTER]==true || m_iKey[K_LEFT]==true)
            {
                switch(m_Menu.getSelectedItem())
                {
                    case 0:
                        m_Param.saveParam();
                        m_Menu.deleteMenu();
                        m_iPhase=P_NEW_GAME;
                    break;
                    case 1:
                        m_Param.flipMusique();
                    break;
                    case 2:
                        m_Param.flipSon();
                    break;
                    case 3:
                        m_Param.flipFullscreen();
                        if (m_Param.getFullscreen()==0) flipFullScreen(false);
                        else flipFullScreen(true);
                    break;
                    case 4:
                        m_Menu.makeScores(m_Highscore);
                        m_iPhase=P_SCORE;
                    break;
                    case 5:
                        m_Param.saveParam();
                        Quit();
                    break;
                }
            }
            else if (m_iKey[K_ESCAPE]==true)
            {
                m_Param.saveParam();
                Quit();
            }

            for (int i=0;i<K_NBKEYS;i++) {m_iKey[i]=false;}
        break;
        case P_LAUNCHER:

            if (m_iKey[K_UP]==true || m_iKey[K_DOWN]==true)
            {
                m_Launcher.change();

                if (m_Launcher.getStep()==0)
                    m_Param.flipLangue();
                else
                    m_Param.flipClavier();
            }
            else if (m_iKey[K_RIGHT]==true || m_iKey[K_ENTER]==true)
            {

                m_Launcher.nextStep();
                if (m_Launcher.getStep()==2)
                {
                    m_Param.saveParam();
                    m_Launcher.deleteLauncher();
                    m_iPhase=P_INTRO;
                    m_Param.getTxtLang(langTxt);
                    m_Intro.initIntro(langTxt);

                }

            }
            else if (m_iKey[K_ESCAPE]==true)
            {

                Quit();
                m_Param.saveParam();
                m_Launcher.deleteLauncher();
                m_iPhase=P_INTRO;
                m_Param.getTxtLang(langTxt);
                m_Intro.initIntro(langTxt);

            }

            for (int i=0;i<K_NBKEYS;i++) {m_iKey[i]=false;}
        break;
        case P_SCORE:
            if (m_iKey[K_RIGHT]==true || m_iKey[K_ENTER]==true || m_iKey[K_LEFT]==true || m_iKey[K_ESCAPE]==true)
            {
                m_iPhase=P_MENU;
            }
            for (int i=0;i<K_NBKEYS;i++) {m_iKey[i]=false;}
        break;
        case P_NEW_GAME:
            //Cette phase sert a gerer la transition avec le jeu, par exemple avec un écran de chargement.
            //Ou encore un décompte de temps et bloquant.
            bool b_musique,b_sons;
            if(m_Param.getMusique()==0) b_musique=false;
            else b_musique=true;
            if(m_Param.getSon()==0) b_sons=false;
            else b_sons=true;
            m_Jeu.initJeu(b_musique,b_sons,langTxt);

            m_iPhase=P_GAME;
        break;
        case P_END_GAME:
            if (m_iKey[K_ENTER]==true || m_iKey[K_ESCAPE]==true)
            {
                m_Jeu.deleteJeu();
                m_iPhase=P_MENU;
                m_Menu.initMenu(langTxt);
                for (int i=0;i<K_NBKEYS;i++) {m_iKey[i]=false;}
            }
        break;
        case P_PAUSE_GAME:
            if (m_iKey[K_P]==true)
            {
                m_iPhase=P_GAME;
                m_Jeu.reprendreJeu();
                for (int i=0;i<K_NBKEYS;i++) {m_iKey[i]=false;}
            }
            else if (m_iKey[K_ESCAPE]==true)
            {
                m_Jeu.deleteJeu();
                m_iPhase=P_MENU;
                m_Menu.initMenu(langTxt);
                for (int i=0;i<K_NBKEYS;i++) {m_iKey[i]=false;}
            }
        break;
    }

    //tempTest1=st_get_ticks()-tempTest1;

}
//Affichage à partir des données disponibles.
void CMyEngine::Render( SDL_Surface* pDestSurface )
{
    //tempTest2=st_get_ticks();
    switch(m_iPhase)
    {
        case P_GAME:
            m_Jeu.drawJeu(pDestSurface);
        break;
        case P_SCORE:
            m_Menu.drawScores(pDestSurface);
        break;
        case P_INTRO:
            m_Intro.drawIntro(pDestSurface);
        break;
        case P_MENU:
            bool b_musique,b_sons,b_pleinEcran;
            if(m_Param.getMusique()==0) b_musique=false;
            else b_musique=true;
            if(m_Param.getSon()==0) b_sons=false;
            else b_sons=true;
            if(m_Param.getFullscreen()==0) b_pleinEcran=false;
            else b_pleinEcran=true;
            m_Menu.drawMenu(pDestSurface,b_musique,b_sons,b_pleinEcran);
        break;
        case P_LAUNCHER:
            m_Launcher.drawLauncher(pDestSurface);
        break;
        case P_END_GAME:
            m_Jeu.drawGameOver(pDestSurface);
        break;
        case P_PAUSE_GAME:
            m_Jeu.drawPause(pDestSurface);
        break;
    }

    /*tempTest2=st_get_ticks()-tempTest2;
    if (m_iKey[K_ENTER]==true)
    {
        m_iKey[K_ENTER]=false;
        fprintf( stderr, "Think : %ld Render: %ld\n",tempTest1,tempTest2);
    }*/


}
bool EZX_Touched(int x, int y, int w, int h)
{
	if(touched_x>=x && touched_x<=x+w && touched_y>=y && touched_y<=y+h) return true;
	return false;
}
// EZX: Process touch
void CMyEngine::ProcessTouch()
{
	if(touched) {
		switch(m_iPhase)
		{
			case P_GAME:				m_Jeu.deplacerRaquetteByTouch();			break;
			case P_MENU:				
						if(EZX_Touched(59,  29, 202, 22)) { m_Menu.selectItem(0); KeyDown(EZX_MENU_RETURN); } else
						if(EZX_Touched(59,  64, 202, 22)) { m_Menu.selectItem(1); KeyDown(EZX_MENU_RETURN); } else
						if(EZX_Touched(59,  99, 202, 22)) { m_Menu.selectItem(2); KeyDown(EZX_MENU_RETURN); } else
						if(EZX_Touched(59, 134, 202, 22)) { m_Menu.selectItem(3); KeyDown(EZX_MENU_RETURN); } else
						if(EZX_Touched(59, 169, 202, 22)) { m_Menu.selectItem(4); KeyDown(EZX_MENU_RETURN); } else
						if(EZX_Touched(59, 204, 202, 22)) { m_Menu.selectItem(5); KeyDown(EZX_MENU_RETURN); }
			break;
			case P_LAUNCHER:														break;
			case P_SCORE:				KeyDown(EZX_MENU_RETURN);					break;
			case P_INTRO:				KeyDown(EZX_MENU_RETURN);					break;
			case P_END_GAME:			KeyDown(EZX_MENU_RETURN);					break;
			case P_PAUSE_GAME:			KeyDown(EZX_GAME_PAUSE);					break;
		}
		touched=false;
	}
}
void CMyEngine::KeyDown(const int& iKeyEnum)
{
    switch(m_iPhase)
    {
        case P_GAME:
            switch (iKeyEnum)
            {
            case EZX_GAME_BRICKS_LEFT:
                m_iKey[K_LEFT]=true;
            break;
            case EZX_GAME_BRICKS_RIGHT:
                m_iKey[K_RIGHT]=true;
            break;
            case EZX_GAME_TETRIS_LEFT:
                m_iKey[K_Q]=true;
            break;
            case EZX_GAME_TETRIS_RIGHT:
                m_iKey[K_D]=true;
            break;
            case EZX_GAME_TETRIS_DROP:
                m_iKey[K_S]=true;
            break;
			case EZX_GAME_TETRIS_ROTATE:
                m_iKey[K_Z]=true;
            break;
            case EZX_GAME_TETRIS_DROP2:
                m_iKey[K_SPACE]=true;
            break;
            case EZX_GAME_PAUSE:
                m_iKey[K_P]=true;
            break;
            case EZX_GAME_ESCAPE:
                m_iKey[K_ESCAPE]=true;
            break;
            case EZX_GAME_RETURN:
                m_iKey[K_ENTER]=true;
            break;
            }
        break;
        case P_MENU:
            switch (iKeyEnum)
            {
            case EZX_MENU_UP:
                m_iKey[K_UP]=true;
            break;
            case EZX_MENU_DOWN:
                m_iKey[K_DOWN]=true;
            break;
            case EZX_MENU_LEFT:
                m_iKey[K_LEFT]=true;
            break;
            case EZX_MENU_RIGHT:
                m_iKey[K_RIGHT]=true;
            break;
            case EZX_MENU_RETURN:
                m_iKey[K_ENTER]=true;
            break;
            case EZX_MENU_ESCAPE:
                m_iKey[K_ESCAPE]=true;
            break;
            }
        break;
        case P_LAUNCHER:
            switch (iKeyEnum)
            {
            case EZX_MENU_UP:
                m_iKey[K_UP]=true;
            break;
            case EZX_MENU_DOWN:
                m_iKey[K_DOWN]=true;
            break;
            case EZX_MENU_RIGHT:
                m_iKey[K_RIGHT]=true;
            break;
            case EZX_MENU_RETURN:
                m_iKey[K_ENTER]=true;
            break;
            case EZX_MENU_ESCAPE:
                m_iKey[K_ESCAPE]=true;
            break;
            }
        break;
        case P_SCORE:
            switch (iKeyEnum)
            {
            case EZX_MENU_LEFT:
                m_iKey[K_LEFT]=true;
            break;
            case EZX_MENU_RIGHT:
                m_iKey[K_RIGHT]=true;
            break;
            case EZX_MENU_RETURN:
                m_iKey[K_ENTER]=true;
            break;
            case EZX_MENU_ESCAPE:
                m_iKey[K_ESCAPE]=true;
            break;
            }
        case P_INTRO:
            switch (iKeyEnum)
            {
            case EZX_MENU_RETURN:
                m_iKey[K_ENTER]=true;
            break;
            case EZX_MENU_ESCAPE:
                m_iKey[K_ESCAPE]=true;
            break;
            }
        break;
        case P_END_GAME:
            switch (iKeyEnum)
            {
            case EZX_MENU_RETURN:
                m_iKey[K_ENTER]=true;
            break;
            case EZX_MENU_ESCAPE:
                m_iKey[K_ESCAPE]=true;
            break;
            }
        break;
        case P_PAUSE_GAME:
            switch (iKeyEnum)
            {
            case EZX_GAME_PAUSE:
                m_iKey[K_P]=true;
            break;
            case EZX_GAME_ESCAPE:
                m_iKey[K_ESCAPE]=true;
            break;
            }
        break;
    }

}

//Rien encore de définie, car non utilisé par le menu (mais par le jeu).
void CMyEngine::KeyUp (const int& iKeyEnum)
{
    switch(m_iPhase)
    {
        case P_GAME:
            switch (iKeyEnum)
            {
            case EZX_GAME_BRICKS_LEFT:
                m_iKey[K_LEFT]=false;
            break;
            case EZX_GAME_BRICKS_RIGHT:
                m_iKey[K_RIGHT]=false;
            break;
            case EZX_GAME_TETRIS_ROTATE:
                m_iKey[K_S]=false;
            break;
            case EZX_GAME_TETRIS_LEFT:
                m_iKey[K_Q]=false;
            break;
            case EZX_GAME_TETRIS_RIGHT:
                m_iKey[K_D]=false;
            break;
            }
        break;
    }

}

//Ici, on fait une mise en pause du jeu.
//A définir dans le jeu.
void CMyEngine::WindowInactive()
{
    switch(m_iPhase)
    {
        case P_MENU:
        //Pour le menu, ca ne change rien, donc on ne fait rien.
        //A voir éventuellement si animations dans le menu.
        break;
        case P_NEW_GAME:

        break;
        case P_GAME:
            m_iPhase=P_PAUSE_GAME;
            m_Jeu.pauseJeu(langTxt);
        break;
        case P_END_GAME:

        break;
    }
}

