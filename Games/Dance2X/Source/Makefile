# --- your build environment
# - EZX crosscompile with Cygwin

#ROOT_DIR=/home/ezx/root
#EZXTOOLCHAIN = 
EZXCC     = $(SDK_CC)
EZXCXX    = $(SDK_CXX)
EZXSTRIP  = $(SDK_STRIP)
EZXCFLAGS = $(SDK_CFLAGS) `sdl-config --cflags`
EZXCXXFLAGS = $(EZXCFLAGS)
EZXLIBS   = -msoft-float -lSDL -lSDLmain -lSDL_mixer -lmad -lSDL_ttf -lfreetype -lSDL_image
EZXINCLUDE = 
EZXSDLCONFIG = sdl-config

# --- source code target

#
TARGET = ../dance2x.ezx

CFLAGS = -O2 -Wall
CXXFLAGS = -DGP2X -Wall -O3 

EZXOBJS = DanceMachine/DanceMachine.o DanceMachine/DwiParser.o \
							Game/Main.o Game/PlayState.o Game/RatingState.o Game/SongSelectionState.o Game/SplashPageState.o Game/TitleScreenState.o \
							GfxModule/BGManager.o GfxModule/BGObject.o GfxModule/FontManager.o GfxModule/GfxModuleWrapper.o GfxModule/Sprite.o GfxModule/SpriteManager.o GfxModule/SurfaceManager.o \
							InputModule/Input.o \
							Kernel/Kernel.o \
							Logger/Logger.o \
							SoundModule/MusicManager.o SoundModule/SfxManager.o SoundModule/SoundModuleWrapper.o \
							StateModule/GameStateManager.o \
							Timer/FpsCounter.o Timer/SdlTimer.o \
							ToolKit/SDL_rotozoom.o

# ----- suffix
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $<


# ----- target


all : ezx

ezx : CC = $(EZXCC)
ezx : CXX = $(EZXCXX)
ezx : OBJS = $(EZXOBJS)
ezx : LIBS += $(EZXLIBS)
ezx : LIBS += $(shell $(EZXSDLCONFIG) --libs)
ezx : CFLAGS += $(EZXCFLAGS)
ezx : CFLAGS += $(shell $(EZXSDLCONFIG) --cflags)
ezx : CXXFLAGS += $(EZXCXXFLAGS)
ezx : CXXFLAGS += $(shell $(EZXSDLCONFIG) --cflags)
ezx : INCLUDE = $(EZXINCLUDE)
ezx : ezxobjs
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)
	$(EZXSTRIP) $(TARGET)

ezxobjs : $(EZXOBJS)

clean:
		rm -f $(EZXOBJS)
		rm -f $(TARGET)


