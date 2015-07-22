// app : app.h
// demo application for EZX-crosstool
// by eakrin @ gmail.com
// use with FoxE6's SDK

#ifndef  __app__
#define __app__

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

bool FileExists(const char* filename);
QString FilenameFromLink(QString link);
int SplitCount(QString text, QString delimiter);
QString SplitStr(QString text, QString delimiter, int element);
QString SplitStrLastRest(QString text, QString delimiter);
QString SplitStrLast(QString text, QString delimiter);
QString SplitStrFirst(QString text, QString delimiter);
QString RemoveCh(QString text, QString ch);

class app : public ZMainWidget
{
	Q_OBJECT
	public:
		app( int, char**);
};


#endif //__app__
