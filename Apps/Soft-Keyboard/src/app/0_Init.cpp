#ifndef _OnInit_
#define _OnInit_

#include "../main.h"

bool OnInit() {
	#ifdef DEBUG
		std::cout<<" > Initializing SDL...\n";
	#endif
	if(SDL_Init(SDL_INIT_VIDEO) < 0) { std::cout<<" >> ERROR: Failed initializing SDL Video.\n"; return false; }
	
	if(!Graphics::Init()) { return false; }
	
	#ifdef DEBUG
		std::cout<<" > Initializing SDL TTF...\n";
	#endif
	if(TTF_Init() < 0) { std::cout<<" >> ERROR: Failed initializing TTF Font.\n"; return false; }
	
	if(!Engine::LoadData()) { std::cout<<" >> ERROR: Failed loading data.\n"; return false; }

	#ifdef EZX	
		Mouse::HideSDLDefault();
	#endif
	Engine::SetWindowTitle();
	
	Engine::ShowSplash();
	
	Engine::AddBackground();
		
	SK::Init();					// TODO: Initiate the keyboard (data loading and preparing)
	SK::SetTitle("Testing SK");	// TODO: Set a title to the keyboard (shown on the top bar)
		
	if(Config::ShowCursor) { Mouse::ResetCoord(); Mouse::Move(); Graphics::NeedScreenUpdate=true; }
	
	
	return true;
}

#endif /* _OnInit_ */
