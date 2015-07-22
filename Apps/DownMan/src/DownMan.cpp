#include "DownMan.h"

DownMan::DownMan( int argc, char** argv )
	: ZMainWidget( false, NULL, NULL, 0 )
{
	// Print all the arguments
	//for(int i=0; i<argc; i++) printf("%d=%s\n", i, argv[i]);
	
	// Init titlebar
	title = new QLabel( "", this );
	setTitleBarWidget( title );
	resizeButton=new ZPushButton( "Dialog_Down_Arr",getTitleBarWidget() );
	resizeButton->setFixedSize(QSize(32,24));
	resizeButton->move(ZGlobal::getScreenR().width()-32,0);
	connect(resizeButton,SIGNAL(clicked()),this,SLOT(toggleWindowSize()));
	
	// Init bottom buttons
	CST = new UTIL_CST ( this, "" );
	setCSTWidget( CST );
	// Left
	CSTLeft = CST->getLeftBtn();
	CSTLeftMenu = new QPopupMenu(CST);
	MenuEntry[ABOUT] = CSTLeftMenu->insertItem("About");
	CSTLeftMenu->insertSeparator();
	MenuEntry[ADD_LINK] = CSTLeftMenu->insertItem("Add Link");
	CSTLeftMenu->insertSeparator();
	MenuEntry[DAEMON] = CSTLeftMenu->insertItem("Daemon");
	CSTLeftMenu->insertSeparator();
	MenuEntry[SETTINGS] = CSTLeftMenu->insertItem("Settings");
	CSTLeft->setPopup(CSTLeftMenu);
	connect(CSTLeftMenu, SIGNAL(activated(int)), SLOT(MenuSelect(int)));
	// Middle
	CSTMidLeft = new ZPushButton(CST->getMidBtn());
	CSTMidLeft->setGeometry(ZGlobal::mapFromGlobalR((CSTMidLeft),ZGlobal::getCst4b_2R()));
	CSTMidLeft->setText(tr(""));
	CSTMidRight = new ZPushButton(CST->getMidBtn());
	CSTMidRight->setGeometry(ZGlobal::mapFromGlobalR((CSTMidRight),ZGlobal::getCst4b_3R()));
	CSTMidRight->setText(tr(""));
	// Right
	CSTRight = CST->getRightBtn();
	connect( CSTRight, SIGNAL( clicked() ), qApp, SLOT( slotQuickQuit() ) );

	CST->showMaximized();

	// Correct CST buttons position
	toggleWindowSize();	
	
	// Start state
	Downloads();
	
	// If got link as argument open the add_link dialog		
	if (argc == 3 && QString(argv[1]) == QString("-d"))
		Add_Link(QString(argv[2]));
}

DownMan::~DownMan() {}

void DownMan::MenuSelect(int arg)
{
	int x; // FIXME: REMOVE
	     if (arg == MenuEntry[ABOUT])		About();
	else if (arg == MenuEntry[ADD_LINK])	Add_Link();
	else if (arg == MenuEntry[DAEMON])		Daemon();
	else if (arg == MenuEntry[SETTINGS])	//Settings();
		x=1;
	else 
		printf("WARNING: Got unknown menu entry: %d\n", arg);
}

void DownMan::toggleWindowSize()
{
	if(rect()!=ZGlobal::getScreenR()) {
		showMaximized();
		CST->move(0,ZGlobal::getScreenR().height()-36);
		resizeButton->setResourceID("Dialog_Down_Arr");
	} else {
		showNormal();
		CST->move(0,rect().height()-36);
		resizeButton->setResourceID("Dialog_Up_Arr");
	}
}

// Dialogs
#include "about.part"
#include "add_link.part"
#include "daemon.part"
// States
#include "downloads.part"
// Global functions
#include "global.part"

