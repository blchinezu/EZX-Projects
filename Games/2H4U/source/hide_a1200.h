#include "SDL.h"
#include "SDL_mixer.h"

union SDL_Event;
int mySDL_PollEvent(SDL_Event *event);

extern unsigned int st_pause_ticks, st_pause_count;

unsigned int st_get_ticks(void);
void st_pause_ticks_init(void);
void st_pause_ticks_start(void);
void st_pause_ticks_stop(void);
bool st_pause_ticks_started(void);

void suspend_audio();
int resume_audio();
void close_audio( void );

