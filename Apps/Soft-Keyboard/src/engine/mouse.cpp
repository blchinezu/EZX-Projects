#include "mouse.h"

Mouse::Mouse() { }

int				Mouse::mouse_x_r	= Config::ScreenWidth/2;
int				Mouse::mouse_y_r	= Config::ScreenHeight/2;
int				Mouse::mouse_x		= mouse_y_r;
int				Mouse::mouse_y		= mouse_y_r;
SDL_Surface*	Mouse::cursor		= NULL;
bool			Mouse::NeedUpdate	= true;


bool Mouse::LoadData() {
	#ifdef DEBUG
		std::cout<<"   > Loading Mouse Data\n";
	#endif
	cursor = Graphics::LoadImage("cursor", true); if(!cursor) { return false; } else { return true; }
}
void Mouse::Cleanup() {
	#ifdef DEBUG
		std::cout<<"   > Releasing Mouse Data\n";
	#endif
	Graphics::ReleaseSurface(cursor);
}

void Mouse::SwitchShowCursor() { Config::ShowCursor=!Config::ShowCursor; Graphics::NeedScreenUpdate=true; }
void Mouse::GetCoord() { GetCoordTo(mouse_x, mouse_y); }
void Mouse::GetCoordTo(int &x, int &y) { SDL_PumpEvents(); SDL_GetMouseState(&x, &y); }
void Mouse::SetCoord(int x, int y) { mouse_x=x; mouse_y=y; }
void Mouse::Move() { SDL_WarpMouse(mouse_x, mouse_y); }
void Mouse::MoveTo(int x, int y) { SetCoord(x, y); Move(); }
void Mouse::ResetCoord() { SetCoord(mouse_x_r, mouse_y_r); }
bool Mouse::Over(int x, int y, int w, int h) { if((mouse_x<x) || (mouse_x>(x+w)) || (mouse_y<y) || (mouse_y>(y+h))) return false; return true; }
bool Mouse::PointOver(int cx, int cy, int x, int y, int w, int h) {	if((cx<x) || (cx>(x+w)) || (cy<y) || (cy>(y+h))) return false; return true; }
void Mouse::Render() { RenderTo(Graphics::Screen); Graphics::NeedScreenUpdate=true; }
void Mouse::RenderTo(SDL_Surface* destination) {
	if(Config::ShowCursor) {
		Graphics::AddSurface(mouse_x, mouse_y, cursor, destination, NULL);
		Graphics::NeedScreenUpdate=true;
	}
//	#ifdef DEBUG
		std::ostringstream mstatus; mstatus<<mouse_x<<"x"<<mouse_y;
		SDL_Surface* text=Graphics::Text(Graphics::font_normal, mstatus.str(), 255, 255, 255);
		Graphics::AddSurface(Config::ScreenWidth-text->w, 25, text, destination, NULL);
		Graphics::ReleaseSurface(text);
//	#endif
}
void Mouse::Update() {
	if(NeedUpdate) {
		int cx, cy;
		GetCoordTo(cx, cy);
		if(cx!=mouse_x || cy!=mouse_y) SetCoord(cx, cy);
	}
	if(Config::ShowCursor) Render();
}
void Mouse::HideSDLDefault() {
	SDL_ShowCursor(SDL_DISABLE);
}
