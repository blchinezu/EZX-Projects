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
#include "jeu.h"
#include "hide_a1200.h"

CJeu::CJeu()
{
    //Tableau de balles.
    myBalle = new CBalle [MAX_BALLES];
}

void CJeu::initJeu(bool musiqueOn, bool sonsOn, char lang[MAX_LANG][256])
{

    SDL_Surface* surfTemp,* surfTemp2;

    myTerrain.initTerrain(lang);

    myRaquette.initRaquette(120,232,1,0.7);
    surfTemp=IMG_Load("data/images/jeu/balle.png");
    surfTemp2=IMG_Load("data/images/jeu/balle2.png");

    m_pImgMasque=IMG_Load("data/images/jeu/masque.png");

    for (int i=0;i<MAX_BALLES;i++)
    {
        myBalle[i].initBalle(1,surfTemp,surfTemp2);
    }

    m_iNbLignesDetruites=0;
    m_iScore=0;
    m_iNiveau=0;

    //Autres params.
    m_iNbVies=5;
    m_iNbBalles=0; //Permet notamment de savoir quand on a perdu.
    setDureeAttente();
    m_iLastDescente=st_get_ticks();
    m_iDureeStrafe=100;
    m_iLastStrafe=st_get_ticks();
    m_iDureeChute=40;
    m_iLastChute=st_get_ticks();

    playing=true; // variable permettant de déterminer si le joueur n'a pas perdu.

    modifRaquette=-1;

    //Temps pause va augmenter a chaque fois qu'on fait une pause.
    m_iTempsPause=0;
    m_iTimerPause=0;

    //Gestion temporaire, pour tester.

    m_pScore=NULL;
    m_pNiveau=NULL;
    m_pSuivant=NULL;
    m_pLignes=NULL;

    m_pScreenshot=SDL_CreateRGBSurface(SDL_HWSURFACE, 320, 240, 32, 0, 0, 0, 0);
    m_pSpriteOver=NULL;

    //Chargement de la police d'écriture des chiffres :
    policeChiffres = TTF_OpenFont("data/DS-DIGIB.TTF", 24);

    makeChiffre(m_pScore,m_iScore);

    makeChiffre(m_pNiveau,m_iNiveau);

    makeChiffre(m_pSuivant,5); //Ici, ne compte que pour le premier niveau.

    makeChiffre(m_pLignes,m_iNbLignesDetruites);

    mySon.initSon(musiqueOn,sonsOn);

    myBonus.initBonus();

}

void CJeu::pauseJeu(char lang[MAX_LANG][256])
{

    m_iTimerPause=st_get_ticks();
    drawJeu(m_pScreenshot);
    if (m_pSpriteOver!=NULL) SDL_FreeSurface(m_pSpriteOver);

    SDL_Surface* temp=IMG_Load("data/images/jeu/transparent.png");
    m_pSpriteOver=SDL_DisplayFormat(temp);
    SDL_FreeSurface(temp);

    TTF_Font *policeTxt = TTF_OpenFont("data/bgothm.ttf", 32);
    SDL_Color couleurTxt = { 250, 130, 0 };
    SDL_Surface* tempTxt=NULL;
    SDL_Rect centrage;

    int posY[2]={50,160};

    for (int i=0;i<2;i++)
    {
        if (tempTxt!=NULL)
        {
            SDL_FreeSurface(tempTxt);
        }
        tempTxt = TTF_RenderText_Blended(policeTxt, lang[i+17], couleurTxt);

        centrage.x=105-(tempTxt->w/2);
        centrage.y=posY[i];
        centrage.w=tempTxt->w;
        centrage.h=tempTxt->h;
        SDL_BlitSurface(tempTxt,NULL,m_pSpriteOver,&centrage);
    }
    TTF_CloseFont(policeTxt);

    affiche=true;
    m_iLastAffiche=st_get_ticks();
    m_iDureeAffiche=1000;

    mySon.pauseMusic();


}

void CJeu::drawPause(SDL_Surface* destination)
{
    long tempsActuel=st_get_ticks();
    //On affiche le fond.
    SDL_BlitSurface(m_pScreenshot,NULL,destination,NULL);
    if (affiche==true)
    {

        SDL_BlitSurface(m_pSpriteOver,NULL,destination,NULL);
        if (tempsActuel-m_iLastAffiche>=m_iDureeAffiche)
        {
            affiche=false;
            m_iLastAffiche=tempsActuel;
        }

    }
    else
    {
        if (tempsActuel-m_iLastAffiche>=m_iDureeAffiche/2)
        {
            affiche=true;
            m_iLastAffiche=tempsActuel;
        }
    }
}

void CJeu::reprendreJeu()
{
    long dureePause=(st_get_ticks()-m_iTimerPause);
    m_iTempsPause+=dureePause;
    m_iLastStrafe+=dureePause;
    m_iLastDescente+=dureePause;
    m_iLastChute+=dureePause;

    m_iTimerPause=0;

    mySon.resumeMusic();
}

//Fonction qui permet de gérer la durée entre deux descentes puor une briques
void CJeu::setDureeAttente()
{
    m_iDureeAttente=(1000-m_iNiveau*75);
    if (m_iDureeAttente<175) m_iDureeAttente=175; //175 est pas mal. a 100, c'est trop chaud.
}

double CJeu::getVitesseBalle()
{
    double retour;
    retour=(0.10 + m_iNiveau*0.025);
    if (retour>1.0) retour=1.0;
    return retour;
}

int CJeu::getScore()
{
    return m_iScore;
}

//Fais passer le jeu au niveau suivant.
//Ajouter ici des sons.
void CJeu::niveauSuivant()
{
    int i;
    addToScore(SC_NIVEAU);
    m_iNiveau+=1;
    makeChiffre(m_pNiveau,m_iNiveau);

    setDureeAttente();


    m_iNbVies+=2;
    if (m_iNbVies>MAX_VIES) m_iNbVies=MAX_VIES;

    for (int i=0;i<MAX_BALLES;i++)
    {
        if (myBalle[i].getStatutBalle()==1)
        {
            myBalle[i].setSpeed(getVitesseBalle());
        }
    }
    i=0;
    while (myBalle[i].getStatutBalle()==1)
    {
        i++;
    }

    if (m_iNbBalles<MAX_BALLES)
    {
        myBalle[i].startBalle(myRaquette.GetRect()->x + myRaquette.GetRect()->w/2,myRaquette.GetRect()->y-myBalle[0].GetRect()->h,getVitesseBalle());
        m_iNbBalles++;
    }

    mySon.playEffet(S_NIVEAU);

}

bool CJeu::gameOver()
{
    return !playing;
}

//Permet d'enregistrer la dernière image du jeu, pour lorsque l'on a perdu.
void CJeu::saveLastDisplay(char lang[MAX_LANG][256])
{
    mySon.playEffet(S_GAME_OVER);
    drawJeu(m_pScreenshot);
    if (m_pSpriteOver!=NULL) SDL_FreeSurface(m_pSpriteOver);

    SDL_Surface* temp=IMG_Load("data/images/jeu/transparent.png");
    m_pSpriteOver=SDL_DisplayFormat(temp);
    SDL_FreeSurface(temp);

    TTF_Font *policeTxt = TTF_OpenFont("data/bgothm.ttf", 32);
    SDL_Color couleurTxt = { 250, 130, 0 };

    SDL_Surface* tempTxt=NULL;
    SDL_Rect centrage;

    int posY[3]={50,160,184};

    for (int i=0;i<3;i++)
    {
        if (tempTxt!=NULL)
        {
            SDL_FreeSurface(tempTxt);
        }
        tempTxt = TTF_RenderText_Blended(policeTxt, lang[i+14], couleurTxt);

        centrage.x=105-(tempTxt->w/2);
        centrage.y=posY[i];
        centrage.w=tempTxt->w;
        centrage.h=tempTxt->h;
        SDL_BlitSurface(tempTxt,NULL,m_pSpriteOver,&centrage);
    }
    TTF_CloseFont(policeTxt);

    m_iLastAffiche=st_get_ticks();
    m_iDureeAffiche=1000;
    affiche=true;
}

void CJeu::drawGameOver(SDL_Surface* destination)
{
    mySon.pauseMusic();
    long tempsActuel=st_get_ticks();
    //On affiche le fond.
    SDL_BlitSurface(m_pScreenshot,NULL,destination,NULL);
    if (affiche==true)
    {
        SDL_BlitSurface(m_pSpriteOver,NULL,destination,NULL);
        if (tempsActuel-m_iLastAffiche>=m_iDureeAffiche)
        {
            affiche=false;
            m_iLastAffiche=tempsActuel;
        }
    }
    else
    {
        if (tempsActuel-m_iLastAffiche>=m_iDureeAffiche/2)
        {
            affiche=true;
            m_iLastAffiche=tempsActuel;
        }
    }
}

void CJeu::deleteJeu()
{
    SDL_FreeSurface(myBalle[0].getSurface());
    if (m_pScreenshot!=NULL) SDL_FreeSurface(m_pScreenshot);
    if (m_pSpriteOver!=NULL) SDL_FreeSurface(m_pSpriteOver);
    for (int i=0;i<MAX_BALLES;i++)
    {
        myBalle[i].deleteBalle();
    }
    myTerrain.deleteTerrain();

    TTF_CloseFont(policeChiffres);

    SDL_FreeSurface(m_pScore);
    SDL_FreeSurface(m_pNiveau);
    SDL_FreeSurface(m_pSuivant); //Ici, ne compte que pour le premier niveau.
    SDL_FreeSurface(m_pLignes);

    mySon.deleteSon();

}

//Gère le jeu lorsque des lignes sont détruites.
//A gérer mieux ca va pas du tout !!
void CJeu::addLignes(int nbLignes)
{
    int suivant;
    m_iNbLignesDetruites+=nbLignes;
    makeChiffre(m_pLignes,m_iNbLignesDetruites);

    if (m_iNbLignesDetruites>=5 + (m_iNiveau)*15)
    {
        niveauSuivant();
    }
    if (m_iNiveau==0) suivant=5-m_iNbLignesDetruites;
    else
    {
        suivant=15  - (m_iNbLignesDetruites - (5 + (m_iNiveau-1)*15));
    }

    makeChiffre(m_pSuivant,suivant);
}

//déplacements de la figure.
bool CJeu::allMoveFigure(int moveStrafe,bool bas,bool rotation, bool down)
{

    long tempsActuel;
    int nbLignesFull;
    bool testDescente;
    int nbDescentes;
    bool nouvelleFigure=false;

    tempsActuel=st_get_ticks();
    if (moveStrafe!=0)
    {
        if ((tempsActuel-m_iLastStrafe)>=2*m_iDureeStrafe)
        {
            m_iLastStrafe=tempsActuel+2*m_iDureeStrafe;
            if (myTerrain.moveFigure(moveStrafe,0)==true)
                mySon.playEffet(S_STRAFE);
        }
        else if ((tempsActuel-m_iLastStrafe)>=m_iDureeStrafe)
        {
            m_iLastStrafe=tempsActuel;
            if (myTerrain.moveFigure(moveStrafe,0)==true)
                mySon.playEffet(S_STRAFE);
        }
    }

    if (down==true)
    {
        nbDescentes=0;
        while (myTerrain.moveFigure(0,1)==true)
        {
            nbDescentes++;
        }
        addToScore(SC_DESCENTE*nbDescentes);
        //On force le test de descente d'une ligne, qui fait poser la pièce.
        m_iLastDescente=tempsActuel-m_iDureeAttente;
    }
    else
    {
        if (bas==true && ((tempsActuel-m_iLastChute)>=m_iDureeChute))
        {
            m_iLastChute=tempsActuel;
            testDescente=myTerrain.moveFigure(0,1);
            if (testDescente==false)
		{
			m_iLastDescente=tempsActuel-m_iDureeAttente;
		}
        }
    }

    if (rotation==true)
    {
        if (myTerrain.rotationFigure()==true)
            mySon.playEffet(S_ROTATION);
    }

    if (tempsActuel-m_iLastDescente>=m_iDureeAttente)
    {
        m_iLastDescente=tempsActuel;
        if (myTerrain.moveFigure(0,1)==false)
        {
            nouvelleFigure=true;
            mySon.playEffet(S_POSE_BRIQUE);
            if (myTerrain.dropFigure()==true)
            {
                nbLignesFull=myTerrain.findFullLine();
                myTerrain.findEmptyLine(); //Ne fais pas de points, car les lignes la ont été cassées en vol par le tetris.
                //Jouer eventuellement un son.
                switch (nbLignesFull)
                {
                    case 0:break;
                    case 1:
                        addToScore(SC_LIGNE_1);
                        mySon.playEffet(S_CASSE_LIGNE);
                    break;
                    case 2:
                        addToScore(SC_LIGNE_2);
                        mySon.playEffet(S_CASSE_LIGNE);
                    break;
                    case 3:
                        addToScore(SC_LIGNE_3);
                        mySon.playEffet(S_CASSE_LIGNE);
                    break;
                    case 4:
                        addToScore(SC_LIGNE_4);
                        mySon.playEffet(S_CASSE_LIGNE);
                    break;
                }
                addLignes(nbLignesFull);

                //Vérifions maintenant si le terrain est vide.
                //Si c'est le cas, on va le remplir un peu pour rendre le jeu plus marant.
                if (myTerrain.isEmptyTerrain()==true)
                {
                    myTerrain.rndFillTerrain(4);
                    addToScore(SC_EMPTY_TERRAIN);
                }
            }
            else
            {
                playing=false;
            }
        }
    }

    return nouvelleFigure;
}

void CJeu::deplacerRaquette(const int& iElapsedTime, const int& sens)
{
    //On déplace la raquette.
    if (sens!=0) myRaquette.deplacer(iElapsedTime,sens,myTerrain.GetRect());
    if (modifRaquette!=-1 && modifRaquette+10000<st_get_ticks())
    {
        modifRaquette=-1;
        myRaquette.changerLargeur(3, myTerrain.GetRect());
    }
}

// EZX: call for the move function
void CJeu::deplacerRaquetteByTouch()
{
    //On déplace la raquette.
    myRaquette.moveto(touched_x, myTerrain.GetRect());
}

//Permet de remettre ou d'enlever la figure.
void CJeu::flipFigure()
{

    myTerrain.permuterFigure();

}

//La gestion de la balle doit prendre en compte les différents éléments du terrain... Pas facile facile...
//Il faut de plus détruire les briques touchées.
void CJeu::deplacerBalle(const int& iElapsedTime)
{
    int nbCaseDetruites;
    SDL_Rect* raqRect = myRaquette.GetRect();
    int raqMidX=raqRect->x + raqRect->w/2;
    //Ajouter ici éventuellement une boucle pour gérer plusieurs balles.

    //Il faut gérer les collisions avec les briques, les murs, et la perte des balles.
    //Tous ceci se fait dans la fonction de déplacement de la balle.
    //On lui enverra en paramètre le terrain, pour qu'elle puisse procéder a ses test.
    for (int i=0;i<MAX_BALLES;i++)
    {
        if (myBalle[i].getStatutBalle()==1)
        {
            //Indique que la balle a été détruite.
            if (myBalle[i].deplacer(iElapsedTime,myTerrain,raqRect,mySon,myBonus)==true)
            {
                m_iNbBalles--;
                mySon.playEffet(S_PERDU_BALLE);
            }
            //Indique les destructions que la balle a causée.
            if (myTerrain.getBalleDestruction(nbCaseDetruites)==true)
            {
                mySon.playEffet(S_CASSE_LIGNE);
                addToScore(SC_LIGNE_BALLE);
                addLignes(1);
            }
            if (nbCaseDetruites!=0) {mySon.playEffet(S_CASSE_BRIQUE);}
            addToScore(nbCaseDetruites*SC_CASE_CASSEE);
        }
    }
    //On remet en jeu une nouvelle balle.
    //On baisse les vies.
    //+ animations ?
    if (m_iNbBalles==0 && m_iNiveau!=0)
    {

        m_iNbVies--;

        //Game Over !!!!!
        if (m_iNbVies==-1)
        {
            playing=false;
        }
        else
        {
            m_iNbBalles++;
            myBalle[0].startBalle(raqMidX,raqRect->y-myBalle[0].GetRect()->h,getVitesseBalle());
        }
    }

    //Gestion des bonus est faites en même temps que la gestion des balles.
    int bonusObtenu[MAX_BONUS];
    int nbBonus=myBonus.deplacerBonus(bonusObtenu, iElapsedTime, raqRect);
    if (nbBonus>0)
    {
        mySon.playEffet(S_GAGNE_BONUS);
        for (int i=0;i<nbBonus;i++)
        {
            addToScore(SC_BONUS);
            switch (bonusObtenu[i])
            {
                case B_ADD_BALLE:
                    if (m_iNbBalles<MAX_BALLES)
                    {
                        int i=0;
                        while (myBalle[i].getStatutBalle()==1)
                        {
                            i++;
                        }
                        myBalle[i].startBalle(raqMidX,raqRect->y-myBonus.GetSurface()->h,getVitesseBalle());
                        m_iNbBalles++;
                    }
                break;
                case B_ACC_BALLE:
                    for (int i=0;i<MAX_BALLES;i++)
                    {
                        if (myBalle[i].getStatutBalle()==1)
                        {
                            myBalle[i].setSpeed(getVitesseBalle()+0.05);
                        }
                    }
                break;
                case B_RAL_BALLE:
                    for (int i=0;i<MAX_BALLES;i++)
                    {
                        if (myBalle[i].getStatutBalle()==1)
                        {
                            double temp=getVitesseBalle()-0.2;
                            if (temp<0.1) temp=0.1;
                            myBalle[i].setSpeed(temp);
                        }
                    }
                break;
                case B_ADD_SCORE:
                    addToScore(SC_BONUS_SCORE);
                    break;
                case B_ADD_LIFE:
                    if (m_iNbVies<MAX_VIES) m_iNbVies++;
                break;
                case B_DELETE_LINE:
                    flipFigure();
                        //Supprime deux demi lignes.
                        myTerrain.deleteLine(NBCASEY-1,false);
                        myTerrain.deleteLine(NBCASEY-1,true);
                    flipFigure();
                break;
                case B_BLOQUE_STRAFE:
                    //On bloque la figure latéralement pendant 10 secondes.
                    m_iLastStrafe+=10000;
                break;
                case B_SMALL_RAQ:
                    myRaquette.changerLargeur(1, myTerrain.GetRect());
                    modifRaquette=st_get_ticks();
                break;
                case B_LARGE_RAQ:
                    myRaquette.changerLargeur(4, myTerrain.GetRect());
                    modifRaquette=st_get_ticks();
                break;
                case B_EXTRA_LARGE_RAQ:
                    myRaquette.changerLargeur(10, myTerrain.GetRect());
                    modifRaquette=st_get_ticks();
                break;
                case B_EXTRA_SMALL_RAQ:
                    myRaquette.changerLargeur(0, myTerrain.GetRect());
                    modifRaquette=st_get_ticks();
                break;
            }
        }
    }

}

void CJeu::makeChiffre(SDL_Surface* &destination,int chiffre)
{

    SDL_Color couleurChiffres = { 255, 50, 0 };
    char buffer[256];

    sprintf(buffer,"%d",chiffre);

    if (destination!=NULL)
    {
        SDL_FreeSurface(destination);
    }

    destination = TTF_RenderText_Blended(policeChiffres, buffer, couleurChiffres);

}

void CJeu::drawJeu(SDL_Surface* destination)
{

    int demi;

    SDL_Rect posTemp;

    myTerrain.drawTerrain(destination);
    myBonus.drawBonus(destination);
    myRaquette.drawRaquette(destination);
    for (int i=0;i<MAX_BALLES;i++)
        if (myBalle[i].getStatutBalle()==1){myBalle[i].drawBalle(destination);}

    //Dessinons le nombre de vies restantes !
    if (m_iNbVies>5) demi=5;
    else demi=m_iNbVies;

    posTemp.w=myBalle[0].getSurface()->w;
    posTemp.h=myBalle[0].getSurface()->h;

    posTemp.x=232;
    posTemp.y=194;

    for(int i=0;i<demi;i++)
    {
        SDL_BlitSurface(myBalle[0].getSurface(),NULL,destination,&posTemp);
        posTemp.x+=14;
    }

    if (m_iNbVies>5)
    {
        posTemp.x=232;
        posTemp.y=208;
        for(int i=5;i<m_iNbVies;i++)
        {
            SDL_BlitSurface(myBalle[0].getSurface(),NULL,destination,&posTemp);
            posTemp.x+=14;
        }
    }

    posTemp.w=m_pScore->w;
    posTemp.h=m_pScore->h;
    posTemp.x=264 - m_pScore->w/2;
    posTemp.y=85;

    SDL_BlitSurface(m_pScore,NULL,destination,&posTemp);

    posTemp.w=m_pNiveau->w;
    posTemp.h=m_pNiveau->h;
    posTemp.x=264 - m_pNiveau->w/2;
    posTemp.y=104;
    SDL_BlitSurface(m_pNiveau,NULL,destination,&posTemp);

    posTemp.w=m_pSuivant->w;
    posTemp.h=m_pSuivant->h;
    posTemp.x=264 - m_pSuivant->w/2;
    posTemp.y=124;
    SDL_BlitSurface(m_pSuivant,NULL,destination,&posTemp);

    posTemp.w=m_pLignes->w;
    posTemp.h=m_pLignes->h;
    posTemp.x=264 - m_pLignes->w/2;
    posTemp.y=144;
    SDL_BlitSurface(m_pLignes,NULL,destination,&posTemp);


    if (m_iNiveau==0) SDL_BlitSurface(m_pImgMasque,NULL,destination,NULL);

}

void CJeu::addToScore(int valeur)
{
    m_iScore+=valeur;
    makeChiffre(m_pScore,m_iScore);
}

/////////////////////////////////////////////////////////////////////////////////////////////

CRaquette::CRaquette(){}


void CRaquette::initRaquette(int posX,int posY,int prop,double adX)
{
    properties=prop;
    m_pRaqGauche = IMG_Load("data/images/jeu/raqGauche.png");
    m_pRaqCentre = IMG_Load("data/images/jeu/raqCentre.png");
    m_pRaqDroite = IMG_Load("data/images/jeu/raqDroite.png");
    myRectRaquette.x = posX;
    myRectRaquette.y = posY;
    largeur=3;
    myRectRaquette.w = m_pRaqGauche -> w + largeur*m_pRaqCentre -> w + m_pRaqDroite -> w;
    myRectRaquette.h = m_pRaqGauche -> h;
    dX=adX;
}

//Gère les déplacement de raquette.
void CRaquette::deplacer(const int& iElapsedTime, const int& sens, SDL_Rect *terrainRect)
{
    int x=myRectRaquette.x;
    x+=(int)((((double)(sens*iElapsedTime))*dX))/4;

    int maxX=(terrainRect->x+terrainRect->w)-myRectRaquette.w;
    if (x<terrainRect->x) x=terrainRect->x;
    else if (x>maxX) x=maxX ;
    myRectRaquette.x = x;
}

// EZX: Move it by touch (direct positioning, not relative to previous position)
void CRaquette::moveto(int x, SDL_Rect *terrainRect)
{
	x-=myRectRaquette.w/2;
    int maxX=(terrainRect->x+terrainRect->w)-myRectRaquette.w;
    if (x<terrainRect->x) x=terrainRect->x;
    else if (x>maxX) x=maxX ;
    myRectRaquette.x = x;
    touched=false;
}

SDL_Rect* CRaquette::GetRect(){return &myRectRaquette;}

void CRaquette::drawRaquette(SDL_Surface* destination)
{
    SDL_Rect tempPos;
    tempPos.y=myRectRaquette.y;

    tempPos.x=myRectRaquette.x;
    SDL_BlitSurface(m_pRaqGauche,NULL,destination,&tempPos);

    for (int i=0;i<largeur;i++)
    {
        tempPos.x=myRectRaquette.x+m_pRaqGauche -> w + m_pRaqCentre -> w*i;
        SDL_BlitSurface(m_pRaqCentre,NULL,destination,&tempPos);
    }
    tempPos.x=myRectRaquette.x+m_pRaqGauche -> w + m_pRaqCentre -> w*largeur;
    SDL_BlitSurface(m_pRaqDroite,NULL,destination,&tempPos);
}

void CRaquette::changerLargeur(int newLargeur, SDL_Rect *terrainRect)
{
    largeur=newLargeur;
    myRectRaquette.w = m_pRaqGauche -> w + largeur*m_pRaqCentre -> w + m_pRaqDroite -> w;
    int maxX=(terrainRect->x+terrainRect->w)-myRectRaquette.w;
    if (myRectRaquette.x>maxX) myRectRaquette.x=maxX ;
}

void CRaquette::deleteRaquette()
{
    SDL_FreeSurface(m_pRaqGauche);
    SDL_FreeSurface(m_pRaqCentre);
    SDL_FreeSurface(m_pRaqDroite);
}


