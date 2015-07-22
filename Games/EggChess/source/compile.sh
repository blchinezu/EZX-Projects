#!/bin/bash

target=../eggchess.ezx

sge_list='sge_blib.cpp sge_bm_text.cpp sge_collision.cpp sge_misc.cpp sge_primitives.cpp sge_rotation.cpp sge_shape.cpp sge_surface.cpp sge_textpp.cpp sge_tt_text.cpp'
obj_list='eggchess.o  sge/sge_blib.o sge/sge_bm_text.o sge/sge_collision.o sge/sge_misc.o sge/sge_primitives.o sge/sge_rotation.o sge/sge_shape.o sge/sge_surface.o sge/sge_textpp.o sge/sge_tt_text.o'

compile() {
	echo "$CC `sdl-config --cflags` $SDK_CFLAGS -c $1"
	$CC `sdl-config --cflags` $SDK_CFLAGS -Isge -c $1
	echo '---------------------------------------'
}

cd sge; for file in $sge_list; do compile $file; done; cd ..
compile 'eggchess.cpp'
echo '---------------------------------------'

echo "$CC -o $target $obj_list `sdl-config --libs` -lSDL -lSDLmain -lSDL_image $SDK_LDFLAGS"
$CC -o $target $obj_list `sdl-config --libs` -lSDL -lSDLmain -lSDL_image $SDK_LDFLAGS
echo '---------------------------------------'

echo "$STRIP $target";		$STRIP $target
echo "rm -f $obj_list";		rm -f $obj_list

