#
# Copyright 2003 Milan Babuskov
#
# This file is part of Scalar (http://scalar.sourceforge.net).
#
# Scalar is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# Scalar is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Scalar in file COPYING; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#

# directory where the games are installed (/usr, /usr/local, ... or something)
ifndef PREFIX
#PREFIX=
endif

PROGRAM = scalar.ezx
OBJECTS = scalar.o hiscore.o njamfont.o njamutils.o soft-keyboard.o
CC = $(CXX)

# If you hate warnings under Cygwin use these flags instead
# MY_CFLAGS = -I/usr/local/include/SDL -Dmain=SDL_main -DWIN32 -Uunix -mno-cygwin
MY_CFLAGS = `sdl-config --cflags` $(SDK_CFLAGS)

%.o:	%.cpp
	$(CC) -O3 -c $(MY_CFLAGS) $< -o $@

all:	$(PROGRAM)

$(PROGRAM):	$(OBJECTS)
	$(CC) `sdl-config --libs` $(SDK_LFLAGS) -o$(PROGRAM) $(OBJECTS) -lSDL -lSDLmain -lSDL_image
	
	$(RM) *.o log.txt
	cp $(PROGRAM) $(PROGRAM).unstripped
	$(STRIP) $(PROGRAM)

clean:
	$(RM) *.o log.txt $(PROGRAM) $(PROGRAM).unstripped

run: $(PROGRAM)
	./$(PROGRAM) -w

