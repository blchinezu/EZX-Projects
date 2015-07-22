CPP  = $(CXX)
#CC   = $(SDK_CC)
OBJ  = ezx/BaseEntity.o ezx/sdl_extra.o ezx/EntBox.o ezx/EntMushroom.o ezx/EntPlayer.o ezx/FontSystem.o ezx/GameWorld.o ezx/Rand.o ezx/Vector2D.o ezx/MushRlt.o ezx/Sound.o ezx/ContentManager.o ezx/HighScores.o ezx/Menu.o ezx/EntButterfly.o ezx/EntWell.o ezx/PathFinder.o ezx/EntHat.o ezx/EffectManager.o
LINKOBJ  = ezx/BaseEntity.o ezx/sdl_extra.o ezx/EntBox.o ezx/EntMushroom.o ezx/EntPlayer.o ezx/FontSystem.o ezx/GameWorld.o ezx/Rand.o ezx/Vector2D.o ezx/MushRlt.o ezx/Sound.o ezx/ContentManager.o ezx/HighScores.o ezx/Menu.o ezx/EntButterfly.o ezx/EntWell.o ezx/PathFinder.o ezx/EntHat.o ezx/EffectManager.o
LIBS =  `sdl-config --libs` $(SDK_LDFLAGS) -lSDLmain
INCS =  `sdl-config --cflags` $(SDK_CFLAGS)
CXXINCS =  $(INCS) 
BIN  = mr.ezx
CXXFLAGS = $(CXXINCS) -fexpensive-optimizations -O3
CFLAGS = $(INCS) -D_REENTRANT -fexpensive-optimizations -O3
RM = rm -f
RMDIR = rmdir
MKDIR = mkdir -p

.PHONY: all all-before all-after clean clean-custom

all: all-before $(BIN) all-after

all-before:
	$(MKDIR) "ezx"


clean: clean-custom
	${RM} $(OBJ) $(BIN)
	${RMDIR} ezx

$(BIN): $(OBJ)
	$(CPP) $(LINKOBJ) -o $@ $(LIBS)

ezx/BaseEntity.o: BaseEntity.cpp
	$(CPP) -c BaseEntity.cpp -o ezx/BaseEntity.o $(CXXFLAGS)

ezx/sdl_extra.o: sdl_extra.cpp
	$(CPP) -c sdl_extra.cpp -o ezx/sdl_extra.o $(CXXFLAGS)

ezx/EntBox.o: EntBox.cpp
	$(CPP) -c EntBox.cpp -o ezx/EntBox.o $(CXXFLAGS)

ezx/EntMushroom.o: EntMushroom.cpp
	$(CPP) -c EntMushroom.cpp -o ezx/EntMushroom.o $(CXXFLAGS)

ezx/EntPlayer.o: EntPlayer.cpp
	$(CPP) -c EntPlayer.cpp -o ezx/EntPlayer.o $(CXXFLAGS)

ezx/FontSystem.o: FontSystem.cpp
	$(CPP) -c FontSystem.cpp -o ezx/FontSystem.o $(CXXFLAGS)

ezx/GameWorld.o: GameWorld.cpp
	$(CPP) -c GameWorld.cpp -o ezx/GameWorld.o $(CXXFLAGS)

ezx/Rand.o: Rand.cpp
	$(CPP) -c Rand.cpp -o ezx/Rand.o $(CXXFLAGS)

ezx/Vector2D.o: Vector2D.cpp
	$(CPP) -c Vector2D.cpp -o ezx/Vector2D.o $(CXXFLAGS)

ezx/MushRlt.o: MushRlt.cpp
	$(CPP) -c MushRlt.cpp -o ezx/MushRlt.o $(CXXFLAGS)

ezx/Sound.o: Sound.cpp
	$(CPP) -c Sound.cpp -o ezx/Sound.o $(CXXFLAGS)

ezx/ContentManager.o: ContentManager.cpp
	$(CPP) -c ContentManager.cpp -o ezx/ContentManager.o $(CXXFLAGS)

ezx/HighScores.o: HighScores.cpp
	$(CPP) -c HighScores.cpp -o ezx/HighScores.o $(CXXFLAGS)

ezx/Menu.o: Menu.cpp
	$(CPP) -c Menu.cpp -o ezx/Menu.o $(CXXFLAGS)

ezx/EntButterfly.o: EntButterfly.cpp
	$(CPP) -c EntButterfly.cpp -o ezx/EntButterfly.o $(CXXFLAGS)

ezx/EntWell.o: EntWell.cpp
	$(CPP) -c EntWell.cpp -o ezx/EntWell.o $(CXXFLAGS)

ezx/PathFinder.o: PathFinder.cpp
	$(CPP) -c PathFinder.cpp -o ezx/PathFinder.o $(CXXFLAGS)

ezx/EntHat.o: EntHat.cpp
	$(CPP) -c EntHat.cpp -o ezx/EntHat.o $(CXXFLAGS)

ezx/EffectManager.o: EffectManager.cpp
	$(CPP) -c EffectManager.cpp -o ezx/EffectManager.o $(CXXFLAGS)
