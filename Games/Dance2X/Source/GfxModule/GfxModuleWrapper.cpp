#include "./SurfaceManager.h"
#include "./GfxModuleWrapper.h"
#include "./BGManager.h"
#include "./SpriteManager.h"
#include "./FontManager.h"

#include "../Logger/Logger.h"

using namespace GfxModule;
using namespace LogModule;

GfxModuleWrapper::GfxModuleWrapper(
    const unsigned int NumberOfLayers,
    const unsigned int NumberOfSprites,
    const unsigned int NumberOfFonts,
    const SDL_Surface * const ScreenSurface
    ) :
    _SurfaceManager(0),
    _BGManager(0),
    _SpriteManager(0),
    _NumberOfLayers(NumberOfLayers)
{
    ErrLog << "<< Creating a GfxModuleWrapper >>\n";

    // Create new instances of the Managers
    _SurfaceManager = new SurfaceManager();
    _BGManager 		= new BGManager(NumberOfLayers);
    _SpriteManager 	= new SpriteManager(NumberOfLayers, NumberOfSprites);
    _FontManager	= new FontManager(NumberOfFonts, ScreenSurface);

    ErrLog << "<< Created a GfxModuleWrapper >>\n";
}

GfxModuleWrapper::~GfxModuleWrapper(void)
{
    ErrLog << "<< Destroying a GfxModuleWrapper >>\n";

    // Check if the managers exist and free up the memory
    if(_BGManager)
        delete _BGManager;

    if(_SurfaceManager)
        delete _SurfaceManager;

    if(_SpriteManager)
        delete _SpriteManager;

	if(_FontManager)
		delete _FontManager;

    ErrLog << "<< Destroyed a GfxModuleWrapper >>\n";
}

/*
History
=======
2006-07-02: Added support for the Font Manager
2006-06-21: Added the Sprite Manager
2006-06-15: Created file and added the functionality
*/
