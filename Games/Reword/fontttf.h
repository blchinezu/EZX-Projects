//fontttf.h


#ifndef FONTTTF_H
#define FONTTTF_H

#include <SDL.h>
#include <SDL_ttf.h>	//for TTF_ functions
#include <string>

#include "surface.h"

class FontTTF
{
public:
	FontTTF();
	FontTTF(std::string fileName, int size);
	~FontTTF();

	bool load(std::string fontName, int size);

	int put_text(Surface *s, int x, int y, const char *textstr, const SDL_Color &textColour, bool bShadow = false);
	int put_text(Surface *s, int y, const char *textstr, const SDL_Color &textColour, bool bShadow = false);
	int put_text_right(Surface *s, int y, const char *textstr, const SDL_Color &textColour, bool bShadow = false);
	int put_number(Surface *s, int x, int y, unsigned int number, const char *format, const SDL_Color &textColor, bool bShadow = false);
	int put_number(Surface *s, int y, unsigned int number, const char *format, const SDL_Color &textColor, bool bShadow = false);
	void setShadowColour(SDL_Color &c);

protected:
	void cleanUp();

private:
	TTF_Font *	_font;
	int			_size;
	bool		_init;
	SDL_Color	_shadow;		//font shadow colour
	char		_buffer[100];	//general purpose char buffer (for number formatting etc)
};

#endif //FONTTTF_H
