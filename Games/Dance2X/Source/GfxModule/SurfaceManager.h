//! A class to manage and create SDL_Surfaces

#ifndef SURFACEMANAGER_H
#define SURFACEMANAGER_H

#include <vector>
#include <queue>
#include <string>

namespace GfxModule
{
    // Forward declaration of a class
    class SurfaceWrapper;

	class SurfaceManager
	{
	private:
		//! List of pointers to SDL_Surfaces
		std::vector<SurfaceWrapper*> _VectorOfSurfacePointers;

		//! Queue of free Surface Wrapper pointers
		std::queue<SurfaceWrapper*> _QueueOfFreeSurfacePointers;

	public:
		//! Standard Construtor. Creates the vector to be of size NumberOfSurfaces
		//! All Surfaces are free and added to the Queue
		SurfaceManager(const unsigned int NumberOfSurfaces = 128);

		//! Standard Destructor. Frees up any reminder SDL_Surfaces in the list
		~SurfaceManager(void);

		//! Creates a new SDL surface with the a SurfaceWrapper that is freeusing the image Filename
		//! Returns a Surface Wrapper pointer or NULL if none
		SurfaceWrapper * CreateSurface(const std::string &Filename);

        //! Get a free Surface
        //! Returns a Surface Wrapper pointer or NULL if none
        // Note: May be unsafe to use, the pointer may not be from the VectorOfSurfacePointers
        SurfaceWrapper * GetFreeSurface(void);

		//! Loads a new image of Filename to the Surface of the Wrapper pointer passed to it
		//! Returns a SurfaceWrapper
		SurfaceWrapper * LoadSurface(SurfaceWrapper *ASurfaceWrapper, const std::string &Filename);

		//! Clears/Resets a surface
		//! Frees the surface
		void ClearSurface(SurfaceWrapper *ASurfaceWrapper);

		//! Clears/Resets all surfaces in the list
		void ClearAllSurfaces(void);

		//! Frees the surface of the Surface Wrapper and adds it to the queue of free pointers
		//! Returns the SurfaceWrapper
		void FreeSurface(SurfaceWrapper *ASurfaceWrapper);

		//! Frees all the surfaces of the Surface Wrappers in the vector and adds it to the queue of free pointers
		void FreeAllSurfaces(void);
	};
};

//! Evil global: A transparent 1 pixel by 1 pixel square
extern GfxModule::SurfaceWrapper * BLANK_SURFACEWRAPPER;

#endif // SURFACEMANAGER_H

/*
History
=======
2006-07-22: Attempt at speeding up the gfx Renderer by getting rid of as many if statements as possible
2006-06-17: Major redeisgn of the graphics module. Surface Manager is heavily affected
2006-06-15: Moved SurfaceWrapper to a new file
2006-06-09: Created file and class
*/
