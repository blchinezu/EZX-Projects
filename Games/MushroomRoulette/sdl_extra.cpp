#include "sdl_extra.h"

#pragma comment(lib, "zdll.lib")
#include "zlib.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


//Get's a pixel from a surface (SDL Wiki Code)
Uint32 getpixel(SDL_Surface *surface, int x, int y)
{
    if(x < 0){return 0;}
    if(y < 0){return 0;}
    if(x > surface->w-1){return 0;}
    if(y > surface->h-1){return 0;}

    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        return *p;

    case 2:
        return *(Uint16 *)p;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;

    case 4:
        return *(Uint32 *)p;

    default:
        return 0;       /* shouldn't happen, but avoids warnings */
    }
}

//Sets a pixel to a surface (SDL Wiki Code)
void setpixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    if(x < 0){return;}
    if(y < 0){return;}
    if(x > surface->w-1){return;}
    if(y > surface->h-1){return;}

    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to set */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        *p = pixel;
        break;

    case 2:
        *(Uint16 *)p = pixel;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        } else {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;

    case 4:
        *(Uint32 *)p = pixel;
        break;
    }
}

//Creates an SDL_Rect
SDL_Rect SDLE_Rect(const int x, const int y, const int w, const int h)
{
	const SDL_Rect current_rect = {x, y, w, h};
	return current_rect;
}

//Keys Kreen to Alpha
void SDLE_KeyGreenToAlpha(SDL_Surface* image)
{
	SDL_SetColorKey(image, SDL_SRCCOLORKEY, SDL_MapRGB(image->format, 0, 255, 0));
}

//A wait function
int start_time;
void SDLE_Wait(const int ms)
{
	start_time = GetTicks();
	do{}while(GetTicks() != start_time+ms);
}

//Replaces a colour!
void SDLE_ReplaceColour(SDL_Surface* surf, SDL_Rect area, Uint32 fcolour, Uint32 ncolour)
{
	Uint32 pixel;

	for(int y = area.y; y < area.y+area.h; y++)
	{
		for(int x = area.x; x < area.x+area.w; x++)
		{
			pixel = getpixel(surf, x, y);
			if(pixel == fcolour){setpixel(surf, x, y, ncolour);}
		}
	}
}


//Blit's an image with specified opactiy
void SDLE_OpacityBlit(SDL_Surface* in, SDL_Surface* out, int ix, int iy, float opacity)
{
	Uint32 p1,p2;
	Uint8 r1, g1, b1;
	Uint8 r2, g2, b2;
	Uint8 rn, gn, bn;
	Uint32 colourkey = SDL_MapRGB(in->format, 0,255,0);

	for(int y = 0; y < in->h; y++)
	{
		for(int x = 0; x < in->w; x++)
		{
			p1 = getpixel(in, x, y);
			if(p1 == colourkey){continue;}

			SDL_GetRGB(p1, in->format, &r1, &g1, &b1);

			p2 = getpixel(out, ix+x, iy+y);
			SDL_GetRGB(p2, out->format, &r2, &g2, &b2);

			rn = (1 - opacity) * r1 + (opacity * r2);
			gn = (1 - opacity) * g1 + (opacity * g2);
			bn = (1 - opacity) * b1 + (opacity * b2);

			setpixel(out, ix+x, iy+y, SDL_MapRGB(out->format, rn, gn, bn));
		}
	}
}

//Encrypts data using simple xor
void XorCrypt(char* in, char* out, int effector)
{
	char c;
	int size;

	FILE* f = fopen(in, "r");
	FILE* o = fopen(out, "w");
	if(f != NULL && o != NULL)
	{
		fseek(f, 0, SEEK_END);
		size = ftell(f);
		fseek(f, 0, SEEK_SET);

		for(int i = 0; i < size; i++)
		{
			c = fgetc(f);
			c = c ^ effector;
			fputc(c, o);
		}

		fclose(f);
		fclose(o);
	}
}

//Clears a file content
void ClearFile(char* in)
{
	FILE* t = fopen(in, "w");
	if(t != NULL)
	{
		fprintf(t, "");
		fclose(t);
	}

	//DeleteFile(in);
}


//Draws an alpha blended rectangle
void SDLE_FillRectAlpha(SDL_Surface* surf, const SDL_Rect* irect, const Uint32 icolour, const float alpha)
{
	Uint32 p1;
	Uint8 r1, g1, b1;
	Uint8 r2, g2, b2;
	Uint8 rn, gn, bn;
	 
	for(int x = irect->x; x < irect->x+irect->w; x++)
	{
		for(int y = irect->y; y < irect->y+irect->h; y++)
		{
			p1 = getpixel(surf, x, y);
			SDL_GetRGB(p1, surf->format, &r1, &g1, &b1);
			SDL_GetRGB(icolour, surf->format, &r2, &g2, &b2);

			rn = (1 - alpha) * r1 + (alpha * r2);
			gn = (1 - alpha) * g1 + (alpha * g2);
			bn = (1 - alpha) * b1 + (alpha * b2);

			setpixel(surf, x, y, SDL_MapRGB(surf->format, rn, gn, bn));
		}
	}
}


//Decompress a file compressed with ZLIB
void ZLIB_Decompress(char* path)
{
	char newpath[256];
	memset(&newpath, 0x00, sizeof(newpath));
	sprintf(newpath, "%s.un", path);

	gzFile infile = gzopen(path, "rb");
	FILE *outfile = fopen(newpath, "wb");
	if(!infile || !outfile){return;}

	char buffer[128];
	int num_read = 0;

	while((num_read = gzread(infile, buffer, sizeof(buffer))) > 0)
	{
		fwrite(buffer, 1, num_read, outfile);
	}

	gzclose(infile);
	fclose(outfile);
}



//Loads a FIF file into memory
bool SDLE_LoadFIF(char* file, SDL_Surface** surf)
{
	char loadpath[256];
	char out[32];
	int headerlen=2;
	int width=0, height=0;
	char* data;

	//Decompress FIF
	ZLIB_Decompress(file);

	//Set loadpath
	sprintf(loadpath, "%s.un", file);

	//Load FIF
	FILE* in = fopen(loadpath, "rb");
	if(in != NULL)
	{
		//Get Image Width
		fscanf(in, "%s ", out);
		headerlen += strlen(out);
		width = atoi(out);

		//Get Image Height
		fscanf(in, "%s\n", out);
		headerlen += strlen(out);
		height = atoi(out);

		//Get Image Data
		data = new char[width*height*4];
		fseek(in, headerlen, SEEK_SET);
		fread(data, 1, width*height*4, in);

		fclose(in);
	}else{return 0;}

	//Delete Decompressed
	ClearFile(loadpath);

	//Write it to the SDL surface
	*surf = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, 16, 0, 0, 0, 0);

	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			Uint8 r,g,b;
			const int sc = ((y*width)+x)*4;
			r = data[sc+2];
			g = data[sc+1];
			b = data[sc];
			setpixel(*surf, x, y, SDL_MapRGB((*surf)->format, r, g, b));
		}
	}

	//Done
	return 1;
}


