#include "./MusicManager.h"
#include "../Logger/Logger.h"

using namespace SoundModule;
using namespace LogModule;

// Constants to handle the volume levels
const int MIN_VOLUME_LEVEL = 0;
const int MED_VOLUME_LEVEL = 64;
const int MAX_VOLUME_LEVEL = 128;

// Declare static variables
bool MusicManager::_MusicHasStopped = true;

MusicManager::MusicManager(void) : _MixMusic(0), _CurrentVolume(MED_VOLUME_LEVEL)
{
    ErrLog << "<< Creating a Music Manager >>\n";
    
    // Set start volume
		_CurrentVolume = 5;
		Mix_VolumeMusic(_CurrentVolume);

    ErrLog << "<< Created a Music Manager >>\n";
}

MusicManager::~MusicManager(void)
{
    ErrLog << "<< Destroying a Music Manager >>\n";
    // Check if we have Mix Music data
    if(_MixMusic)
    {
        // If so, stop and free the data
        this->FreeMusic();
    }
    ErrLog << "<< Destroyed a Music Manager >>\n";
}

void MusicManager::LoadMusic(const std::string &Filename)
{
    ErrLog << "Loading new Music file: " << Filename << "\n";
    // Check if there is a current Music file loaded
    if(_MixMusic)
    {
        // If so, stop and free the music
        this->FreeMusic();
    }

    // Attempt to load the new music file
    _MixMusic = Mix_LoadMUS(Filename.c_str() );

    // Check for errors
    if(!_MixMusic)
    {
        ErrLog << "Error loading Music file\n";
    }
    else
    {
        // Set the volume
        Mix_VolumeMusic(_CurrentVolume);
        // Hook a function to SDL_mixer
        Mix_HookMusicFinished(this->MusicHasStopped);
        ErrLog << "Loaded Music file\n";
    }
}

void MusicManager::FreeMusic(void)
{
    ErrLog << "Freeing the Music stream\n";
    // Check if the Music stream exists
    if(_MixMusic)
    {
        // If so, Stop the music
        Mix_HaltMusic();
        // Free the stream
        Mix_FreeMusic(_MixMusic);
        _MixMusic = 0;
        _MusicHasStopped = true;
        // Unhook SDL_mixer to the function
        Mix_HookMusicFinished(0);
        ErrLog << "Freeed the Music stream\n";
    }
    else
    {
        ErrLog << "No Music Stream to be freed\n";
    }
}

void MusicManager::Play(const int NumberOfLoops)
{
    ErrLog << "Playing the Music stream\n";
    // Check if there is a stream to be played
    if(_MixMusic)
    {
        Mix_PlayMusic(_MixMusic, NumberOfLoops);
        _MusicHasStopped = false;
    }
    else
    {
        ErrLog << "No Music stream\n";
    }
}

void MusicManager::Stop(void)
{
    ErrLog << "Stopping the Music stream\n";
    // Check if there is a stream to be stopped
    if(_MixMusic)
    {
        Mix_HaltMusic();
        _MusicHasStopped = true;
    }
    else
    {
        ErrLog << "No Music stream\n";
    }
}

void MusicManager::Pause(void)
{
    ErrLog << "Pausing the Music stream\n";
    // Check if there is a stream to be stopped
    if(_MixMusic)
    {
        Mix_PauseMusic();
    }
    else
    {
        ErrLog << "No Music stream\n";
    }
}

void MusicManager::Resume(void)
{
    ErrLog << "Resuming the Music stream\n";
    // Check if there is a stream to be stopped
    if(_MixMusic)
    {
        Mix_ResumeMusic();
    }
    else
    {
        ErrLog << "No Music stream\n";
    }
}

void MusicManager::Rewind(void)
{
    ErrLog << "Rewind the Music stream\n";
    // Check if there is a stream to be stopped
    if(_MixMusic)
    {
        Mix_RewindMusic();
        Mix_HaltMusic();
        _MusicHasStopped = true;
    }
    else
    {
        ErrLog << "No Music stream\n";
    }
}

bool MusicManager::HasMusicStopped(void) const
{
	return _MusicHasStopped;
}

void MusicManager::VolumeUp(void)
{
    // Check the current volume is above 0
    if(_CurrentVolume < MAX_VOLUME_LEVEL)
    {
        // Change the volume level up a notch
        _CurrentVolume += 5;
        Mix_VolumeMusic(_CurrentVolume);
    }
}

void MusicManager::VolumeDown(void)
{
    // Check the current volume is below 128
    if(_CurrentVolume > MIN_VOLUME_LEVEL)
    {
        // Change the volume level up a notch
        _CurrentVolume -= 5;
        Mix_VolumeMusic(_CurrentVolume);
    }
}

/*
History
=======
2006-08-03: Added a function to see if the music has stopped playing
2006-07-06: Create file and basic functionality
*/
