#include "wdg_info.h"

INFO_PANEL::INFO_PANEL() {
	ibg = bkgImage = 
	button[0] = button[1] =
	status_surf = NULL;
		
	font = NULL;
		
	connected = charging =
	statusX = statusY = 
	buttonX = buttonY = 
	relX = relY = 0;
		
	button_clicked = dragging = false;
}

void INFO_PANEL::Init(SDL_Surface* bkg) {
	char pth[MAX_PATH_LEN];
	
	sprintf(pth, "%s/%s", Path_Data, "wdg_info/bg.png");
	ibg = gfx.LoadImage(pth);
	sprintf(pth, "%s/%s", Path_Data, "wdg_info/but.png");
	button[0] = gfx.LoadImage(pth);
	sprintf(pth, "%s/%s", Path_Data, "wdg_info/butp.png");
	button[1] = gfx.LoadImage(pth);

	relX = (ScreenWidth - ibg->w)/2;
	relY = 0-(max_visible()-min_visible());
	
	bkgImage = bkg;
	
	sprintf(pth, "%s", (const char*)FONT_PATH);
	font = gfx.LoadFont(pth,FONT_SMALL);
	
	sprintf(status_char, " ");
	status_surf = gfx.Text(font, status_char, FONT_COLOR_R, FONT_COLOR_G, FONT_COLOR_B);
	
	statusX = (ibg->w - status_surf->w) / 2;
	statusY = INFO_STAT_Y;
	
	buttonX = (ibg->w - button[0]->w) / 2;
	buttonY = INFO_BUT_Y;
	
	draw(gfx.Screen);
	updateData();
}

void INFO_PANEL::updateData() {
	char ip[15];
	bool up = false;
	ezx.usb_status(usb_mode, ip, &connected, &charging);
//	PowerStatus pstat;

	if ( strcmp(usb_mode,"None") ) {
		char tmp[MAX_NAME_LEN];
		sprintf(tmp,"USB Connection: %s",usb_mode);
		if ( strcmp(status_char,tmp)) {
			sprintf(status_char,"USB Connection: %s",usb_mode);
			up = true;
		}
	} else if ( charging != 0 ) {
		if ( strcmp(status_char,"CHARGING")) {
			sprintf(status_char,"CHARGING");
			up = true;
		}
	} else if ( strcmp(status_char,"Phone Locked") ) {
		sprintf(status_char,"Phone Locked");
		up = true;
	}
	
	if (up) {
		gfx.ReleaseSurface(status_surf);
		status_surf = gfx.Text(font, status_char, FONT_COLOR_R, FONT_COLOR_G, FONT_COLOR_B);
		statusX = (ibg->w - status_surf->w) / 2;
		redraw(gfx.Screen);
	}
}

void INFO_PANEL::moveRel(int rX, int rY) {
	move(relX+rX, relY+rY);
}

void INFO_PANEL::move(int x, int y) {
	if (x + ibg->w > ScreenWidth) x = ScreenWidth - ibg->w;
	else if (x < 0) x = 0;
	if (y > 0) y = 0;
	else if (y < 0-(max_visible()-min_visible())) {
		y = 0-(max_visible()-min_visible());
	}

	// Push clock
	if (y + ibg->h > iclock.relY) {
		if (relY + ibg->h > iclock.relY)
			y = iclock.relY - ibg->h;
		else
		if	(iclock.relY == relY + ibg->h && relY < 0)
			iclock.moveRel(0, y-relY);
	}

	if ( relX != x || relY != y) {
	
		int x0 = x, x1 = relX - x;
		if (x1 < 0) { x1 = 0 - x1; x0 = relX; };
		x1 += ibg->w;
		if (x0+x1 > ScreenWidth) x1 = ScreenWidth - x0;
		
		int y1;
		if (y < relY)	y1 = ibg->h+relY;
		else 			y1 = ibg->h+y;
			
		if(y < 0) {
			SDL_Rect clip = gfx.SetClip(relX, 0, ibg->w, ibg->h+relY);
			gfx.AddSurface(relX, 0, bkgImage, gfx.Screen, &clip);
		} else {
			SDL_Rect clip = gfx.SetClip(relX, relY, ibg->w, ibg->h);
			gfx.AddSurface(relX, relY, bkgImage, gfx.Screen, &clip);
		}

		relX = x; relY = y;
		
		draw(gfx.Screen, false);
		gfx.Update(gfx.Screen, x0, 0, x1, y1);
	}
	
}

void INFO_PANEL::moveAuto(int rY) {
	if ( rY > 0 ) {
		while (relY < 0) moveRel(0, 1);
	} else if (rY < 0) {
		while (relY+ibg->h > min_visible()) moveRel(0, -1);
	}
}

void INFO_PANEL::redraw(SDL_Surface* scr) {
	if(relY < 0) {
		SDL_Rect clip = gfx.SetClip(relX, 0, ibg->w, ibg->h+relY);
		gfx.AddSurface(relX, 0, bkgImage, scr, &clip);
	} else {
		SDL_Rect clip = gfx.SetClip(relX, 0, ibg->w, ibg->h);
		gfx.AddSurface(relX, relY, bkgImage, scr, &clip);
	}
	draw(scr);
}

void INFO_PANEL::draw(SDL_Surface* scr, bool upd) {
	if(relY < 0) {
		SDL_Rect clip = gfx.SetClip(0, 0-relY, ibg->w, ibg->h+relY); 
		gfx.AddSurface(relX, 0, ibg, scr, &clip);
	} else {
		gfx.AddSurface(relX, relY, ibg, scr, NULL);
	}
	draw_status(scr, false);
	draw_button(scr, false, false);
	if (upd)
		if(relY < 0)
			gfx.Update(scr, relX, 0, ibg->w, ibg->h+relY);
		else
			gfx.Update(scr, relX, relY, ibg->w, ibg->h);
}
 
bool INFO_PANEL::hover(int x, int y) {
	if	(x >= relX && x <= relX+ibg->w && y >= relY && y <= relY+ibg->h)
		return true;
	return false;
}
 
bool INFO_PANEL::hover_button(int x, int y) {
	if	(x >= relX+buttonX && x <= relX+buttonX+button[0]->w &&
		 y >= relY+buttonY && y <= relY+buttonY+button[0]->h)
		return true;
	return false;
}
 
/* ---------------------------------------------------------------- */

void INFO_PANEL::redraw_status(SDL_Surface* scr) { // XXX: BUGGED
	if(relY+statusY < 0 && relY+statusY+status_surf->h > 0) {
		SDL_Rect clip = gfx.SetClip(relX+statusX, 0-relY+statusY, status_surf->w, relY+statusY+ibg->h); 
		gfx.AddSurface(relX+statusX, 0, bkgImage, scr, &clip);
		SDL_Rect clip2 = gfx.SetClip(statusX, statusY, status_surf->w, status_surf->h);
		gfx.AddSurface(relX+statusX, 0, ibg, scr, &clip);
	} else 
	if(relY+statusY >= 0) {
		SDL_Rect clip = gfx.SetClip(relX+statusX, 0-relY+statusY, status_surf->w, relY+statusY+ibg->h); 
		gfx.AddSurface(relX+statusX, relX+relY, bkgImage, scr, &clip);
		SDL_Rect clip2 = gfx.SetClip(statusX, statusY, status_surf->w, status_surf->h);
		gfx.AddSurface(relX+statusX, relX+relY, ibg, scr, &clip);
	}
	draw_status(scr);
}

void INFO_PANEL::draw_status(SDL_Surface* scr, bool upd) {
	if(relY+statusY < 0 && relY+statusY+status_surf->h > 0) {
		SDL_Rect clip = gfx.SetClip(0, status_surf->h-(relY+statusY+status_surf->h), status_surf->w, relY+statusY+status_surf->h); 
		gfx.AddSurface(relX+statusX, 0, status_surf, scr, &clip);
	} else 
	if(relY+statusY >= 0) {
		gfx.AddSurface(relX+statusX, relY+statusY, status_surf, scr, NULL);
	}
	if (upd)
		if(relY+statusY < 0 && relY+statusY+status_surf->h > 0)
			gfx.Update(scr, relX+statusX, 0, status_surf->w, relY+statusY+ibg->h);
		else
		if(relY+statusY >= 0)
			gfx.Update(scr, relX+statusX, relY+statusY, status_surf->w, status_surf->h);
}

void INFO_PANEL::draw_button(SDL_Surface* scr, bool pressed, bool upd) {
	SDL_Surface		*img = NULL;
	if (pressed) 	img = button[1];
	else			img = button[0];
	if(img) {
		if(relY+buttonY < 0 && relY+buttonY+img->h > 0) {
			SDL_Rect clip = gfx.SetClip(0, img->h-(relY+buttonY+img->h), img->w, relY+buttonY+img->h); 
			gfx.AddSurface(relX+buttonX, 0, img, scr, &clip);
		} else 
		if(relY+buttonY >= 0) {
			gfx.AddSurface(relX+buttonX, relY+buttonY, img, scr, NULL);
		}
		if (upd)
			if(relY+buttonY < 0 && relY+buttonY+img->h > 0)
				gfx.Update(scr, relX+buttonX, 0, img->w, relY+buttonY+ibg->h);
			else
			if(relY+buttonY >= 0)
				gfx.Update(scr, relX+buttonX, relY+buttonY, img->w, img->h);
	}
}

/* ---------------------------------------------------------------- */

int INFO_PANEL::max_visible() { return ibg->h; }
int INFO_PANEL::min_visible() { return INFO_MIN_VIS; }

INFO_PANEL::~INFO_PANEL() {
	gfx.ReleaseSurface(ibg);
	gfx.ReleaseSurface(button[0]);
	gfx.ReleaseSurface(button[1]);
}

