#include "./DwiParser.h"

#include <string>
#include <fstream>
#include <sstream>
#include "../Logger/Logger.h"

using namespace std;
using namespace LogModule;

// Global constant for the gap (in pixels) between each 1/8 beat step
const FixedPoint 	BEAT_8_STEP 	= 16,
					BEAT_16_STEP 	= 8,
					BEAT_24_STEP	= 128.0f / 24.0f,
					BEAT_64_STEP 	= 2,
					BEAT_192_STEP	= 128.0f / 192.0f;

const string 	CARRIAGE_RETURN = "\x0D",
				NEW_LINE		= "\x0A";

// Function to strip a string (StripString) from another string
void StripString( string &InString, const string &StripString)
{
	// Strip out all the line breaks
	string::size_type Loc = 0;
	while(true)
	{
		// Find the line breaks in the string
		Loc = InString.find( StripString, 0 );
		// If there is a line break then remove from the string
		if( Loc != string::npos)
		{
			InString.erase(Loc, StripString.length() );
		}
		else
		{
			// else break out of the loop
			break;
		}
	}
}

bool ParseDwiFileHeader( DwiHeaderInformation &DwiHeaderObject, const string &FilePath)
{
	ErrLog << "<< Parsing Dwi File Header >>\n";
	// Status of the parse
	bool Status = true;
	// Temp string
	string TempString = "";

	// Does the file exist
	ifstream DwiFile(FilePath.c_str() );
	if(!DwiFile)
	{
		ErrLog << "Parsing DwiFile Failed: " << FilePath << "\n";
		Status = false;
	}
	else
	{
		// Find all the header information
		while( getline(DwiFile, TempString, ':') )
		{
			// Strip all the line breaks
			StripString(TempString, NEW_LINE);
			StripString(TempString, CARRIAGE_RETURN);
			// Try to find the BPM
			if(TempString == "#BPM")
			{
				// If found get the line till ';'
				string BmpString = "";
				getline(DwiFile, BmpString, ';');

				// Convert to a float
				istringstream IStringStream(BmpString);
				float FloatBpm = 0.0f;
				IStringStream >> FloatBpm;
				DwiHeaderObject._Bpm = FloatBpm;
			}
			else
			// Try to find the GAP
			if(TempString == "#GAP")
			{
				// If foudn get the line till ';'
				string GapString = "";
				getline(DwiFile, GapString, ';');

				// Convert to an int
				istringstream IStringStream(GapString);
				IStringStream >> DwiHeaderObject._Gap;
			}
			else
			// Try to get all the steps for single player
			if(TempString == "#SINGLE")
			{
				// Check the difficulty level
				getline(DwiFile, TempString, ':');

				if(TempString == "BASIC")
				{
					// Get the rating
					string RatingString = "";
					getline(DwiFile, RatingString, ':');
					// Convert to an int
					istringstream IStringStream(RatingString);
					IStringStream >> DwiHeaderObject._BasicDifficulty;

					// Get rid of all the steps
					getline(DwiFile, TempString, ';');
				}
				else
				if(TempString == "ANOTHER")
				{
					// Get the rating
					string RatingString = "";
					getline(DwiFile, RatingString, ':');
					// Convert to an int
					istringstream IStringStream(RatingString);
					IStringStream >> DwiHeaderObject._AnotherDifficulty;

					// Get all the steps
					getline(DwiFile, TempString, ';');
				}
				else
				if(TempString == "MANIAC")
				{
					// Get the rating
					string RatingString = "";
					getline(DwiFile, RatingString, ':');
					// Convert to an int
					istringstream IStringStream(RatingString);
					IStringStream >> DwiHeaderObject._ManiacDifficulty;

					// Get all the steps
					getline(DwiFile, TempString, ';');
				}
			}
			else // If anything else
			{
				// Remove everything up to the end of ;
				getline(DwiFile, TempString, ';');
			}
		}
	}

	// Close the file
	DwiFile.close();

	// Print out the information to the Errorlog
	ErrLog << "Parsing Dwi Header: " << FilePath << "\n";
	ErrLog << DwiHeaderObject.ConvertToString();

	return Status;
}

void ParseDwiFileSteps( std::vector<StepObject> &VectorOfStepObjects, const std::string &FilePath, const SongDifficulty SongDifficultySetting)
{
	ErrLog << "<< Parsing Dwi File Steps >>\n";
	// Does the file exist
	ifstream DwiFile(FilePath.c_str() );
	if(!DwiFile)
	{
		ErrLog << "Parsing DwiFile Failed: " << FilePath << "\n";
	}
	else
	{
		// Create a string based on the difficulty level
		string DifficultyString = "";
		switch(SongDifficultySetting)
		{
		case BASIC:
			{
				DifficultyString = "BASIC";
			} break;
		case ANOTHER:
			{
				DifficultyString = "ANOTHER";
			} break;
		case MANIAC:
			{
				DifficultyString = "MANIAC";
			} break;
		default: break;
		}

		// String to hold the steps parsed
		string StringOfSteps = "", TempString = "";

		// Find how many steps need to be generated (including all the freeze step)
		while( getline(DwiFile, TempString, ':') )
		{
			// Strip all the line breaks
			StripString(TempString, NEW_LINE);
			StripString(TempString, CARRIAGE_RETURN);
			// Try to find the BPM
			// Try to get all the steps for single player
			if(TempString == "#SINGLE")
			{
				// Get the difficulty
				getline(DwiFile, TempString, ':');

				if(TempString == DifficultyString)
				{
					// Skip the difficulty
					getline(DwiFile, TempString, ':');
					// Get all the steps
					getline(DwiFile, StringOfSteps, ';');
				}
				else // If anything else
				{
					// Remove everything up to the end of ;
					getline(DwiFile, TempString, ';');
				}
			}
			else // If anything else
			{
				// Remove everything up to the end of ;
				getline(DwiFile, TempString, ';');
			}
		}

		DwiFile.close();

		// Strip the spaces and line breaks from the string
		StripString(StringOfSteps, NEW_LINE);
		StripString(StringOfSteps, CARRIAGE_RETURN);
		StripString(StringOfSteps, " ");
		ErrLog << "Steps String: "<< StringOfSteps << "\n";
		// Resize the temporay vector to hold them all
		VectorOfStepObjects.resize( StringOfSteps.size() * 2 );

		ErrLog << "Total size of temporay Step buffer is: " << VectorOfStepObjects.size() << "\n";

		// Parse the steps into the game
		// Temp variable to hold the step
		char TempStep = 0;

		// Iterator to point at the current step to modify
		vector<StepObject>::iterator StepObjectItr = VectorOfStepObjects.begin(), BackStepObjectItr = VectorOfStepObjects.begin();

		FixedPoint CurrentDistanceFromDeadline(0);

		// Variables to say whether a freeze arrow is active
		//bool LeftHold = false, DownHold = false, UpHold = false, RightHold = false;

		// Current beat distance
		FixedPoint CurrentBeatDistance = BEAT_8_STEP;

		istringstream StepStream(StringOfSteps);
		while(StepStream >> TempStep)
		{
			// Check the Hold Arrows
			/*if(LeftHold)
			{
				if(TempStep != '1' && TempStep != '4' && TempStep != '7')
					(*StepObjectItr)._StepType = LEFT_HOLD_BODY;
				else
				{
					(*StepObjectItr)._StepType = LEFT_HOLD_END;
					LeftHold = false;
				}

				(*StepObjectItr)._DistanceFromDeadline = CurrentDistanceFromDeadline;
				++StepObjectItr;
			}

			if(DownHold)
			{
				if(TempStep != '1' && TempStep != '2' && TempStep != '3')
					(*StepObjectItr)._StepType = DOWN_HOLD_BODY;
				else
				{
					(*StepObjectItr)._StepType = DOWN_HOLD_END;
					DownHold = false;
				}

				(*StepObjectItr)._DistanceFromDeadline = CurrentDistanceFromDeadline;
				++StepObjectItr;
			}

			if(UpHold)
			{
				if(TempStep != '7' && TempStep != '8' && TempStep != '9')
					(*StepObjectItr)._StepType = UP_HOLD_BODY;
				else
				{
					(*StepObjectItr)._StepType = UP_HOLD_END;
					UpHold = false;
				}

				(*StepObjectItr)._DistanceFromDeadline = CurrentDistanceFromDeadline;
				++StepObjectItr;
			}

			if(RightHold)
			{
				if(TempStep != '3' && TempStep != '6' && TempStep != '9')
					(*StepObjectItr)._StepType = RIGHT_HOLD_BODY;
				else
				{
					(*StepObjectItr)._StepType = RIGHT_HOLD_END;
					RightHold = false;
				}
				(*StepObjectItr)._DistanceFromDeadline = CurrentDistanceFromDeadline;
				++StepObjectItr;
			}*/

			switch(TempStep)
			{
			case '0':		// No step
				{
					CurrentDistanceFromDeadline += CurrentBeatDistance;
				} break;
			case '1':		// Down Left Step
				{
					(*StepObjectItr)._StepType = LEFT_SINGLE;
					(*StepObjectItr)._DistanceFromDeadline = CurrentDistanceFromDeadline;
					++StepObjectItr;

					(*StepObjectItr)._StepType = DOWN_SINGLE;
					(*StepObjectItr)._DistanceFromDeadline = CurrentDistanceFromDeadline;
					++StepObjectItr;

					CurrentDistanceFromDeadline += CurrentBeatDistance;
				} break;
			case '2':		// Down Step
				{
					(*StepObjectItr)._StepType = DOWN_SINGLE;
					(*StepObjectItr)._DistanceFromDeadline = CurrentDistanceFromDeadline;
					++StepObjectItr;

					CurrentDistanceFromDeadline += CurrentBeatDistance;
				} break;
			case '3':		// Down Right Step
				{
					(*StepObjectItr)._StepType = RIGHT_SINGLE;
					(*StepObjectItr)._DistanceFromDeadline = CurrentDistanceFromDeadline;
					++StepObjectItr;

					(*StepObjectItr)._StepType = DOWN_SINGLE;
					(*StepObjectItr)._DistanceFromDeadline = CurrentDistanceFromDeadline;
					++StepObjectItr;

					CurrentDistanceFromDeadline += CurrentBeatDistance;
				} break;
			case '4':		// Left Step
				{
					(*StepObjectItr)._StepType = LEFT_SINGLE;
					(*StepObjectItr)._DistanceFromDeadline = CurrentDistanceFromDeadline;
					++StepObjectItr;

					CurrentDistanceFromDeadline += CurrentBeatDistance;
				} break;
			case '6':		// Right Step
				{
					(*StepObjectItr)._StepType = RIGHT_SINGLE;
					(*StepObjectItr)._DistanceFromDeadline = CurrentDistanceFromDeadline;
					++StepObjectItr;

					CurrentDistanceFromDeadline += CurrentBeatDistance;
				} break;
			case '7':		// Up Left Step
				{
					(*StepObjectItr)._StepType = LEFT_SINGLE;
					(*StepObjectItr)._DistanceFromDeadline = CurrentDistanceFromDeadline;
					++StepObjectItr;

					(*StepObjectItr)._StepType = UP_SINGLE;
					(*StepObjectItr)._DistanceFromDeadline = CurrentDistanceFromDeadline;
					++StepObjectItr;

					CurrentDistanceFromDeadline += CurrentBeatDistance;
				} break;
			case '8':		// Up Step
				{
					(*StepObjectItr)._StepType = UP_SINGLE;
					(*StepObjectItr)._DistanceFromDeadline = CurrentDistanceFromDeadline;
					++StepObjectItr;

					CurrentDistanceFromDeadline += CurrentBeatDistance;
				} break;
			case '9':		// Up Right Step
				{
					(*StepObjectItr)._StepType = RIGHT_SINGLE;
					(*StepObjectItr)._DistanceFromDeadline = CurrentDistanceFromDeadline;
					++StepObjectItr;

					(*StepObjectItr)._StepType = UP_SINGLE;
					(*StepObjectItr)._DistanceFromDeadline = CurrentDistanceFromDeadline;
					++StepObjectItr;

					CurrentDistanceFromDeadline += CurrentBeatDistance;
				} break;
			case 'A':		// Up Down Step
				{
					(*StepObjectItr)._StepType = UP_SINGLE;
					(*StepObjectItr)._DistanceFromDeadline = CurrentDistanceFromDeadline;
					++StepObjectItr;

					(*StepObjectItr)._StepType = DOWN_SINGLE;
					(*StepObjectItr)._DistanceFromDeadline = CurrentDistanceFromDeadline;
					++StepObjectItr;

					CurrentDistanceFromDeadline += CurrentBeatDistance;
				} break;
			case 'B':		// Left Right Step
				{
					(*StepObjectItr)._StepType = LEFT_SINGLE;
					(*StepObjectItr)._DistanceFromDeadline = CurrentDistanceFromDeadline;
					++StepObjectItr;

					(*StepObjectItr)._StepType = RIGHT_SINGLE;
					(*StepObjectItr)._DistanceFromDeadline = CurrentDistanceFromDeadline;
					++StepObjectItr;

					CurrentDistanceFromDeadline += CurrentBeatDistance;
				} break;
			case '!':		// Freeze step
				{
					// TODO : Freeze step. For now, skip the next step
					char HACK;
					StepStream >> HACK;
					/*// Check the next letter
					char HoldStep = 0;
					StepStream >> HoldStep;

					switch(HoldStep)
					{
					case '1':	// Down left
						{
							BackStepObjectItr = StepObjectItr - 1;
							while( (*BackStepObjectItr)._StepType != LEFT_SINGLE)
							{
								--BackStepObjectItr;
							}
							(*BackStepObjectItr)._StepType = LEFT_HOLD_HEAD;
							LeftHold = true;

							BackStepObjectItr = StepObjectItr - 1;
							while( (*BackStepObjectItr)._StepType != DOWN_SINGLE)
							{
								--BackStepObjectItr;
							}
							(*BackStepObjectItr)._StepType = DOWN_HOLD_HEAD;
							DownHold = true;
						} break;
					case '2':	// Down
						{
							BackStepObjectItr = StepObjectItr - 1;
							while( (*BackStepObjectItr)._StepType != DOWN_SINGLE)
							{
								--BackStepObjectItr;
							}
							(*BackStepObjectItr)._StepType = DOWN_HOLD_HEAD;
							DownHold = true;
						} break;
					case '3':	// Down Right
						{
							BackStepObjectItr = StepObjectItr - 1;
							while( (*BackStepObjectItr)._StepType != DOWN_SINGLE)
							{
								--BackStepObjectItr;
							}
							(*BackStepObjectItr)._StepType = DOWN_HOLD_HEAD;
							DownHold = true;

							BackStepObjectItr = StepObjectItr - 1;
							while( (*BackStepObjectItr)._StepType != RIGHT_SINGLE)
							{
								--BackStepObjectItr;
							}
							(*BackStepObjectItr)._StepType = RIGHT_HOLD_HEAD;
							RightHold = true;
						} break;
					case '4':	// Left
						{
							BackStepObjectItr = StepObjectItr - 1;
							while( (*BackStepObjectItr)._StepType != LEFT_SINGLE)
							{
								--BackStepObjectItr;
							}
							(*BackStepObjectItr)._StepType = LEFT_HOLD_HEAD;
							LeftHold = true;
						} break;
					case '6':	// Right
						{
							BackStepObjectItr = StepObjectItr - 1;
							while( (*BackStepObjectItr)._StepType != RIGHT_SINGLE)
							{
								--BackStepObjectItr;
							}
							(*BackStepObjectItr)._StepType = RIGHT_HOLD_HEAD;
							RightHold = true;
						} break;
					case '7':	// Up Left
						{
							BackStepObjectItr = StepObjectItr - 1;
							while( (*BackStepObjectItr)._StepType != LEFT_SINGLE)
							{
								--BackStepObjectItr;
							}
							(*BackStepObjectItr)._StepType = LEFT_HOLD_HEAD;
							LeftHold = true;
							UpHold = true;
						} break;
					case '8':	// Up
						{
							BackStepObjectItr = StepObjectItr - 1;
							while( (*BackStepObjectItr)._StepType != UP_SINGLE)
							{
								--BackStepObjectItr;
							}
							(*BackStepObjectItr)._StepType = UP_HOLD_HEAD;
							UpHold = true;
						} break;
					case '9':	// Up Right
						{
							BackStepObjectItr = StepObjectItr - 1;
							while( (*BackStepObjectItr)._StepType != UP_SINGLE)
							{
								--BackStepObjectItr;
							}
							(*BackStepObjectItr)._StepType = UP_HOLD_HEAD;
							UpHold = true;

							BackStepObjectItr = StepObjectItr - 1;
							while( (*BackStepObjectItr)._StepType != RIGHT_SINGLE)
							{
								--BackStepObjectItr;
							}
							(*BackStepObjectItr)._StepType = RIGHT_HOLD_HEAD;
							RightHold = true;
						} break;
					case 'A':	// Up Down
						{
							BackStepObjectItr = StepObjectItr - 1;
							while( (*BackStepObjectItr)._StepType != UP_SINGLE)
							{
								--BackStepObjectItr;
							}
							(*BackStepObjectItr)._StepType = UP_HOLD_HEAD;
							UpHold = true;

							BackStepObjectItr = StepObjectItr - 1;
							while( (*BackStepObjectItr)._StepType != DOWN_SINGLE)
							{
								--BackStepObjectItr;
							}
							(*BackStepObjectItr)._StepType = DOWN_HOLD_HEAD;
							DownHold = true;
						} break;
					case 'B':	// Left Right
						{
							BackStepObjectItr = StepObjectItr - 1;
							while( (*BackStepObjectItr)._StepType != LEFT_SINGLE)
							{
								--BackStepObjectItr;
							}
							(*BackStepObjectItr)._StepType = LEFT_HOLD_HEAD;
							LeftHold = true;

							BackStepObjectItr = StepObjectItr - 1;
							while( (*BackStepObjectItr)._StepType != RIGHT_SINGLE)
							{
								--BackStepObjectItr;
							}
							(*BackStepObjectItr)._StepType = RIGHT_HOLD_HEAD;
							RightHold = true;
						} break;
					default: break;
					}*/

				} break;
			case '(':		// Begin 1/16 steps
				{
					CurrentBeatDistance = BEAT_16_STEP;
				} break;
			case '[':		// Begin 1/24 steps
				{
					CurrentBeatDistance = BEAT_24_STEP;
				} break;
			case '{':		// Begin 1/64 steps
				{
					CurrentBeatDistance = BEAT_64_STEP;
				} break;
			case '`':		// Begin 1/192 steps
				{
					CurrentBeatDistance = BEAT_192_STEP;
				} break;
			case ')':		// End 1/16 steps
			case ']':		// End 1/24 steps
			case '}':		// End 1/64 steps
			case '\'':		// End 1/192 steps
					CurrentBeatDistance = BEAT_8_STEP;
					break;
			default: break;
			}
		}

		// Erase all the vectors from the StepObjectItr to the end
		VectorOfStepObjects.erase(StepObjectItr, VectorOfStepObjects.end() );
		ErrLog << "Finished parsing. Actual size of Buffer is: " << VectorOfStepObjects.size() << "\n";

	} // end else case
}

/*
History
=======
2006-07-19: Created file and added base functionality
*/
