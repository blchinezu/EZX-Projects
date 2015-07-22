// Copyright 2010, Abrek
// Modified by BruceLee @ MororolaFans.com

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <string.h>

#include <qobject.h>
#include <qlayout.h>
#include <qframe.h>
#include <qtimer.h>

#include <ZApplication.h>
#include <ZLanguage.h>
#include <ZPushButton.h>
#include <ZMainWidget.h>
#include <ZMessageBox.h>
#include <ZBaseDialog.h>
#include <SETUP_SET_Manager.h>
#include <ezxres.h>

#include "ezxpower.h"


void vibro() 
{
	#define VIBRATOR_DEV "/dev/vibrator"
	#define VIBRATOR_MAJOR	108
	#define VIBRATOR_IOCTL_BASE	0xbb
	#define VIBRATOR_ENABLE		_IOW (VIBRATOR_IOCTL_BASE,1,int)
	#define VIBRATOR_DISABLE	_IO (VIBRATOR_IOCTL_BASE,2)
	
	int fd, arg=1, argg=0;
	fd = open(VIBRATOR_DEV,  O_RDWR);	
	ioctl(fd, VIBRATOR_ENABLE, &arg);
	usleep(100000);
	ioctl(fd, VIBRATOR_DISABLE, &argg);
	close(fd);
}

Power::Power(QWidget *parent,const char *name, uint f)
: ZMainWidget(parent,name, f)
{
#ifdef WITH_HIDE_BUTTON
	int start=40, step=50;
#else
	int start=65, step=50;
#endif
	
 	const QString lng="ezxpower_";
 	ZLanguage::getLanguageStringByLanguageCode(language_string_path);
	language=ZLanguage::getSystemLanguageCode();
	
	ts=new QTranslator(this);
	qApp->installTranslator(ts);	
	ts->load(lng+language,language_string_path);
 	
	bt = new ZPushButton(this,0);
	bt->setText( tr("Reboot") );
	bt->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed));
	bt->setGeometry(40,start, 160, 40 );
	connect(bt,SIGNAL(clicked()),this,SLOT(reboot()));
	
	bt1 = new ZPushButton(this,0);
	bt1->setText( tr ("Fast Reboot") );
	bt1->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed));
	bt1->setGeometry(40,start+step, 160, 40 );
	connect(bt1,SIGNAL(clicked()),this,SLOT(freboot()));
	
	bt2 = new ZPushButton(this,0);
	bt2->setText( tr ("Shutdown") );
	bt2->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed));
	bt2->setGeometry(40,start+(2*step), 160, 40 );
	connect(bt2,SIGNAL(clicked()),this,SLOT(shutdown()));
	
	bt3 = new ZPushButton(this,0);
	bt3->setText( tr ("Fast Shoutdown") );
	bt3->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed));
	bt3->setGeometry(40,start+(3*step), 160, 40 );
	connect(bt3,SIGNAL(clicked()),this,SLOT(fshutdown()));

#ifdef WITH_HIDE_BUTTON
	bt4 = new ZPushButton(this,0);
	bt4->setText( tr ("Hide") );
	bt4->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed));
	bt4->setGeometry(40,start+(4*step), 160, 40 );
	connect(bt4,SIGNAL(clicked()),this,SLOT(slot_hide()));
#endif
	
	QHBoxLayout *layout = new QHBoxLayout;
	layout->addWidget( bt );
	layout->addWidget( bt1 );
	layout->addWidget( bt2 );
	layout->addWidget( bt3 );
#ifdef WITH_HIDE_BUTTON
	layout->addWidget( bt4 );
#endif
	
	// Update the 'Shutdown' button text
	ttime=WAIT_TILL_EXIT;
	updateButtonText();
	
	// Initialize timer
    QTimer * counter = new QTimer( this );
    connect( counter, SIGNAL(timeout()),
             this, SLOT(updateTimer()) );
    counter->start( 1000 );
}

void Power::reboot() {
	vibro();
	msg=QString(tr ("\n Reboot...") );
	system("start-stop-daemon -S -c root -x /sbin/reboot");
	smessage();
}

void Power::freboot() {
	vibro();
	msg=QString(tr ("\n Reboot...") );
	system("reboot -ndf");
	smessage();
}

void Power::shutdown() {
	vibro();
	msg=QString(tr ("\n Shutdown...") );
	system("start-stop-daemon -S -c root -x /sbin/poweroff");
	smessage();
}

void Power::fshutdown() {
	vibro();
	msg=QString(tr ("\n Shutdown...") );
	system("poweroff -ndf"); 
	smessage();
}

void Power::slot_hide() {
	// Mark the button press and quit the QT app
	vibro();
	//glob_hide_press=true;
	qApp->exit(1);
}

void Power::updateTimer() {
	if (ttime > 0) {
		// Update button text
		ttime--;
		updateButtonText();
	}
	else if (ttime == 0) {
		// Shutdown when time is out
		msg=QString(tr ("\n Shutdown...") );
		system("start-stop-daemon -S -c root -x /sbin/poweroff");
		smessage();
	}
}

void Power::updateButtonText()
{
	// Create a string var to hold the number
	char num[10];
	// Put the number into the string
	sprintf(num, " [%d]", ttime);
	// Update the button text
	bt2->setText( QString( tr ("Shutdown") ) + QString(num) );
}

void Power::smessage()
{
	SETUP_SET_Manager mgr("/ezx_user/download/appwrite/setup/ezx_theme.cfg", false);
	mgr.getSettingInfo(QString("THEME_CONFIG_TABLE"), QString("WallPaper"), FileName, 0, 0);

	RES_ICON_Reader resIcon;
	QPixmap pix(0,0);
	QPixmap pix1(FileName);
	pix = resIcon.getIcon("Dialog_Exclamatory_Mark.gif",false);

	ZBaseDialog *dlg = new ZBaseDialog(this,0,ZBaseDialog::APP_MODAL,0);
	dlg->setBackgroundPixmap(pix1);
	dlg->resize(240,320);
	dlg->show();

	ZMessageBox* msgbox = new ZMessageBox(dlg, pix, msg, 10);
	msgbox->exec();
	delete msgbox;
	delete dlg;
}

