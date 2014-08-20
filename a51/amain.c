#include "common/global.h"
#pragma hdrstop
#include "a51.h"
#include "findf.h"

////////////////////////////////////////////////////////////////////////////
// Global data

const int VerMinor = 38;
const int VerMajor = 0;
const int VerBuild = 2;

int  NumErrors = 0;
int  NumWarnings = 0;
BOOL bAnyErrors = FALSE;

// Options
BOOL bCaseSensitive = FALSE;
int  MaxErrors = 20;
int  MaxWarnings = 100;
BOOL bMakeObj = FALSE;
BOOL bWarnings = TRUE;
BOOL bMakeLST  = FALSE;
BOOL bMakeXref = FALSE;
BOOL bNoMod51  = FALSE;

// compatibility option that may cause trouble when linking
//   multiple objects, because we don't know the abs. addresses
//   at assembly time
BOOL bOptCALL = FALSE;

#ifdef A_DEBUG
BOOL bTraceGen = FALSE;
BOOL bDumpSymb = FALSE;
#endif
#ifdef HEAP_DBG
BOOL bPrintHeap = FALSE;
#endif


char szInputFile[FILENAME_MAX+1];
char szOutputDir[FILENAME_MAX+1];
char szOutputFile[FILENAME_MAX+1];
char szListFile[FILENAME_MAX+1];
char szBareInputName[FILENAME_MAX+1];

FILE * ListFile = NULL;
int    ListLine = 0;

////////////////////////////////////////////////////////////////////////////
// local data

static const char szInputExt[]  = ".a51";
static const char szOutputExt[] = ".obj";
static const char szListExt[]   = ".lst";

static const char CFGName[] = "a51.cfg";

static BOOL bPrintedHelp = FALSE;
static BOOL bHaveFile = FALSE;

static FILE * CMDFile = NULL;   // response file or CFG file

////////////////////////////////////////////////////////////////////////////
// Local functions

static void PrintUsage ( void )
{
  printf(
"usage: A51 [ options... ] filename[%s]\n"
"options:\n"
"  -h/-?        Show this help screen\n"
"  -I<dir>      Add to include path\n"
"  -ml          Case sensitive symbols\n"
"  -l           Generate list file\n"
"  -c           Generate cross reference in listing\n"
"  -DSYM[=num]  Define SYM = num (decimal)\n"
"  -oc          Optimize CALL/JMP to ACALL/AJMP\n"
"  -w0,-w1      Set warning level: w0=none, w1=warnings on\n"
"  -gN          Stop after N warnings\n"
"  -jN          Stop after N errors\n"
"  -t<dir>      Set output directory <dir> for .LST & .OBJ files\n"
#ifdef A_DEBUG
"  -z[flags]    Debug flags:\n"
#if YYDEBUG
"     y         YACC trace\n"
#endif
"     i         Instruction trace\n"
"     s         Dump symbols\n"
#ifdef HEAP_DBG
"     h         Dump heap\n"
#endif
#endif
    ,
    szInputExt
  );
};

//--------------------------------------------------------------------------
// Name         FatalError
//
// Description  Displays a fatal error message not connected to a file
//              and line number. Then longjumps to error jump.
//--------------------------------------------------------------------------
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


//--------------------------------------------------------------------------
// Name         ProcessDefine
//
// Description  Process a statement: SYM[=xxx]
//              if =xxx is missing the symbol is defined = 0
//
// NOTE: This is a very primitive way for defining symbols from the command
// line. The symbol is not parsed, the value must be a decimal number.
//--------------------------------------------------------------------------
static void ProcessDefine ( const char * str )
{
  char * e;
  char * ident;
  char * endPtr;

  UINT hash;
  TIdent * pIdent;
  int len;

  if (e = strchr( str, '=' ))
  {
    if (e == str)
      FatalError( "Invalid format for -D option" );

    // allocate buffer on stack to copy the ident
    len = (const char *)e - str;
    if ((ident = alloca( len + 1 )) == NULL)
      OutOfMemory();

    memcpy( ident, str, len );  // copy the ident to the buffer
    ident[len] = 0;             // zero terminate it

    ++e;        // skip the '='
  }
  else
  {
    len = strlen( str );
    if (len == 0)
      FatalError( "Invalid format for -D option" );

    if ((ident = alloca( len + 1 )) == NULL)
      OutOfMemory();

    memcpy( ident, str, len + 1 );  // copy the ident to the buffer

    e = "0";                        // if no "=num" assume "=0"
  }

  if (!bCaseSensitive)        // if case insensitive upcase the ident
    strupr( ident );

  hash = CalcStringHash( (UCHAR *)ident );

  if ((pIdent = FindIdent( (UCHAR *)ident, hash )) == NULL)
  {
    pIdent = AllocTIdent( len, (UCHAR *)ident, hash );
    AddIdent( pIdent );
  }

  pIdent->e.uValue = (UINT)strtoul( e, &endPtr, 10 );
  if (*endPtr)
    FatalError( "Invalid number in -D" );
  pIdent->e.rType = RELOC_CONST;
};

//--------------------------------------------------------------------------
// Name         ProcessOption
//
// Description  Processes a string containing a command line option.
//              (The '-' has been skipped).
//              On error prints fatal error.
//
// NOTE: In earlier versions of this function I used a switch statement
// for the first letter of the option string. Then I did complex checks to
// determine the rest of the option string. It was fast but ugly. Then I
// realized that this function is the last I need speed in.
//--------------------------------------------------------------------------
static void ProcessOption ( const char * szOption )
{
  if (_stricmp( szOption, "h") == 0 ||
      _stricmp( szOption, "?" ) == 0)
  {
    if (!bPrintedHelp)
    {
      PrintUsage();
      bPrintedHelp = TRUE;
    }
    return;
  }
  if (toupper( szOption[0] ) == 'I') // -I<dir> 
  {
    if (!AddToIncludePath( szOption + 1 ))
      FatalError( "Too long include path" );
    return;
  }
  if (_stricmp( szOption, "ml" ) == 0)
  {
    bCaseSensitive = TRUE;
    return;
  }
  if (_stricmp( szOption, "l" ) == 0)
  {
    bMakeLST = TRUE;
    return;
  }
  if (_stricmp( szOption, "c" ) == 0)
  {
    bMakeXref = TRUE;
    return;
  }
  if (toupper( szOption[0] ) == 'D')    // define -DSYM[=XXX]
  {
    ProcessDefine( szOption + 1 );
    return;
  }
  if (_stricmp( szOption, "oc" ) == 0)
  {
    bOptCALL = TRUE;
    return;
  }
  if (_stricmp( szOption, "w0" ) == 0)
  {
    bWarnings = FALSE;
    return;
  }
  if (_stricmp( szOption, "w1" ) == 0)
  {
    bWarnings = TRUE;
    return;
  }
  if (toupper( szOption[0] ) == 'G') // -gN stop after N warnings
  {
    if ((MaxWarnings = atoi( szOption + 1 )) < 1)
      goto optionError;
    return;
  }
  if (toupper( szOption[0] ) == 'J') // -jN stop after N errors
  {
    if ((MaxErrors = atoi( szOption + 1 )) < 1)
      goto optionError;
    return;
  }
  if (toupper( szOption[0] ) == 'T') // -t<dir> set output directory <dir>
  {
    strcpy( szOutputDir, szOption + 1 );

    // if the string is not empty check for and append trailing '\'
    if (szOutputDir[0]) 
    {
      char * p = strchr( szOutputDir, 0 );
      if (p[-1] != '\\')
      {
        p[0] = '\\';
        p[1] = 0;
      }
    }
    return;
  }
#ifdef A_DEBUG
  if (toupper( szOption[0] ) == 'Z')
  {
    const char * s = szOption;
    while (*++s)
    {
      switch (toupper( *s ))
      {
#if YYDEBUG
        case 'Y':
          yydebug = TRUE;
          break;
#endif

        case 'I':
          bTraceGen = TRUE;
          break;

        case 'S':
          bDumpSymb = TRUE;
          break;

#ifdef HEAP_DBG
        case 'H':
          bPrintHeap = TRUE;
          break;
#endif

        default:
          FatalError( "Invalid debug flag: '%c'", *s );
      }
    }
    return;
  }
#endif

optionError:
  FatalError( "Invalid command line option: '-%s'", szOption );
};

//--------------------------------------------------------------------------
// Name         BuildFileNames
//
// Description  Constructs the input and output file names based on the
//              name obtained from the command line and the output directory
//              (if specified).
//              Also sets szBareInputName
//--------------------------------------------------------------------------
static void BuildFileNames ( void )
{
  char * p, * ext;

  // search for a '\\' or ':' which ever comes first (also mark the extension)
  p = strchr( szInputFile, 0 );
  ext = NULL;
  while (p > szInputFile && p[-1] != ':' && p[-1] != '\\' && p[-1] != '/')
  {
    --p;
    if (ext == NULL && *p == '.')
      ext = p;
  }
#if 0
  if (!ext) // if no source extension => add it
  {
    ext = strchr( szInputFile, 0 );    // point ext before extension
    strcat( szInputFile, szInputExt );
  }
#endif
  if (!ext) // if no source extension => find a file with matching extension
  {
    // Under UNIX the file names are case sensitive, so we must check if
    // there is more than one file with extension ".a51" (in any case).
    // We also use this to find the right case of the input file extension
    //
    FF_DAT * ff;
    struct findfilestruct ffs;
    int    extLen;

    ext = strchr( szInputFile, 0 );    // point ext before extension
    extLen = strlen( szInputExt );
    memcpy( ext, szInputExt, extLen + 1 ); // add the extension

    case_sensitive_match = FALSE;      // make sure we seach case-insensitive
    if (!(ff = find_open( szInputFile, FFIND_FILES )))
      FatalError( "While processing command line: %s", strerror( errno ) );

    *ext = 0;           // remove the extension we added

    if (find_file( ff, &ffs ) == 0)
    {
      // add the extension found to the file name
      memcpy( ext, strchr( ffs.filename, 0 ) - extLen, extLen + 1 );

      if (find_file( ff, &ffs ) == 0)
        FatalError( "More than one file matching the input file name" );
      printf( "Assembling: %s\n", szInputFile ); 
    }
    // if no file is found we will use the filename without extension
  }

  // copy the file name without path && without extension
  memcpy( szBareInputName, p, ext - p );
  szBareInputName[ext - p] = 0;

  // We construct the output files as the directory + bareInputName + extension

  strcpy( szOutputFile, szOutputDir );
  strcpy( szListFile, szOutputDir );

  strcat( szOutputFile, szBareInputName );
  strcat( szListFile, szBareInputName );
  
  strcat( szOutputFile, szOutputExt );
  strcat( szListFile,   szListExt );
};

//--------------------------------------------------------------------------
// Name         ProcessCmdToken
//
// Description  Parses a single command token. (This is a string surrounded
//              by spaces on the command line, response file or CFG file)
//              If it is an option parses the option, if it is a file name
//              saves it.
//--------------------------------------------------------------------------
static void ProcessCmdToken ( const char * token )
{
  if (token[0] == '-')
    ProcessOption( token + 1 );
  else
  {
    if (bHaveFile)
      FatalError( "Too many files specified" );
    else
    {
      bHaveFile = TRUE;
      strcpy( szInputFile, token );
    }
  }
};

//--------------------------------------------------------------------------
// Name         ProcessCmdFile
//
// Description  Processes a command file (response or configuration) with
//              ProcessCmdToken
//--------------------------------------------------------------------------
static void ProcessCmdFile ( FILE * f, const char * fileName )
{
  char buf[256];
  static const char seps[] = " \t\n\r";

  while (fgets( buf, sizeof( buf ), f ))
  {
    char * p;

    p = strtok( buf, seps );
    while (p && !bPrintedHelp)
    {
      ProcessCmdToken( p );
      p = strtok( NULL, seps );
    }
  }

  if (ferror( f ))
    FatalError( "Error reading '%s'", fileName );
};

//--------------------------------------------------------------------------
// Name         ProcessCmdLine
//
// Description  Processes the command line with ParseCmdToken
//              On a token @xxx parses a response file xxx
//--------------------------------------------------------------------------
static void ProcessCmdLine ( int argc, char ** argv )
{
  int i;
  for( i = 1; i < argc && !bPrintedHelp; ++i )
  {
    // check for response file
    if (argv[i][0] == '@')
    {
      if ((CMDFile = fopen( argv[i] + 1, "rt" )) == NULL)
        FatalError( "Can't open responce file '%s': %s", argv[i] + 1, strerror( errno ) );
      ProcessCmdFile( CMDFile, argv[i] + 1 );
      fclose( CMDFile );
      CMDFile = NULL;
    }
    else
      ProcessCmdToken( argv[i] );
  }

  if (!bPrintedHelp)
  {
    if (!bHaveFile)
      FatalError( "No input file specified" );

    BuildFileNames();
  }
};

static void ProcessCFGFile ( const char * exePath )
{
  // if not in current directory search it in the dir of the executable
  if ((CMDFile = fopen( CFGName, "rt" )) == NULL)
  {
    char baseCfg[FILENAME_MAX+1];
    char * p;
    int  len;

    // find the beginning of the executable name
    p = strchr( exePath, 0 );
    while (p > exePath && p[-1] != '\\')
      --p;
    len = (const char *)p - exePath;
    memcpy( baseCfg, exePath, len );
    strcpy( baseCfg + len, CFGName );

    if ((CMDFile = fopen( baseCfg, "rt" )) == NULL)
      return;   // no CFG file found

    ProcessCmdFile( CMDFile, baseCfg );
  }
  else
    ProcessCmdFile( CMDFile, CFGName );

  fclose( CMDFile );
  CMDFile = NULL;
};

///////////////////////////////////////////////////////////////////////////
// Global functions

int CDECL main ( int argc, char ** argv )
{
  int res;
  BOOL bFatal = FALSE;

  printf( "A51 Version %d.%02d.%03d"
#ifdef A_DEBUG
    " (Debug version) /" __DATE__ ", " __TIME__ "/"
#endif
    "\n", VerMajor, VerMinor, VerBuild );

  if (argc == 1)
  {
    PrintUsage();
    return 0;
  }

  ListFile = 
  ObjF     = 
  CMDFile  = NULL;

  if ((res = setjmp( ErrJmpBuf )) == 0)
  {
    // Initialize these before we process the command line
    // because comand line routines use ident tables if macros are
    // defined from the command line
    //
    IdentTable_Init();  
    Ident_Init();

    szOutputDir[0] = 0;         // no output directory yet
    ProcessCFGFile( argv[0] );
    ProcessCmdLine( argc, argv );
    if (bPrintedHelp)
    {
      // must exit right away, but we must do some clean up work first
      Ident_Done();
      IdentTable_Done();
      return 0;
    }

    Asm_Init();
    Scan_Init();
    Obj_Init();

    OpenInput( szInputFile );
    Pass1();

    bMakeObj = !bAnyErrors;
    if (bMakeLST || bMakeXref)
    {
      if ((ListFile = fopen( szListFile, "wb" )) == NULL)
        FatalError( "Can't create listing file" );
      else
        setvbuf( ListFile, NULL, _IOFBF, 8192 );
    }
    Pass2();

    FreeAllXref();
    Obj_Done();
    Scan_Done();
    Asm_Done();
    Ident_Done();
    IdentTable_Done();

    if (NumErrors != 0)
      res = 2;

#ifdef HEAP_DBG
    if (bPrintHeap)
      _dbg_print_heap( "\n\n-------- Heap at end --------\n\n" );
#endif
  }
  else
    bFatal = TRUE;

  if (CMDFile)
    fclose( CMDFile );
  if (ListFile)
    fclose( ListFile );
  if (ObjF)
    fclose( ObjF );

  if (bFatal || bAnyErrors)
    unlink( szOutputFile );

  return res;
};

#ifdef __DJGPP__
// Minimize the size of the executable for DJGPP
int __crt0_glob_function ( void ) { return 0; };
void __crt0_load_environment_file ( void ) {};
#endif

