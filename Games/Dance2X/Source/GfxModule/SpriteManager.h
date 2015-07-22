//! A class that manages all the sprites in the games

#ifndef SPRITEMANAGER_H
#define SPRITEMANAGER_H

#include <vector>
#include <list>
#include <queue>
#include <SDL/SDL.h>

namespace GfxModule
{
    // Forward declare some classes
    class Sprite;
    class SurfaceWrapper;

    class SpriteManager
    {
    private:
        //! Vector of all the Sprites pointers
        std::vector<Sprite *> _VectorOfSpritePointers;

        //! Queue of free Sprites pointers
        std::queue<Sprite *> _QueueOfFreeSpritePointers;

        //! Vector containing a list of sprites for each layer
        std::vector<std::list<Sprite *> > _VectorOfSpriteLayers;

    public:
        //! Standard constructor. Allocates memory for the sprites and Layer Surfaces
        SpriteManager(
            const unsigned int NumberOfLayers,
            const unsigned int NumberOfSprites
        );

        //! Standard destructor. Frees the memory used for the sprites and layer Surfaces
        ~SpriteManager(void);

        //! Returns a free sprite pointer or NULL if there are no free sprites
        Sprite * GetFreeSprite(void);

        //! Resets a sprite defaulting the position, width, surface etc
        void ClearSprite(Sprite *ASprite);

        //! Resets all sprites
        void ClearAllSprites(void);

        //! Frees and clears a sprite, declares that the sprite is no longer used
        // Note: May be unsafe to use, the pointer may not be from the _VectorOfSpritePointers
        void FreeSprite(Sprite *ASprite);

        //! Frees and clears all sprites
        void FreeAllSprites(void);

        //! Returns a pointer of the vector of lists of sprite layers
        const std::vector<std::list<Sprite *> > * GetVectorOfSpriteLayers(void)
        {
            return &_VectorOfSpriteLayers;
        }

        //! Must be called prior to display. Clears the lists in the vector and resorts all the sprites
        // TODO : When this is switched to a singleton, have the sprite call the manager to switch lists rather then
        //			having to do this every frame
        void SortSprites(void);
    };
}

#endif // SPRITEMANAGER_H

/*
History
=======
2006-07-24: Removing the SDL_Surfaces of the layers and replacing with a vector of lists
2006-06-20: Created file
*/
