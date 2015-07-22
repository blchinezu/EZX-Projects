#include "app.h"

#ifdef DEBUG
int gcounter=1;
#endif

/** ***********************************************************************/
/** **************************[ SETTINGS ]*********************************/
/** ***********************************************************************/

/****************************[ CONSTRUCTOR ]*******************************/

SETTINGS::SETTINGS(SDL_Surface *wall) {
	#ifdef DEBUG
	printf("\n LOADING SETTINGS CLASS: %d\n\n", gcounter++);
	#endif
	
	// Initialize variables values
	wallpaper = NULL;
	old_wallpaper = NULL;
	button_clicked = false;
	Running	= false;
	Focused = true;
	
	// Set screen settings
	ezx.set_brightness(ezx.current_br);
	ezx.backlight(true);
	ezx.set_LCDOffSeconds(ezx.LCDOffSeconds);
	
	// Keep old screen for slide effect
	old_wallpaper = gfx.CopySurface(gfx.Screen);
	
	// Create new screen base
	wallpaper = gfx.NewSurface(ScreenWidth, ScreenHeight);
	SDL_Rect clip = gfx.SetClip(0, 0, ScreenWidth, ScreenHeight);
	gfx.AddSurfaceTopLeft(wall, wallpaper, &clip);

	// Add black transparent window
	char pth[MAX_PATH_LEN];
	sprintf(pth, "%s/%s", Path_Data, "settings/bg.png");
	SDL_Surface *black = gfx.LoadImage(pth);
	gfx.AddSurfaceTopLeft(black, wallpaper, NULL);
	gfx.ReleaseSurface(black);
	
	// Add delimiter between scroll list and button
	sprintf(pth, "%s/%s", Path_Data, "settings/delim1.png");
	SDL_Surface *delim1 = gfx.LoadImage(pth);
	gfx.AddSurface(0, 275, delim1, wallpaper, NULL);
	gfx.AddSurface(0, 30, delim1, wallpaper, NULL);

	// Initialize scroll window
	swin.Init(wallpaper, 8, 30+delim1->h-iYoffset, 224, 279-(30+delim1->h-iYoffset));
	gfx.ReleaseSurface(delim1);
	
	SDL_Surface *title = gfx.Text(swin.font, "SETTINGS", FONT_COLOR_R, FONT_COLOR_G, FONT_COLOR_B);
	gfx.AddSurfaceCenteredX(8, title, wallpaper, NULL);
	gfx.ReleaseSurface(title);
	
	// Initialize soft-keyboard
	SK::Init();
	
	// Create new temporary surface to keep the background intact for further use
	SDL_Surface *tmp_wallpaper = gfx.CopySurface(wallpaper);
	
		// Draw scroll list window
		swin.draw(tmp_wallpaper);
	
		// Load button images and add it
		sprintf(pth, "%s/%s", Path_Data, "settings/but.png");
		button[0] = gfx.LoadImage(pth);
		sprintf(pth, "%s/%s", Path_Data, "settings/butp.png");
		button[1] = gfx.LoadImage(pth);
		gfx.AddSurface((ScreenWidth-button[0]->w)/2, 278, button[0], tmp_wallpaper, NULL);
		
		// Slide screen
		gfx.SlideScreen(LEFT, tmp_wallpaper, gfx.Screen);
		
	// Delete the temporary surface
	gfx.ReleaseSurface(tmp_wallpaper);
	
	// Start the settings loop
	Run();
}
/******************************[ START ]***********************************/

bool SETTINGS::Run() {
	#ifdef DEBUG
	Timer fps;
	fps.start();
	#endif
	Timer maxfps;

	Running = true;
	while(Running) {
		maxfps.start();
		
		HandleEvent();
		if(swin.goFlow) swin.selfFlow();
		gfx.FullUpdate(gfx.Screen);
	   
		Timer::frame++;
		// Limit FPS
		if((LIMITFPS) && (maxfps.get_ticks()<1000/SCREENMAXFPS)) {
			SDL_Delay((1000/SCREENMAXFPS)-maxfps.get_ticks());
		}

		#ifdef DEBUG
		// Get current frame rateSETTINGS
		if(fps.get_ticks()>1000) {
			//Timer::FPS=Timer::frame;
			printf("FPS: %d\n", Timer::frame);
			fps.start();
			Timer::frame=0;
		}
		#endif
	}
	return false;
}

/****************************[ ON_EVENT ]**********************************/

void SETTINGS::HandleEvent() {
	while(SDL_PollEvent(&Event))
		switch(Event.type) {

			case SDL_QUIT:
				Running = false;
				app.Running = false;
				break;

			case SDL_MOUSEBUTTONDOWN:
				MouseDown(Event.motion.x,Event.motion.y,
						Event.motion.xrel,Event.motion.yrel);
				break;

			case SDL_MOUSEBUTTONUP:
				MouseUp(Event.motion.x,Event.motion.y,
						Event.motion.xrel,Event.motion.yrel);
				break;

			case SDL_MOUSEMOTION:
				MouseMove(Event.motion.x,Event.motion.y,
						Event.motion.xrel,Event.motion.yrel);
				break;

			case SDL_ACTIVEEVENT:
				app.HandleActiveEvent();
				break;
		};
}

void SETTINGS::MouseMove(int mX, int mY, int relX, int relY) {
	#ifdef DEBUG
	printf("MOUSE MOVE: %dx%d (%dx%d)\n", mX, mY, relX, relY);
	#endif
	
//	last_relY = relY;
	if (swin.click) {
		if( swin.last_rel_count > MovesTillDrag	) {
			swin.click = false;
			swin.dragging = true;
		} else {
			swin.last_relY[swin.last_rel_count++] = relY;
		}
	}
	if (swin.dragging) swin.moveRel(relY);
}

void SETTINGS::MouseDown(int mX, int mY, int relX, int relY) {
	#ifdef DEBUG
	printf("MOUSE DOWN: %dx%d (%dx%d)\n", mX, mY, relX, relY);
	#endif
	
	if(swin.hover(mX, mY)) {
		swin.click = true;
		swin.dragging = false;
		swin.selfFlow(false);
	} else
	if	(
		mX >= (ScreenWidth-button[0]->w)/2 && 
		mX <= (ScreenWidth-button[0]->w)/2+button[0]->w &&
		mY >= 278 && mY <= 278 + button[0]->h) {
		button_clicked = true;
		gfx.AddSurface((ScreenWidth-button[0]->w)/2, 278, button[1], gfx.Screen, NULL);
	}
}

void SETTINGS::MouseUp(int mX, int mY, int relX, int relY) {
	#ifdef DEBUG
	printf("MOUSE UP: %dx%d (%dx%d)\n", mX, mY, relX, relY);
	#endif

	if(button_clicked) {
		ezx.vibrate(VIB_TIME);
		gfx.AddSurface((ScreenWidth-button[0]->w)/2, 278, button[0], gfx.Screen, NULL);
		gfx.FullUpdate(gfx.Screen);
		// TODO: Save values
		Running = false;
	}
	if(swin.click) {
		swin.click = false;
		swin.selfFlow(false);
		int hh = 0;
		for(int i=0; i<swin.items_counter; i++) {
			if (
				!( hh < swin.yScroll && hh+swin.item[i].h <= swin.yScroll ) &&
				!( hh >= swin.yScroll+swin.h )
				) {
				if ( hh-swin.yScroll < 0 ) {
					if(	mY >= swin.y+iYoffset &&
						mY <= swin.y+iYoffset+(hh-swin.yScroll)+swin.item[i].h )
						swin.item[i].gui_val();
				} else {
					if(	mY >= swin.y+iYoffset+(hh-swin.yScroll) &&
						mY <= swin.y+iYoffset+(hh-swin.yScroll)+swin.item[i].h )
						swin.item[i].gui_val();
				}
			}
			hh += swin.item[i].h + 6;
		}
	}
	if(swin.dragging) {
		swin.dragging = false;
		if(swin.last_rel_count>2) {
			swin.moveRel(relY);
			swin.goFlow = true;
		}
	}
}

/***************************[ DESTRUCTOR ]*********************************/

SETTINGS::~SETTINGS() {
	#ifdef DEBUG
	printf("\n * CLOSING SETTINGS CLASS\n");
	#endif

	gfx.SlideScreen(RIGHT, old_wallpaper, gfx.Screen);
	if(!INFO_VISIBLE) infop.moveAuto(-1);
	
	gfx.ReleaseSurface(wallpaper);
	gfx.ReleaseSurface(old_wallpaper);
	gfx.ReleaseSurface(button[0]);
	gfx.ReleaseSurface(button[1]);
	SK::Quit();
	
	#ifdef DEBUG
	printf(" * CLOSED\n\n");
	#endif
}


/** ***********************************************************************/
/** *************************[ SCROLL WINDOW ]*****************************/
/** ***********************************************************************/

/****************************[ CONSTRUCTOR ]*******************************/

SCROLLWINDOW::SCROLLWINDOW() {
	bkgImage = ibg = delim = NULL;
	font = NULL;
	click = dragging = goFlow = returning = false;
	x = y = w = h = 
	yScroll = hScroll = relyFlow =
	last_relY[0] = last_relY[1] = last_relY[2] =
	last_rel_count =
	items_counter = 
	0;
}

/****************************[ INITIALIZER ]*******************************/

void SCROLLWINDOW::Init(SDL_Surface* background, int X, int Y, SDL_Surface* ibgg) {
	Init(background, X, Y, ibgg->w, ibgg->h, ibgg);
}

void SCROLLWINDOW::Init(SDL_Surface* background, int X, int Y, int W, int H, SDL_Surface* ibgg) {
	bkgImage = background;
	ibg = ibgg;
	x = X; y = Y;
	w = W; h = H;
	
	char pth[MAX_PATH_LEN];
	// Load font
	sprintf(pth, "%s", (const char*)FONT_PATH);
	font = gfx.LoadFont(pth,FONT_SMALL);
	
	// Load delimiter image
	sprintf(pth, "%s/%s", Path_Data, "settings/delim0.png");
	delim = gfx.LoadImage(pth);
	
	// Set options list
	item[0].set(this, "Show top panel at startup",		INFO_VISIBLE	);
	item[1].set(this, "Show bottom panel at startup",	UNLOCK_VISIBLE	);
	item[2].set(this, "Enable brightness fade",			ENABLE_FADE		);
	item[3].set(this, "Normal LCD Sleep time (sec)",	LCDOFFSECONDS	);
	item[4].set(this, "bLock LCD Sleep time (sec)",		LIGHTTIME		);
	item[5].set(this, "Screen slide speed",				SETT_SLIDE_SPD	);
	item[6].set(this, "Light Up Key",					LIGHTUPKEY, iKEY);
	item[7].set(this, "Font Size: Small",				FONT_SMALL		);
	item[8].set(this, "Font Size: Normal",				FONT_NORMAL		);
	item[9].set(this, "Font Size: Big",					FONT_BIG		);
}

/*****************************[ DESTRUCTOR ]*******************************/

SCROLLWINDOW::~SCROLLWINDOW() {
	#ifdef DEBUG
	printf("REMOVING SCROLLWINDOW CLASS\n");
	#endif
	SET_INFO_VISIBLE(item[0].hold_bool);
	SET_UNLOCK_VISIBLE(item[1].hold_bool);
	SET_ENABLE_FADE(item[2].hold_bool);
	SET_LCDOFFSECONDS(item[3].hold_int);
	SET_LIGHTTIME(item[4].hold_int);
	SET_SETT_SLIDE_SPD(item[5].hold_int);
	SET_LIGHTUPKEY(item[6].hold_key);
	SET_FONT_SMALL(item[7].hold_int);
	SET_FONT_NORMAL(item[8].hold_int);
	SET_FONT_BIG(item[9].hold_int);
	
	gfx.ReleaseSurface(ibg);
	gfx.ReleaseSurface(delim);
	gfx.ReleaseFont(font);
}

/*******************************[ DRAW ]***********************************/

void SCROLLWINDOW::draw(SDL_Surface* scr) {
	SDL_Surface *temp = gfx.NewSurface(w-(2*iXoffset), h-(2*iYoffset));
	SDL_Rect clip = gfx.SetClip(x+iXoffset, y+iYoffset, w-(2*iXoffset), h-(2*iYoffset));
			gfx.AddSurface(0, 0, bkgImage, temp, &clip);
	if(ibg)	gfx.AddSurface(0, 0, ibg, temp, &clip);
	int hh = 0;
	for(int i=0; i<items_counter; i++) {
		if (!( hh < yScroll && hh+item[i].h <= yScroll ) && !( hh >= yScroll+h ))
			item[i].draw(0, hh-yScroll, temp);
		hh += item[i].h + 6;
		if(i == items_counter-1)
			hh -= 6;
		else
			if (hh >= yScroll && hh<= yScroll+h-delim->h) {
				clip.y = 0;
				clip.h = delim->h;
				gfx.AddSurface(0, hh-4-yScroll, delim, temp, &clip);
			}
	}
	hScroll = hh;
	gfx.AddSurface(x+iXoffset, y+iYoffset, temp, scr, NULL);
	gfx.ReleaseSurface(temp);
}

/****************************[ MOVE RELATIVE ]*****************************/

void SCROLLWINDOW::moveRel(int rely, bool upd) {
	if((( yScroll != -MaxOutArea && rely > 0 ) || 
		( yScroll != hScroll+MaxOutArea-(h-iYoffset*2) && rely < 0)) &&
		hScroll>=h) {
		last_relY[0] = last_relY[1]; last_relY[1] = last_relY[2];
		last_relY[2] = rely;
		last_rel_count++;
		yScroll -= rely;
		if( yScroll < -MaxOutArea) {
			yScroll = -MaxOutArea;
		} else
		if( hScroll!=0 && yScroll > hScroll+MaxOutArea-(h-iYoffset*2)) {
			yScroll = hScroll+MaxOutArea-(h-iYoffset*2);
		}
		
		draw(gfx.Screen);
		
		if(upd)
			gfx.Update(
				gfx.Screen, x+iXoffset,
				y+iYoffset, w-(2*iXoffset), h-(2*iYoffset)
				);
	}
}

void SCROLLWINDOW::selfFlow(bool running) {
	if( running && hScroll>h-iYoffset*2 ) {
		#ifdef DEBUG
		printf(" selfFlow() step: %d\n", relyFlow);
		#endif
		if(goFlow) {
			if(relyFlow == 0 && !returning) {
				relyFlow = (last_relY[0]+last_relY[1]+last_relY[2])/3;
				last_relY[0] = last_relY[1] = last_relY[2] = 0;
			}
			// Flow along the drag
			if(relyFlow>0)	moveRel(relyFlow--, false);	else
			if(relyFlow<0)	moveRel(relyFlow++, false);
			// If reached maximum cut the flow
			if( yScroll == -MaxOutArea) {
				relyFlow=0;
				returning=true;
			} else
			if( hScroll!=0 && yScroll == hScroll+MaxOutArea-(h-iYoffset*2)) {
				yScroll = hScroll+MaxOutArea-(h-iYoffset*2);
				relyFlow=0;
				returning=true;
			}
			// Consider returning if went too far
			if(relyFlow==0)
			{
				if(yScroll < 0) {
					if(!returning) returning = true;
					if(yScroll>-FlowRetStep)	moveRel(yScroll);
					else						moveRel(-FlowRetStep);
				} else
				if(yScroll > hScroll-(h-iYoffset*2)) {
					if(!returning) returning = true;
					if(yScroll-hScroll+(h-iYoffset*2)<FlowRetStep)
						moveRel(yScroll-hScroll+(h-iYoffset*2));
					else
						moveRel(FlowRetStep);
				} else
					// Kill the flow after everything is done
					selfFlow(false);
			}
		}
	} else {
		dbg(INFO, "selfFlow() > Killed");
		goFlow = returning = false;
		relyFlow = 0;
		last_relY[0]=last_relY[1]=last_relY[2]=last_rel_count=0;
	}
}

/*******************************[ HOVER ]**********************************/

bool SCROLLWINDOW::hover(int xx, int yy) {
	if ( xx >= x && xx <= x+w && yy >= y && yy<= y+h )
		return true;
	return false;
}

/** ***********************************************************************/
/** *****************************[ ITEM ]**********************************/
/** ***********************************************************************/

/****************************[ CONSTRUCTOR ]*******************************/

ITEM::ITEM() {
	h = 0;
	type = iNONE;
	name = NULL;
	value = NULL;
	hold_bool = false;
	hold_int = hold_key = 0;
}

/*****************************[ INITIALIZER ]*****************************/

void ITEM::setCommon(SCROLLWINDOW* ssw, char *descr) {
	sw = ssw;
	sw->items_counter++;
	maxX = sw->w - (iXoffset*2);
	
	sprintf(nameC, "%s", descr);
	
	char tmp[MAX_NAME_LEN*2];
	sprintf(tmp, "%s", descr);
	name = gfx.Text(sw->font, tmp, FONT_COLOR_R, FONT_COLOR_G, FONT_COLOR_B);
	while( name->w > maxX ) {
		gfx.ReleaseSurface(name);
		tmp[strlen(tmp)-4]='\0';
		sprintf(tmp, "%s...", tmp);
		name = gfx.Text(sw->font, tmp, FONT_COLOR_R, FONT_COLOR_G, FONT_COLOR_B);
	}
}

void ITEM::set(SCROLLWINDOW* ssw, char *descr, int val, int tip) {
	type = tip;
	setCommon(ssw, descr);
	set_val(val);
	h = name->h + value->h;
}

void ITEM::set(SCROLLWINDOW* ssw, char *descr, bool val) {
	type = iBOOL;
	setCommon(ssw, descr);
	set_val(val);
	h = name->h + value->h;
}

void ITEM::set(SCROLLWINDOW* ssw, char *descr, int val) {
	type = iINT;
	setCommon(ssw, descr);
	set_val(val);
	h = name->h + value->h;
}

void ITEM::set(SCROLLWINDOW* ssw, char *descr, char *val) {
	type = iCHAR;
	setCommon(ssw, descr);
	set_val(val);
	h = name->h + value->h;
}

/***************************[ SET/GET VALUE ]******************************/

void ITEM::set_val(bool val) {
	hold_bool = val;
	gfx.ReleaseSurface(value);
	if(val)	value = gfx.Text(sw->font, "YES", FONT_COLOR_R, FONT_COLOR_G, FONT_COLOR_B);
	else	value = gfx.Text(sw->font, "NO", FONT_COLOR_R, FONT_COLOR_G, FONT_COLOR_B);
}

void ITEM::set_val(int val) {
	switch(type) {
	case iINT: {
		hold_int = val;
		char tt[6];
		sprintf(tt, "%d", val);
		gfx.ReleaseSurface(value);
		value = gfx.Text(sw->font, tt, FONT_COLOR_R, FONT_COLOR_G, FONT_COLOR_B);
		}; break;
	case iKEY: {
		if(val != 0) hold_key = val;
		char tt2[MAX_NAME_LEN];
		key2char(val, tt2);
		gfx.ReleaseSurface(value);
		value = gfx.Text(sw->font, tt2, FONT_COLOR_R, FONT_COLOR_G, FONT_COLOR_B);
		}; break;
	}
}

void ITEM::set_val(char *str) {
	sprintf(hold_char, "%s", str);
	gfx.ReleaseSurface(value);
	value = gfx.Text(sw->font, str, FONT_COLOR_R, FONT_COLOR_G, FONT_COLOR_B);
}

void ITEM::gui_val() {
	ezx.vibrate(VIB_TIME);
	switch(type) {
	case iBOOL: {
		set_val(!hold_bool);
		sw->draw(gfx.Screen);
		}; break;
	case iINT: {
		char cc[50];
		SDL_Surface *old_scr = gfx.CopySurface(gfx.Screen);
		sprintf(cc, "%d", hold_int);
		SK::Mode='N';
		SK::SetTitle(nameC);
		SK::GetString(cc);
		gfx.AddSurfaceTopLeft(old_scr, gfx.Screen, NULL);
		gfx.ReleaseSurface(old_scr);
		set_val(atoi(cc));
		sw->draw(gfx.Screen);
		}; break;
	case iKEY: {
		bool is = true;
		SDL_Surface *old_scr2 = gfx.CopySurface(gfx.Screen);

		char pth[MAX_PATH_LEN];
		sprintf(pth, "%s/%s", Path_Data, "sk/bkg.png");
		SDL_Surface* black = gfx.LoadImage(pth);
		for(int i=0; i<5; i++)
			gfx.AddSurfaceTopLeft(black,gfx.Screen, NULL);
		gfx.ReleaseSurface(black);
		
		SDL_Surface *txt = gfx.Text(sw->font, "Waiting for a key press", FONT_COLOR_R, FONT_COLOR_G, FONT_COLOR_B);
		gfx.AddSurfaceCenteredX(145, txt, gfx.Screen, NULL);
		gfx.ReleaseSurface(txt);
		txt = gfx.Text(sw->font, "(Touch the screen to cancel)", FONT_COLOR_R, FONT_COLOR_G, FONT_COLOR_B);
		gfx.AddSurfaceCenteredX(145+txt->h+10, txt, gfx.Screen, NULL);
		gfx.ReleaseSurface(txt);
		
		gfx.FullUpdate(gfx.Screen);
		
		int got = 0;
		while(is) {
			SDL_Event e;
			SDL_WaitEvent(&e);
			switch (e.type) {
				case SDL_QUIT:
					app.Quit();
					is = false;
					break;
				case SDL_ACTIVEEVENT: {
					app.HandleActiveEvent();
					}; break;
				case SDL_MOUSEBUTTONDOWN:
					is = false;
					break;
				case SDL_KEYDOWN:
					got = e.key.keysym.sym;
					is = false;
					break;
			}
		}

		gfx.AddSurfaceTopLeft(old_scr2, gfx.Screen, NULL);
		gfx.ReleaseSurface(old_scr2);
		if(got != 0) {
			set_val(got);
			
			sw->draw(gfx.Screen);
		}
		}; break;
	case iCHAR: {
		SK::SetTitle(nameC);
		SK::GetString(hold_char);
		set_val(hold_char);
		}; break;
	}
}
/********************************[ DRAW ]**********************************/

void ITEM::draw(int dx, int dy, SDL_Surface* scr) { draw(dx, dy, 0, h, scr); }

void ITEM::draw(int dx, int dy, int rely, int relh, SDL_Surface* scr) {
	if( rely < name->h ) {
		if( name->h-rely > relh ) {
			SDL_Rect clip1 = gfx.SetClip(0, rely, name->w, relh);
			gfx.AddSurface(dx, dy, name, scr, &clip1);
		} else {
			SDL_Rect clip2 = gfx.SetClip(0, rely, name->w, name->h-rely);
			gfx.AddSurface(dx, dy, name, scr, &clip2);
		}
	}

	if( rely+relh > name->h ) {
		if ( rely <= name->h ) {
			SDL_Rect clip3 = gfx.SetClip(0, 0, value->w, rely+relh-name->h);
			gfx.AddSurface(dx, dy+name->h, value, scr, &clip3);
		} else {
			SDL_Rect clip4 = gfx.SetClip(0, rely-name->h, value->w, rely+relh-name->h);
			gfx.AddSurface(dx, dy+name->h, value, scr, &clip4);
		}
	}
}

/*****************************[ OTHERS ]*******************************/

bool ITEM::is_num(char c) {
	if(c-48 >= 0 && c-48 <= 9) return true;
	return false;
}

int ITEM::atoi(char *str) {
	int nr=0, mult=1;
	for(int i=strlen(str)-1; i>=0; i--)
		if(is_num(str[i])) { nr+=(str[i]-48)*mult; mult*=10; }
	return nr;
}

void ITEM::key2char(int val, char *tt2) {
	switch(val) {
		case SDLK_SPACE:	sprintf(tt2, "Call");			break;
		case SDLK_ESCAPE:	sprintf(tt2, "Hang");			break;
		case SDLK_PAUSE:	sprintf(tt2, "Camera");			break;
		case SDLK_PLUS:		sprintf(tt2, "Vol+");			break;
		case SDLK_MINUS:	sprintf(tt2, "Vol-");			break;
		case SDLK_RETURN:	sprintf(tt2, "OK");				break;
		case SDLK_RIGHT:	sprintf(tt2, "Right");			break;
		case SDLK_LEFT:		sprintf(tt2, "Left");			break;
		case SDLK_DOWN:		sprintf(tt2, "Down");			break;
		case SDLK_UP:		sprintf(tt2, "Up");				break;
		case SDLK_o:		sprintf(tt2, "Previous");		break;
		case SDLK_p:		sprintf(tt2, "Pause");			break;
		case SDLK_q:		sprintf(tt2, "Forward");		break;
		case SDLK_r:		sprintf(tt2, "Browser");		break;
		case SDLK_f:		sprintf(tt2, "Mod + Call");		break;
		case SDLK_a:		sprintf(tt2, "Mod + Hang");		break;
		case SDLK_e:		sprintf(tt2, "Mod + Camera");	break;
		case SDLK_c:		sprintf(tt2, "Mod + Vol+");		break;
		case SDLK_d:		sprintf(tt2, "Mod + Vol-");		break;
		case SDLK_b:		sprintf(tt2, "Mod + OK");		break;
		case SDLK_j:		sprintf(tt2, "Mod + Right");	break;
		case SDLK_i:		sprintf(tt2, "Mod + Left");		break;
		case SDLK_h:		sprintf(tt2, "Mod + Down");		break;
		case SDLK_g:		sprintf(tt2, "Mod + Up");		break;
		case SDLK_k:		sprintf(tt2, "Mod + Previous");	break;
		case SDLK_l:		sprintf(tt2, "Mod + Pause");	break;
		case SDLK_m:		sprintf(tt2, "Mod + Forward");	break;
		case SDLK_n:		sprintf(tt2, "Mod + Browser");	break;
		default:			sprintf(tt2, "%d", val);
	}
}

/*****************************[ DESTRUCTOR ]*******************************/

ITEM::~ITEM() {
	#ifdef DEBUG
	printf("REMOVING ITEM CLASS\n");
	#endif
	gfx.ReleaseSurface(name);
	gfx.ReleaseSurface(value);
}
