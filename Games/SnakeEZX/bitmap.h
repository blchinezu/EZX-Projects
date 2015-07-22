#ifndef BITMAP_H
#define BITMAP_H

class CBitmap {
	private:
		SDL_Surface *image;
	
	public:
		CBitmap()
		{
			image = NULL;
		}
		
		
		CBitmap(char *filename)
		{
			image = NULL;
			loadFromFile(filename);
		}
		
		~CBitmap()
		{
			if (image!=NULL) {SDL_FreeSurface(image);	}
		}
		

		void loadFromFile(char *filename)
		{
			if (image!=NULL) { SDL_FreeSurface(image); }
	
			image = SDL_LoadBMP(filename);
			if (image == NULL)
			{
				fprintf(stderr, "Couldn't load imgae %s\n", SDL_GetError());
				return;
			}
		}
		
		void draw(SDL_Surface *screen, int x, int y)
		{
			SDL_Rect destRect = {x, y, image->w, image->h};
			if(SDL_BlitSurface(image, NULL, screen, &destRect) < 0)
			{
				fprintf(stderr, "BlitSurface error: %s\n", SDL_GetError());
			}
		}
		
		int w()
		{
			int _w = 0;
			if (image>=NULL) _w = image->w;
			return _w;		
		}

		int h()
		{
			int _h = 0;
			if (image>=NULL) _h = image->h;
			return _h;
		}
};

#endif
