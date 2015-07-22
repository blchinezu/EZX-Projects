////////////////////////////////////////////////////////////////////
/*

File:			audio.cpp

Class impl:		Audio

Description:	A class to manage the SDL audio interface
				It was intended to use the Singleton template class to create it, 
				but some compilers (VC6) can't handle it so haven't used it (yet)

Author:			Al McLuckie (al-at-purplepup-dot-org)

Date:			06 April 2007

Licence:		This program is free software; you can redistribute it and/or modify
				it under the terms of the GNU General Public License as published by
				the Free Software Foundation; either version 2 of the License, or
				(at your option) any later version.

				This software is distributed in the hope that it will be useful,
				but WITHOUT ANY WARRANTY; without even the implied warranty of
				MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
				GNU General Public License for more details.

				You should have received a copy of the GNU General Public License
				along with this program; if not, write to the Free Software
				Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

*/
////////////////////////////////////////////////////////////////////

#include "global.h"
#include "audio.h"

Audio::Audio() : _init(false), _volume(0), _volTest(0)
{
}

Audio::~Audio()
{
	if (_volTest) 
		Mix_FreeChunk(_volTest);

	if (_init) Mix_CloseAudio();
}

void Audio::init()
{
	if (_init) return;

#if defined GP2X
	//open audio with chunksize of 128 for gp2x, as smaller this is, 
	//the more often the sound hooks will be called, reducing lag
	if (Mix_OpenAudio(22050, AUDIO_S16, 2, 128) == -1)
#else
	if (Mix_OpenAudio(22050, AUDIO_S16, 2, 2048) == -1)
#endif
	{
//#		setLastError("Warning: Couldn't set audio\nReason: %s\n");
		return;	//not set _init
	}

	_volTest = Mix_LoadWAV("data/sounds/honk.wav"); //also used in game
	setVolume((MIX_MAX_VOLUME / 2), false); //no test sound
/*
    MikMod_RegisterAllDrivers();
    MikMod_RegisterAllLoaders();
    md_mode |= DMODE_SOFT_MUSIC;
    if(MikMod_Init("")) {
    	//setLastError("Warning: Couldn't init MikMod audio\nReason: %s\n");
        return;	//failed
    }
*/
	_init = true;
}


void Audio::setVolume(Sint16 newvol, bool bTest)
{
	if (newvol < 0) newvol = 0;
	if (newvol > MIX_MAX_VOLUME) newvol = MIX_MAX_VOLUME;

	_volume = newvol;

	Mix_Volume(-1,newvol);
//	Mix_VolumeMusic(mVolume);

	//play a test beep/sound at the new volume so player can tell 
	if (bTest) Mix_PlayChannel(-1,_volTest,0);
}

void Audio::volumeUp()
{
	setVolume(_volume+=8);
}

void Audio::volumeDown()
{
	setVolume(_volume-=8);
}

bool modLoad(std::string filename)
{
/*    _modMusic = Player_Load(filename, 64, 0);
    if(!_modMusic) {
        return false;
    }
    MikMod_EnableOutput();
*/
    return true;
}

void Audio::modStart()
{
//    Player_Start(_modMusic);
}

void Audio::modStop()
{
	//Player_Stop() ???
}

void Audio::modUpdate()	//MikMod update call
{
//	if (Player_Active()) MikMod_Update();
}
