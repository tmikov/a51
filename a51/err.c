#include "common/global.h"
#pragma hdrstop

#define ERR_C   // we need the error strings
#include "a51.h"

// TODO: Error buffering & sorting

static char * szErrTitle[] =
{
  "Fatal  ",
  "Error  ",
  "Info   ",
  "Warning"
};

static void VPrintError ( int severity, const char * msg, va_list ap, BOOL p1 )
{
  if (severity < ERR_WARN0)
    bAnyErrors = TRUE;
  if (severity >= ERR_WARN0 && !bWarnings)
    return;
  if (!bGenerate && !p1 && severity != ERR_FATAL)
    return;

  if (IncludeTop)
  {
    printf( "%s %s %d: ",
            severity < _countof( szErrTitle ) ?
               szErrTitle[ severity ] :
               szErrTitle[ _countof( szErrTitle ) - 1 ],
            IncludeTop->pName->name, IncludeTop->CurLine );
  }
  else
  {
    printf( "%s: ",
            severity < _countof( szErrTitle ) ?
            szErrTitle[ severity ] :
            szErrTitle[ _countof( szErrTitle ) - 1 ] );
  }


  vprintf( msg, ap );
  putchar( '\n' );

  va_end( ap );

  NumErrors   += severity == ERR_ERROR;
  NumWarnings += severity == ERR_WARN0;

  if (severity == ERR_FATAL)
    ErrorJump( 1 );

  if (NumErrors >= MaxErrors)
    PrintFatal( FATAL_MANY_ERRS );
  if (NumWarnings >= MaxWarnings)
    PrintFatal( FATAL_MANY_WARNS );
};

void PrintErrorStr ( int severity, const char * msg, ... )
{
  va_list ap;
  va_start( ap, msg );
  VPrintError( severity, msg, ap, FALSE );
  va_end( ap );
};

void PrintError ( int code, ... )
{
  va_list ap;
  va_start( ap, code );
  VPrintError( ERR_ERROR, ErrMsg[code], ap, FALSE );
  va_end( ap );
};

void PrintWarning ( int code, ... )
{
  va_list ap;
  va_start( ap, code );
  VPrintError( ERR_WARN0, ErrMsg[code], ap, FALSE );
  va_end( ap );
};

void PrintFatal ( int code, ... )
{
  va_list ap;
  va_start( ap, code );
  VPrintError( ERR_FATAL, ErrMsg[code], ap, FALSE );
  va_end( ap );
};

void OutOfMemory ( void )
{
//  PrintFatal( FATAL_NO_MEMORY );
  PrintErrorStr( ERR_FATAL, strerror( ENOMEM ) );
};

void PrintError1 ( int code, ... )
{
  va_list ap;
  va_start( ap, code );
  VPrintError( ERR_ERROR, ErrMsg[code], ap, TRUE );
  va_end( ap );
};

void PrintWarning1 ( int code, ... )
{
  va_list ap;
  va_start( ap, code );
  VPrintError( ERR_WARN0, ErrMsg[code], ap, TRUE );
  va_end( ap );
};


