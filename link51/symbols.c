/* symbols.c */

/*
Public & extern symbols processing functions
*/

#include "common/global.h"
#pragma	hdrstop

#include "link51.h"

#ifdef HAVE_IO_H
#include <io.h>
#endif

struct Collect ExternsCollection;  /* of ExternDefData */
struct Collect PublicsCollection;  /* of PublicDefData */
struct Collect ModuleNamesCollection;  /* of strings */

static struct Collect LibExternsCollection;  /* of ExternDefData */
static struct Collect LibPublicsCollection;  /*	of PublicDefData */

static struct Collect PreviewPublicsCollection;  /* of PublicDefData */
static struct Collect PreviewExternsCollection;  /* of ExternDefData */
struct Collect NamesCollection;	/* of strings */
static TObjStart strt;
static long startpos, pos;
static char szInputFile[FILENAME_MAX];

/* ************************************************************************
   Function: ReadString
   Description:
     Reads a string from a file
*/
void ReadString(char *pStrBuf)
{
  while (*pStrBuf++ = ReadByte());
}

/* ************************************************************************
   Function: GetModuleName
   Description:
     Returns objdect module name included in a library from
     ModuleNamesCollection
*/
char *GetModuleName(int nLibModule)
{
  ASSERT(nLibModule >= 0);

  return ((char *)AtItem(&ModuleNamesCollection, nLibModule));
}

#ifdef _DEBUG
/* ************************************************************************
   Function: GetObjFileName
   Description:
     Returns object file name from ObjFilesCollection
*/
static char *GetObjFileName(int nObjFile)
{
  ASSERT(nObjFile >= 0);

  return (((struct ObjFileData *)AtItem(&ObjFilesCollection, nObjFile))
    ->szObjName);
}

/* ************************************************************************
   Function: GetLibName
   Description:
     Returns library file name from LibFilesCollection
*/
static char *GetLibName(int nLibFile)
{
  ASSERT(nLibFile >= 0);

  return (((struct LibFileData *)AtItem(&LibFilesCollection, nLibFile))
    ->szLibName);
}

/* ************************************************************************
   Function: DumpAllSymbols
   Description:
     Dumps all the symbols from PublicsCollection, ExternsCollection,
     LibPublicsCollection and LibExternsCollection.
*/
static void DumpAllSymbols(void)
{
  int i;
  struct PublicDefData *p;
  struct ExternDefData *e;

  TRACE0("Publics from obj modules\n");
  for (i = 0; i < PublicsCollection.cnt; i++)
  {
    p = AtItem(&PublicsCollection, i);
    TRACE2("%s: %s\n", p->psSymbolName, GetObjFileName(p->nObjFile));
  }
  TRACE0("Externs from obj modules\n");
  for (i = 0; i < ExternsCollection.cnt; i++)
  {
    e = AtItem(&ExternsCollection, i);
    TRACE2("%s: %s\n", e->psSymbolName, GetObjFileName(e->nObjFile));
  }

  TRACE0("Publics from libraries\n");
  for (i = 0; i < LibPublicsCollection.cnt; i++)
  {
    p = AtItem(&LibPublicsCollection, i);
    TRACE3("%s: %s (%s)\n", p->psSymbolName, GetLibName(p->nLibFile),
      GetModuleName(p->nLibModule));
  }
  TRACE0("Externs from libraries\n");
  for (i = 0; i < LibExternsCollection.cnt; i++)
  {
    e = AtItem(&LibExternsCollection, i);
    TRACE3("%s: %s (%s)\n", e->psSymbolName, GetLibName(e->nLibFile),
      GetModuleName(e->nLibModule));
  }
}
#endif

/* ************************************************************************
   Function: cmpsymbols
   Description:
     Call back function to compare two symbols records
*/
int cmpsymbols(const void *s1, const void *s2)
{
  const struct ExternDefData *e1, *e2;

  e1 = s1;
  e2 = s2;
  if (bCaseSensitive)
    return (strcmp(e1->psSymbolName, e2->psSymbolName));
  else
    return (_stricmp(e1->psSymbolName, e2->psSymbolName));
}

/* ************************************************************************
   Function: MovePreviewToGlobal
   Description:
     Moves what	is placed in PreviewExternsCollect and in PreviewPublicCollect
     into ExternsCollection and PublicsCollection respectively.
*/
static void MovePreviewToGlobal(void)
{
  int i;

  for (i = 0; i < PreviewExternsCollection.cnt; i++)
    InsertItemSorted(&ExternsCollection, AtItem(&PreviewExternsCollection, i), cmpsymbols);

  for (i = 0; i < PreviewPublicsCollection.cnt; i++)
    InsertItemSorted(&PublicsCollection, AtItem(&PreviewPublicsCollection, i), cmpsymbols);

  /* Dispose pointers from the index */
  PreviewExternsCollection.cnt = 0;
  PreviewPublicsCollection.cnt = 0;
}

/* ************************************************************************
   Function: MovePreviewToLib
   Description:
     Moves what	is placed in PreviewExternsCollect and in PreviewPublicCollect
     into LibExternsCollection and LibPublicsCollection respectively.
*/
static void MovePreviewToLib(void)
{
  int i;

  for (i = 0; i < PreviewExternsCollection.cnt; i++)
    InsertItemSorted(&LibExternsCollection, AtItem(&PreviewExternsCollection, i), cmpsymbols);

  for (i = 0; i < PreviewPublicsCollection.cnt; i++)
    InsertItemSorted(&LibPublicsCollection, AtItem(&PreviewPublicsCollection, i), cmpsymbols);

  /* Dispose pointers from the index */
  PreviewExternsCollection.cnt = 0;
  PreviewPublicsCollection.cnt = 0;
}

/* ************************************************************************
   Function: LoadNames
   Description:
     Loads a names section
*/
static void LoadNames(void)
{
  int nNumberOfNames;
  char sNameBuffer[256];
  char *pName;

  nNumberOfNames = ReadWord();
  ASSERT(nNumberOfNames >= 0);

  for (; nNumberOfNames > 0; nNumberOfNames--)
  {
    ReadString(sNameBuffer);
    ASSERT(strlen(sNameBuffer) < 256);

    pName = xmalloc(strlen(sNameBuffer) + 1);
    strcpy(pName, sNameBuffer);

    InsertItem(&NamesCollection, pName);
  }
}

/* ************************************************************************
   Function: AllocExternDef
   Description:
     Allocates an extern definitions in the heap.
*/
static struct ExternDefData *AllocExternDef(char *name, int nSegment,
  int nObjFile, int nLibFile, int nLibPos, int nExternIndex)
{
  struct ExternDefData *b;

  b = xmalloc(sizeof(struct ExternDefData));
  b->psSymbolName = name;
  b->nSegment = nSegment;
  b->nObjFile = nObjFile;
  b->nLibFile = nLibFile;
  b->nLibPos = nLibPos;
  if (nLibPos != -1)
    b->nLibModule = ModuleNamesCollection.cnt - 1;
  else
    b->nLibModule = -1;
  b->bResolved = FALSE;
  b->pResolved = NULL;
  b->bModuleAttached = FALSE;
  b->nExternIndex = nExternIndex;
  return (b);
}

/* ************************************************************************
   Function: AllocPublicDef
   Description:
     Allocates an public definitions in the heap.
*/
static struct PublicDefData *AllocPublicDef(char *name, int nSegment, WORD offset,
  int nObjFile, int nLibFile, int nLibPos)
{
  struct PublicDefData *b;

  b = xmalloc(sizeof(struct PublicDefData));
  b->psSymbolName = name;
  b->nSegment = nSegment;
  b->offset = offset;
  b->nObjFile = nObjFile;
  b->nLibFile = nLibFile;
  b->nLibPos = nLibPos;
  if (nLibPos != -1)
    b->nLibModule = ModuleNamesCollection.cnt - 1;
  else
    b->nLibModule = -1;
  b->bModuleAttached = FALSE;
  return (b);
}

/* ************************************************************************
   Function: LoadExterns
   Description:
     Loads extern symbols in PreviewExternSymbols.
*/
static void LoadExterns(int nObjFile, int nLibFile, int nLibPos)
{
  int nNumberOfExterns;
  int nNameIndex;
  int nSegment;

  nNumberOfExterns = ReadWord();

  for (; nNumberOfExterns > 0; nNumberOfExterns--)
  {
    nNameIndex = ReadWord();
    if (nNameIndex >= NamesCollection.cnt)
      Fatal("Invalid extern definition name in file %s", szInputFile);
    nSegment = ReadByte();
    if (nSegment >= 6)
      Fatal("Invalid extern definition segment in file %s", szInputFile);
    InsertItem(&PreviewExternsCollection,
      AllocExternDef(AtItem(&NamesCollection, nNameIndex), nSegment,
      nObjFile, nLibFile, nLibPos, nNameIndex));
  }
}

/* ************************************************************************
   Function: LoadPublics
   Description:
     Loads public symbols in PreviewPublicSymbols
*/
static void LoadPublics(int nObjFile, int nLibFile, int nLibPos)
{
  int nNumberOfExterns;
  int nNameIndex;
  int nSegment;
  WORD offset;
  char *psSymbolName;
  struct PublicDefData *p;

  nNumberOfExterns = ReadWord();

  for (; nNumberOfExterns > 0; nNumberOfExterns--)
  {
    nNameIndex = ReadWord();
    if (nNameIndex >= NamesCollection.cnt)
      Fatal("Invalid public definition name in file %s", szInputFile);
    nSegment = ReadByte();
    if (nSegment >= 6)
      Fatal("Invalid public definition segment in file %s", szInputFile);
    offset = ReadWord();
    psSymbolName = AtItem(&NamesCollection, nNameIndex);
    p = AllocPublicDef(psSymbolName, nSegment, offset, nObjFile, nLibFile, nLibPos);
    InsertItem(&PreviewPublicsCollection, p);
  }
}

/* ************************************************************************
   Function: LoadObjFileSymbols
   Description:
     Loads obj file extern and public symbols in PreviewExternsCollection
     and PreviewPublicsCollection respectively.
*/
static void LoadObjFileSymbols(int nObjFile, int nLibFile)
{
  char ModuleName[255];
  char *pModuleName;

  /*
    startpos is the position of the obj module in InFile. This is in
    case the objmodule is part from a library
  */
  startpos = ftell(InFile);

  #ifdef _DEBUG
  if (nObjFile == -1)
    TRACE1("Processing obj module at position %d\n", startpos);
  #endif

  strt.objSize = ReadDWord();
  strt.magic = ReadDWord();
  strt.verMaj = ReadByte();
  strt.verMin = ReadByte();

  if (strt.magic != MagicID)
    Fatal("Invalid object file (%s)", szInputFile);
  if (strt.verMaj * 256 + strt.verMin > OBJ_MAJ * 256 + OBJ_MIN)
    puts("Warning: Greater version object file");

  while ((pos = ftell(InFile)) - startpos < strt.objSize)
  {
    switch (ReadHdr())
    {
      case OBLK_NAMES:
        LoadNames();
        break;
      case OBLK_EXTERN:
	LoadExterns(nObjFile, nLibFile, startpos);
        break;
      case OBLK_PUBLIC:
        LoadPublics(nObjFile, nLibFile, startpos);
        break;
      case OBLK_MODNAME:
        ReadString(ModuleName);
	pModuleName = xmalloc(strlen(ModuleName) + 1);
	strcpy(pModuleName, ModuleName);
	InsertItem(&ModuleNamesCollection, pModuleName);
	#ifdef _DEBUG
	if (nLibFile != -1)
	  TRACE1("Module name %s\n", ModuleName);
	#endif
        break;
      default:
        /*TRACE3("%04X: Skipping: block(%d), size(%d)\n", pos, hdr.blkType, hdr.blkLength);*/
        fseek(InFile, hdr.blkLength - OBJ_BLOCK_HEADER_SIZE, SEEK_CUR);
    }
  }

  #ifdef _DEBUG
  {
    int i;
    struct ExternDefData *e;
    struct PublicDefData *p;
    char buf2[80];
    int nBufLen;

    TRACE1("%d symbols loaded\n", NamesCollection.cnt);

    nBufLen = 0;
    TRACE1("%d externs\n", PreviewExternsCollection.cnt);
    for (i = 0; i < PreviewExternsCollection.cnt; i++)
    {
      e = AtItem(&PreviewExternsCollection, i);
      sprintf(buf2, "%s ID: %d -> %s; ", e->psSymbolName, e->nExternIndex,
        segnames[e->nSegment]);
      nBufLen += strlen(buf2);
      if (nBufLen > 75)
      {
	TRACE0("\n");
	nBufLen = 0;
      }
      TRACE0(buf2);
    }
    TRACE0("\n");

    nBufLen = 0;
    TRACE1("%d publics\n", PreviewPublicsCollection.cnt);
    for (i = 0; i < PreviewPublicsCollection.cnt; i++)
    {
      p = AtItem(&PreviewPublicsCollection, i);
      sprintf(buf2, "%s: %s (%#x); ", p->psSymbolName, segnames[p->nSegment], p->offset);
      nBufLen += strlen(buf2);
      if (nBufLen > 75)
      {
	TRACE0("\n");
	nBufLen = 0;
      }
      TRACE0(buf2);
    }
    TRACE0("\n");
  }
  #endif

  /* As the pointers are copyed */
  NamesCollection.cnt = 0;
}

/* ************************************************************************
   Function: MoveLibExterns
   Description:
     This function is called for a obj module from a library when a public
     resolves an extern from another module, to move all the declarations
     of the module to ExternsCollection and PublcisCollection. Adds module
     name in ObjFilesCollection as well.
     Returns FALSE for a module that has already being processed.
*/
static BOOL MoveLibExterns(int nPublicIndex)
{
  int i;
  int nLibFile;
  int nLibPos;
  int nLibModule;
  struct PublicDefData *p;
  struct ExternDefData *e;
  struct ObjFileData *o;
  struct LibFileData *l;
  char *psModuleName;
  char *psLibName;
  BOOL bExternsCopyed;

  ASSERT(nPublicIndex >= 0);

  p = AtItem(&LibPublicsCollection, nPublicIndex);

  /* Ensure this is from lib module */
  ASSERT(p->nObjFile == -1);
  ASSERT(p->nLibFile != -1);

  /* The nLibFile/nLibPos pair uniquely describes an obj module in lib file */
  nLibFile = p->nLibFile;
  nLibPos = p->nLibPos;
  nLibModule = p->nLibModule;

  bExternsCopyed = FALSE;

  /* Check bModuleAttached */
  if (p->bModuleAttached)
  {
    #ifdef _DEBUG
    /* Check whether all the symbols for this module has bModuleAttached set */
    for (i = 0; i < LibExternsCollection.cnt; i++)
    {
      e = (struct ExternDefData *)AtItem(&LibExternsCollection, i);
      if (e->nLibFile == nLibFile && e->nLibPos == nLibPos)
        ASSERT(e->bModuleAttached == TRUE);
    }
    for (i = 0; i < LibPublicsCollection.cnt; i++)
    {
      p = AtItem(&LibPublicsCollection, i);
      if (p->nLibFile == nLibFile && p->nLibPos == nLibPos)
        ASSERT(p->bModuleAttached == TRUE);
    }
    #endif
  }
  else
  {
    TRACE2("Moving symbols for the module %s (%s)\n",
      GetModuleName(nLibModule), GetLibName(nLibFile));
    /* Copy all the symbols for this particular module */
    for (i = 0; i < LibExternsCollection.cnt; i++)
    {
      e = AtItem(&LibExternsCollection, i);
      if (e->nLibFile == nLibFile && e->nLibPos == nLibPos)
      {
        ASSERT(e->bModuleAttached != TRUE);
        InsertItemSorted(&ExternsCollection, e, cmpsymbols);
	e->bModuleAttached = TRUE;
	e->nObjFile = ObjFilesCollection.cnt;
        bExternsCopyed = TRUE;
      }
    }
    for (i = 0; i < LibPublicsCollection.cnt; i++)
    {
      p = (struct PublicDefData *)AtItem(&LibPublicsCollection, i);
      if (p->nLibFile == nLibFile && p->nLibPos == nLibPos)
      {
        ASSERT(p->bModuleAttached != TRUE);
        InsertItemSorted(&PublicsCollection, p, cmpsymbols);
	p->bModuleAttached = TRUE;
	p->nObjFile = ObjFilesCollection.cnt;
      }
    }

    /* Add the module to ObjFilesCollection */
    psModuleName = AtItem(&ModuleNamesCollection, nLibModule);
    l = AtItem(&LibFilesCollection, nLibFile);
    psLibName = l->szLibName;
    /* Insert at position ObjFilesCollection.cnt */
    InsertItem(&ObjFilesCollection,
      AllocObjFileData(psModuleName, psLibName, nLibPos, nLibModule));
  }
  return (bExternsCopyed);
}

#ifdef _DEBUG
/* ************************************************************************
   Function: CheckModule
   Description:
*/
static void CheckModule(int nLibFile, int nObjFile)
{
  const char *psLibName2;
  const char *psLibName;

  ASSERT(nLibFile != -1);
  ASSERT(nObjFile != -1);
  psLibName = ((struct LibFileData *)AtItem(&LibFilesCollection, nLibFile))->szLibName;
  psLibName2 = ((struct ObjFileData *)AtItem(&ObjFilesCollection, nObjFile))->szLibName;
  ASSERT(strcmp(psLibName, psLibName2) == 0);
}
#endif

/* ************************************************************************
   Function: SelectLibSymbols
   Description:
     Selects which obj modules included in libraries to be linked.
   On entry:
     ExternsCollection, PublicsCollect, LibExternsCollection,
     LibPublicsCollection.
   On exit:
     ExternsCollection (should be completely resolvled).
     PublicsCollection.
     LibExternsCollection and LibPublicsCollection shut down.
     Into ObjFilesCollection added obj modules from the libaries to be linked.
*/
static void SelectLibSymbols(void)
{
  int i;
  int nIndex;
  struct ExternDefData *e;
  struct PublicDefData *p;
  struct ExternDefData Key;
  const char *psModuleName;
  const char *psLibName;

  NewPass:
  for (i = 0; i < ExternsCollection.cnt; i++)
  {
    e = AtItem(&ExternsCollection, i);
    if (e->bResolved)
      continue;
    Key.psSymbolName = e->psSymbolName;
    TRACE3("extern: %s (ID: %d, nModule: %d) -> ", e->psSymbolName,
      e->nExternIndex, e->nObjFile);
    if (SearchItem(&PublicsCollection, &Key, &nIndex, cmpsymbols))
    {
      e->bResolved = TRUE;
      e->pResolved = AtItem(&PublicsCollection, nIndex);
      #ifdef _DEBUG
      p	= AtItem(&PublicsCollection, nIndex);
      TRACE1("resolved in %s\n", GetObjFileName(p->nObjFile));
      #endif
      continue;
    }
    else
      /* Search in library symbols */
      if (SearchItem(&LibPublicsCollection, &Key, &nIndex, cmpsymbols))
      {
        e->bResolved = TRUE;
        e->pResolved = AtItem(&LibPublicsCollection, nIndex);
        #ifdef _DEBUG
        p = AtItem(&LibPublicsCollection, nIndex);
        TRACE2("resolved in %s (%s)\n", GetModuleName(p->nLibModule),
          GetLibName(p->nLibFile));
        #endif
        if (MoveLibExterns(nIndex))
          goto NewPass;  /* Lib extern definitions moved to ExternsCollection */
      }
      else
        TRACE0("UNRESOLVED\n");
  }

  /* Dump the unresolved externs */
  for (i = 0; i < ExternsCollection.cnt; i++)
  {
    e = AtItem(&ExternsCollection, i);
    if (e->bResolved)
      continue;

    /* get module name or lib name */
    if (e->nObjFile != -1)
    {
      psModuleName = ((struct ObjFileData *)AtItem(
        &ObjFilesCollection, e->nObjFile))->szObjName;
      printf("Unresolved extern %s from module %s\n", e->psSymbolName, psModuleName);
    }
    else
    {
      ASSERT(e->nLibFile != -1);

      psLibName = ((struct LibFileData *)AtItem(
        &ObjFilesCollection, e->nLibFile))->szLibName;
      psModuleName = AtItem(&ModuleNamesCollection, e->nLibModule);
      printf("Unresolved extern %s from module %s (lib: %s)\n", e->psSymbolName,
        psModuleName, psLibName);
    }
  }

  /*
  Free all the symbol allocations from the libraries
  that will not take part in the linking. This will prepare
  LibExterns and LibPublics collections to be shut down.
  */
  for (i = 0; i < LibExternsCollection.cnt; i++)
  {
    e = AtItem(&LibExternsCollection, i);
    if (!e->bModuleAttached)
    {
      xfree(e->psSymbolName);
      xfree(e);
    }
    #ifdef _DEBUG
    else
    {
      /* Check whether the symbol is in ExternsCollection */
      Key.psSymbolName = e->psSymbolName;
      ASSERT(SearchItem(&ExternsCollection, &Key, &nIndex, cmpsymbols));
      /* Check whether the module is in ObjFilesCollection */
      CheckModule(e->nLibFile, e->nObjFile);
    }
    #endif
  }
  for (i = 0; i < LibPublicsCollection.cnt; i++)
  {
    p = AtItem(&LibPublicsCollection, i);
    if (!p->bModuleAttached)
    {
      xfree(p->psSymbolName);
      xfree(p);
    }
    #ifdef _DEBUG
    else
    {
      /* Check whether the symbol is in PublicsCollection */
      Key.psSymbolName = p->psSymbolName;
      ASSERT(SearchItem(&PublicsCollection, &Key, &nIndex, cmpsymbols));
      /* Check whether the module is in ObjFilesCollection */
      CheckModule(p->nLibFile, p->nObjFile);
    }
    #endif
  }

  LibExternsCollection.cnt = 0;
  LibPublicsCollection.cnt = 0;
  FreeAll(&LibFilesCollection);
}

/* ************************************************************************
   Function: flen
   Description:
     Returns the length of a file
*/
static long flen(FILE *f)
{
#ifdef HAVE_FILELENGTH
  return (filelength(fileno(f)));
#else
  long res;
  long oldpos = ftell(f);
  if (oldpos < 0)
    return -1;
  if (fseek(f, 0, SEEK_END) < 0)
    return -1;
  res = ftell(f);
  if (fseek(f, oldpos, SEEK_SET) < 0)
    return -1;
  return res;
#endif
}

/* ************************************************************************
   Function: ProcessSymbols
   Description:
     Loads all object and lib files symbols. Selects
     which obj files to be linked.
*/
void ProcessSymbols(void)
{
  int i;
  long nLibFileSize;

  ASSERT(ObjFilesCollection.cnt != 0);

  InitCollect(&NamesCollection, 64);
  InitCollect(&PreviewPublicsCollection, 64);
  InitCollect(&PreviewExternsCollection, 64);
  InitCollect(&ExternsCollection, 64);
  InitCollect(&PublicsCollection, 64);
  InitCollect(&LibExternsCollection, 64);
  InitCollect(&LibPublicsCollection, 64);
  InitCollect(&ModuleNamesCollection, 64);

  InsertItemSorted(&PublicsCollection,
    AllocPublicDef(AllocString(LASTDATALEN, LASTDATANAME), O51_DSEG, 0, 0, -1, -1),
    cmpsymbols);

  for (i = 0; i < ObjFilesCollection.cnt; i++)
  {
    strcpy(szInputFile, (char *)AtItem(&ObjFilesCollection, i));
    if ((InFile = fopen(szInputFile, "rb")) == NULL)
      Fatal("Can't open obj file (%s)", szInputFile);

    TRACE1("Processing the module %s\n\n", szInputFile);

    LoadObjFileSymbols(i, -1);
    MovePreviewToGlobal();

    fclose(InFile);
    TRACE1("Closing the module %s\n\n", szInputFile);
    InFile = NULL;
  }

  for (i = 0; i < LibFilesCollection.cnt; i++)
  {
    strcpy(szInputFile, (char *)AtItem(&LibFilesCollection, i));
    if ((InFile = fopen(szInputFile, "rb")) == NULL)
      Fatal("Can't open lib file (%s)", szInputFile);

    nLibFileSize = flen(InFile) - 1;

    TRACE1("Processing the library %s\n\n", szInputFile);

    while (ftell(InFile) < nLibFileSize)
    {
      LoadObjFileSymbols(-1, i);
      MovePreviewToLib();
    }

    fclose(InFile);
    TRACE1("Closing the library %s\n\n", szInputFile);
    InFile = NULL;
  }

  #ifdef _DEBUG
  /* DumpAllSymbols(); */
  #endif

  SelectLibSymbols();

  ShutDownCollect(&NamesCollection);
  ShutDownCollect(&PreviewPublicsCollection);
  ShutDownCollect(&PreviewExternsCollection);
  ShutDownCollect(&LibExternsCollection);
  ShutDownCollect(&LibPublicsCollection);
  ShutDownCollect(&LibFilesCollection);
}

