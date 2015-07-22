//platform.h
//

#if defined(WIN32) || defined(_WIN32)
//win32 pragmas to help prevent STL truncation warnings
#pragma warning(disable:4503)
#pragma warning(disable:4786)
#pragma warning(disable:4788) 
//MSVC compiler + runtime
#define round(x) floor(x+0.5)
#define strcasecmp(s1,s2) stricmp(s1,s2)
#else
//GCC compiler
#endif

