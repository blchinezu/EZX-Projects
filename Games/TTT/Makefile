CFLAGS=-Wall -O2 $(SDL_CFLAGS)
LIBS=$(SDL_LIBS)

SDL_CFLAGS=$(shell sdl-config --cflags)
SDL_LIBS=$(shell sdl-config --libs) -lSDL_mixer -lmad -lSDL_ttf -lfreetype


all:    ttt

clean:
	-rm ttt.ezx
	-rm ttt.o

ttt: ttt.o
	$(CC) $(CFLAGS) ttt.o -o ttt.ezx $(LIBS)
	rm ttt.o

ttt.o:       ttt.c
