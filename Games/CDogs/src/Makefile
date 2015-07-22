
binary=cdogs.ezx

### Variables ###
DESTDIR=../dist/ # might be handy for distro peeps
#DESTDIR=/ 
#PREFIX=/usr/local
PREFIX=.
BINDIR=$(PREFIX)
DATADIR=$(PREFIX)/data/

#DATADIR=./ # for windows
#PREFIX=./  # or self contained install

#CC=$(CXX)
#STRIP=gp2x-strip

#CFLAGS=-O2 -Wall `sdl-config --cflags` -std=c99 -funroll-loops
CF_OPTIMISE=-O2
CF_DEBUG=-ggdb -Wall
CFLAGS=${CF_OPTIMISE} ${CF_DEBUG} `sdl-config --cflags`

INCLUDES=-I./include -I./missions
LDFLAGS= $(SDK_LFLAGS) -lSDL_mixer -lsmpeg `sdl-config --libs` -lmikmod -lvorbisfile -lvorbisenc -lvorbis -logg -lm -lstdc++
DEFS=-DCDOGS_DATA_DIR=\"$(DATADIR)\"

# Windows data dir (current dir)
#DEFS=-DCDOGS_DATA_DIR=\"./\"

### Don't Touch These ###

dogs_OBJS = cdogs.o draw.o pics.o actors.o map.o sounds.o defs.o objs.o \
	    gamedata.o ai.o triggers.o input.o prep.o hiscores.o automap.o \
	    mission.o game.o mainmenu.o password.o files.o menu.o joystick.o \
	    sprcomp.o grafx.o blit.o text.o keyboard.o

dogsed_OBJS = cdogsed.o draw.o pics.o actors.o map.o sounds.o defs.o objs.o gamedata.o \
	      triggers.o input.o hiscores.o automap.o mission.o game.o ai.o charsed.o \
	      events.o joystick.o sprcomp.o grafx.o blit.o text.o keyboard.o name.o

### Targets ###

cdogs: $(dogs_OBJS)
	$(CC) $(dogs_OBJS) -o $(binary) $(LDFLAGS)
	$(STRIP) $(binary)
	mv $(binary) ../

# editor: $(dogsed_OBJS)
# 	gcc $(dogsed_OBJS) -o dogsed $(LDFLAGS)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) \
	$(DEFS) -c $<

install:
	export DESTDIR=$(DESTDIR) ; \
	export DATADIR=$(DATADIR) ; \
	export BINDIR=$(BINDIR) ; \
	./install.sh

clean:
	rm -f *.o ../$(binary) $(binary)

tidy:
	indent -kr -i8 *.c *.h
	rm *~

