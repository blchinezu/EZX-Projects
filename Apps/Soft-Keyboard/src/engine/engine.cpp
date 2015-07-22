#include "engine.h"

bool Engine::Running	= true;
bool Engine::UpdateAll	= true;

void Engine::Sleep(int msec) {
	#ifdef DEBUG
		std::cout<<" Sleep("<<msec<<")\n";
	#endif
	SDL_Delay(msec);
}

void Engine::SetWindowTitle() {
	std::string title; title.clear(); title.append(Config::AppName); title.append(" "); title.append(Config::AppVersion);
	SDL_WM_SetCaption( title.c_str(), NULL );
}

bool Engine::LoadData() {
	#ifdef DEBUG
		std::cout<<" > LoadData() {\n";
	#endif
	Graphics::Blank(Graphics::Screen);
	if(!Graphics::LoadData() || !Mouse::LoadData()) return false;
	return true;
}
void Engine::ToggleUpdateAll() {
	UpdateAll=!UpdateAll; std::cout<<"UpdateScreen@EveryFrame = "<<UpdateAll<<"\n";
}
// Show Splash
void Engine::ShowSplash() {
	#ifdef DEBUG
		std::cout<<" ShowSplash("<<Graphics::Screen<<") {\n";
	#endif
	if(Config::ShowSplash) {
		#ifdef DEBUG
			std::cout<<" > Showing splash screen.\n";
		#endif
		std::string text;
		SDL_Surface* surface = NULL;
		Graphics::Blank(Graphics::Screen);
		
		text.clear(); text.append(Config::AppName); text.append(" "); text.append(Config::AppVersion);
		surface = Graphics::Text(Graphics::font_small, text.c_str(), 128, 128, 128);
		Graphics::AddSurfaceBottomRight(surface, Graphics::Screen, NULL); Graphics::ReleaseSurface(surface);
		
		text.clear(); text.append("Example:"); surface = Graphics::Text(Graphics::font_normal, text.c_str(), 150, 150, 150);
		Graphics::AddSurfaceCenteredX(50, surface, Graphics::Screen, NULL); Graphics::ReleaseSurface(surface);
		text.clear(); text.append("SDL Soft-Keyboard"); surface = Graphics::Text(Graphics::font_normal, text.c_str(), 255, 255, 255);
		Graphics::AddSurfaceCenteredX(70, surface, Graphics::Screen, NULL); Graphics::ReleaseSurface(surface);
		
		text.clear(); text.append("Developer:"); surface = Graphics::Text(Graphics::font_normal, text.c_str(), 150, 150, 150);
		Graphics::AddSurfaceCenteredX(120, surface, Graphics::Screen, NULL); Graphics::ReleaseSurface(surface);
		text.clear(); text.append("BruceLee"); surface = Graphics::Text(Graphics::font_normal, text.c_str(), 255, 255, 255);
		Graphics::AddSurfaceCenteredX(140, surface, Graphics::Screen, NULL); Graphics::ReleaseSurface(surface);
		
		int i; for(i=0; i<10; i++) { Graphics::Update(Graphics::Screen); Engine::Sleep(100); }
		Engine::Sleep(2700);
	} else {
		#ifdef DEBUG
			std::cout<<" > Skipping splash screen.\n";
		#endif
	}
	#ifdef DEBUG
		std::cout<<" } // ShowSplash("<<Graphics::Screen<<")\n";
	#endif
}

// AddBackground
void Engine::AddBackground() { Graphics::AddSurface(0, 0, Graphics::background, Graphics::Screen, NULL); }

