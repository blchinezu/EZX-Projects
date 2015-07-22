CFLAGS=-O2 -Wall $(SDL_CFLAGS)

SDL_CFLAGS=`sdl-config --cflags`
SDL_LIBS=`sdl-config --libs` -lSDL_mixer -lSDL_image -lmad

all:	teetertorture

clean:
	-rm teetertorture.ezx
	-rm obj/*

teetertorture:	obj/teetertorture.o
	$(CC) $(CFLAGS) obj/teetertorture.o -o teetertorture.ezx $(SDL_LIBS)

obj/teetertorture.o:	src/teetertorture.c
	$(CC) $(CFLAGS) src/teetertorture.c -c -o obj/teetertorture.o

