#include "app.h"

int main( int argc, char** argv )
{
	ZApplication a( argc, argv );
	app *Window = new app( argc, argv );

	a.setMainWidget(Window);
	return a.exec();
}
