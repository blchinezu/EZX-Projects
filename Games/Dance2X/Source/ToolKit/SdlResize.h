//resize.c - has all you need to get high-quality interpolated image scaling - up & down!
//please see resize.c for more information about who has contributed to this library

#ifndef RESIZE_FILTERS
#define RESIZE_FILTERS

#define BOX_FILTER      1
#define TRIANGLE_FILTER 2
#define BELL_FILTER     3
#define BSPLINE_FILTER  4
#define HERMITE_FILTER  5
#define MITCHELL_FILTER 6
#define LANCZOS_FILTER  7

#include <SDL/SDL.h>

//Here are the only 2 functions you need:
SDL_Surface* SDL_ResizeFactor(SDL_Surface *image, float scalefactor,    int filter);
SDL_Surface* SDL_ResizeXY    (SDL_Surface *image, int new_w, int new_h, int filter);

//Here are overloaded C++ versions, with filter default as high quality.
#ifdef __cplusplus
SDL_Surface* SDL_Resize(SDL_Surface *image, float scalefactor,    int filter = LANCZOS_FILTER );
SDL_Surface* SDL_Resize(SDL_Surface *image, int new_w, int new_h, int filter = LANCZOS_FILTER);
#endif

/*The passed-in surface is freed by SDL_Resize, so it works nicely to pass in surfaces
  as themselves:
  e.g. pic = SDL_ResizeFactor(pic, 0.75, 7); (or pic = SDL_Resize(pic, 0.75);)
  This will shrink pic to 75% of original size. No other cleanup necessary.
  Another good way to use it is on initialization:
  e.g. SDL_Surface *pic = SDL_ResizeXY(SDL_LoadBMP("mypic.bmp"),50,50,7);
  This will give you mypic.png at size 50x50(regardless of original dimensions)
*/

/*
Filters are as follows:
1 = box filter - fastest/ugliest.
2 =	triangle filter - possible visual anomalies
3 = bell filter - possible visual anomalies
4 = B_spline filter - here is where it starts to get good.
5 =	hermite filter - relatively fast, good quality
6 =	Mitchell filter - also speedy, good quality
7 = Lanczos3 filter - slowest, but by far best quality. Very sharp!
If filter is not specified, Lanczos3 will be selected by default
*/

/*The code should compile as either C or C++, without any fuss, except maybe you specifying
  which kind to compile it as... in MSVC++2005 it works to compile as c++ code, but I couldn't
  get it to compile as plain C, probably because I don't know what I'm doing! -Dave Olsen
*/

#endif
