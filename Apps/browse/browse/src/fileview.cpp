#include "fileview.h"

//const QString head[]={"Name","Size","Time","Permision"}; //time: last read/modified
FileView::FileView(const QString& namefilter,int filter,QWidget *p,int sort,const char* name,WFlags f)
	:QListView(p,name,f),Filter(filter),Sort(sort),FolderOnly(false),NameFilter(namefilter)
{
	//Filter= QDir::All | QDir::Hidden | QDir::System ;//| QDir::Dirs; 
	//if(!FolderOnly) Filter |= QDir::Dirs;
	//Sort=QDir::Name | QDir::DirsFirst | QDir::IgnoreCase ;
	
	RES_ICON_Reader ir;
	jar_xpm = ir.getIcon("FMMS_File_Game_S.g",true);
	jpg_xpm = ir.getIcon("FMMS_File_Image_S.g",true);
	pf_xpm = ir.getIcon("FMMS_Personal_File_S.g",true);
	dir_xpm = ir.getIcon("FMMS_Personal_Folder_S.g",true);
	mp3_xpm = ir.getIcon("FMMS_Folder_Video_S.g",true);
	avi_xpm = ir.getIcon("FMMS_Folder_Audio_S.g",true);
	txt_xpm = ir.getIcon("FMMS_Folder_Note_S.g",true);
	script_xpm = ir.getIcon("FMMS_Folder_Program_S.g",true);
	mmc_xpm = ir.getIcon("FMMS_Personal_MMC_SD.g",true);
	//for(int i=0;i<4;i++) addColumn(head[i]);
	//if(!FolderOnly) addColumn("Size");
	//如何判断只显示dir?
	if(Filter==QDir::Dirs || Filter==(QDir::Dirs |QDir::Hidden)){
		//qDebug("Dirs only");
		FolderOnly=true;
		//setColumnWidth(1,195);
		//header()->setResizeEnabled( false );
		addColumn("                                               ");
	} else {
		addColumn("Name");
		addColumn("Size");
	}
	header()->hide();
	
	connect(this,SIGNAL(clicked(QListViewItem*,const QPoint&,int)),SLOT(itemClicked(QListViewItem*,const QPoint&,int)));
	connect(this,SIGNAL(pressed(QListViewItem*,const QPoint&,int)),SLOT(itemPressed(QListViewItem*,const QPoint&,int)));
}

FileView::~FileView()
{}

inline QPixmap FileView::getFileXpm(const QString& file) const
{
		QString ext=QFileInfo(file).extension().upper();
		if(QFileInfo(file).isDir()){
 			if(file.length()==10 && file.left(8)=="/mmc/mmc") return mmc_xpm;
 			return dir_xpm;
 		}
 		if(ext=="JPG"||ext=="JPEG"||ext=="GIF"||ext=="TIFF"||ext=="PNG"||ext=="BMP"||ext=="XPM") return jpg_xpm;
 		if(ext=="MP3"||ext=="WMA"||ext=="WAV"||ext=="MID"||ext=="M4A"||ext=="AAC"||ext=="FLAC") return mp3_xpm;
 		if(ext=="AVI"||ext=="MP4"||ext=="FLV"||ext=="3GP"||ext=="WMV") return avi_xpm;
 		if(ext=="LIN"||ext=="SH"||ext=="PL"||ext=="PY") return script_xpm;
 		if(ext=="JAR") return jar_xpm;
// 		if(ext=="TXT"||ext=="CFG"||ext=="INI"||ext=="CONF") return txt_xpm; 
 		return txt_xpm;
}

//void int getFileType(const QString& f) //判断char-dev,pipe,socket,hardlink
void FileView::setDir(const QString& d)
{
	if(QDir(d).isRoot()) emit rootDir(true);
	else emit rootDir(false);
	
	QDir dir(d,NameFilter);
	dir.setMatchAllDirs(true);
	
	dir.setSorting(Sort);//QDir::Name | QDir::DirsFirst | QDir::IgnoreCase); //QListView::sort()后会乱次序
	dir.setFilter(Filter);//QDir::All | QDir::Hidden | QDir::System);  //若是pipe,socket,dev,setDir()出错
	
	if(!dir.isReadable()) return;
	clear();
	
	const QFileInfoList *list=dir.entryInfoList(NameFilter,Filter,Sort);
	QFileInfoListIterator it( *list );      // create list iterator
	QFileInfo *fi;
    
	while((fi=it.current())) 
	{
		QString file=fi->fileName();
		if((file!= ".")&&(file!= ".."))
		{
			//qDebug(d+ d.right(1)=="/" ? "" : "/"+file);
			bool is_dir=QFileInfo(fi->absFilePath()).isDir();
			if(!is_dir && FolderOnly) break;
			item=new FileViewItem(this,file,FolderOnly ? QString::null : (is_dir ? QString::null : QString::number(fi->size())));
			item->setPixmap(0,getFileXpm(fi->absFilePath()));
		}
		++it;
		//qApp->processEvents();
	}
	setSorting(0,false);  //Folder first sorting
	folder=dir.absPath();
}

void FileView::itemPressed(QListViewItem* itm,const QPoint&,int)
{
	if(!itm) return;
	path= (folder=="/") ? "/"+itm->text(0) : folder+"/"+itm->text(0);
	//qDebug("highlighted: "+path);
}

void FileView::itemClicked(QListViewItem* itm,const QPoint&,int)
{
	if(!itm) return;

	//path= (folder=="/") ? "/"+itm->text(0) : folder+"/"+itm->text(0);  //hightlight first, so we can remove this line
	//if(QFileInfo(path).isSymLink()) { qDebug("SymLink"); }//path=QFileInfo(path).readLink(); }
	//qDebug("clicked: "+path);
	if(QFileInfo(path).isDir()) setDir(path);
}

QString FileView::getFolder() const { return folder; }

QString FileView::getAbsPath() const { return path; }

void FileView::setFilter(int ft) { Filter=ft; }

void FileView::setFolderOnly(bool fo) { 
	FolderOnly=fo; 
	//if(FolderOnly) Filter &= ~QDir::Dirs;
	//if(!FolderOnly) Filter |= QDir::Dirs;
}

QString FileViewItem::key(int column,bool ascending) const
{
	if(column==0) return QString::null;
	return QListViewItem::key(column,ascending);
}

