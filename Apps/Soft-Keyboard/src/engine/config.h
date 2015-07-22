#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "engine.h"

class Config
{
    public:
    	Config();
    	
    public:
		static std::string	AppName;
		static std::string	AppVersion;

		static bool			ShowSplash;
		static bool			ShowCursor;
    	static bool			LimitFPS;
		static bool			ShowFPS;

		static int			ScreenWidth;
		static int			ScreenHeight;
		static int			ScreenBpp;
		static int			ScreenAngle;
		static int			ScreenMaxFPS;

		static int			FontSizeBig;
		static int			FontSizeNormal;
		static int			FontSizeSmall;

		static std::string	DataPath;
		static std::string	ImagesPath;

		static bool			AllowAlphaImages;
		static bool			AllowBlendedFonts;
};

#endif /* _CONFIG_H_ */

