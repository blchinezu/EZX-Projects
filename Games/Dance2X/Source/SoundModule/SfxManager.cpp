#include "./ChunkWrapper.h"
#include "./SfxManager.h"
#include "../Logger/Logger.h"

using namespace LogModule;
using namespace SoundModule;
using namespace std;

// Constants to handle the volume levels
const int MIN_VOLUME_LEVEL = 0;
const int MED_VOLUME_LEVEL = 64;
const int MAX_VOLUME_LEVEL = 128;

SfxManager::SfxManager(const unsigned int NumberOfSfx, const unsigned int NumberOfChannels) : _CurrentVolume(MED_VOLUME_LEVEL)
{
    ErrLog << "<< Creating SfxManager >>\n";

    // Open the number of channels
    ErrLog << "Opening " << NumberOfChannels << " Channels\n";
    unsigned int NumberOfChannelsOpened = Mix_AllocateChannels(NumberOfChannels);
    ErrLog << "Opened " << NumberOfChannelsOpened << " Channels\n";

    // Resize the vector to hold the ChunkWrappers
    _VectorOfChunkPointers.resize(NumberOfSfx);
    // Create new ChunkWrappers for every item in the vector
    vector<ChunkWrapper *>::iterator Itr = _VectorOfChunkPointers.begin();
    _CurrentVolume = 2;
    while(Itr != _VectorOfChunkPointers.end() )
    {
        (*Itr) = new ChunkWrapper();
        // Push all the ChunkWrappers on to the Free Queue
        _QueueOfFreeChunkPointers.push(*Itr);
        if( (*Itr)->_MixChunk) (*Itr)->_MixChunk->volume = _CurrentVolume;
        ++Itr;
    }

    ErrLog << "<< Created SfxManager >>\n";
}

SfxManager::~SfxManager(void)
{
    ErrLog << "<< Destroying the Sfx Manager >>\n";

    // Stop all channels
    this->StopAllChannels();

    // Clear the queue
    while(!_QueueOfFreeChunkPointers.empty() )
    {
        _QueueOfFreeChunkPointers.pop();
    }

    // Free the memory used by the ChunkWrappers in the Vector
    vector<ChunkWrapper *>::iterator Itr = _VectorOfChunkPointers.begin();
    while(Itr != _VectorOfChunkPointers.end() )
    {
        delete (*Itr);
        ++Itr;
    }

    // Clear the vectors
    _VectorOfChunkPointers.clear();
    ErrLog << "<< Destroyed the Sfx Manager >>\n";
}

ChunkWrapper * SfxManager::CreateSfx(const string &Filename)
{
    ErrLog << "Creating a new Sfx Chunk with Filename: " << Filename << "\n";
    // Check if there are any free ChunkWrappers
    if(_QueueOfFreeChunkPointers.empty() )
    {
        ErrLog << "No free Sfx Chunk left\n";
        // If not, return 0
        return 0;
    }
    // Else
    // Create a temp pointer to the front of the queue
    ChunkWrapper * AChunkWrapper = _QueueOfFreeChunkPointers.front();
    // Load the sound from the file
    AChunkWrapper->_MixChunk = Mix_LoadWAV(Filename.c_str() );

    // Check if it has loaded correctly
    if(!AChunkWrapper->_MixChunk)
    {
        ErrLog << "Loading Sfx file failed\n";
        // If not, return 0
        return 0;
    }
    // Else
    // Pop the queue
    _QueueOfFreeChunkPointers.pop();
    // Else change the volume to be at the same level as the chunk
    AChunkWrapper->_MixChunk->volume = _CurrentVolume;
    ErrLog << "Created a Sfx Chunk\n";
    // Return the temp pointer to the ChunkWrapper
    return AChunkWrapper;
}

ChunkWrapper * SfxManager::LoadSfx(ChunkWrapper * AChunkWrapper, const string &Filename)
{
    ErrLog << "Loading Sfx to a Sfx Chunk with Filename: " << Filename << "\n";
    // Check if the ChunkWrapper is valid
    if(!AChunkWrapper)
    {
        // If not, return 0
        ErrLog << "ChunkWrapper not valid\n";
        return 0;
    }

    // If so, free the Mix_Chunk
    ErrLog << "Freeing the Mix_Chunk\n";
    Mix_FreeChunk(AChunkWrapper->_MixChunk);

    // Load the sound from the file
    AChunkWrapper->_MixChunk = Mix_LoadWAV(Filename.c_str() );

    // Check if it has loaded correctly
    if(!AChunkWrapper->_MixChunk)
    {
        ErrLog << "Loading Sfx file failed\n";
        // If not, return 0
        return 0;
    }
    // Else change the volume to be at the same level as the chunk
    AChunkWrapper->_MixChunk->volume = _CurrentVolume;
    ErrLog << "Loaded Sfx to a Sfx Chunk\n";
    // Return the temp pointer to the ChunkWrapper
    return AChunkWrapper;
}

void SfxManager::ClearSfx(ChunkWrapper * AChunkWrapper)
{
    ErrLog << "Clearing a Chunk\n";
    // Check that the ChunkWrapper is valid
    if(AChunkWrapper)
    {
        // If so, free the Mix_Chunk
        Mix_FreeChunk(AChunkWrapper->_MixChunk);
        ErrLog << "Cleared a Chunk\n";
    }
    // Else
    {
        ErrLog << "Chunk not valid\n";
    }
}

void SfxManager::ClearAllSfx(void)
{
    ErrLog << "Clearing all Chunks\n";
    // Create an interator
    vector<ChunkWrapper *>::iterator Itr = _VectorOfChunkPointers.begin();
    // Go through all the ChunkWrappers in the vector
    while(Itr != _VectorOfChunkPointers.end() )
    {
        Mix_FreeChunk( (*Itr)->_MixChunk);
        ++Itr;
    }
    // Free all Mix_Chunks
    ErrLog << "Cleared all Chunks\n";
}

void SfxManager::FreeSfx(ChunkWrapper * AChunkWrapper)
{
    ErrLog << "Freeing a Chunk\n";
    // Check the Chunk is valid
    if(!AChunkWrapper)
    {
		// If not, end
		ErrLog << "Chunk not valid\n";
		return;
	}
    // Else
    // Free the Mix_Chunk
	Mix_FreeChunk(AChunkWrapper->_MixChunk);
	AChunkWrapper->_MixChunk = 0;
    // Add to the queue of free Chunks
	_QueueOfFreeChunkPointers.push(AChunkWrapper);
    ErrLog << "Freeed a Chunk\n";
}

void SfxManager::FreeAllSfx(void)
{
	ErrLog << "Freeing all Chunks\n";
	// Clear the queue
    while(!_QueueOfFreeChunkPointers.empty() )
    {
        _QueueOfFreeChunkPointers.pop();
    }
	// Create an iterator
	vector<ChunkWrapper *>::iterator Itr = _VectorOfChunkPointers.begin();
	// Go through all the Chunks in the vector
	while(Itr != _VectorOfChunkPointers.end() )
	{
		// Clear the Mix_Chunk
		Mix_FreeChunk( (*Itr)->_MixChunk );
		(*Itr)->_MixChunk = 0;
		// Add to the Queue of Free Chunks
		_QueueOfFreeChunkPointers.push(*Itr);
		++Itr;
	}
	ErrLog << "Freeed all Chunks\n";
}

const int SfxManager::PlaySfx(ChunkWrapper * AChunkWrapper, const unsigned int NumberOfTimes)
{
	// Check that the ChunkWrapper is valid
	if(!AChunkWrapper)
	{
		// If not, end
		ErrLog << "Playing Chunk not valid\n";
		return -1;
	}
	// Else, play the sound NumberOftimes number of times on any free channel
	int ChannelPlayed = Mix_PlayChannel(-1, AChunkWrapper->_MixChunk, NumberOfTimes);
	//ErrLog << "Playing a sound effect on channel " << ChannelPlayed << ", "
	//	<< NumberOfTimes << " number of times\n";
	// Return the channel that it was played
	return ChannelPlayed;
}

void SfxManager::PauseAllChannels(void)
{
	// Pause each channel
	Mix_Pause(-1);
	ErrLog << "Paused all Channels\n";
}

void SfxManager::ResumeAllChannels(void)
{
	// Resume each channel
	Mix_Resume(-1);
	ErrLog << "Resumed all Channels\n";
}

void SfxManager::StopAllChannels(void)
{
	// Stop each channel
 	Mix_HaltChannel(-1);
	ErrLog << "Stopped all Channels\n";
}

void SfxManager::VolumeDown(void)
{
    // Check the current volume is above 0
    if(_CurrentVolume > MIN_VOLUME_LEVEL)
    {
        // Change the volume level down a notch
        _CurrentVolume -= 2;
        // For all Chunk wrappers
        vector<ChunkWrapper *>::iterator Itr = _VectorOfChunkPointers.begin();
        while(Itr != _VectorOfChunkPointers.end() )
        {
            // Check if a Chunk exists
            if( (*Itr)->_MixChunk)
            {
                // If so, change the volume
                (*Itr)->_MixChunk->volume = _CurrentVolume;
            }
            ++Itr;
        }
    }
}

void SfxManager::VolumeUp(void)
{
    // Check the current volume is below 128
    if(_CurrentVolume < MAX_VOLUME_LEVEL)
    {
        // Change the volume level up a notch
        _CurrentVolume += 2;
        // For all Chunk wrappers
        vector<ChunkWrapper *>::iterator Itr = _VectorOfChunkPointers.begin();
        while(Itr != _VectorOfChunkPointers.end() )
        {
            // Check if a Chunk exists
            if( (*Itr)->_MixChunk)
            {
                // If so, change the volume
                (*Itr)->_MixChunk->volume = _CurrentVolume;
            }
            ++Itr;
        }
    }
}

/*
History
=======
2006-07-15: Fixed bug with FreeChunk which was not NULLing the pointer.
2006-07-06: Adding volume mamangement and forget to increment an iterator in FreeAllSfx
2006-07-02: Removed Error Log comments when playing a sound
2006-07-01: Fixed error in freeing all Sfx
2006-06-30: Created file and implemented functionality
*/
