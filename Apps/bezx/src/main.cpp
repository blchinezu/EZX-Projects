#include "main.h"

int toInt(char *str)
{
	int i=0, ret=0;
	for(; i < strlen(str); i++)
		if( str[i] >= 48 && str[i] <= 57 )
			ret = (ret * 10) + (str[i] - 48);
		else
			return -777;
	return ret;
}

int help()
{
	printf("\n Syntax: ./bezx <options>");
	printf("\n");
	printf("\n -h                  This Help");
	printf("\n -v <milliseconds>   Vibrate");
	printf("\n -T <seconds>        Set LCD Timeout");
	printf("\n -b                  Get brightness");
	printf("\n -B <value>          Set brightness");
	printf("\n -bl <1|0>           Backlight On/Off");
	printf("\n -kl <1|0>           Keyslight On/Off");
	printf("\n -w                  Get wallpaper path");
	printf("\n");
	printf("\n -fi                 Brightness fade in");
	printf("\n     <start_brightness_value>");
	printf("\n     <normal_brightness_value>");
	printf("\n     <duration_in_milliseconds>");
	printf("\n");
	printf("\n -fo                 Brightness fade out");
	printf("\n     <start_brightness_value>");
	printf("\n     <normal_brightness_value>");
	printf("\n     <duration_in_milliseconds>");
	printf("\n");
	printf("\n");
	return 1;
}

// Starting the application
int main(int argc, char *argv[]) {
	
	if(argc <= 1) return help();
	
	CEZX ezx;
	
	int i=1;
	for(; i < argc; i++)
	{
		// Vibrate
		if( !strcmp(argv[i], "-v"))
		{
			if( i+1 >= argc )
				return dbg(ERROR, argv[i], " got no number");
			if( toInt(argv[i+1]) == -777 )
				return dbg(ERROR, argv[i], " got unknown number: ", argv[i+1]);
			ezx.vibrate( toInt(argv[i+1]) );
			i++;
		} else
		
		// Set brightness
		if( !strcmp(argv[i], "-B") )
		{
			if( i+1 >= argc )
				return dbg(ERROR, argv[i], " got no number");
			if( toInt(argv[i+1]) == -777 )
				return dbg(ERROR, argv[i], " got unknown number: ", argv[i+1]);
			ezx.set_brightness( toInt(argv[i+1]) );
			i++;
		} else
		
		// Get brightness path
		if( !strcmp(argv[i], "-b") )
		{
			printf("%d", ezx.get_brightness());
		}
		else
		
		// Set LCD Timeout
		if( !strcmp(argv[i], "-T") )
		{
			if( i+1 >= argc )
				return dbg(ERROR, argv[i], " got no number");
			if( toInt(argv[i+1]) == -777 )
				return dbg(ERROR, argv[i], " got unknown number: ", argv[i+1]);
			ezx.set_LCDOffSeconds( toInt(argv[i+1]) );
			i++;
		} else
		
		// Set LCD Backlight
		if( !strcmp(argv[i], "-BL") )
		{
			if( i+1 >= argc )
				return dbg(ERROR, argv[i], " got no number");
			if( toInt(argv[i+1]) == -777 )
				return dbg(ERROR, argv[i], " got unknown number: ", argv[i+1]);
			switch(toInt(argv[i+1]))
			{
				case 0:  ezx.backlight( false ); break;
				case 1:  ezx.backlight( true );  break;
				default: return dbg(ERROR, argv[i], " got unknown number");
			}
			i++;
		} else
		
		// Set Key Backlight
		if( !strcmp(argv[i], "-KL") )
		{
			if( i+1 >= argc )
				return dbg(ERROR, argv[i], " got no number");
			if( toInt(argv[i+1]) == -777 )
				return dbg(ERROR, argv[i], " got unknown number: ", argv[i+1]);
			switch(toInt(argv[i+1]))
			{
				case 0:  ezx.keyslight( false ); break;
				case 1:  ezx.keyslight( true );  break;
				default: return dbg(ERROR, argv[i], " got unknown number");
			}
			i++;
		} else
		
		// Fade In
		if( !strcmp(argv[i], "-fi") )
		{
			if( i+3 >= argc )
				return dbg(ERROR, argv[i], " doesn't have enough args");
			if( toInt(argv[i+1]) == -777 )
				return dbg(ERROR, argv[i], " got unknown number: ", argv[i+1]);
			if( toInt(argv[i+2]) == -777 )
				return dbg(ERROR, argv[i], " got unknown number: ", argv[i+2]);
			if( toInt(argv[i+3]) == -777 )
				return dbg(ERROR, argv[i], " got unknown number: ", argv[i+3]);
			ezx.current_br = toInt(argv[i+1]);
			ezx.normal_br  = toInt(argv[i+2]);
			ezx.fadein_screen(toInt(argv[i+3]));
			i += 3;
		} else
		
		// Fade Out
		if( !strcmp(argv[i], "-fo") )
		{
			if( i+3 >= argc )
				return dbg(ERROR, argv[i], " doesn't have enough args");
			if( toInt(argv[i+1]) == -777 )
				return dbg(ERROR, argv[i], " got unknown number: ", argv[i+1]);
			if( toInt(argv[i+2]) == -777 )
				return dbg(ERROR, argv[i], " got unknown number: ", argv[i+2]);
			if( toInt(argv[i+3]) == -777 )
				return dbg(ERROR, argv[i], " got unknown number: ", argv[i+3]);
			ezx.current_br = toInt(argv[i+1]);
			ezx.normal_br  = toInt(argv[i+2]);
			ezx.fadeout_screen(toInt(argv[i+3]));
			i += 3;
		} else
		
		// Get wallpaper path
		if( !strcmp(argv[i], "-w") )
		{
			char ret[1024];
			ezx.get_wallpaper( ret );
			printf("%s", ret);
		}
		else
		
		// Get wallpaper path
		if( !strcmp(argv[i], "-h") )
		{
			return help();
		}
		else
		
		// Unknown argument
			return dbg(ERROR, argv[i], " is not a known argument");
	}
	
	return 0;
}

