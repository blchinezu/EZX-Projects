//! A wrapper of all the graphic module managers

#ifndef GFXMODULE_H
#define GFXMODULE_H

#include <SDL/SDL.h>

namespace GfxModule
{
    // Forward declare classes
    class BGManager;
    class SpriteManager;
    class SurfaceManager;
    class FontManager;

    class GfxModuleWrapper
    {
    private:
        //! Pointer to the SurfaceManager
        SurfaceManager *_SurfaceManager;

        //! Pointer to the BGManager
        BGManager *_BGManager;

        //! Pointer to the SpriteManager
        SpriteManager *_SpriteManager;

        //! Pointer to the Font Manager
        FontManager *_FontManager;

        //! Number of layers to use
        const unsigned int _NumberOfLayers;

    public:
        //! Standard constructor. Creates an instance of each of the Gfx Managers
        GfxModuleWrapper(
            const unsigned int NumberOfLayers,
            const unsigned int NumberOfSprites,
            const unsigned int NumberOfFonts,
            const SDL_Surface * const ScreenSurface
        );

        //! Standard destructor. Frees all the GfxManagers
        ~GfxModuleWrapper(void);

        //! Returns a pointer to the Surface Manager
        SurfaceManager * GetSurfaceManager(void)
        {
            return _SurfaceManager;
        }

        //! Returns a pointer to the Background Manager
        BGManager * GetBGManager(void)
        {
            return _BGManager;
        }

        //! Returns a pointer to the Sprite Manager
        SpriteManager * GetSpriteManager(void)
        {
            return _SpriteManager;
        }

        //! Returns a pointer to the FontManager
        FontManager * GetFontManager(void)
        {
			return _FontManager;
		}

        //! Returns the number of layers
        const unsigned int GetNumberOfLayers(void) const
        {
            return _NumberOfLayers;
        }
    };
}

#endif // GFXMODULE_H

/*
History
=======
2006-07-02: Added support for the Font Manager
2006-06-21: Added the Sprite Manager. Added Header guards
2006-06-15: Created file and added the functionality
*/
