#include "../Logger/Logger.h"
#include "./SurfaceWrapper.h"
#include "./BGManager.h"
#include "./BGObject.h"
#include "./SurfaceManager.h"

using namespace LogModule;
using namespace GfxModule;

BGManager::BGManager(const unsigned int NumberOfLayers)
{
	ErrLog << "<< Creating the BGManager with " << NumberOfLayers << " layers >>\n";
	// Create a number of BGObjects equal to the number of layers
	_VectorOfObjectPointers.resize(NumberOfLayers);
	std::vector<BGObject *>::iterator Itr = _VectorOfObjectPointers.begin();
	while(Itr != _VectorOfObjectPointers.end() )
	{
		(*Itr) = new BGObject();
		(*Itr)->SetSurfaceWrapper(BLANK_SURFACEWRAPPER);
		++Itr;
	}
	ErrLog << "<< Created the BGManager >>\n";
}

BGManager::~BGManager(void)
{
	ErrLog << "<< Destroying the BGManager >>\n";
	// Free the memory used
	std::vector<BGObject *>::iterator Itr = _VectorOfObjectPointers.begin();

	while(Itr != _VectorOfObjectPointers.end() )
	{
		delete (*Itr);
		++Itr;
	}

	ErrLog << "<< Destroyed the BGManager >>\n";
}

BGObject * BGManager::GetBGObject(const unsigned int LayerNumber)
{
	if(LayerNumber >= _VectorOfObjectPointers.size())
	{
		ErrLog << "Getting BGLayer number " << LayerNumber << " does not exist\n";
		return 0;
	}

	return _VectorOfObjectPointers[LayerNumber];
}

void BGManager::ClearBGObject(BGObject * ABGObject)
{
	// Check if the object exists
	if(ABGObject)
	{
		// If so, clear the object
		ABGObject->SetXYPosition(0, 0);
		ABGObject->SetVisibility(true);
		ABGObject->SetSurfaceWrapper(BLANK_SURFACEWRAPPER);
		ErrLog << "Cleared a BG Object\n";
	}
}

void BGManager::ClearAllBGObjects(void)
{
	// Reset all the object pointers
	std::vector<BGObject *>::iterator Itr = _VectorOfObjectPointers.begin();

	while(Itr != _VectorOfObjectPointers.end() )
	{
		(*Itr)->SetXYPosition(0, 0);
		(*Itr)->SetVisibility(true);
		(*Itr)->SetSurfaceWrapper(BLANK_SURFACEWRAPPER);
		++Itr;
        ErrLog << "Cleared a BG Object\n";
	}
}

/*
History
=======
2006-07-22: Attempt at speeding up the gfx Renderer by getting rid of as many if statements as possible
2006-06-30: Used the vector.resize() method to populate the vector instead of the standard puhs_back
2006-06-17: Changed the functionality of the entire class. See diagram for details
2006-06-16: Made changes to some of the functions due to the BGObjects revamped code
2006-06-12: Added a method to return a layer's SurfaceWrapper.
2006-06-10: Created class and files
*/
