#include "debug.h"

// Debug output
bool dbg(int TYPE, char* m1) { return dbg(TYPE, m1, "", "", "", ""); }
bool dbg(int TYPE, char* m1, char* m2) { return dbg(TYPE, m1, m2, "", "", ""); }
bool dbg(int TYPE, char* m1, char* m2, char* m3) { return dbg(TYPE, m1, m2, m3, "", ""); }
bool dbg(int TYPE, char* m1, char* m2, char* m3, char* m4) { return dbg(TYPE, m1, m2, m3, m4, ""); }
bool dbg(int TYPE, char* m1, char* m2, char* m3, char* m4, char* m5) {
	switch(TYPE) {
		#ifdef DEBUG
		case INFO:		printf(" > %s%s%s%s%s\n", m1,m2,m3,m4,m5);													break;
		case WARNING:	printf("\n WARNING: \n >> %s%s%s%s%s\n\n", m1,m2,m3,m4,m5);									break;
		#endif

		case ERROR:		printf("\n ERROR: \n >> %s%s%s%s%s\n\n", m1,m2,m3,m4,m5);										break;
		case DIE:		printf("\n CRITICAL ERROR: \n >> %s%s%s%s%s\n >> EXITING\n\n", m1,m2,m3,m4,m5); app.Quit();	return false;
			
		#ifdef DEBUG
		default:		printf("\n WARNING: \n >> Got unknown TYPE value\n >> dbg(%i, %s, %s, %s, %s, %s)\n\n", TYPE,m1,m2,m3,m4,m5);
		#endif
	}
	return true;
}



// Unite char strings
char* un(char* m1, char* m2)							{ return un(m1, m2, "", "", ""); }
char* un(char* m1, char* m2, char* m3)					{ return un(m1, m2, m3, "", ""); }
char* un(char* m1, char* m2, char* m3, char* m4)		{ return un(m1, m2, m3, m4, ""); }
char* un(char* m1, char* m2, char* m3, char* m4, char* m5) {
	char buff[MAX_UN_LEN];
	sprintf(buff, "%s%s%s%s%s", m1, m2, m3, m4, m5);
	return buff;
}

