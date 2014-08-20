#include "common/global.h"
#pragma hdrstop
#define ERR_MODULE
#include "err.h"
#include "common/misc.h"

int  NumErrors;
int  NumWarnings;
BOOL fAnyErrors;
int MaxErrors;
int MaxWarnings;

static char * szErrTitle[] =
{
  "Fatal  ",
  "Error  ",
  "Info   ",
  "Warning"
};

static void VPrintError ( int severity, const char * msg, va_list ap )
{
  if (severity < ERR_WARN0)
    fAnyErrors = TRUE;

  printf( "%s: ",
          severity < _countof( szErrTitle ) ?
          szErrTitle[ severity ] :
          szErrTitle[ _countof( szErrTitle ) - 1 ] );

  vprintf( msg, ap );
  putchar( '\n' );

  va_end( ap );

  NumErrors   += severity == ERR_ERROR;
  NumWarnings += severity == ERR_WARN0;

  if (severity == ERR_FATAL)
    ErrorJump( 1 );

  if (NumErrors >= MaxErrors)
    Fatal( "Too many errors" );
  if (NumWarnings >= MaxWarnings)
    Fatal( "Too many warnings" );
};

void PrintErrorStr ( int severity, const char * msg, ... )
{
  va_list ap;
  va_start( ap, msg );
  VPrintError( severity, msg, ap );
  va_end( ap );
};

void Error ( const char * fmt, ... )
{
  va_list ap;
  va_start( ap, fmt );
  VPrintError( ERR_ERROR, fmt, ap );
  va_end( ap );
};

void Warning ( const char * fmt, ... )
{
  va_list ap;
  va_start( ap, fmt );
  VPrintError( ERR_WARN0, fmt, ap );
  va_end( ap );
};

void Fatal ( const char * fmt, ... )
{
  va_list ap;
  va_start( ap, fmt );
  VPrintError( ERR_FATAL, fmt, ap );
  va_end( ap );
};

void OutOfMemory ( void )
{
  Fatal( "Out of memory" );
};

