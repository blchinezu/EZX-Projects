//! A class to manage the Background layers. Layers start from 0 (Furthest) to X (Nearest)
#ifndef BGMANAGER_H
#define BGMANAGER_H

#include <vector>
#include <string>

namespace GfxModule
{
	// Forward declare classes
	class SurfaceWrapper;
	class BGObject;

	class BGManager
	{
	private:
		//! Vector of Background objects. The size should be equal to the number of layers
		std::vector<BGObject *> _VectorOfObjectPointers;

	public:
		//! Standard constructor. Resizes the vector equal to the number of layers.
		BGManager(const unsigned int NumberOfLayers);

		//! Standard destructor. Frees memory taken up by the background object
		~BGManager(void);

		//! Get the number of layers which is defined by the size of the Vector
		const unsigned int GetNumberOfLayers (void) const
		{
			return _VectorOfObjectPointers.size();
		}

		//! Get the Background Wrapper to the layer number
		BGObject * GetBGObject(const unsigned int LayerNumber);

		//! Get a pointer to the vector of BGObjects
		const std::vector<BGObject *> * const GetVectorOfBGObjects(void) const
		{
            return &_VectorOfObjectPointers;
        }

        //! Clear a background object
        void ClearBGObject(BGObject * ABGOBject);

        //! Clear/Reset all the background pointers
        void ClearAllBGObjects(void);
	};
}

#endif // BGMANAGER_H

/*
History
=======
2006-06-17: Changed the functionality of the entire class. See diagram for details. No longer need the Surface Manager
2006-06-15: Moved the class out of BGManager to BGObject.h.
2006-06-12: Added a method to return a layer's SurfaceWrapper.
2006-06-10: Created class and files
*/
