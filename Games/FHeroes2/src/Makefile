# makefile
# project: Free Heroes2
#

TARGET := fheroes2
SDL_LIBS := $(shell sdl-config --libs)
SDL_FLAGS := $(shell sdl-config --cflags)

CFLAGS := $(CFLAGS) -Wall -fsigned-char
LIBS := -lz -lSDL_ttf -lfreetype -lSDL_mixer

CFLAGS := $(SDL_FLAGS) $(CFLAGS)
LIBS := $(SDL_LIBS) $(LIBS)

PLATFORM := unix

include Makefile.$(PLATFORM)

export CXX AR LINK WINDRES CFLAGS LIBS PLATFORM

.PHONY: clean

all:
ifndef WITHOUT_XML
	$(MAKE) -C xmlccwrap
endif
	$(MAKE) -C engine
	$(MAKE) -C dist

clean:
ifndef WITHOUT_XML
	$(MAKE) -C xmlccwrap clean
endif
ifdef WITH_TOOLS
	$(MAKE) -C tools clean
endif
	$(MAKE) -C dist clean
	$(MAKE) -C engine clean
