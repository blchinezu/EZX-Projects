#ifndef FILEVIEW_H
#define FILEVIEW_H

#include <qlistview.h>
#include <qfileinfo.h>
#include <qdir.h>
#include <qheader.h>

#include <ezxres.h>
#include <UTIL_ListViewItem.h>
#include <ZGlobal.h>
#include <ZApplication.h>

class QListView;
class QListViewItem;

class FileViewItem :public QListViewItem
{
public:
	FileViewItem(QListView* parent,QString label0,/*bool key,*/QString label1=QString::null,QString label2=QString::null,QString label3=QString::null, \
	QString label4=QString::null,QString label5=QString::null,QString label6=QString::null,QString label7= QString::null)
		:QListViewItem(parent,label0,/*key,*/label1,label2,label3,label4,label5,label6,label7)
	{}
	
	~FileViewItem() {};
	QString key(int column,bool ascending) const; //用于排序
};


class FileView :public QListView
{
	Q_OBJECT
public:
	//enum FilterSpec { All=QDir::All | QDir::Hidden | QDir::System, Dir=QDir
	FileView(const QString& namefilter="*",int filter=QDir::All |QDir::Hidden |QDir::System,
	QWidget* parent=0,int sort=QDir::Name |QDir::DirsFirst |QDir::IgnoreCase,const char* name=0,WFlags f=0);
	~FileView();
	
	void	setFolderOnly	(bool);
	void	setFilter		(int);
	void	setDir			(const QString&);	// { dir=d;}
	QString	getFolder		() const;
	QString	getAbsPath		() const; 			//folder or file highlighted
	//inline 
	QPixmap	getFileXpm		(const QString &) const ;

signals:
	void 	rootDir			(bool);	
private:
	//QDir::FilterSpec 
	int		Filter;
	//QDir::SortSpec 
	int		Sort;
	bool	FolderOnly;
	QString	NameFilter;
	QString	folder;
	QString	path; //folder or file path
	QPixmap	mmc_xpm, dir_xpm, jar_xpm, mp3_xpm, jpg_xpm, avi_xpm, script_xpm, txt_xpm, pf_xpm; //doc pdf excel
	FileViewItem *item;

private slots:
	void itemClicked(QListViewItem*,const QPoint&,int);
	void itemPressed(QListViewItem*,const QPoint&,int);
};

#endif //FILEVIEW_H

