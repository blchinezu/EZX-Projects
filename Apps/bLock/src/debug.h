#ifndef _DEBUG_H_
#define _DEBUG_H_

#include "main.h"

// Debug message importance
#define DIE		3
#define ERROR	2
#define WARNING	1
#define INFO	0

// Debug output
bool	dbg	(int, char*);
bool	dbg	(int, char*, char*);
bool	dbg	(int, char*, char*, char*);
bool	dbg	(int, char*, char*, char*, char*);
bool	dbg	(int, char*, char*, char*, char*, char*);

// Unite char strings
char*	un	(char*, char*);
char*	un	(char*, char*, char*);
char*	un	(char*, char*, char*, char*);
char*	un	(char*, char*, char*, char*, char*);

#endif /* _DEBUG_H_ */

