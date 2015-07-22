#include "DownMan.h"

int main( int argc, char** argv )
{
	ZApplication app( argc, argv );
	DownMan *Window = new DownMan( argc, argv );

	app.setMainWidget(Window);
	Window->showMaximized();
	return app.exec();
}
