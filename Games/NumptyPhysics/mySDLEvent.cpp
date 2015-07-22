
#include "mySDLEvent.h"

int full_screen = 0, full_screen_x = 0, full_screen_y = 0;
int return_up = 1;
int mouse_x = 0, mouse_y = 0, mouse_speed = 2;
int navikey[8] = {0,0,0,0,0,0,0,0};

int mySDL_PollEvent(SDL_Event *event){
	int r = SDL_PollEvent(event);
	
	switch(event->type){
		case SDL_MOUSEMOTION:
			mouse_x = (800*event->motion.x)/320;
			mouse_y = event->motion.y*2;
			break;
		case SDL_KEYDOWN:
			switch (event->key.keysym.sym){
				case SDLK_ESCAPE:
					exit(-1);
					break;
				case SDLK_PLUS: // left mouse down
					event->type = SDL_MOUSEBUTTONDOWN;
					event->button.button = SDL_BUTTON_LEFT;
					break;
				case SDLK_MINUS: // right mouse down
					event->type = SDL_MOUSEBUTTONDOWN;
					event->button.button = SDL_BUTTON_RIGHT;
					break;
			}
			break;
			
		case SDL_KEYUP:
			switch (event->key.keysym.sym){
				case SDLK_PLUS: // left mouse up
					event->type = SDL_MOUSEBUTTONUP;
					event->button.button = SDL_BUTTON_LEFT;
					break;		
				case SDLK_MINUS: //right mouse up
					event->type = SDL_MOUSEBUTTONUP;
					event->button.button = SDL_BUTTON_RIGHT;
					break;						
			
			}
	}
	
	event->button.x = mouse_x;
	event->button.y = mouse_y;
	//printf("%s-%i  x=%i,y=%i\n",__FILE__,__LINE__,event->button.x,event->button.y);
	//printf("%s-%i  event.type=%i\n",__FILE__,__LINE__,event->type);
	return r;
}

void mySDL_GetMouseState(int *x, int *y){
	*x = mouse_x;
	*y = mouse_y;
	//printf("%s-%i  x=%i,y=%i\n",__FILE__,__LINE__,x,y);
}

int mySDL_WaitEvent(SDL_Event *event){
	//SDL_Delay(10);
	//return 1;
}


/*

#include "mySDLEvent.h"

int full_screen = 0, full_screen_x = 0, full_screen_y = 0;
int return_up = 1;
int mouse_x = 0, mouse_y = 0, mouse_speed = 2;
int navikey[8] = {0,0,0,0,0,0,0,0};

int mySDL_PollEvent(SDL_Event *event){
	int r = SDL_PollEvent(event);
	
	switch(event->type){
		case SDL_KEYDOWN:
			switch (event->key.keysym.sym){
				case SDLK_UP:
					if (full_screen){
						navikey[4] = 1;
						}
					break;
				case SDLK_DOWN:
					if (full_screen){
						navikey[5] = 1;
						}
					break;
				case SDLK_LEFT:
					if (full_screen){
						navikey[6] = 1;
						}
					break;
				case SDLK_RIGHT:
					if (full_screen){
						navikey[7] = 1;
						}
					break;
				case SDLK_3:
					full_screen = 1;
					break;
				case SDLK_1:
					full_screen = 0;
					break;
				case SDLK_BACKSPACE:
					exit(-1);
					break;
				case SDLK_7:
					mouse_speed+=2;
					if (mouse_speed>20) mouse_speed = 2;
					break;
				case SDLK_6:			
					navikey[0] = 1;
					event->type = SDL_MOUSEMOTION;
					break;
				case SDLK_4:		
					navikey[1] = 1;
					event->type = SDL_MOUSEMOTION;
					break;
				case SDLK_2:
					navikey[2] = 1;
					event->type = SDL_MOUSEMOTION;
					break;
				case SDLK_8:
					navikey[3] = 1;
					event->type = SDL_MOUSEMOTION;
					break;
				case SDLK_PLUS: // left mouse down
					event->type = SDL_MOUSEBUTTONDOWN;
					event->button.button = SDL_BUTTON_LEFT;
					break;
				case SDLK_MINUS: // right mouse down
					event->type = SDL_MOUSEBUTTONDOWN;
					event->button.button = SDL_BUTTON_RIGHT;
					break;
			}
			break;
			
		case SDL_KEYUP:
			switch (event->key.keysym.sym){
				case SDLK_UP:
					navikey[4] = 0;
					break;
				case SDLK_DOWN:
					navikey[5] = 0;
					break;
				case SDLK_LEFT:
					navikey[6] = 0;
					break;
				case SDLK_RIGHT:
					navikey[7] = 0;
					break;
					
				case SDLK_6:			
					navikey[0] = 0;
					event->type = SDL_MOUSEMOTION;
					break;
				case SDLK_4:		
					navikey[1] = 0;
					event->type = SDL_MOUSEMOTION;
					break;
				case SDLK_2:
					navikey[2] = 0;
					event->type = SDL_MOUSEMOTION;
					break;
				case SDLK_8:
					navikey[3] = 0;
					event->type = SDL_MOUSEMOTION;
					break;
				case SDLK_PLUS: // left mouse up
					event->type = SDL_MOUSEBUTTONUP;
					event->button.button = SDL_BUTTON_LEFT;
					break;		
				case SDLK_MINUS: //right mouse up
					event->type = SDL_MOUSEBUTTONUP;
					event->button.button = SDL_BUTTON_RIGHT;
					break;						
			
			}
	}
	
	if (navikey[0]) mouse_y-=mouse_speed;
	if (navikey[1]) mouse_y+=mouse_speed;
	if (navikey[2]) mouse_x-=mouse_speed;
	if (navikey[3]) mouse_x+=mouse_speed;
	
	if (mouse_x < 0) mouse_x = 0;
	if (mouse_y < 0) mouse_y = 0;
	
	if (mouse_x > 800) mouse_x = 800;
	if (mouse_y > 480) mouse_y = 480;
	
	if (full_screen){
		if (navikey[4]) full_screen_y-=mouse_speed;
		if (navikey[5]) full_screen_y+=mouse_speed;
		if (navikey[6]) full_screen_x-=mouse_speed;
		if (navikey[7]) full_screen_x+=mouse_speed;
		
		if (full_screen_x < 0) full_screen_x = 0;
		if (full_screen_y < 0) full_screen_y = 0;
		
		if (full_screen_x > 800-320) full_screen_x = 800-320;
		if (full_screen_y > 240) full_screen_y = 240;
		}
	
	event->button.x = mouse_x;
	event->button.y = mouse_y;
	//printf("%s-%i  x=%i,y=%i\n",__FILE__,__LINE__,event->button.x,event->button.y);
	//printf("%s-%i  event.type=%i\n",__FILE__,__LINE__,event->type);
	return r;
}

void mySDL_GetMouseState(int *x, int *y){
	*x = mouse_x;
	*y = mouse_y;
	//printf("%s-%i  x=%i,y=%i\n",__FILE__,__LINE__,x,y);
}

int mySDL_WaitEvent(SDL_Event *event){
	//SDL_Delay(10);
	//return 1;
}
*/
