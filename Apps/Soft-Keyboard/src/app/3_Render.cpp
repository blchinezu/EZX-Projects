#ifndef _OnRender_
#define _OnRender_

#include "../main.h"

void OnRender() {
	if(Engine::UpdateAll) Graphics::NeedScreenUpdate=true;
	if(Graphics::NeedScreenUpdate) {
		Engine::AddBackground();
		
		Mouse::Update();
		Timer::FPSRender();
//		SK::img_char=Graphics::LoadImage("tmp", true); Graphics::AddSurfaceCentered(SK::img_char, Graphics::Screen, NULL);
		SK::Render();
		
		Graphics::UpdateScreen();
	}
}

#endif /* _OnRender_ */

