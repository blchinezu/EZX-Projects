//! A class to give shared memory/resources base functionality for thread safety

#ifndef SDL_SHAREDMEMORY_H
#define SDL_SHAREDMEMORY_H

#include <SDL/SDL_thread.h>
#include <iostream>

class Sdl_SharedMemory
{
private:
	//! SDL Semaphore lock
	SDL_sem * _SemaphoreLock;

public:
	//! Standard constructor. Creates the mutex
	Sdl_SharedMemory(void) : _SemaphoreLock(0)
	{
		_SemaphoreLock = SDL_CreateSemaphore(1);

		// Check if succeded
		if(!_SemaphoreLock)
			std::cerr << "Creation of Semaphore lock failed" << std::endl;
	}

	//! Virtual destructor. Destroys the mutex
	virtual ~Sdl_SharedMemory(void)
	{
		SDL_DestroySemaphore(_SemaphoreLock);
	}

	//! Wait for Semaphore to be unlocked indefinitely. Will lock immediately afterwards
	void WaitForLock(void)
	{
		SDL_SemWait(_SemaphoreLock);
	}

	//! Wait for Semaphore to be unlocked for X number of milliseconds. Will lock immediately afterwards
	void WaitForLock(const unsigned int TimeOutMs)
	{
		SDL_SemWaitTimeout(_SemaphoreLock, TimeOutMs);
	}

	//! Unlock the Semphore
	void Unlock(void)
	{
		SDL_SemPost(_SemaphoreLock);
	}
};

#endif // SDL_SHAREDMEMORY_H

/*
History
=======
2006-07-22: Created class and base functionality
*/
