//! A class to manage and process all special sound effects in the game

#ifndef SFXMANAGER_H
#define SFXMANAGER_H

#include <vector>
#include <queue>
#include <string>

namespace SoundModule
{
	// Forward declare some classes
	class ChunkWrapper;

	class SfxManager
	{
	private:
		//! Vector to hold all the ChunkWrappers
		std::vector<ChunkWrapper*> _VectorOfChunkPointers;

		//! Queue to hold all the free ChunkWrappers that can be used
		std::queue<ChunkWrapper*> _QueueOfFreeChunkPointers;

		//! Current volume
		int _CurrentVolume;

	public:
		//! Standard constructor is passed with the maximum number of Sfx. Opens NumberOfChannels channels
		SfxManager(const unsigned int NumberOfSfx, const unsigned int NumberOfChannels);

		//! Standard deconstructor. Frees the memory of all the ChunkWrappers
		~SfxManager(void);

		//! Create a Sfx from Filename if there are any ChunkWrappers free and returns it
		ChunkWrapper * CreateSfx(const std::string &Filename);

		//! Load a new Sfx from Filename using an existing ChunkWrapper
		ChunkWrapper * LoadSfx(ChunkWrapper * AChunkWrapper, const std::string &Filename);

		//! Clear a sound effect from a ChunkWrapper (releases the Mix_Chunk)
		void ClearSfx(ChunkWrapper * AChunkWrapper);

		//! Clear all sound effects from the vector of ChunkWrappers (releases the Mix_Chunk)
		void ClearAllSfx(void);

		//! Frees a ChunkWrapper by Clearing it and pushing it back in the queue (releases the Mix_Chunk)
		void FreeSfx(ChunkWrapper * AChunkWrapper);

		//! Frees all ChunkWrappers. Clears all the ChunkWrappers (releases the Mix_Chunk)
		void FreeAllSfx(void);

		//! Play a Sfx. Returns the channel that it is played in
		const int PlaySfx(ChunkWrapper * AChunkWrapper, const unsigned int NumberOfTimes = 0);

		//! Pause the sound from all the channels
		void PauseAllChannels(void);

		//! Resume playing sound from all the channels
		void ResumeAllChannels(void);

		//! Stop playing sound from all the channels
		void StopAllChannels(void);

		//! Lowers the volume of all chunks. Returns the new volume
		void VolumeDown(void);

		//! Raises the volume of all chunks. Returns the new volume
		void VolumeUp(void);

		//! Get the current volume
		const int GetCurrentVolume(void) const
		{
            return _CurrentVolume;
        }
	};
}

#endif // SFXMANAGER_H

/*
History
=======
2006-07-06: Adding volume mamangement
2006-06-30: Created file and implemented functionality
*/
