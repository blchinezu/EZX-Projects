echo -e "$CXX main.cpp -lSDL -lSDLmain `sdl-config --cflags` $CFLAGS `sdl-config --libs` $LFLAGS -lpthread -lSDL_ttf -lfreetype -o snake.ezx\n"
$CXX main.cpp -lSDL -lSDLmain `sdl-config --cflags` $CFLAGS `sdl-config --libs` $LFLAGS -lpthread -lSDL_ttf -lfreetype -o snake.ezx

