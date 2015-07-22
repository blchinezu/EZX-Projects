// DownMan : DownMan.h
// demo application for EZX-crosstool
// by eakrin @ gmail.com
// use with FoxE6's SDK

#ifndef  __DownMan__
#define __DownMan__

#include <ZApplication.h>
#include <ZMainWidget.h>
#include <ZPushButton.h>
#include <ZScrollView.h>
#include <ZMultiLineEdit.h>
#include <ZMessageBox.h>
#include <ezxres.h>
#include <ezxutilcst.h>
#include <ezxutildialog.h>

#include <qlabel.h>
#include <qhbox.h>
#include <qdir.h>
#include <qlineedit.h>

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "filebrowser.h"
#include "defines.h"

class UTIL_CST;
class QLabel;
class ZMultiLineEdit;

class DownMan : public ZMainWidget
{
	Q_OBJECT
	public:
		DownMan( int, char**);
		~DownMan();
		
	public slots:
		void toggleWindowSize	();
		void MenuSelect			(int);
		
		// Dialogs
		void	About			();
		void	AboutClose		();
		void	Daemon			();
		void	DaemonStart		();
		void	DaemonStop		();
		void	DaemonClose		();
		void	Add_Link		();
		void	Add_Link		(QString);
		void	Add_Link		(QString,QString,QString);
		void	Add_LinkOK		();
		void	Add_LinkClose	();
		void	Add_LinkSelLoc	();
		
		// States
		void 	Downloads		();

	private:
		int CurrentState;
		RES_ICON_Reader ir;
		
		// Secondary Windows
		UTIL_Dialog	*AboutWind;
		UTIL_Dialog	*DaemonWind;
		UTIL_Dialog	*Add_LinkWind;
		
		// Main Window
			// Title bar
			QLabel			*title;
			ZPushButton		*resizeButton;
		
			// Content
		
			// Bottom buttons/menus
			UTIL_CST		*CST;
			ZPushButton		*CSTLeft;
			QPopupMenu		*CSTLeftMenu;
			int				MenuEntry[ENTRIES];
			ZPushButton		*CSTMidLeft;
			ZPushButton		*CSTMidRight;
			ZPushButton		*CSTRight;
		
		// Add Link Dialog
			ZMultiLineEdit	*Link;
			ZMultiLineEdit	*DownPath;
};

// From global.part
bool	FileExists		(const char*);
int		SplitCount		(QString, QString);
QString	SplitStr		(QString, QString, int);
QString	SplitStrLastRest(QString, QString);
QString	SplitStrLast	(QString, QString);
QString	SplitStrFirst	(QString, QString);
QString	RemoveCh		(QString, QString);
QString	FilenameFromLink(QString);


#endif //__DownMan__
