//! A class to manager all the fonts in the game.
//! The text surface is considered to be 'on top' of all the layers

#ifndef FONTMANAGER_H
#define FONTMANAGER_H

#include <vector>
#include <queue>
#include <string>
#include <SDL/SDL.h>

#include "./ColourRGBA.h"

enum Alignment
{
	ALIGN_LEFT 	= 0,
	ALIGN_RIGHT = 1
};

namespace GfxModule
{
	// Forward declaring classes
	class FontWrapper;
	class SurfaceWrapper;

	class FontManager
	{
	private:
		//! Vector to hold all the Font Wrapper pointers
		std::vector<FontWrapper *> _VectorOfFontPointers;

		//! Queue of free Font Wrapper pointers
		std::queue<FontWrapper *> _QueueOfFreeFontPointers;

		//! Pointer to the Text Surface
		SurfaceWrapper * _TextSurface;

	public:
		//! Standard constructor
		/*!
			Resizes the vector to the max number of fonts and populates the queue
			Creates a copy of the screen surface to be used as the Text Surface Wrapper
		*/
		FontManager(const unsigned int NumberOfFonts, const SDL_Surface * const ScreenSurface);

		//! Standard desconstructor, frees all memory used by the manager
		~FontManager(void);

		//! Creates a font and returns the font pointer
		FontWrapper * CreateFont(const std::string &Filename, const int PtSize);

		//! Load a new font to a Font Wrapper
		void LoadFont(FontWrapper * AFontWrapper, const std::string &Filename, const int PtSize);

		//! Clears a font but doesn't free it
		void ClearFont(FontWrapper * AFontWrapper);

		//! Clears all fonts but doesn't free them
		void ClearAllFonts(void);

		//! Frees and clears a font from use
		void FreeFont(FontWrapper * AFontWrapper);

		//! Frees and clears all fonts from use
		void FreeAllFonts(void);

		//! Write Text to the screen
		void WriteText(
			const FontWrapper * const AFontWrapper,
			const int XPositionOnScreen,
			const int YPositionOnScreen,
			const std::string &Text,
			const char Red,
			const char Green,
			const char Blue,
			const Alignment TextAlign = ALIGN_LEFT
		);

		//! Overload function of the above function
		void WriteText(
			const FontWrapper * const AFontWrapper,
			const int XPositionOnScreen,
			const int YPositionOnScreen,
			const std::string &Text,
			const ColourRGBA &Colour,
			const Alignment TextAlign = ALIGN_LEFT
		);

		//! Clear the text surface
		void ClearTextSurface(void);

		//! Returns a SurfaceWrapper to the text surface
		const SurfaceWrapper * const GetTextSurface(void) const
		{
			return _TextSurface;
		}
	};
}

#endif // FONTMANAGER_H

/*
History
=======
2006-07-22: Added support for text alignment
2006-07-14: Added support for using the colour class
2006-07-02: Created file and base functionality
*/
