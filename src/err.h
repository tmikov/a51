// err.h: Error codes, functions, etc.

// error severity codes
#define ERR_FATAL       0       // fatal error
#define ERR_ERROR       1       // error
#define ERR_INFO        2       // info
#define ERR_WARN0       3       // warning level 0

void PrintErrorStr ( int severity, const char * msg, ... );
void PrintError ( int code, ... );
void PrintWarning ( int code, ... );
void PrintFatal ( int code, ... );
void OutOfMemory ( void );
void PrintError1 ( int code, ... );
void PrintWarning1 ( int code, ... );

// include the error codes
#include "errmsg.inc"

