#include "./SpriteManager.h"
#include "../Logger/Logger.h"
#include "./Sprite.h"
#include "./SurfaceWrapper.h"
#include "./SurfaceManager.h"

using namespace LogModule;
using namespace GfxModule;
using namespace std;

SpriteManager::SpriteManager(
    const unsigned int NumberOfLayers,
    const unsigned int NumberOfSprites
    )
{
    ErrLog << "<< Creating a Sprite Manager of " << NumberOfSprites << " Sprites and " << NumberOfLayers << " Layers >>\n";
    // Resize the vector of Sprite pointers to NumberOfSprites all NULLed
    _VectorOfSpritePointers.resize(NumberOfSprites, 0);
    // For every sprite in the vector, create a new sprite object
    vector<Sprite *>::iterator SpriteItr = _VectorOfSpritePointers.begin();
    while(SpriteItr != _VectorOfSpritePointers.end() )
    {
        // Add the pointer to the queue of free pointers
        *SpriteItr = new Sprite();
        (*SpriteItr)->SetSurfaceWrapper(BLANK_SURFACEWRAPPER);
        _QueueOfFreeSpritePointers.push(*SpriteItr);
        ++SpriteItr;
    }
    ErrLog << "Sprites Created\n";

	// Resize the Vector of sprite layers to the size passed
	_VectorOfSpriteLayers.resize(NumberOfLayers);

    ErrLog << "Layered Sprite lists Created\n" << "<< Created a Sprite Manager >>\n";
}

SpriteManager::~SpriteManager(void)
{
    ErrLog << "<< Destroying a Sprite Manager >>\n";
    // Create an iterator pointing to the beginning of the vector of Sprites
    vector<Sprite *>::iterator SpriteItr = _VectorOfSpritePointers.begin();
    // For every sprite in the vector
    while(SpriteItr != _VectorOfSpritePointers.end() )
    {
        // Free the memory
        delete (*SpriteItr);
        (*SpriteItr) = 0;
        ++SpriteItr;
    }
    ErrLog << "Freeed memory from all Sprites\n";

    // Clear the vector of sprite pointers
    _VectorOfSpritePointers.clear();
    // Clear the queue of sprite pointers
    while(!_QueueOfFreeSpritePointers.empty() )
    {
        _QueueOfFreeSpritePointers.pop();
    }

    // Clear the vector of Sprite layers
    _VectorOfSpriteLayers.clear();

    // Clear the vector of SurfaceWrapper pointers
    ErrLog << "Freeed memory from Surface Layers\n" << "<< Destroyed a Sprite Manager >>\n";
}

Sprite * SpriteManager::GetFreeSprite(void)
{
    ErrLog << "Getting a Free Sprite\n";
    // Check if there are any free sprites in the queue
    if(_QueueOfFreeSpritePointers.empty() )
    {
        // If not, return NULL
        ErrLog << "No more free sprites\n";
        return 0;
    }

    // Create a temp pointer to the Sprite that is at the front of the list
    Sprite * TempSpritePointer = _QueueOfFreeSpritePointers.front();
    // Dequeue the list of sprite pointers
    _QueueOfFreeSpritePointers.pop();
    // Return the free pointer
    return TempSpritePointer;
}

void SpriteManager::ClearSprite(Sprite *ASprite)
{
    ErrLog << "Clearing a Sprite\n";
    // Check that sprite is valid
    if(!ASprite)
    {
        ErrLog << "Sprite pointer is NULL\n";
        return;
    }
    // If so, reset all values of the sprite
    ASprite->SetXPosition(0);
    ASprite->SetYPosition(0);
    ASprite->SetSurfaceWrapper(BLANK_SURFACEWRAPPER);
    ASprite->SetPriority(0);
    ASprite->SetSpriteDimensions(0, 0);
    ASprite->SetSpriteAnimation(0, 0, 0);
    ErrLog << "Cleared a Sprite\n";
}

void SpriteManager::ClearAllSprites(void)
{
    ErrLog << "Clearing all Sprites\n";
    // Create an iterator pointing at the beginning of the vector of sprites
    vector<Sprite *>::iterator SpriteItr = _VectorOfSpritePointers.begin();
    // For every item in the vector of sprites
    while(SpriteItr != _VectorOfSpritePointers.end() )
    {
        // Clear the sprite
        (*SpriteItr)->SetXPosition(0);
        (*SpriteItr)->SetYPosition(0);
        (*SpriteItr)->SetSurfaceWrapper(BLANK_SURFACEWRAPPER);
        (*SpriteItr)->SetPriority(0);
        (*SpriteItr)->SetSpriteDimensions(0, 0);
		(*SpriteItr)->SetSpriteAnimation(0, 0, 0);
        ++SpriteItr;
    }
    ErrLog << "Cleared all Sprites\n";
}

void SpriteManager::FreeSprite(Sprite *ASprite)
{
    ErrLog << "Freeing a Sprite\n";
    // Check if the Sprite is valid
    if(!ASprite)
    {
        ErrLog << "Sprite pointer is NULL\n";
        return;
    }
    // If so, clear the sprite
    this->ClearSprite(ASprite);
    // Add to the queue of free sprites
    _QueueOfFreeSpritePointers.push(ASprite);
    ErrLog << "Freeed a Sprite\n";
}

void SpriteManager::FreeAllSprites(void)
{
    ErrLog << "Freeing all the Sprites\n";
    // Clear the queue of the pointers
    while(!_QueueOfFreeSpritePointers.empty() )
    {
        _QueueOfFreeSpritePointers.pop();
    }
    // Create an iterator pointing at the beginning of the vector of sprites
    vector<Sprite *>::iterator SpriteItr = _VectorOfSpritePointers.begin();
    // For every item in the vector of sprites
    while(SpriteItr != _VectorOfSpritePointers.end() )
    {
        // Clear the sprite
        (*SpriteItr)->SetXPosition(0);
        (*SpriteItr)->SetYPosition(0);
        (*SpriteItr)->SetSurfaceWrapper(BLANK_SURFACEWRAPPER);
        (*SpriteItr)->SetPriority(0);
        (*SpriteItr)->SetSpriteDimensions(0, 0);
		(*SpriteItr)->SetSpriteAnimation(0, 0, 0);
        // Push the sprite on to the queue of free sprites
        _QueueOfFreeSpritePointers.push( (*SpriteItr) );
        ++SpriteItr;
    }
    ErrLog << "Freeed all the Sprites\n";
}

void SpriteManager::SortSprites(void)
{
	// Clear the lists
	vector<list<Sprite *> >::iterator SpriteLayerItr = _VectorOfSpriteLayers.begin();
	const vector<list<Sprite *> >::iterator SpriteLayerItrEnd = _VectorOfSpriteLayers.end();
	while(SpriteLayerItr != SpriteLayerItrEnd)
    {
		(*SpriteLayerItr).clear();
		++SpriteLayerItr;
    }

    // Create an iterator pointing at the beginning of the vector of sprites
    vector<Sprite *>::iterator SpriteItr = _VectorOfSpritePointers.begin();
    const vector<Sprite *>::iterator SpriteItrEnd = _VectorOfSpritePointers.end();
    // For every sprite in the list
    while(SpriteItr != SpriteItrEnd )
    {
		_VectorOfSpriteLayers[ (*SpriteItr)->GetPriority() ].push_back( (*SpriteItr) );
        // Update the sprite
        (*SpriteItr)->Update();
        ++SpriteItr;
    }
}

/*
History
=======
2006-07-24: Removing the SDL_Surfaces of the layers and replacing with a vector of lists
2006-07-22: Attempt at speeding up the gfx Renderer by getting rid of as many if statements as possible
2006-06-22: Added in the function call to update the sprites.
Fixed error in constructor for creating the new surfaces
Cleared all the layers to black every frame
2006-06-20: Created file
*/
