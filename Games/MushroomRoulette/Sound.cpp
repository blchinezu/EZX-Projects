/*
	Code written by James Fletcher (LiX). 
	Copyright 2009 - 2010. 
	ethocybin@googlemail.com
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "GameWorld.h"

#include "Sound.h"
cSound sound;


//MixAudio
SDL_AudioSpec fmt;
sample snd;
Uint32 amount;


//Callback
void mixaudio(void *unused, Uint8 *stream, int len)
{
    amount = (snd.dlen-snd.dpos);
    if(amount > len){amount = len;}

	if(amount == 0){return;} //Nothing to play

    SDL_MixAudio(stream, &snd.data[snd.dpos], amount, SDL_MIX_MAXVOLUME);

    snd.dpos += amount;
}

//Constructor
cSound::cSound()
{
	/* Set 16-bit stereo audio at 22Khz */
	fmt.freq = 22050;
	fmt.format = AUDIO_S16;
	fmt.channels = 2;
	fmt.samples = 512;        /* A good value for games */
	fmt.callback = mixaudio;
	fmt.userdata = NULL;

	//Open Audio Device
	SDL_OpenAudio(&fmt, 0);

	//unpause (play)
	SDL_PauseAudio(0);
}

//Destructor
cSound::~cSound()
{
	SDL_CloseAudio();
}


//Play sound straight from file
void cSound::PlaySound(char *file)
{
	if(gGameWorld.gMenu.m_sound == 0){return;}

	SDL_AudioSpec wave;
	Uint8 *data;
	Uint32 dlen;
	SDL_AudioCVT cvt;

	/* Load the sound file and convert it to 16-bit stereo at 22kHz */
	if(SDL_LoadWAV(file, &wave, &data, &dlen) == NULL){return;}

	SDL_BuildAudioCVT(&cvt, wave.format, wave.channels, wave.freq, AUDIO_S16, 2, 22050);
	cvt.buf = (Uint8*)malloc(dlen*cvt.len_mult);
	memcpy(cvt.buf, data, dlen);
	cvt.len = dlen;
	SDL_ConvertAudio(&cvt);
	SDL_FreeWAV(data);

	/* Put the sound data in the slot (it starts playing immediately) */
	if(snd.data != NULL){free(snd.data);}

	SDL_LockAudio();
	snd.data = cvt.buf;
	snd.dlen = cvt.len_cvt;
	snd.dpos = 0;
	SDL_UnlockAudio();
}




