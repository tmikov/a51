#ifndef GLOBAL_H
#define GLOBAL_H

////////////////////////////////////////////////////////////////////
// Standart Windows compatible type definitions

#ifndef USE_WINDOWS

// FAR     : modifier for far pointer in large DOS models
// _DSPTR  : modifier for DS pointers in large DOS models
#if defined(MSDOS) && defined(__TURBOC__)
#  define _DSPTR  _ds
#  define FAR _far
#else
#  define _DSPTR
#  define FAR 
#endif 

// Fixed size types
typedef unsigned long  DWORD;
typedef unsigned short WORD;
typedef unsigned char  BYTE;

typedef int           BOOL;
typedef unsigned      UINT;
typedef unsigned long ULONG;
typedef long          LONG;

typedef void *        PVOID;
typedef void FAR *    LPVOID;

typedef unsigned char UCHAR;

typedef unsigned char BOOLEAN;

typedef const char *   LPCTSTR;

#ifdef _UNICODE
#  define _T( x )     L ## x
#else
#  define _T( x )     x
#endif

#define TRUE  1
#define FALSE 0

///////////////////////////////////////////////////////////////////
// Useful macros defined in Windows.h

#define MulDiv( a, b, c )     ((long)(a) * (b) / (c))
#define MAKELONG( l, h )      ((WORD)(l) + ((DWORD)((WORD)(h)) << 16))

#ifdef __STDC__
#  define CDECL 
#else
#  define CDECL  _cdecl
#endif

#else  //USE_WINDOWS

#  include <windows.h>

#endif //USE_WINDOWS

////////////////////// All include files ////////////////////////
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <limits.h>
#include <setjmp.h>
#include <stdarg.h>
#include <ctype.h>

///////////////////////////////////////////////////////////////////
// Useful macros

#ifndef offsetof        
#  define offsetof( s, x )      ((UINT)&(((s *)0)->x))
#endif
#ifndef _countof
#  define _countof( x )         (sizeof( x ) / sizeof( (x)[0] ))
#endif

//////////////////////////////////////////////////////////////////
// Debug support

void debug_trace ( const char * fmt, ... );

#ifdef _DEBUG
#  define ASSERT( x )       assert( x )
#  define VERIFY( x )       ASSERT( x )
#  define TRACE0( x )       debug_trace( x )
#  define TRACE1( x, a1 )   debug_trace( x, a1 )
#  define TRACE2( x, a1, a2 )  debug_trace( x, a1, a2 )
#  define TRACE3( x, a1, a2, a3 )  debug_trace( x, a1, a2, a3 )
extern char szDebugLogName[];
extern BOOL bDebugTraceEnabled;
#else
#  define ASSERT( x )
#  define VERIFY( x )       (x)
#  define TRACE0( x )
#  define TRACE1( x, a1 )
#  define TRACE2( x, a1, a2 )
#  define TRACE3( x, a1, a2, a3 )
#endif

///////////////////////////////////////////////////////////////////////////////
// definitions that could optimize the code but are specific for the compiler

// INLINE :specifies an explicit inline function
#if (_MSC_VER >= 1000) || (__GNUC__ > 1)
#  define INLINE  __inline
#  define SUPPORT_INLINE  1
#else
#  define INLINE  
#endif

#define INTEL_BYTE_ORDER  1

// The section below is used to iron out the diferences between verious
// compilers I have used. The general case is the last.
//
#if  defined(__TURBOC__)

//char * ultoa ( unsigned long num, char * res, int radix );
//char * ltoa ( long num, char * res, int radix );
//char * itoa ( int num, char * res, int radix );

#define  _stricmp stricmp

#elif defined(_MSC_VER)

//char * ultoa ( unsigned long num, char * res, int radix );
//char * ltoa ( long num, char * res, int radix );
//char * itoa ( int num, char * res, int radix );

//int _stricmp ( const char * dst, const char * src  );

#elif defined(__DJGPP__)

char * ultoa ( unsigned long num, char * res, int radix );
char * ltoa ( long num, char * res, int radix );
char * itoa ( int num, char * res, int radix );

#define  _stricmp stricmp

#elif defined(__WATCOMC__)

char * ultoa ( unsigned long num, char * res, int radix );
char * ltoa ( long num, char * res, int radix );
char * itoa ( int num, char * res, int radix );

#define  _stricmp stricmp

#else

char * ultoa ( unsigned long num, char * res, int radix );
char * ltoa ( long num, char * res, int radix );
char * itoa ( int num, char * res, int radix );

int _stricmp ( const char * dst, const char * src  );
char * strupr ( char * s );
#endif

#endif // GLOBAL_H
