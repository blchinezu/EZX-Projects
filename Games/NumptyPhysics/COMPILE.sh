#!/bin/bash

### GET CURRENT LOCATION
here=`echo $0 | sed -e 's/\/'$(basename $0)'$//'`
if [ "$here" = "`basename $0`" ] || [ "$here" = "." ]; then here="`pwd`"; fi

### LOAD ENV
if [ -f "/opt/toolchains/motoezx/setenv-a1200.sh" ]; then
	. /opt/toolchains/motoezx/setenv-a1200.sh sdl
fi

### COMPILE SHARED LIBRARY
if [ ! -f "$here/libbox2d.so" ]; then
	cd "$here/Box2D"
	make $@
fi

### COMPILE GAME BINARY
if [ -f "$here/libbox2d.so" ]; then
	cd "$here"
	make $@
fi
