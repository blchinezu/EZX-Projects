//! A Wrapper to hold all the sound managers

#ifndef SOUNDMODULEWRAPPER_H
#define SOUNDMODULEWRAPPER_H

namespace SoundModule
{
	// Forward declare classes
	class SfxManager;
	class MusicManager;

	class SoundModuleWrapper
	{
	private:
		//! Pointer to the Sfx Manager
		SfxManager *_SfxManager;

		//! Pointer to the Music Manager
		MusicManager *_MusicManager;

	public:
		//! Standard constructor, creates the managers. The Number of Sfx and Channels are passed
		SoundModuleWrapper(const unsigned int NumberOfSfx, const unsigned int NumberOfChannels);

		//! Standard deconstructor, destroys the managers
		~SoundModuleWrapper(void);

		//! Get the SfxManager
		SfxManager * GetSfxManager(void)
		{
			return _SfxManager;
		}

		//! Get the MusicManager
		MusicManager * GetMusicManager(void)
		{
			return _MusicManager;
		}
	};
}

#endif // SOUNDMODULEWRAPPER_H

/*
History
=======
2006-06-30: Created the file and current functionality
*/
