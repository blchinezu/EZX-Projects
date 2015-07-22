#include "app.h"
#include <iostream.h>

bool FileExists(const char* filename) {
	struct stat info;
	int ret = -1;
	ret = stat(filename, &info);
	if(ret == 0)	return true;
	else			return false;
}

QString FilenameFromLink(QString link) {
	QString fname;
	fname = SplitStrFirst(link,  "#"); // Remove anchors in link
	fname = SplitStrFirst(fname, "?"); // Remove php args
	fname = SplitStrLast (fname, "/"); // Remove dir path to file
	// Return file name
	return fname;
}

int SplitCount(QString text, QString delimiter) {
	int i=0;
	// Text from QString to char
	char str[strlen((const char*)text)];
	sprintf(str, "%s", (const char*)text);
	// Get the first token
	char *p = strtok(str, (const char*)delimiter);
	// Loop and split to tokens while counting
	while (p) { i++; p = strtok(NULL, (const char*)delimiter); }
	// Return tokens number
	return i;
}

// string, delimiter character, requested token number
QString SplitStr(QString text, QString delimiter, int element) {
	int i=0;
	// Text from QString to char
	char str[strlen((const char*)text)];
	sprintf(str, "%s", (const char*)text);
	// Count maximum resulting tokens
	int tokens=SplitCount(text, delimiter);
	// Make sure the requested element is not out of range
	while(element>=tokens) element--;
	// Get the first token
	char *p = strtok(str, (const char*)delimiter);
	// Enter loop until it gets to the requested element
	while (p) {
		// If this is the element break loop
		if(i==element) break;
		p = strtok(NULL, (const char*)delimiter);
		i++;
	}
	// Return token
	return QString(p);
}

QString SplitStrLastRest(QString text, QString delimiter) {
	QString ret="";
	// Text from QString to char
	char str[strlen((const char*)text)];
	sprintf(str, "%s", (const char*)text);
	// Count maximum resulting tokens
	int tokens=SplitCount(text, delimiter);
	// Get the first token
	char *p = strtok(str, (const char*)delimiter);
	for(int i=1; i<tokens; i++) {
		ret.append(delimiter); ret.append(p);
		p = strtok(NULL, (const char*)delimiter);
	}
	// Return token
	return ret;
}

QString SplitStrLast(QString text, QString delimiter) 
{ return SplitStr(text, delimiter, SplitCount(text,delimiter)-1); }

QString SplitStrFirst(QString text, QString delimiter) 
{ return SplitStr(text, delimiter, 0); }

QString RemoveCh(QString text, QString ch) {
	QString ret="";
	// Text from QString to char
	char str[strlen((const char*)text)];
	sprintf(str, "%s", (const char*)text);
	// Get the first token
	char *p = strtok(str, (const char*)ch);
	// Append the rest of them
	while (p)
		{ ret.append(p); p = strtok(NULL, (const char*)ch); }
	// Return string
	return ret;
}

app ::app ( int argc, char** argv )
	: ZMainWidget( false, NULL, NULL, 0 )
{
	// Print all the arguments
	//for(int i=0; i<argc; i++) printf("%d=%s\n", i, argv[i]);

	QString syntax = "Syntax: "+QString(argv[0])+" [-f|-d|-s] <path>\n";
	QString dir = "/";
	QString ret;
	char mode = 'f';
	
	if( argc == 2 || argc > 3 )
	{
		cout<<syntax;
		exit(1);
	}
	
	if( QString(argv[1]) == QString("-f") ||
		QString(argv[1]) == QString("-d") ||
		QString(argv[1]) == QString("-s")
	  )
	{
		mode = argv[1][1];
		dir = argv[2];
	}

	switch(mode)
	{
		case 'f':
			ret = FileDlg::getFileDirect(this, dir, "*");
			break;
			
		case 'd':
			ret = FileDlg::getDirDirect(this, dir, "Choose directory");
			break;
			
		case 's':
			ret = FileDlg::saveTo(this, dir);
			break;
	}

	cout<<ret;
	
	exit(0);
}

