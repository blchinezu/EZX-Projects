/*
	Code written by James Fletcher (LiX). 
	Copyright 2009 - 2010. 
	ethocybin@googlemail.com
*/

#include "FontSystem.h"

#include <stdio.h>
#include "sdl_extra.h"


//Constructor
FontSystem::FontSystem()
{
	char file[256];
	char c;

	//Load Lowercase
	c=97;
	for(int i = 0; i < 26; i++)
	{
		memset(&file, 0x00, sizeof(file));
		sprintf(file, "sprites/font/%c.fif", c);
		SDLE_LoadFIF(file, &m_surf[i]);
		SDLE_LoadFIF(file, &m_surf_red[i]);
		SDLE_LoadFIF(file, &m_surf_green[i]);
		SDLE_LoadFIF(file, &m_surf_blue[i]);
		SDLE_LoadFIF(file, &m_surf_white[i]);
		c++;
	}

	//Load Uppercase
	c=97;
	for(int i = 26; i < 52; i++)
	{
		memset(&file, 0x00, sizeof(file));
		sprintf(file, "sprites/font/u%c.fif", c);
		SDLE_LoadFIF(file, &m_surf[i]);
		SDLE_LoadFIF(file, &m_surf_red[i]);
		SDLE_LoadFIF(file, &m_surf_green[i]);
		SDLE_LoadFIF(file, &m_surf_blue[i]);
		SDLE_LoadFIF(file, &m_surf_white[i]);
		c++;
	}

	//Load Numbers
	c=48;
	for(int i = 52; i < 62; i++)
	{
		memset(&file, 0x00, sizeof(file));
		sprintf(file, "sprites/font/%c.fif", c);
		SDLE_LoadFIF(file, &m_surf[i]);
		SDLE_LoadFIF(file, &m_surf_red[i]);
		SDLE_LoadFIF(file, &m_surf_green[i]);
		SDLE_LoadFIF(file, &m_surf_blue[i]);
		SDLE_LoadFIF(file, &m_surf_white[i]);
		c++;
	}

	//Load Special
	SDLE_LoadFIF("sprites/font/s1.fif", &m_surf[62]);
	SDLE_LoadFIF("sprites/font/s2.fif", &m_surf[63]);
	SDLE_LoadFIF("sprites/font/s3.fif", &m_surf[64]);
	SDLE_LoadFIF("sprites/font/s4.fif", &m_surf[65]);
	SDLE_LoadFIF("sprites/font/s5.fif", &m_surf[66]);
	SDLE_LoadFIF("sprites/font/s6.fif", &m_surf[67]);
	SDLE_LoadFIF("sprites/font/s7.fif", &m_surf[68]);
	SDLE_LoadFIF("sprites/font/s1.fif", &m_surf_red[62]);
	SDLE_LoadFIF("sprites/font/s2.fif", &m_surf_red[63]);
	SDLE_LoadFIF("sprites/font/s3.fif", &m_surf_red[64]);
	SDLE_LoadFIF("sprites/font/s4.fif", &m_surf_red[65]);
	SDLE_LoadFIF("sprites/font/s5.fif", &m_surf_red[66]);
	SDLE_LoadFIF("sprites/font/s6.fif", &m_surf_red[67]);
	SDLE_LoadFIF("sprites/font/s7.fif", &m_surf_red[68]);
	SDLE_LoadFIF("sprites/font/s1.fif", &m_surf_green[62]);
	SDLE_LoadFIF("sprites/font/s2.fif", &m_surf_green[63]);
	SDLE_LoadFIF("sprites/font/s3.fif", &m_surf_green[64]);
	SDLE_LoadFIF("sprites/font/s4.fif", &m_surf_green[65]);
	SDLE_LoadFIF("sprites/font/s5.fif", &m_surf_green[66]);
	SDLE_LoadFIF("sprites/font/s6.fif", &m_surf_green[67]);
	SDLE_LoadFIF("sprites/font/s7.fif", &m_surf_green[68]);
	SDLE_LoadFIF("sprites/font/s1.fif", &m_surf_blue[62]);
	SDLE_LoadFIF("sprites/font/s2.fif", &m_surf_blue[63]);
	SDLE_LoadFIF("sprites/font/s3.fif", &m_surf_blue[64]);
	SDLE_LoadFIF("sprites/font/s4.fif", &m_surf_blue[65]);
	SDLE_LoadFIF("sprites/font/s5.fif", &m_surf_blue[66]);
	SDLE_LoadFIF("sprites/font/s6.fif", &m_surf_blue[67]);
	SDLE_LoadFIF("sprites/font/s7.fif", &m_surf_blue[68]);
	SDLE_LoadFIF("sprites/font/s1.fif", &m_surf_white[62]);
	SDLE_LoadFIF("sprites/font/s2.fif", &m_surf_white[63]);
	SDLE_LoadFIF("sprites/font/s3.fif", &m_surf_white[64]);
	SDLE_LoadFIF("sprites/font/s4.fif", &m_surf_white[65]);
	SDLE_LoadFIF("sprites/font/s5.fif", &m_surf_white[66]);
	SDLE_LoadFIF("sprites/font/s6.fif", &m_surf_white[67]);
	SDLE_LoadFIF("sprites/font/s7.fif", &m_surf_white[68]);

	//Key green to Alpha
	for(int i = 0; i < MAX_CHARS; i++)
	{
		SDLE_KeyGreenToAlpha(m_surf[i]);
		SDLE_KeyGreenToAlpha(m_surf_red[i]);
		SDLE_KeyGreenToAlpha(m_surf_green[i]);
		SDLE_KeyGreenToAlpha(m_surf_blue[i]);
		SDLE_KeyGreenToAlpha(m_surf_white[i]);
	}


	/* Setup Alternative Colours */
    Uint32 pixel;
	Uint8 r,g,b;

	for(int i = 0; i < MAX_CHARS; i++) //Normal
	{
		for(int ix = 0; ix < m_surf[i]->w; ix++)
		{
			for(int iy = 0; iy < m_surf[i]->h; iy++)
			{
				pixel = getpixel(m_surf[i], ix, iy);
				SDL_GetRGB(pixel, m_surf[i]->format, &r, &g, &b);
				if( (r >= 104 && r <= 111) && (g >= 105 && g <= 111) && (b >= 110 && b <= 120) ){setpixel(m_surf[i], ix, iy, SDL_MapRGB(m_surf[i]->format, 
					147,150,155));}
			}
		}
	}

	for(int i = 0; i < MAX_CHARS; i++) //Red
	{
		for(int ix = 0; ix < m_surf_red[i]->w; ix++)
		{
			for(int iy = 0; iy < m_surf_red[i]->h; iy++)
			{
				pixel = getpixel(m_surf_red[i], ix, iy);
				SDL_GetRGB(pixel, m_surf_red[i]->format, &r, &g, &b);
				if( (r >= 104 && r <= 111) && (g >= 105 && g <= 111) && (b >= 110 && b <= 120) ){setpixel(m_surf_red[i], ix, iy, SDL_MapRGB(m_surf_red[i]->format, 
					250,0,0));}
			}
		}
	}

	for(int i = 0; i < MAX_CHARS; i++) //Green
	{
		for(int ix = 0; ix < m_surf_green[i]->w; ix++)
		{
			for(int iy = 0; iy < m_surf_green[i]->h; iy++)
			{
				pixel = getpixel(m_surf_green[i], ix, iy);
				SDL_GetRGB(pixel, m_surf_green[i]->format, &r, &g, &b);
				if( (r >= 104 && r <= 111) && (g >= 105 && g <= 111) && (b >= 110 && b <= 120) ){setpixel(m_surf_green[i], ix, iy, SDL_MapRGB(m_surf_green[i]->format, 
					0,250,0));}
			}
		}
	}

	for(int i = 0; i < MAX_CHARS; i++) //Blue
	{
		for(int ix = 0; ix < m_surf_blue[i]->w; ix++)
		{
			for(int iy = 0; iy < m_surf_blue[i]->h; iy++)
			{
				pixel = getpixel(m_surf_blue[i], ix, iy);
				SDL_GetRGB(pixel, m_surf_blue[i]->format, &r, &g, &b);
				if( (r >= 104 && r <= 111) && (g >= 105 && g <= 111) && (b >= 110 && b <= 120) ){setpixel(m_surf_blue[i], ix, iy, SDL_MapRGB(m_surf_blue[i]->format, 
					0,120,230));}
			}
		}
	}

	for(int i = 0; i < MAX_CHARS; i++) //White
	{
		for(int ix = 0; ix < m_surf_white[i]->w; ix++)
		{
			for(int iy = 0; iy < m_surf_white[i]->h; iy++)
			{
				pixel = getpixel(m_surf_white[i], ix, iy);
				SDL_GetRGB(pixel, m_surf_white[i]->format, &r, &g, &b);
				if( (r >= 104 && r <= 111) && (g >= 105 && g <= 111) && (b >= 110 && b <= 120) ){setpixel(m_surf_white[i], ix, iy, SDL_MapRGB(m_surf_white[i]->format, 
					255,255,255));}
			}
		}
	}
}

//Destructor
FontSystem::~FontSystem()
{
	for(int i = 0; i < MAX_CHARS; i++)
	{
		SDL_FreeSurface(m_surf[i]);
		SDL_FreeSurface(m_surf_red[i]);
		SDL_FreeSurface(m_surf_green[i]);
		SDL_FreeSurface(m_surf_blue[i]);
		SDL_FreeSurface(m_surf_white[i]);
	}
}

//Check if character is an uppercase alphabetical
bool IsUpper(char c)
{
     if(c == 'A'){return 1;}
     if(c == 'B'){return 1;}
     if(c == 'C'){return 1;}
     if(c == 'D'){return 1;}
     if(c == 'E'){return 1;}
     if(c == 'F'){return 1;}
     if(c == 'G'){return 1;}
     if(c == 'H'){return 1;}
     if(c == 'I'){return 1;}
     if(c == 'J'){return 1;}
     if(c == 'K'){return 1;}
     if(c == 'L'){return 1;}
     if(c == 'M'){return 1;}
     if(c == 'N'){return 1;}
     if(c == 'O'){return 1;}
     if(c == 'P'){return 1;}
     if(c == 'Q'){return 1;}
     if(c == 'R'){return 1;}
     if(c == 'S'){return 1;}
     if(c == 'T'){return 1;}
     if(c == 'U'){return 1;}
     if(c == 'V'){return 1;}
     if(c == 'W'){return 1;}
     if(c == 'X'){return 1;}
     if(c == 'Y'){return 1;}
     if(c == 'Z'){return 1;}
     
     return 0;
}

//Check if character is an alphabetical
bool IsAlpha(char c)
{
     if(c == 'A'){return 1;}
     if(c == 'B'){return 1;}
     if(c == 'C'){return 1;}
     if(c == 'D'){return 1;}
     if(c == 'E'){return 1;}
     if(c == 'F'){return 1;}
     if(c == 'G'){return 1;}
     if(c == 'H'){return 1;}
     if(c == 'I'){return 1;}
     if(c == 'J'){return 1;}
     if(c == 'K'){return 1;}
     if(c == 'L'){return 1;}
     if(c == 'M'){return 1;}
     if(c == 'N'){return 1;}
     if(c == 'O'){return 1;}
     if(c == 'P'){return 1;}
     if(c == 'Q'){return 1;}
     if(c == 'R'){return 1;}
     if(c == 'S'){return 1;}
     if(c == 'T'){return 1;}
     if(c == 'U'){return 1;}
     if(c == 'V'){return 1;}
     if(c == 'W'){return 1;}
     if(c == 'X'){return 1;}
     if(c == 'Y'){return 1;}
     if(c == 'Z'){return 1;}
	 if(c == 'a'){return 1;}
     if(c == 'b'){return 1;}
     if(c == 'c'){return 1;}
     if(c == 'd'){return 1;}
     if(c == 'e'){return 1;}
     if(c == 'f'){return 1;}
     if(c == 'g'){return 1;}
     if(c == 'h'){return 1;}
     if(c == 'i'){return 1;}
     if(c == 'j'){return 1;}
     if(c == 'k'){return 1;}
     if(c == 'l'){return 1;}
     if(c == 'm'){return 1;}
     if(c == 'n'){return 1;}
     if(c == 'o'){return 1;}
     if(c == 'p'){return 1;}
     if(c == 'q'){return 1;}
     if(c == 'r'){return 1;}
     if(c == 's'){return 1;}
     if(c == 't'){return 1;}
     if(c == 'u'){return 1;}
     if(c == 'v'){return 1;}
     if(c == 'w'){return 1;}
     if(c == 'x'){return 1;}
     if(c == 'y'){return 1;}
     if(c == 'z'){return 1;}
     
     return 0;
}

//Draw String
void FontSystem::DrawString(SDL_Surface* dest, int x, int y, string text, int colour)
{
	int xo = x;
	int cn=-1;

	for(int i = 0; i < text.size(); i++)
	{
		cn=-1;
		if(text[i] == ' '){xo += 4; continue;} //If space
		if(IsUpper(text[i]) == 0){cn = text[i]-97;} //If lowercase letter
		if(IsUpper(text[i]) == 1){cn = 26+(text[i]-65);} //If uppercase letter
		if(IsAlpha(text[i]) == 0){cn = 52+(text[i]-48);} //If number
		if(text[i] == '-'){cn=62;} //If -
		if(text[i] == '%'){cn=63;} //If %
		if(text[i] == ':'){cn=64;} //If :
		if(text[i] == '.'){cn=65;} //If .
		if(text[i] == ','){cn=66;} //If ,
		if(text[i] == '!'){cn=67;} //If !
		if(text[i] == '?'){cn=68;} //If ?
		if(cn < 0 || cn > MAX_CHARS){continue;} //Unrecognised do nothing!

		if(colour == FONT_NORMAL){SDL_BlitSurface(m_surf[cn], 0, dest, &SDLE_Rect(xo, y, m_surf[cn]->w, m_surf[cn]->h));}
		if(colour == FONT_RED){SDL_BlitSurface(m_surf_red[cn], 0, dest, &SDLE_Rect(xo, y, m_surf_red[cn]->w, m_surf_red[cn]->h));}
		if(colour == FONT_GREEN){SDL_BlitSurface(m_surf_green[cn], 0, dest, &SDLE_Rect(xo, y, m_surf_green[cn]->w, m_surf_green[cn]->h));}
		if(colour == FONT_BLUE){SDL_BlitSurface(m_surf_blue[cn], 0, dest, &SDLE_Rect(xo, y, m_surf_blue[cn]->w, m_surf_blue[cn]->h));}
		if(colour == FONT_WHITE){SDL_BlitSurface(m_surf_white[cn], 0, dest, &SDLE_Rect(xo, y, m_surf_white[cn]->w, m_surf_white[cn]->h));}

		xo += m_surf[cn]->w;
	}
}

//Draw String with Opacity
void FontSystem::DrawStringO(SDL_Surface* dest, int x, int y, string text, int colour, float opacity)
{
	int xo = x;
	int cn=-1;

	for(int i = 0; i < text.size(); i++)
	{
		cn=-1;
		if(text[i] == ' '){xo += 4; continue;} //If space
		if(IsUpper(text[i]) == 0){cn = text[i]-97;} //If lowercase letter
		if(IsUpper(text[i]) == 1){cn = 26+(text[i]-65);} //If uppercase letter
		if(IsAlpha(text[i]) == 0){cn = 52+(text[i]-48);} //If number
		if(text[i] == '-'){cn=62;} //If -
		if(text[i] == '%'){cn=63;} //If %
		if(text[i] == ':'){cn=64;} //If :
		if(text[i] == '.'){cn=65;} //If .
		if(text[i] == ','){cn=66;} //If ,
		if(text[i] == '!'){cn=67;} //If !
		if(text[i] == '?'){cn=68;} //If ?
		if(cn < 0 || cn > MAX_CHARS){continue;} //Unrecognised do nothing!

		if(colour == FONT_NORMAL){SDLE_OpacityBlit(m_surf[cn], dest, xo, y, opacity);}
		if(colour == FONT_RED){SDLE_OpacityBlit(m_surf_red[cn], dest, xo, y, opacity);}
		if(colour == FONT_GREEN){SDLE_OpacityBlit(m_surf_green[cn], dest, xo, y, opacity);}
		if(colour == FONT_BLUE){SDLE_OpacityBlit(m_surf_blue[cn], dest, xo, y, opacity);}
		if(colour == FONT_WHITE){SDLE_OpacityBlit(m_surf_white[cn], dest, xo, y, opacity);}

		xo += m_surf[cn]->w;
	}
}






