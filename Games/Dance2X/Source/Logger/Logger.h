//! A class to write logs to a file

#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <iostream>
#include <string>

namespace LogModule
{
	class Logger
	{
	private:
		//! Stream to the file
//		std::ofstream _OutFileStream;
#define _OutFileStream std::cout

	public:
		// Lots of operator overloads
		// TODO[2006-06-01]: Need a cleaner way of doing this
		//! Operator << overloader for strings and char*
		std::ostream& operator << (const std::string &OutString)
		{
			#ifdef VERBOSE
			_OutFileStream << OutString;
			_OutFileStream.flush();
			#ifdef GP2X
			sync();
			#endif
			#endif
			return _OutFileStream;
		}

		//! Operator << overloader for signed intergers
		std::ostream& operator << (const signed int &OutInt)
		{
			#ifdef VERBOSE
			_OutFileStream << OutInt;
			_OutFileStream.flush();
			#ifdef GP2X
			sync();
			#endif
			#endif
			return _OutFileStream;
		}

		//! Operator << overloader for unsigned intergers
		std::ostream& operator << (const unsigned int &OutInt)
		{
			#ifdef VERBOSE
			_OutFileStream << OutInt;
			_OutFileStream.flush();
			#ifdef GP2X
			sync();
			#endif
			#endif
			return _OutFileStream;
		}

		//! Operator << overloader for signed chars
		std::ostream& operator << (const signed char &OutChar)
		{
			#ifdef VERBOSE
			_OutFileStream << OutChar;
			_OutFileStream.flush();
			#ifdef GP2X
			sync();
			#endif
			#endif
			return _OutFileStream;
		}

		//! Operator << overloader for unsigned chars
		std::ostream& operator << (const unsigned char &OutChar)
		{
			#ifdef VERBOSE
			_OutFileStream << OutChar;
			_OutFileStream.flush();
			#ifdef GP2X
			sync();
			#endif
			#endif
			return _OutFileStream;
		}

		//! Operator << overloader for floats
		std::ostream& operator << (const float &OutFloat)
		{
			#ifdef VERBOSE
			_OutFileStream << OutFloat;
			_OutFileStream.flush();
			#ifdef GP2X
			sync();
			#endif
			#endif
			return _OutFileStream;
		}

		//! Operator << overloader for doubles
		std::ostream& operator << (const double &OutDouble)
		{
			#ifdef VERBOSE
			_OutFileStream << OutDouble;
			_OutFileStream.flush();
			#ifdef GP2X
			sync();
			#endif
			#endif
			return _OutFileStream;
		}

		/*//! Operator << overloader for boolean
		std::ostream& operator << (const bool &OutBool)
		{
			_OutFileStream << OutBool;
			_OutFileStream.flush();
			return _OutFileStream;
		}*/
	};

	//! Global error log
	extern Logger ErrLog;
};

#endif // LOGGER_H

/*
History
=======
2006-07-22: Added #define VERBOSE to stop anything writing to the file
2006-06-01: Created a global error log file
2006-06-01: Created header and source file with code
*/
