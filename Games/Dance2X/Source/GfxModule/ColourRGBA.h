//! A class to represent the a colour with 4 attributes

#ifndef COLOURRGBA_H
#define COLOURRGBA_H

namespace GfxModule
{
	class ColourRGBA
	{
	public:
		//! Red Componment
		char _Red;

		//! Green Componment
		char _Green;

		//! Blue Componment
		char _Blue;

		//! Alpha Componment
		char _Alpha;

		//! Standard constructor
		ColourRGBA(void)
		{
			_Red = 0; _Green = 0; _Blue = 0; _Alpha = 0;
		}

		//! Constructor that initialises the RGBA value
		ColourRGBA(const char Red, const char Green, const char Blue, const char Alpha)
		{
			_Red = Red; _Green = Green; _Blue = Blue; _Alpha = Alpha;
		}

		//! Standard destructor. Does nothing
		~ColourRGBA(void)
		{
			// Intentionally left blank
		}
	};
}

#endif // COLOURRGBA_H

/*
History
=======
2006-07-14: Created file and added basic functionality
*/
