#ifndef DWIPARSER_H
#define DWIPARSER_H

// DWI format explained here: http://dwi.ddruk.com/readme.php

#include <vector>
#include <string>

#include <sstream>

#include "../ToolKit/FixedPoint.h"

//! Enum of step types
enum StepType
{
	NONE,
	LEFT_SINGLE,
	DOWN_SINGLE,
	UP_SINGLE,
	RIGHT_SINGLE,
	LEFT_HOLD_HEAD,
	DOWN_HOLD_HEAD,
	UP_HOLD_HEAD,
	RIGHT_HOLD_HEAD,
	LEFT_HOLD_BODY,
	DOWN_HOLD_BODY,
	UP_HOLD_BODY,
	RIGHT_HOLD_BODY,
	LEFT_HOLD_END,
	DOWN_HOLD_END,
	UP_HOLD_END,
	RIGHT_HOLD_END
};

//! Enum for the difficultly setting
enum SongDifficulty
{
	BEGINNER,
	BASIC,
	ANOTHER,
	MANIAC,
	EXTREME
};

// Forward declare a class
namespace GfxModule
{
	class Sprite;
}

//! Class for Step Object
class StepObject
{
public:
	//! The type of step
	StepType _StepType;

	//! Distance from the deadline (if negative, it is past the deadline)
	FixedPoint _DistanceFromDeadline;

	//! Pointer to the current sprite used for this step
	GfxModule::Sprite * _Sprite;

	//! Standard Constructor
	StepObject(void) : _StepType(NONE), _DistanceFromDeadline(0), _Sprite(0)
	{
		// Intentionally left blank
	}

	//! Standard Destructor
	~StepObject(void)
	{
		// Intentionally left blank
	}
};

//! Class to hold the header information about the Dwi file
class DwiHeaderInformation
{
public:
	//! Stores the BPM
	FixedPoint _Bpm;

	//! Stores the Gap between the start of the song and the first step (number of miilseconds before the program start counting beats)
	unsigned int _Gap;

	//! Difficulty of the Basic level
	unsigned int _BasicDifficulty;

	//! Difficulty of the Another level
	unsigned int _AnotherDifficulty;

	//! Difficulty of the Maniac level
	unsigned int _ManiacDifficulty;

	//! Standard Constructor
	DwiHeaderInformation(void)
		: _Bpm(0), _Gap(0), _BasicDifficulty(0), _AnotherDifficulty(0), _ManiacDifficulty(0)
	{
		// Intentionally left blank
	}

	//! Standard Destructor
	~DwiHeaderInformation(void)
	{
		// Intentionally left blank
	}

	//! Print all the information to a stream
	std::string ConvertToString (void) const
	{
		std::ostringstream OStringStream;
		OStringStream
				<< "#BPM: " << _Bpm.ToFloat() << "\n"
				<< "#GAP: " << _Gap << "\n"
				<< "#Basic Difficulty: " << _BasicDifficulty << "\n"
				<< "#AnotherDifficulty: " << _AnotherDifficulty << "\n"
				<< "#ManiacDifficulty: " << _ManiacDifficulty << "\n";
		return OStringStream.str();
	}
};

//! Function to parse the Dwi header information
/*!
	Pass a DwiHeaderInformation object and the Dwi filepath, it will write the header information
	to the DwiHeaderInformation object. Returns true if succusful, else false.
*/
bool ParseDwiFileHeader( DwiHeaderInformation &DwiHeaderObject, const std::string &FilePath);

//! Function to create all steps from the Dwi file
void ParseDwiFileSteps( std::vector<StepObject> &VectorOfStepObjects, const std::string &FilePath, const SongDifficulty SongDifficultySetting);

#endif // DWIPARSER_H

/*
History
=======
2006-07-19: Created file and added base functionality
*/
