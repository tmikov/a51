// err.h: Error codes, functions, etc.

#ifndef ERR
#define ERR

// error severity codes
#define ERR_FATAL       0       // fatal error
#define ERR_ERROR       1       // error
#define ERR_INFO        2       // info
#define ERR_WARN0       3       // warning level 0

void PrintErrorStr ( int severity, const char * msg, ... );
void Error ( const char * fmt, ... );
void Warning ( const char * fmt, ... );
void Fatal ( const char * fmt, ... );
void OutOfMemory ( void );

extern int  NumErrors;
extern int  NumWarnings;
extern BOOL fAnyErrors;
extern int MaxErrors;
extern int MaxWarnings;

#endif /* err.h */

