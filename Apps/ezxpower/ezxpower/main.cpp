// Copyright 2010, Abrek
// Modified by BruceLee @ MororolaFans.com

#include <ZApplication.h>
#include <ZMainWidget.h>

#include "ezxpower.h"

#ifdef WITH_HIDE_BUTTON
#include <unistd.h>

bool glob_hide_press=false;
#endif

int main(int argc, char** argv) 
{
	enum WidgetFlags {
		WStyle_Customize	= 0x00000010,
		WStyle_StaysOnTop	= 0x00001000,
	};
 
	ZApplication app(argc,argv);
	Power *w = new Power(0,0,(WStyle_Customize|WStyle_StaysOnTop));	
	w->setGeometry(0,0,240,320);	  
  	app.setMainWidget(w);
  	w->show();
	int r=app.exec();
	delete &app;
#ifdef WITH_HIDE_BUTTON
	if (glob_hide_press) while (true) usleep(60000000);
#endif
	return r;
}

