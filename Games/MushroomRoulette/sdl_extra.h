#include <SDL.h>
#include "Rand.h"

SDL_Rect SDLE_Rect(const int x, const int y, const int w, const int h);
void SDLE_KeyGreenToAlpha(SDL_Surface* image);
Uint32 getpixel(SDL_Surface *surface, int x, int y);
void setpixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
void SDLE_Wait(const int ms);
void SDLE_ReplaceColour(SDL_Surface* surf, SDL_Rect area, Uint32 fcolour, Uint32 ncolour);
void SDLE_OpacityBlit(SDL_Surface* in, SDL_Surface* out, int x, int y, float opacity);
void XorCrypt(char* in, char* out, int effector);
void ClearFile(char* in);
void SDLE_FillRectAlpha(SDL_Surface* surf, const SDL_Rect* irect, const Uint32 icolour, const float alpha);
bool SDLE_LoadFIF(char* file, SDL_Surface** surf);


