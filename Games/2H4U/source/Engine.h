/*
The CEngine Framework was found on :

http://gpwiki.org/index.php/SDL:Tutorials:Simple_Engine_Framework

It was written by Anders "Sion" Nissen and last updated 14. August 2005.

Content of this file is available under GNU Free Documentation License 1.2. :
http://www.gnu.org/copyleft/fdl.html

We modified the file to make it feet our requirements.
*/

#ifndef ENGINE_H
#define ENGINE_H

#include "sdl_includes.h"

#include <stdlib.h>

extern int touched_x;
extern int touched_y;
extern bool touched;

/** The base engine class. **/
class CEngine
{
private:
	/** Last iteration's tick value **/

	long m_lLastRender;
	long m_lLastThink;

	int m_iFrameLenght; //Durée en millisecondes d'une frame.

	bool m_bFullscreen;

	/** Window width **/
	int m_iWidth;
	/** Window height **/
	int m_iHeight;

	/** Has quit been called? **/
	bool m_bQuit;

	/** The title of the window **/
	const char* m_czTitle;

	/** Screen surface **/
	SDL_Surface* m_pScreen;

	/** Is the window minimized? **/
	bool m_bMinimized;


protected:
	void DoThink();
	void DoRender();

	void SetSize(const int& iWidth, const int& iHeight);
    void SetFullScreen(const int& iWidth, const int& iHeight);
    void flipFullScreen(bool fullscreen);
	void HandleInput();
	void Quit();

public:
	CEngine();
	virtual ~CEngine();

	void Init();
	void Start();
    void End();
	/** OVERLOADED - Data that should be initialized when the application starts. **/
	virtual void AdditionalInit	() {}

    virtual void ParamInit	(int,int) {}

	/** OVERLOADED - All the games calculation and updating.
		@param iElapsedTime The time in milliseconds elapsed since the function was called last.
	**/
	virtual void Think		( const int& iElapsedTime ) {}
	/** OVERLOADED - All the games rendering.
		@param pDestSurface The main screen surface.
	**/
	virtual void Render		( SDL_Surface* pDestSurface ) {}

	/** OVERLOADED - Allocated data that should be cleaned up. **/
	virtual void AdditionalEnd		() {}

	/** OVERLOADED - Window is active again. **/
	virtual void WindowActive	() {}

	/** OVERLOADED - Window is inactive. **/
	virtual void WindowInactive	() {}


	/** OVERLOADED - Keyboard key has been released.
		@param iKeyEnum The key number.
	**/
	virtual void KeyUp		(const int& iKeyEnum) {}

	/** OVERLOADED - Keyboard key has been pressed.
		@param iKeyEnum The key number.
	**/
	virtual void KeyDown		(const int& iKeyEnum) {}
	
	void SetTitle (const char* czTitle);
	const char* 	GetTitle	();
	void SetIcon(const char* czChemin);

	SDL_Surface* 	GetSurface	();

};

#endif // ENGINE_H
