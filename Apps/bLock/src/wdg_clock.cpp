#include "wdg_clock.h"

iCLOCK::iCLOCK() {
	year = day = hour = minute = -1;
	month = "";
	dragging = false;
	time_surface = NULL;
	date_surface = NULL;
	ibg = NULL;
	bkgImage = NULL;
	font[0] = NULL;
	font[1] = NULL;
}

void iCLOCK::updateTime() {
	QTime time = QTime::currentTime();
	int h = time.hour();
	int m = time.minute();
	
	if (hour != h || minute != m) {
		hour = h;
		minute = m;
		QDate date = QDate::currentDate();
		if (date.day() != day) {
			year = date.year();
			month = date.monthName(date.month());
			day = date.day();
		}
		redraw(gfx.Screen);
	}
}

void iCLOCK::Init(int x, int y, SDL_Surface* bkg) {
	relX = x;
	relY = y;
	bkgImage = bkg;
	
	char pth[MAX_PATH_LEN];
	sprintf(pth, "%s/%s", Path_Data, "wdg_clock/bg.png");
	ibg = gfx.LoadImage(pth);
	
	sprintf(pth, "%s", (const char*)FONT_PATH);
	font[0] = gfx.LoadFont(pth,FONT_SMALL);
	font[1] = gfx.LoadFont(pth,FONT_BIG);
	
	updateTime();
}

void iCLOCK::moveRel(int rX, int rY) {
	move(relX+rX, relY+rY);
}

void iCLOCK::move(int x, int y) {
	bool brk = false;
	
	if (x + ibg->w > ScreenWidth) x = ScreenWidth - ibg->w;
	else if (x < 0) x = 0;
	if (y + ibg->h > ScreenHeight-unLock.min_visible()) y = ScreenHeight-unLock.min_visible() - ibg->h;
	else if (y < infop.min_visible()) y = infop.min_visible();
	
	// Push info panel if needed
	if (y < infop.relY+infop.max_visible()) {
		if (relY > infop.relY+infop.max_visible())
			y = infop.relY+infop.max_visible();
		else
		if	(relY == infop.relY+infop.max_visible() &&
			infop.relY+infop.max_visible() > infop.min_visible())
			infop.moveRel(0, y-relY);
		else brk = true;
	}

	// Push unlocker if needed
	if (y + ibg->h > unLock.relY) {
		if (relY + ibg->h < unLock.relY)
			y = unLock.relY - ibg->h;
		else
		if	(relY + ibg->h == unLock.relY &&
			ScreenHeight-unLock.relY > unLock.min_visible())
			unLock.moveRel(0, y-relY);
		else brk = true;
	}
	
	if ( (relX != x || relY != y) && !brk ) {
		int x0 = x, x1 = relX - x;
		int y0 = y, y1 = relY - y;
		if (x1 < 0) { x1 = 0 - x1; x0 = relX; }; x1 += ibg->w;
		if (y1 < 0) { y1 = 0 - y1; y0 = relY; }; y1 += ibg->h;
	
		SDL_Rect clip = gfx.SetClip(relX, relY, ibg->w, ibg->h);
		gfx.AddSurface(relX, relY, bkgImage, gfx.Screen, &clip);

		relX = x; relY = y;
		
		draw(gfx.Screen, false);
		gfx.Update(gfx.Screen, x0, y0, x1, y1);
	}
}

void iCLOCK::redraw(SDL_Surface* scr) {
	SDL_Rect clip = gfx.SetClip(relX, relY, ibg->w, ibg->h);
	gfx.AddSurface(relX, relY, bkgImage, scr, &clip);
	
	char text[30];
	
	sprintf(text, "%d%d:%d%d", int(hour/10), hour%10, int(minute/10), minute%10);
	gfx.ReleaseSurface(time_surface);
	time_surface = gfx.Text(font[1], text, FONT_COLOR_R, FONT_COLOR_G, FONT_COLOR_B);
	sprintf(text, "%d %s %d", day, (const char*)month, year);
	gfx.ReleaseSurface(date_surface);
	date_surface = gfx.Text(font[0], text, FONT_COLOR_R, FONT_COLOR_G, FONT_COLOR_B);
	
	draw(scr);
}

void iCLOCK::draw(SDL_Surface* scr, bool upd) {
	gfx.AddSurface(relX, relY, ibg, scr, NULL);
	gfx.AddSurface(relX+(ibg->w - time_surface->w)/2, relY, time_surface, scr, NULL);
	gfx.AddSurface(relX+(ibg->w - date_surface->w)/2, relY+47, date_surface, scr, NULL);
	if (upd)
		gfx.Update(scr, relX, relY, ibg->w, ibg->h);
}

bool iCLOCK::hover(int x, int y) {
	if	(x >= relX && x <= relX+ibg->w && y >= relY && y <= relY+ibg->h)
		return true;
	return false;
}

int iCLOCK::width() { return ibg->w; }
int iCLOCK::height() { return ibg->h; }

iCLOCK::~iCLOCK() {
	gfx.ReleaseSurface(ibg);
	for (int s=0; s<=1; s++)	gfx.ReleaseFont(font[s]);
}

