# Project: BallGame
# Makefile created by Dev-C++ 4.9.9.2

CPP  = $(CXX)
#CC   = 
RES  = 
OBJ  = defineSprites.o drawSprites.o drawText.o dropBall.o gameLoop_Singleplayer.o intro.o main.o SDL_initialize.o SDL_input.o SDL_surfaces.o defineEnvironment.o killBalls.o levels.o defreeze.o animations.o $(RES)
LINKOBJ  = defineSprites.o drawSprites.o drawText.o dropBall.o gameLoop_Singleplayer.o intro.o main.o SDL_initialize.o SDL_input.o SDL_surfaces.o defineEnvironment.o killBalls.o levels.o defreeze.o animations.o $(RES)
LIBS =  $(SDK_LFLAGS) `sdl-config --libs` -lSDL -lSDLmain

#-L"C:/devkitGP2X/lib" -mwindows -lSDLmain -lSDL -static  


INCS =  -I/mnt/data/Motorola/development/dev-ezx-0.2.0/include/SDL -DQWS -D_REENTRANT -Dmain=SDL_main -I/mnt/data/Motorola/development/dev-ezx-0.2.0/include/
#-I"C:/devkitGP2X/include"
CXXINCS =  $(SDK_CFLAGS) `sdl-config --cflags` -DQWS -D_REENTRANT -Dmain=SDL_main
#-I"C:/devkitGP2X/lib/gcc/arm-linux/4.0.2/include"  -I"C:/devkitGP2X/include/c++/4.0.2/backward"  -I"C:/devkitGP2X/include/c++/4.0.2/arm-linux"  -I"C:/devkitGP2X/include/c++/4.0.2"  -I"C:/devkitGP2X/include" 
BIN  = BallGame.ezx
CXXFLAGS = $(CXXINCS)  
CFLAGS = $(INCS)   
RM = rm -f

.PHONY: all all-before all-after clean clean-custom

all: all-before $(BIN) all-after


clean: clean-custom
	${RM} $(OBJ) $(BIN)

$(BIN): $(OBJ)
	$(CPP) $(LINKOBJ) -o $(BIN) $(LIBS)
	$(STRIP) $(BIN)

defineSprites.o: defineSprites.cpp
	$(CPP) -c defineSprites.cpp -o defineSprites.o $(CXXFLAGS)

drawSprites.o: drawSprites.cpp
	$(CPP) -c drawSprites.cpp -o drawSprites.o $(CXXFLAGS)

drawText.o: drawText.cpp
	$(CPP) -c drawText.cpp -o drawText.o $(CXXFLAGS)

dropBall.o: dropBall.cpp
	$(CPP) -c dropBall.cpp -o dropBall.o $(CXXFLAGS)

gameLoop_Singleplayer.o: gameLoop_Singleplayer.cpp
	$(CPP) -c gameLoop_Singleplayer.cpp -o gameLoop_Singleplayer.o $(CXXFLAGS)

intro.o: intro.cpp
	$(CPP) -c intro.cpp -o intro.o $(CXXFLAGS)

main.o: main.cpp
	$(CPP) -c main.cpp -o main.o $(CXXFLAGS)

SDL_initialize.o: SDL_initialize.cpp
	$(CPP) -c SDL_initialize.cpp -o SDL_initialize.o $(CXXFLAGS)

SDL_input.o: SDL_input.cpp
	$(CPP) -c SDL_input.cpp -o SDL_input.o $(CXXFLAGS)

SDL_surfaces.o: SDL_surfaces.cpp
	$(CPP) -c SDL_surfaces.cpp -o SDL_surfaces.o $(CXXFLAGS)

defineEnvironment.o: defineEnvironment.cpp
	$(CPP) -c defineEnvironment.cpp -o defineEnvironment.o $(CXXFLAGS)

killBalls.o: killBalls.cpp
	$(CPP) -c killBalls.cpp -o killBalls.o $(CXXFLAGS)

levels.o: levels.cpp
	$(CPP) -c levels.cpp -o levels.o $(CXXFLAGS)

defreeze.o: defreeze.cpp
	$(CPP) -c defreeze.cpp -o defreeze.o $(CXXFLAGS)

animations.o: animations.cpp
	$(CPP) -c animations.cpp -o animations.o $(CXXFLAGS)
