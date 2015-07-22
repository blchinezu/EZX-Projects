#include "hide_a1200.h"

// Phone event support for motoezx
int mySDL_PollEvent(SDL_Event *event)
{
  int r = SDL_PollEvent(event);
  if(!r) return 0;
  if(event->type == SDL_ACTIVEEVENT){
    if(event->active.state == SDL_APPINPUTFOCUS && !event->active.gain){
      bool st_pause = st_pause_ticks_started();
      if(!st_pause) st_pause_ticks_start();
      suspend_audio();
      for(;;){
        printf("wait\n");
        r = SDL_WaitEvent(event);
        printf("wait done\n");
        if(!r) continue;
        if(event->type == SDL_QUIT) return 1;
        if(event->type != SDL_ACTIVEEVENT) continue;
        if(event->active.state == SDL_APPINPUTFOCUS && event->active.gain){
          resume_audio();
          if(!st_pause) st_pause_ticks_stop();
          return 1;
        }
      }
    }
  }
  
  return r;
}

unsigned int st_pause_ticks, st_pause_count;

unsigned int st_get_ticks(void) {
	if (st_pause_count != 0)
		return /*SDL_GetTicks()*/ - st_pause_ticks /*- SDL_GetTicks()*/ + st_pause_count;
	else
		return SDL_GetTicks() - st_pause_ticks;
}

void st_pause_ticks_init(void) {
	st_pause_ticks = 0;
	st_pause_count = 0;
}

void st_pause_ticks_start(void) {
	if (st_pause_count == 0)
		st_pause_count = SDL_GetTicks();
}

void st_pause_ticks_stop(void) {
	if (st_pause_count == 0)
		return;
	st_pause_ticks += SDL_GetTicks() - st_pause_count;
	st_pause_count = 0;
}

bool st_pause_ticks_started(void) {
	if (st_pause_count == 0)
		return false;
	else
		return true;
}


/* --- SUSPEND THE AUDIO DEVICE --- */

void suspend_audio()
{
  Mix_SuspendAudio();
}


/* --- RESUME THE AUDIO DEVICE --- */

int resume_audio()
{
  int r = Mix_ResumeAudio();
  if(r == -1){
    close_audio();
    return -1;
  }
}

/* --- CLOSE THE AUDIO DEVICE --- */

void close_audio( void )
{
      Mix_CloseAudio();
}