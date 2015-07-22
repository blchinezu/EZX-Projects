#include "filebrowser.h"


// Browser only
FileDlg::FileDlg(QWidget* p,const QString& dir,const QString& nameFilter,int filter,int sort)
	:UTIL_Dialog(UTIL_Dialog::DTLargeSize,true,p,"OpenFile Dialog",1,0)
{
	setHasTitle(true);
	setDlgTitle(tr("Choose path"));
   //QVBox *cw = new QVBox(this);
   //setDlgContent(cw);
	browser=new FileView(nameFilter,filter, this/*getDlgContent()*/,sort, "browser");
	setDlgContent(browser);
	
	if(dir == "") browser->setDir("/");
	else browser->setDir(dir);

	QHBox *hb = new QHBox(this);
	setDlgCst(hb);

	ZPushButton *okBtn = new ZPushButton(hb, 0, -1, -1);
	okBtn->setText(tr("OK"));	
	connect(okBtn,SIGNAL(clicked()),this,SLOT(accept()));
	
	ZPushButton *cancelBtn=new ZPushButton(hb, 0, -1, -1);
	cancelBtn->setText(tr("Cancel"));	
	connect(cancelBtn,SIGNAL(clicked()),this,SLOT(reject()));
	
	ZPushButton *backBtn=new ZPushButton(hb, 0, -1, -1);
	backBtn->setText(tr("Back"));
	connect(browser,SIGNAL(rootDir(bool)),backBtn,SLOT(setDisabled(bool)));
	connect(backBtn,SIGNAL(clicked()),this,SLOT(dirUp()));
}



// Browser with inputbox
FileDlg::FileDlg(QWidget* p,const QString& dir,const QString& file,const QString& nameFilter,int filter,int sort)
	:UTIL_Dialog(UTIL_Dialog::DTLargeSize,true,p,"browse dialog",1,0)
{
	setHasTitle(false);
	//setDlgTitle(tr("Choose path"));
	QVBox *cw = new QVBox(this);
	setDlgContent(cw);
	
	inputbox=new QLineEdit(cw, "inputbox");
	inputbox->setFrame(true);
	
	inputbox->setText(file);
	
	browser=new FileView(nameFilter,filter, cw/*getDlgContent()*/,sort, "browser");
	
	if(dir == "") browser->setDir("/");
	else browser->setDir(dir);

	QHBox *hb = new QHBox(this);
	setDlgCst(hb);

	ZPushButton *okBtn = new ZPushButton(hb, 0, -1, -1);
	okBtn->setText(tr("OK"));	
	connect(okBtn,SIGNAL(clicked()),this,SLOT(accept()));
	
	ZPushButton *cancelBtn=new ZPushButton(hb, 0, -1, -1);
	cancelBtn->setText(tr("Cancel"));	
	connect(cancelBtn,SIGNAL(clicked()),this,SLOT(reject()));
	
	ZPushButton *backBtn=new ZPushButton(hb, 0, -1, -1);
	backBtn->setText(tr("Back"));
	connect(browser,SIGNAL(rootDir(bool)),backBtn,SLOT(setDisabled(bool)));
	connect(backBtn,SIGNAL(clicked()),this,SLOT(dirUp()));
}

void FileDlg::setTitle(const QString& t) { setDlgTitle(t);}
void FileDlg::setDir(const QString& dir) { browser->setDir(dir);}
void FileDlg::setInputbox(const QString& txt) { inputbox->setText(txt);}
QString FileDlg::getFileAbsPath() const { return browser->getAbsPath();}
QString FileDlg::getDir() const { return browser->getFolder();}
QString FileDlg::getInputbox() const { return inputbox->text();}
FileDlg::~FileDlg() {}


void FileDlg::dirUp()
{
	QString dirNow=browser->getFolder();
	int index=dirNow.findRev("/");
	if(index!=0) browser->setDir(dirNow.left(index));
	else browser->setDir("/");
}


QString FileDlg::getFileDirect(QWidget* p,const QString& dir,const QString& filter)
{
	QString s="";
//	FileDlg* dlg = new FileDlg(p,dir,filter);
	FileDlg* dlg = new FileDlg(p,dir,filter,QDir::All |QDir::Hidden, QDir::Name | QDir::DirsFirst | QDir::IgnoreCase);
	if(dlg->exec()==QDialog::Accepted)
		s=dlg->getFileAbsPath();
	
	delete dlg;
	return s;
}






QString FileDlg::getDirDirect()
	{ return getDirDirect(0, "/", tr("Choose Directory")); }

QString FileDlg::getDirDirect(QWidget* p, const QString& loc, const QString& ttl)
{
	int index;
	QString s="", dd=loc;
	QDir dir(loc,"*");
	
	// If the current folder is not readable go back until a readable folder is found
	while(!dir.isReadable()) {
		dd = dir.path();
		if(dd == "/") {
			dd = "/";
		} else {
			index=dd.findRev("/");
			if(index!=0)	dd = dd.left(index);
			else			dd = "/";
		}
		dir.setPath(dd);
	}
	
	FileDlg* dlg=new FileDlg(p,dd,"*",QDir::Dirs |QDir::Hidden, QDir::Name | QDir::IgnoreCase);
	dlg->setTitle(ttl);
	if(dlg->exec()==QDialog::Accepted)
		s=dlg->getDir();
	
	delete dlg;
	return s;
}




QString FileDlg::saveTo()
	{ return saveTo(0, "/"); }

QString FileDlg::saveTo(QWidget* p, const QString& loc)
{
	int index;
	QString s="", dd="", df="";
	dd = SplitStrLastRest(loc, "/");
	df = SplitStrLast(loc, "/");
	
	// If the location ends with "/" make file null
	char tt[strlen((const char*)loc)];
	sprintf(tt, "%s", (const char*)loc);
	if ( tt[strlen(tt)-1] == '/' ) {
		dd = loc;
		df = "";
	}
	
	QDir dir(dd,"*");
	
	// If the current folder is not readable go back until a readable folder is found
	while(!dir.isReadable()) {
		dd = dir.path();
		if(dd == "/") {
			dd = "/";
		} else {
			index=dd.findRev("/");
			if(index!=0)	dd = dd.left(index);
			else			dd = "/";
		}
		dir.setPath(dd);
	}
	
	FileDlg* dlg = new FileDlg(p,dd,df,"*",
		QDir::Dirs | QDir::Hidden, 
		QDir::Name | QDir::IgnoreCase);
	
	dlg->setTitle(tr("Save to..."));
	if(dlg->exec()==QDialog::Accepted)
//		s = dlg->getDir() + QString("/") + RemoveCh(dlg->getInputbox(),"/");
		s = dlg->getDir() + QString("/") + dlg->getInputbox();
	
	delete dlg;
	
	return s;
}

