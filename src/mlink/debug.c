#include "global.h"
#pragma hdrstop
#include <stdarg.h>

static char szDebugLogName[] = "DEBUG.LOG";
static BOOL used = FALSE;

void debug_trace ( const char * fmt, ... )
{
  FILE * debugLog;

  if (!used)    // if first call to debug_trace => create empty file
  {
    used = TRUE;
    debugLog = fopen( szDebugLogName, "wt" );
  }
  else   // else just append to end of file
    debugLog = fopen( szDebugLogName, "at" );

  if (debugLog)
  {
    va_list ap;
    va_start( ap, fmt );

    vfprintf( debugLog, fmt, ap );
    fclose( debugLog );

    va_end( ap );
  }
};