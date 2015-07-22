//! A class to wrapper around TTF_Font

#ifndef FONTWRAPPER_H
#define FONTWRAPPER_H

#include <SDL/SDL_ttf.h>

namespace GfxModule
{
	class FontWrapper
	{
	public:
		//! Pointer to the TTF_Font object
		TTF_Font * _Font;

		//! Standard constructor, Nulls the pointer
		FontWrapper(void) : _Font(0)
		{
			// Intentionally left blank
		}

		//! Standard desconstructor. Frees the Font
		~FontWrapper(void)
		{
			if(_Font)
				TTF_CloseFont(_Font);
		}
	};
}

#endif // FONTWRAPPER_H

/*
History
=======
2006-07-02: Created file and base functionality
*/
