#include "common/global.h"
#pragma hdrstop
#include "mlink.h"

#ifdef __TURBOC__
#  define_strcmpi  stricmp
#endif

////////////////////////////////////////////////////////////////////////////
// Global data

const int VerMinor = 10;
const int VerMajor = 0;
const int VerBuild = 2;

FILE * fInFile;
FILE * fOutFile;

////////////////////////////////////////////////////////////////////////////
// local data

static const char szInputExt[]  = ".obj";
static const char szOutputExt[] = ".bin";

static BOOL bHaveFile = FALSE;
char * szInputFile = NULL;
char * szOutputFile = NULL;
char * szBareInputName = NULL;
static jmp_buf  ErrJmpBuf;  // used on exit on fatal errors

//--------------------------------------------------------------------------
// Name         ErrorJump
//
// Description  Called when we want to exit on fatal error
//--------------------------------------------------------------------------
void ErrorJump ( int value )
{
  longjmp( ErrJmpBuf, value );
};

////////////////////////////////////////////////////////////////////////////
// Local functions

static void PrintUsage ( void )
{
  printf(
"usage: MLINK [ options... ] filename[%s]\n"
"options:\n"
    ,
    szInputExt
  );
};

void FatalError ( const char * str, ... )
{
  va_list ap;
  va_start( ap, str );

  printf( "Fatal: " );
  vprintf( str, ap );
  putchar( '\n' );

  va_end( ap );

  ErrorJump( 1 );
};

static void ProcessOption ( char * szOption )
{
  char * s = szOption;
/*
  switch (toupper( szOption[0] ))
  {
  }
*/
  FatalError( "Invalid command line option: '-%s'", szOption );
};

//--------------------------------------------------------------------------
// Name         BuildFileNames
//
// Description  Constructs the input and output file names based on the
//              name obtained from the command line.
//              Also sets szBareInputName
//--------------------------------------------------------------------------
static void BuildFileNames ( void )
{
  char * p;
  char * outn, * bare;

  // check if the input file has an extension
  if (p = strrchr( szInputFile, '.' ))
  {
    // copy the inname to outname without the extension
    outn = (char *)malloc( p - szInputFile + sizeof(szOutputExt) );
    memcpy( outn, szInputFile, p - szInputFile );
    outn[p - szInputFile] = 0;  // terminate the string
  }
  else
  {
    char * inn;
    outn = (char *)malloc( strlen(szInputFile) + sizeof(szOutputExt) );
    strcpy( outn, szInputFile );

    // input has no extension, so we have to add one
    inn = (char *)malloc( strlen(szInputFile) + sizeof(szInputExt) );
    strcat( strcpy(inn, szInputFile), szInputExt );
    free( szInputFile );
    szInputFile = inn;
  }

  // now is the time to set szBareInputName
  // search for a '\\' or ':' which ever comes first
  p = strchr( outn, 0 ); // go to end
  while (p > outn && p[-1] != ':' && p[-1] != '\\' && p[-1] != '/')
    --p;
  free( szBareInputName );
  szBareInputName = strdup( p );
  
  strcat( outn, szOutputExt );
  free( szOutputFile );
  szOutputFile = outn;
};

static void ProcessCmdLine ( int argc, char ** argv )
{
  int i;
  for( i = 1; i < argc; ++i )
  {
    if (argv[i][0] == '-' || argv[i][0] == '/')
    {
      ProcessOption( &argv[i][1] );
    }
    else
    {
      if (bHaveFile)
        FatalError( "Too many files specified" );
      else
      {
        bHaveFile = TRUE;
        szInputFile = strdup(argv[i]);
      }
    }
  }

  if (!bHaveFile)
    FatalError( "No input file specified" );

  BuildFileNames();
};

///////////////////////////////////////////////////////////////////////////
// Global functions

int main ( int argc, char ** argv )
{
  int res;

  printf( "MINILINK Version %d.%02d.%03d"
#ifdef A_DEBUG
    " (Debug version)"
#endif
    "\n", VerMajor, VerMinor, VerBuild );

  if (argc == 1)
  {
    PrintUsage();
    return 0;
  }

  fInFile =
  fOutFile = NULL;
  if ((res = setjmp( ErrJmpBuf )) == 0)
  {
    ProcessCmdLine( argc, argv );
    if ((fInFile = fopen( szInputFile, "rb")) == NULL)
      FatalError( "Can't open source file" );
    if ((fOutFile = fopen( szOutputFile, "wb")) == NULL)
      FatalError( "Can't open target file" );

    Process();

    fclose( fOutFile );
    fclose( fInFile );
  }
  else
  {
    // we came here on a fatal error
    if (fInFile)
      fclose( fInFile );
    if (fOutFile)
      fclose( fOutFile );
    unlink( szOutputFile );
  }
  return res;
};


