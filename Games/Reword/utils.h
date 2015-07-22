//utils.h

#ifndef _UTILS_H
#define _UTILS_H

#include "random.h"
#include <string>
#include <vector>

class RandInt
{
public:
	RandInt() { 
#ifdef _SDL_H
#ifdef WIN32
#pragma message("words.h: Using SDL_GetTicks() to seed random\n")
#endif
		setSeed(SDL_GetTicks()); 
#else
#ifdef WIN32
#pragma message("words.h: Using ctime to seed random\n")
#endif
		m_rnd.Randomize();	//uses ctime to seed
#endif
	}	//incase setSeed not called
	void setSeed(unsigned int seed) {
		m_rnd.SetRandomSeed(seed);
		m_rnd.Randomize();
	}
	int random(int limit)
	{
		return (int)m_rnd.Random(limit);
	}
	int operator() (int limit)	//for sorting calls
	{
		return (int)m_rnd.Random(limit);
	}
	CRandom	m_rnd;					//random number generator class
};

static RandInt g_randInt;

class Utils
{
public:

	//string functions
	static void makeUpper(std::string &s);
	static void makeLower(std::string &s);
	static void trimRight(std::string &s, const std::string &t);
	static void trimLeft(std::string &s, const std::string &t); 
	static void trim(std::string &source, const std::string &t); 
	static void makeAlpha(std::string &s);
	static void buildTextPage(std::string &inStr, unsigned int nCharsPerLine, std::vector<std::string> &outVect);

	//math functions
	//static int round(float fl);
	
	//rand functions
//	static int RandomInt(unsigned int limit);

};


#endif //_UTILS_H
