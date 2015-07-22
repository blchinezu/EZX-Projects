#include "./SoundModuleWrapper.h"
#include "./ChunkWrapper.h"
#include "./SfxManager.h"
#include "./MusicManager.h"
#include "../Logger/Logger.h"

using namespace SoundModule;
using namespace LogModule;

SoundModuleWrapper::SoundModuleWrapper(const unsigned int NumberOfSfx, const unsigned int NumberOfChannels)
	: _SfxManager(0), _MusicManager(0)
{
	ErrLog << "<< Creating the SoundModuleWrapper >>\n";
	// Create a new Sfx Manager
	_SfxManager = new SfxManager(NumberOfSfx, NumberOfChannels);
	// Create a new Music Manager
	_MusicManager = new MusicManager();
	ErrLog << "<< Created the SoundModuleWrapper >>\n";
}

SoundModuleWrapper::~SoundModuleWrapper(void)
{
	ErrLog << "<< Destroying the SoundModuleWrapper >>\n";
	// Destroy the Music Manager
	delete _MusicManager;
	// Destroy the Sfx Manager
	delete _SfxManager;
	ErrLog << "<< Destroyed the SoundModuleWrapper >>\n";
}

/*
History
=======
2006-07-06: Added the functions to create the MusicManager
2006-06-30: Created the file and current functionality
*/
