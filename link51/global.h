#ifndef GLOBAL_H
#define GLOBAL_H


///////////////////////////////////////////////////////////////////////////////
// definitions that could optimize the code but are specific for the compiler

// INLINE :specifies an explicit inline function
#if _MSC_VER >= 1000
#  define INLINE  __inline
#else
#  define INLINE  
#endif

// FAR     : modifier for far pointer in large DOS models
// _DSPTR  : modifier for DS pointers in large DOS models
#if defined(_WIN32)
#  define FAR
#  define _DSPTR
#else //_WIN32
#  if defined(__TURBOC__)
#    define _DSPTR  _ds
#    define FAR _far
#  else
#    define _DSPTR
#    define FAR 
#  endif
#endif // _WIN32

#define INTEL_BYTE_ORDER  1

#ifdef __TURBOC__
#define _strcmpi  stricmp
#define _unlink   unlink
#endif
#ifdef __GNUC__
#define _strcmpi  stricmp
#define _unlink   unlink
#endif

#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <limits.h>
#include <setjmp.h>
#include <stdarg.h>
#include <ctype.h>
#include <errno.h>  /* Because of DJGPP */

#if defined(__TURBOC__)
#  include <mem.h>
#endif
#if defined(__GNUC__)
#  include <dir.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif


////////////////////////////////////////////////////////////////////
// Standart Windows compatible type definitions

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

#define TRUE  1
#define FALSE 0

///////////////////////////////////////////////////////////////////
// Useful macros

#define _countof( x )         (sizeof( x ) / sizeof( (x)[0] ))
#define offsetof( s, x )      ((UINT)&(((s *)0)->x))


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

#ifdef __cplusplus
};
#endif

#ifdef __STDC__
#  define CDECL 
#else
#  define CDECL  _cdecl
#endif

#ifndef _MAX_PATH
#  define _MAX_PATH MAXPATH
#endif
#ifndef _MAX_FNAME
#  define _MAX_FNAME MAXFILE
#endif
#ifndef _MAX_EXT
#  define _MAX_EXT MAXEXT
#endif

#endif // GLOBAL_H
