//! A class to manage the music channel

#ifndef MUSICMANAGER_H
#define MUSICMANAGER_H

#include <SDL/SDL_mixer.h>
#include <string>

namespace SoundModule
{
    class MusicManager
    {
    private:
        //! Pointer to the music stream
        Mix_Music *_MixMusic;

        //! Current volume
        int _CurrentVolume;

        //! Flag to announce that the music has stopped
        static bool _MusicHasStopped;

        //! Function for SDL_mixer to hook to when the music stops
		static void MusicHasStopped(void)
		{
			_MusicHasStopped = true;
		}

    public:
        //! Standard constructer. Nulls the pointer
        MusicManager(void);

        //! Standard destructor. Frees the music if it is loaded
        ~MusicManager(void);

        //! Load the music file from filename. Stops and frees the current music if it is already
        void LoadMusic(const std::string &Filename);

        //! Stop and Free the current music
        void FreeMusic(void);

        //! Play the music NumberOfLoops times, if -1, it will repeat indefinitely
        void Play(const int NumberOfLoops = 1);

        //! Stop the music
        void Stop(void);

        //! Pause the music
        void Pause(void);

        //! Resume playing the music
        void Resume(void);

        //! Rewind and stop the music
        void Rewind(void);

        //! Check if the music has stopped
        bool HasMusicStopped(void) const;

        //! Increase the volume
        void VolumeUp(void);

        //! Decrease the volume
        void VolumeDown(void);

        //! Get the current volume
        const int GetCurrentVolume(void) const
        {
            return _CurrentVolume;
        }
    };
}

#endif // MUSICMANAGER_H

/*
History
=======
2006-08-03: Added a function to see if the music has stopped playing
2006-07-06: Create file and basic functionality
*/
