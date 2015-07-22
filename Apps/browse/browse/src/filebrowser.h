#ifndef FILEBROWSER_H
#define FILEBROWSER_H

#include <qcheckbox.h>
#include <qcombobox.h>
#include <qdir.h>
#include <qhbox.h>
#include <qvbox.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qspinbox.h>

#include <ezxutildialog.h>
#include <ZMultiLineEdit.h>
#include <ZPushButton.h>
#include <ZScrollView.h>

#include "app.h"
#include "fileview.h" // The file browser core (slightly modified)

class QCheckBox;
class QComboBox;
class ZMultiLineEdit;
class FileView;

class FileDlg : public UTIL_Dialog
{
	Q_OBJECT
public:
	// Constructor normal
	FileDlg(QWidget* p=0,const QString& dir="/",const QString& nameFilter="*", int filter=QDir::All |QDir::Hidden |QDir::System, \
	int sort=QDir::Name | QDir::DirsFirst | QDir::IgnoreCase);
	// Constructor with inputbox
	FileDlg(QWidget* p=0,const QString& dir="/",const QString& file="",const QString& nameFilter="*", int filter=QDir::All |QDir::Hidden |QDir::System, \
	int sort=QDir::Name | QDir::DirsFirst | QDir::IgnoreCase);
	// Destructor
	~FileDlg();
	
	// Data manip
	void	setTitle		(const QString&);
	void 	setDir			(const QString&);
	void 	setInputbox		(const QString&);
	QString	getFileAbsPath	() const;
	QString	getDir			() const;
	QString	getInputbox		() const;
	
	// File mode
	static QString getFileDirect(QWidget* p=0,const QString& dir="/",const QString& filter="*");
	// Dir mode
	static QString getDirDirect();
	static QString getDirDirect(QWidget* p, const QString&, const QString&);
	static QString saveTo();
	static QString saveTo(QWidget* p, const QString&);
	
public slots:
	void dirUp();
	
private:
	FileView* browser;
	QLineEdit* inputbox;
};

#endif //FILEBROWSER_H

