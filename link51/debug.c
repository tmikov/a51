#include "global.h"
#pragma hdrstop
#include <stdarg.h>

char szDebugLogName[_MAX_PATH] = "DEBUG.LOG";
BOOL bDebugTraceEnabled = TRUE;
static BOOL used = FALSE;

void debug_trace ( const char * fmt, ... )
{
  FILE * debugLog;

  if (!bDebugTraceEnabled)
    return;

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
  else
  {
    printf("Fatal: log file (%s) error - %s\n", szDebugLogName, sys_errlist[errno]);
    abort();
  };
};
