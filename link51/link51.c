/* link51.c */

#include "global.h"
#pragma hdrstop

#include "link51.h"

const int VerMajor = 0;
const int VerMinor = 5;
const int VerBuild = 6;

#ifdef HEAP_DBG
BOOL bPrintHeap = FALSE;  /* Print the heap */
#endif

#ifdef _DEBUG
BOOL fDumpFixups = FALSE;
#endif

UINT OutputFile = OUTPUT_BIN;
char szOutputFileName[_MAX_PATH];
char szMapFileName[_MAX_PATH];

static const char szCFGName[] = "link51.cfg";
static const char szObjExt[] = ".obj";
static const char szLibExt[] = ".lib";
static const char szBinExt[] = ".bin";
static const char szHexExt[] = ".hex";
static const char szMapExt[] = ".map";

struct Collect ObjFilesCollection;
struct Collect LibFilesCollection;

char szOutputDir[_MAX_PATH];
char szLibDir[_MAX_PATH];

static FILE *CMDFile = NULL;   /* Response file or CFG file */

BOOL bProduceMap = FALSE;
BOOL bCaseSensitive = FALSE;
BOOL bSmartDSeg = FALSE;
BOOL fProduceXREF = FALSE;
BOOL fWarnDuplicate = FALSE;

BYTE FillValue = 0xff;

/* ************************************************************************
   Function: PrintUsage
   Description:
     Displays the list of options available
*/
static void PrintUsage (void)
{
  printf(
    "usage: link51 [ options... ] files\n"
    "options:\n"
    "  -m           Generate map file\n"
    "  -c           Case insensitive link\n"
    "  -d           Warn if duplicate symbols in libraries\n"
    "  -e           Smart dseg linking\n"
    "  -oname       Specify executable file name\n"
    "  -t<dir>      Specify output directory\n"
    "  -L<dir>      Specify library search path\n"
    "  -T[flags]    Specify output file type\n"
    "     b         Binary file (.bin) (default)\n"
    "     h         Intel hex file (.hex)\n"
    #ifdef _DEBUG
    "  -z[flags]    Debug flags:\n"
    "     t         Enable link51.log file\n"
    "     f         Dump fixups in link51.log\n"
    #ifdef HEAP_DBG
    "     h         Dump heap leaks\n"
    #endif
    #endif
  );
}

/* ************************************************************************
   Function: pAllocObjFileData
   Description:
     Allocates places for one item of struct ObjFileData.
     if szLibName != NULL the object file is part of a library and then
     ulObjPos holds the absolute obj module position in the library.
*/
struct ObjFileData *AllocObjFileData(const char *szObjName, const char *szLibName,
  long nObjPos, int nModuleName)
{
  struct ObjFileData *p;

  ASSERT(szObjName != NULL);
  ASSERT(nObjPos >= 0);

  p = xmalloc(sizeof(struct ObjFileData));
  strcpy(p->szObjName, szObjName);
  p->szLibName[0] = '\0';  /* Empty by default */
  if (szLibName != NULL)  /* If the object is a part of a library */
    strcpy(p->szLibName, szLibName);
  p->nObjPos = nObjPos;
  p->nModuleName = nModuleName;
  p->CSegOffset = -1;
  p->XSegOffset = -1;
  p->ISegOffset = -1;
  p->BSegOffset = -1;
  p->DChunkIndex = -1;  /* Data chunks relative number in dhunks[] */

  return (p);
}

/* ************************************************************************
   Function: AllocLibFileData
   Description:
*/
struct LibFileData *AllocLibFileData(const char *szLibName)
{
  struct LibFileData *p;

  ASSERT(szLibName != NULL);

  p = xmalloc(sizeof(struct LibFileData));
  strcpy(p->szLibName, szLibName);

  return (p);
}

/* ************************************************************************
   Function: ProcessOption
   Description: Processes a string containing a command line option.
     (The '-' or '/' has been skipped). On error prints fatal error.
*/
static void ProcessOption (const char * szOption)
{
  ASSERT(szOption != NULL);

  if (strcmp(szOption, "m") == 0)
  {
    bProduceMap = TRUE;
    return;
  }
  if (strcmp(szOption, "c") == 0)
  {
    bCaseSensitive = TRUE;
    return;
  }
  if (strcmp(szOption, "e") == 0)
  {
    bSmartDSeg = TRUE;
    return;
  }
  if (strcmp(szOption, "l") == 0)
  {
    fProduceXREF = TRUE;
    return;
  }
  if (strcmp(szOption, "d") == 0)
  {
    fWarnDuplicate = TRUE;
    return;
  }
  if (szOption[0] == 't')
  {
    strcpy(szOutputDir, szOption + 1);
    return;
  }
  if (szOption[0] == 'L')
  {
    strcpy(szLibDir, szOption + 1);
    return;
  }
  if (szOption[0] == 'T')
  {
    switch (szOption[1])
    {
      case 'b':
        OutputFile = OUTPUT_BIN;
        break;

      case 'h':
	OutputFile = OUTPUT_HEX;
        break;

      default:
        Fatal("Invalid output file type: '%c'", szOption[1]);
    }
    return;
  }
  if (szOption[0] == 'o')
  {
    strcpy(szOutputFileName, &szOption[1]);
    return;
  }
  #ifdef _DEBUG
  if (szOption[0] == 'z')
  {
    const char *s = szOption;
    while (*++s)
    {
      switch (*s)
      {
	case 'f':
	  fDumpFixups = TRUE;
	  break;

	case 't':
	  bDebugTraceEnabled = TRUE;
	  break;

	#ifdef HEAP_DBG
	case 'h':
	  bPrintHeap = TRUE;
	  break;
	#endif

        default:
          Fatal("Invalid debug flag: '%c'", *s);
      }
    }
    return;
  }
  #endif

  Fatal("Invalid command line option: '-%s'", szOption);
}

/* ************************************************************************
   Function: AddObjExt
   Description:
     Adds default extention to a object file name if missing
*/
void AddObjExt(const char *name, char *dest)
{
  ASSERT(name != NULL);
  ASSERT(dest != NULL);

  strcpy(dest, name);
  if (!strchr(name, '.'))
    strcat(dest, szObjExt);
}

#define OBJ_FILE 0
#define LIB_FILE 1
#define	NOT_LINK_FILE -1

/* ************************************************************************
   Function: DetectExt
   Description:
     Detects whether the name is .obj or .lib or returns error
*/
int DetectExt(const char *name)
{
  char *p;
  char ext[_MAX_PATH];

  ASSERT(name != NULL);
  ASSERT(strlen(name) < _MAX_PATH);

  p = strchr(name, '.');
  if (p == NULL)
    return (NOT_LINK_FILE);

  strcpy(ext, p);
  strlwr(ext);

  if (strcmp(ext, szLibExt) == 0)
    return (LIB_FILE);

  if (strcmp(ext, szObjExt) == 0)
    return (OBJ_FILE);

  return (NOT_LINK_FILE);
}

/* ************************************************************************
   Function: ProcessCmdToken
   Description:
     Parses a single command token. (This is a string surrounded
     by spaces on the command line, response file or CFG file)
     If it is an option parses the option, if it is a file name
     saves it.
*/
static void ProcessCmdToken(const char *token)
{
  char aname[_MAX_PATH];

  ASSERT(token != NULL);

  if (token[0] == '-' || token[0] == '/')
    ProcessOption(token + 1);
  else
  {
    AddObjExt(token, aname);  /* Get in aname name + extention */
    switch (DetectExt(aname))
    {
      case LIB_FILE:
        InsertItem(&LibFilesCollection, AllocLibFileData(aname));
        break;
      case OBJ_FILE:
        InsertItem(&ObjFilesCollection, AllocObjFileData(aname, NULL, 0, -1));
        break;
      case NOT_LINK_FILE:
        Fatal("Invalid input file: '%s'", token);
        break;
      default:
        ASSERT(0);
    }
  }
}

/* ************************************************************************
   Function: ProcessCmdFile
   Description:
     Processes a command file (response or configuration) with
     ProcessCmdToken
     The argument fileName is passed only in order to be displayed
     in case of error
*/
static void ProcessCmdFile(FILE *f, const char *fileName)
{
  char buf[256];
  static const char seps[] = " \t\n\r";

  ASSERT(f != NULL);
  ASSERT(fileName != NULL);

  while (fgets(buf, sizeof(buf), f))
  {
    char *p;

    p = strtok(buf, seps);
    while (p)
    {
      ProcessCmdToken(p);
      p = strtok(NULL, seps);
    }
  }

  if (ferror(f))
    Fatal("Error reading '%s'", fileName);
}

/* ************************************************************************
   Function: ProcessCmdLine
   Description:
     Processes the command line with ParseCmdToken
     On a token @xxx parses a response file xxx
*/
static void ProcessCmdLine(int argc, char *argv[])
{
  int i;

  ASSERT(argc >= 1);  /* There should be at least 1 argument - the program name */
  ASSERT(argv != NULL);

  for (i = 1; i < argc; ++i)
  {
    /* Check for response file */
    if (argv[i][0] == '@')
    {
      if ((CMDFile = fopen(argv[i] + 1, "rt")) == NULL)
        Fatal("Can't open response file '%s'", argv[i] + 1);
      ProcessCmdFile(CMDFile, argv[i] + 1);
      fclose(CMDFile);
      CMDFile = NULL;
    }
    else
      ProcessCmdToken(argv[i]);
  }

  if (ObjFilesCollection.cnt == 0)
    Fatal("No input file specified");
}

/* ************************************************************************
   Function: ProcessCFGFile
   Description:
     Extracts all the options specified in a config file
*/
static void ProcessCFGFile(const char *exePath)
{
  ASSERT(exePath != NULL);

  /* If not in current directory search it in the dir of the executable */
  if ((CMDFile = fopen(szCFGName, "rt")) == NULL)
  {
    char baseCfg[_MAX_PATH];
    char *p;
    int  len;

    /* Find the beginning of the executable name */
    p = strchr(exePath, 0);
    while (p > exePath && p[-1] != '\\')
      --p;
    len = (unsigned int)((const char *)p - (const char *)exePath);
    memcpy(baseCfg, exePath, len);
    strcpy(baseCfg + len, szCFGName);

    if ((CMDFile = fopen(baseCfg, "rt")) == NULL)
      return;   /* No CFG file found */

    ProcessCmdFile(CMDFile, baseCfg);
  }
  else
    ProcessCmdFile(CMDFile, szCFGName);

  fclose(CMDFile);
  CMDFile = NULL;
}

/* ************************************************************************
   Function: MarkNameExt
   Description:
     Marks the position of the name and the extention
     in a fullname
*/
void MarkNameExt(const char *fullname, char **name, char **ext)
{
  ASSERT(fullname != NULL);
  ASSERT(name != NULL);
  ASSERT(ext != NULL);

  *name = strchr(fullname, 0);
  *ext = NULL;
  while (*name > fullname &&
    (*name)[-1] != ':' && (*name)[-1] != '\\' && (*name)[-1] != '/')
  {
    --(*name);
    if (*ext == NULL && **name == '.')
      *ext = *name;
  }
}

/* ************************************************************************
   Function: AddTrailingSlash
   Description:
     Add the tailing '\\' in a path if not specified
*/
void AddTrailingSlash(char *name)
{
  UINT l;

  ASSERT(name != NULL);

  l = strlen(name);
  if (name[l] != '\\')
  {
    name[l] = '\\';
    name[l + 1] = '\0';
  }
}

/* ************************************************************************
   Function: AddOutputExt
   Description:
     Adds an extention depending on the	output type
*/
void AddOutputExt(const char *fullname, char *dest)
{
  char *name;
  char *ext;

  ASSERT(fullname != NULL);
  ASSERT(dest != NULL);

  MarkNameExt(fullname, &name, &ext);
  if (dest != fullname)
    strcpy(dest, fullname);
  if (!strchr(name, '.'))
    if (OutputFile == OUTPUT_BIN)
      strcat(dest, szBinExt);
    else
      strcat(dest, szHexExt);
}

/* ************************************************************************
   Function: BuildOutputFileName
   Description:
     Builds output file name.
     Takes on consideration the options -o, -t or the name of the first
     object file specified.
*/
void BuildOutputFileName(void)
{
  char *p, *ext;
  char szBareInputName[_MAX_PATH];
  char *pszInputFile;
  UINT l;
  UINT lastpos;
  char buf[_MAX_PATH];
  char buf2[_MAX_PATH];

  if (szOutputFileName[0] == '\0')
  {
    strcpy(buf, AtItem(&ObjFilesCollection, 0));
    MarkNameExt(buf, &p, &ext);
    *ext = '\0';  /* Remove '.obj' extention */
    strcpy(buf, p);  /* Get bare name (without path) */
  }
  else
    strcpy(buf, szOutputFileName);

  if (szOutputDir[0] != '\0')
  {
    strcpy(buf2, szOutputDir);
    AddTrailingSlash(buf2);
    strcat(buf2, buf);
    strcpy(buf, buf2);
  }

  strcpy(szOutputFileName, buf);
  AddOutputExt(buf, szOutputFileName);
}

/* ************************************************************************
   Function: MakeMapFileName
   Description:
     Based on the output file name build the map file name
*/
void MakeMapFileName(void)
{
  char *p;

  strcpy(szMapFileName, szOutputFileName);
  p = strrchr(szMapFileName, '.');
  ASSERT(p != NULL);
  strcpy(p, szMapExt);
}

int CDECL main(int argc, char *argv[])
{
  int res;
  BOOL fFatal = FALSE;
  char Title[100];

  #ifdef _DEBUG
  strcpy(szDebugLogName, "link51.log");
  #endif
  TRACE0("");  /* Clear the debug log file */
  #ifdef _DEBUG
  bDebugTraceEnabled = FALSE;  /* /Zt to be enabled */
  #endif

  sprintf(Title, "Link51 Version %d.%d.%d"
    #ifdef _DEBUG
    " (Debug version) /" __DATE__ ", " __TIME__ "/"
    #endif
    "\n", VerMajor, VerMinor, VerBuild);

  printf(Title);

  MaxErrors = 50;
  MaxWarnings = 50;

  if (argc == 1)
  {
    PrintUsage();
    return (0);
  }

  if ((res = setjmp( ErrJmpBuf )) == 0)
  {
    InitCollect(&ObjFilesCollection, 64);
    InitCollect(&LibFilesCollection, 64);

    szOutputDir[0] = szLibDir[0] = szOutputFileName[0] = '\0';
    ProcessCFGFile(argv[0]);
    ProcessCmdLine(argc, argv);
    BuildOutputFileName();
    MakeMapFileName();

    TRACE0(Title);
    TRACE0("\n");

    #ifdef _DEBUG
    {
      int i;

      for (i = 0; i < ObjFilesCollection.cnt; i++)
        TRACE1("%s\n", ((struct ObjFileData *)AtItem(&ObjFilesCollection, i))->szObjName);
      for (i = 0; i < LibFilesCollection.cnt; i++)
        TRACE1("%s\n", ((struct LibFileData *)AtItem(&LibFilesCollection, i))->szLibName);
      TRACE1(":%s\n\n", szOutputFileName);
    }
    #endif

    ProcessSymbols();  /* Load and resolve extern and public symbols */

    ProcessSegments();  /* Load and calculate absolute segment adresses */
    CalcPublics();  /* Calculate public symbols absolute offsets */

    Link();  /* Process fixups and generate the output file */

    if (bProduceMap)
      GenerateMap();

    FreeSymbols();  /* Dispose symbols names */
    FreeAll(&ExternsCollection);
    ShutDownCollect(&ExternsCollection);
    FreeAll(&PublicsCollection);
    ShutDownCollect(&PublicsCollection);
    FreeAll(&ModuleNamesCollection);
    ShutDownCollect(&ModuleNamesCollection);

    /* Generate the output file */
    OpenOutput();

    if (OutputFile == OUTPUT_BIN)
      GenBin();
    else
      GenHex();

    DoneOutput();

    FreeAll(&ObjFilesCollection);
    ShutDownCollect(&ObjFilesCollection);

    #ifdef HEAP_DBG
    if (bPrintHeap)
      _dbg_print_heap("\n\n-------- Heap at end --------\n\n");
    #endif
  }
  else
    fFatal = TRUE;

  if (fFatal || fAnyErrors)
  {
    if (InFile)
      fclose(InFile);
    if (OutFile)
      fclose(OutFile);
    _unlink(szOutputFileName);
  }

  return (res);
}

/* Minimize the size of the executable for GCC */

int __crt0_glob_function (void)
{
  return 0;
}

void __crt0_load_environment_file (void)
{
}

