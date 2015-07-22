//audio.h

#ifndef _AUDIO_H
#define _AUDIO_H

#include "error.h"
#include "SDL_mixer.h"
//#include "singleton.h"
//#include <mikmod.h>

class Audio : public Error
{
public:
	Audio();
	~Audio();

	void init();
	void setVolume(Sint16 newvol, bool test = true);
	void volumeUp();
	void volumeDown();

	//MikMod functions
	bool modLoad(std::string filename);
	void modStart();
	void modStop();
	void modUpdate();

protected:
	bool	_init;
	Sint16	_volume;			//current volume setting (fx and music)

	Mix_Chunk	*_volTest;
//	MODULE		*_modMusic;		//mikmod
};

#endif //_AUDIO_H
