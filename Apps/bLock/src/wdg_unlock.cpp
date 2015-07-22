#include "wdg_unlock.h"

unLOCK::unLOCK() {
	dragging = false;
	draggingArrow = false;
	returningArrow = false;
	ibg = NULL;
	bkgImage = NULL;
	arrow = NULL;
}

void unLOCK::Init(SDL_Surface* bkg) {
	char pth[MAX_PATH_LEN];
	
	sprintf(pth, "%s/%s", Path_Data, "wdg_unlock/bg.png");
	ibg = gfx.LoadImage(pth);
	sprintf(pth, "%s/%s", Path_Data, "wdg_unlock/arrow.png");
	arrow = gfx.LoadImage(pth);

	relX = (ScreenWidth - ibg->w)/2;
	relY = ScreenHeight-min_visible();
	
	relAX = UNLOCK_ARR_X;
	relAY = UNLOCK_ARR_Y;
	
	bkgImage = bkg;
	
	draw(gfx.Screen);
}

void unLOCK::moveRel(int rX, int rY) {
	move(relX+rX, relY+rY);
}

void unLOCK::move(int x, int y) {
	if (x + ibg->w > ScreenWidth) x = ScreenWidth - ibg->w;
	else if (x < 0) x = 0;
	if (y > ScreenHeight-min_visible()) y = ScreenHeight-min_visible();
	else if (y < ScreenHeight-max_visible()) y = ScreenHeight-max_visible();

	if (y < iclock.relY + iclock.height()) {
		if (relY > iclock.relY + iclock.height())
			y = iclock.relY + iclock.height();
		else
		if	(iclock.relY + iclock.height() == relY &&
			ScreenHeight-relY < max_visible())
			iclock.moveRel(0, y-relY);
	}
	
	if ( relX != x || relY != y) {
	
		int x0 = x, x1 = relX - x;
		if (x1 < 0) { x1 = 0 - x1; x0 = relX; };
		x1 += ibg->w;
		if (x0+x1 > ScreenWidth) x1 = ScreenWidth - x0;
		
		int y0 = y, y1 = relY - y;
		if (y1 < 0) { y1 = 0 - y1; y0 = relY; };
		y1 += ibg->h;
		if (y0+y1 > ScreenHeight) y1 = ScreenHeight - y0;
	
		if(y + ibg->h > ScreenHeight) {
			SDL_Rect clip = gfx.SetClip(relX, relY, ibg->w, ScreenHeight-relY);
			gfx.AddSurface(relX, relY, bkgImage, gfx.Screen, &clip);
		} else {
			SDL_Rect clip = gfx.SetClip(relX, relY, ibg->w, ibg->h);
			gfx.AddSurface(relX, relY, bkgImage, gfx.Screen, &clip);
		}

		relX = x; relY = y;
		
		draw(gfx.Screen, false);
		gfx.Update(gfx.Screen, x0, y0, x1, y1);
	}
	
}

void unLOCK::redraw(SDL_Surface* scr) {
	if(relY+ibg->h > ScreenHeight) {
		SDL_Rect clip = gfx.SetClip(relX, relY, ibg->w, ScreenHeight-relY);
		gfx.AddSurface(relX, relY, bkgImage, scr, &clip);
	} else {
		SDL_Rect clip = gfx.SetClip(relX, relY, ibg->w, ibg->h);
		gfx.AddSurface(relX, relY, bkgImage, scr, &clip);
	}
	draw(scr);
}

void unLOCK::draw(SDL_Surface* scr, bool upd) {
	if(relY+ibg->h > ScreenHeight) {
		SDL_Rect clip = gfx.SetClip(0, 0, ibg->w, ScreenHeight-relY); 
		gfx.AddSurface(relX, relY, ibg, scr, &clip);
	} else {
		gfx.AddSurface(relX, relY, ibg, scr, NULL);
	}
	drawArrow(scr, upd);
	if (upd)
		if(relY+ibg->h > ScreenHeight)
			gfx.Update(scr, relX, relY, ibg->w, ScreenHeight-relY);
		else
			gfx.Update(scr, relX, relY, ibg->w, ibg->h);
}

bool unLOCK::hover(int x, int y) {
	if	(x >= relX && x <= relX+ibg->w && y >= relY && y <= relY+ibg->h)
		return true;
	return false;
}

/* ---------------------------------------------------------------- */

void unLOCK::returnArrowStep() {
	if (relAX == UNLOCK_ARR_X)	returningArrow = false;
	else						moveRelArrow(0-UNLOCK_ARR_RET);
}

void unLOCK::moveRelArrow(int rX) {
	moveArrow(relAX+rX);
}

void unLOCK::moveArrow(int x) {
	if (x > ARR_MAX_REL_X) { x = ARR_MAX_REL_X; app.Running = false; }
	else if (x < UNLOCK_ARR_X)
		x = UNLOCK_ARR_X;
	
	if (x != relAX) {
		/*int x0, x1;
		if (x < relAX) {
			x0 = x;
			x1 = (relAX - x) + arrow->w;
		} else {
			x0 = relAX;
			x1 = (x - relAX) + arrow->w;
		}*/
		
		if(relY+relAY < ScreenHeight) {
			if(ScreenHeight - (relY+relAY) < arrow->h) {
				SDL_Rect clip = gfx.SetClip(relX+relAX, relY+relAY, arrow->w, ScreenHeight - (relY+relAY));
				gfx.AddSurface(relX+relAX, relY+relAY, bkgImage, gfx.Screen, &clip);
				SDL_Rect clip2 = gfx.SetClip(relAX, relAY, arrow->w, ScreenHeight - (relY+relAY));
				gfx.AddSurface(relX+relAX, relY+relAY, ibg, gfx.Screen, &clip2);
			}
			else {
				SDL_Rect clip = gfx.SetClip(relX+relAX, relY+relAY, arrow->w, arrow->h);
				gfx.AddSurface(relX+relAX, relY+relAY, bkgImage, gfx.Screen, &clip);
				SDL_Rect clip2 = gfx.SetClip(relAX, relAY, arrow->w, arrow->h);
				gfx.AddSurface(relX+relAX, relY+relAY, ibg, gfx.Screen, &clip2);
			}
		}
		relAX = x;
		drawArrow(gfx.Screen, false);

		// XXX: This thing is simply fucking with my brain
		// XXX: FullScreen updates run way faster than local minimal updates.
/*		if(relY+relAY < ScreenHeight) {
			if(ScreenHeight - (relY+relAY) < arrow->h) {
				gfx.Update(gfx.Screen, relX+x0, relY+relAY, x1, ScreenHeight-(relY+relAY));
			}
			else {
				gfx.Update(gfx.Screen, relX+x0, relY+relAY, x1, arrow->h);
			}
		}*/
	}
}

void unLOCK::moveAuto(int rY) {
	if ( rY > 0 ) {
		while (ScreenHeight-relY > min_visible()) moveRel(0, 1);
	} else if (rY < 0) {
		while (ScreenHeight-relY < max_visible()) moveRel(0, -1);
	}
}

void unLOCK::redrawArrow(SDL_Surface* scr) {
	if(relY+relAY < ScreenHeight) {
		if(ScreenHeight - (relY+relAY) < arrow->h) {
			SDL_Rect clip = gfx.SetClip(relX+relAX, relY+relAY, arrow->w, ScreenHeight - (relY+relAY));
			gfx.AddSurface(relX+relAX, relY+relAY, bkgImage, scr, &clip);
			SDL_Rect clip2 = gfx.SetClip(relAX, relAY, arrow->w, ScreenHeight - (relY+relAY));
			gfx.AddSurface(relX+relAX, relY+relAY, ibg, scr, &clip2);
		}
		else {
			SDL_Rect clip = gfx.SetClip(relX+relAX, relY+relAY, arrow->w, arrow->h);
			gfx.AddSurface(relX+relAX, relY+relAY, bkgImage, scr, &clip);
			SDL_Rect clip2 = gfx.SetClip(relAX, relAY, arrow->w, arrow->h);
			gfx.AddSurface(relX+relAX, relY+relAY, ibg, scr, &clip2);
		}
		drawArrow(scr);
	}
}

void unLOCK::drawArrow(SDL_Surface* scr, bool upd) {
	#ifdef DEBUG
	upd=false;
	#endif
	if(relY+relAY < ScreenHeight) {
		if(ScreenHeight - (relY+relAY) < arrow->h) {
			SDL_Rect clip = gfx.SetClip(0, 0, arrow->w, ScreenHeight - (relY+relAY));
			gfx.AddSurface(relX+relAX, relY+relAY, arrow, scr, &clip);
//			if (upd)
//				gfx.Update(scr, relX+relAX, relY+relAY, arrow->w, ScreenHeight - (relY+relAY));
		}
		else {
			gfx.AddSurface(relX+relAX, relY+relAY, arrow, scr, NULL);
//			if (upd)
//				gfx.Update(scr, relX+relAX, relY+relAY, arrow->w, arrow->h);
		}
	}
}

bool unLOCK::hoverArrow(int x, int y) {
	if (
		x >= relX+relAX &&
		x <= relX+relAX+arrow->w &&
		y >= relY+relAY &&
		y <= relY+relAY+arrow->h
		)
		return true;
	return false;
}

/* ---------------------------------------------------------------- */

int unLOCK::max_visible() { return ibg->h; }
int unLOCK::min_visible() {
	return UNLOCK_MIN_VIS;
}

unLOCK::~unLOCK() {
	gfx.ReleaseSurface(ibg);
	gfx.ReleaseSurface(arrow);
}

