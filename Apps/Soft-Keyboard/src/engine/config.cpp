#include "config.h"


std::string	Config::AppName				= "Testing App";
std::string	Config::AppVersion			= "v0.1 alpha2";

bool		Config::ShowSplash			= true;
bool		Config::ShowCursor			= true;
bool		Config::LimitFPS			= true;
//	#ifdef DEBUG
bool		Config::ShowFPS				= true;
//	#else
//bool		Config::ShowFPS				= false;
//	#endif

int			Config::ScreenWidth			= 320;
int			Config::ScreenHeight		= 240;
int			Config::ScreenBpp			= 32;
int			Config::ScreenAngle			= 0;
int			Config::ScreenMaxFPS		= 25;

int			Config::FontSizeBig			= 20;
int			Config::FontSizeNormal		= 14;
int			Config::FontSizeSmall		= 10;

std::string	Config::DataPath			= "data/";
std::string	Config::ImagesPath			= Config::DataPath.c_str();

bool		Config::AllowAlphaImages	= true;
bool		Config::AllowBlendedFonts	= true;


Config::Config() { }
