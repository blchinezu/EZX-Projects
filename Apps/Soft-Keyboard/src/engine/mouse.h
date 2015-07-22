#ifndef _MOUSE_H_
#define _MOUSE_H_

#include "engine.h"

class Mouse
{
	public:
		Mouse();
		
		static int				mouse_x_r;
		static int				mouse_y_r;
		static int				mouse_x;
		static int				mouse_y;
		static bool				NeedUpdate;
		static SDL_Surface*		cursor;

	public:
			static bool LoadData			();
			static void Cleanup				();
			static void SwitchShowCursor	();
			static void GetCoord			();
			static void GetCoordTo			(int &x, int &y);
			static void SetCoord			(int x, int y);
			static void Move				();
			static void MoveTo				(int x, int y);
			static void ResetCoord			();
			static bool Over				(int x, int y, int w, int h);
			static bool PointOver			(int cx, int cy, int x, int y, int w, int h);
			static void Render				();
			static void RenderTo			(SDL_Surface* destination);
			static void Update				();
			static void HideSDLDefault		();
};

#endif /* _MOUSE_H_ */
