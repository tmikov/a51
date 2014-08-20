/* process.c */

/*
Performs the fixups, final processings and output file generation.
*/

#include "common/global.h"
#pragma hdrstop

#include "link51.h"

static TObjStart strt;
static char szInputFile[FILENAME_MAX];
static char szModuleName[FILENAME_MAX];
static BOOL bIsModule;
static long startpos, pos;
static int nObjModule;
static BYTE *CodeChunk;  /* Last code chunk loaded */
static WORD ChunkOffset;  /* Offset of the last code chunk */
static char *pExternName;  /* The name of the last resolved extern */
static int nExternSeg;  /* The segment of the last resolved extern */
static char szExternModuleName[FILENAME_MAX];  /* Name of the module where the extern is resolved */

struct Collect ExternDefs;  /* of TExternDef */
struct Collect CodeChunksCollection;  /* of struct CodeChunks* */

/* ************************************************************************
   Function: LoadExterns
   Description:
     Loads the local extern definitions per a module.
     This is necessary to have the valid extern index (ID) for fix-up.
*/
void LoadExterns(void)
{
  int nNumberOfExterns;
  struct TExternDef *t;

  nNumberOfExterns = ReadWord();

  for (; nNumberOfExterns > 0; nNumberOfExterns--)
  {
    t = xmalloc(sizeof(struct TExternDef));
    t->nameIndex = ReadWord();
    t->segment = ReadByte();
    InsertItem(&ExternDefs, t);
  }
}

/* ************************************************************************
   Function: GetExternOffset
   Description:
     Resolves an extern from the list of the publics.
*/
long GetExternOffset(int nExternIndex)
{
  int i, j;
  struct ExternDefData *pExtern;
  struct PublicDefData *pPublic;
  BOOL bExternFound;
  long offset;
  struct ObjFileData *pObj;
  int nDChunk;
  int nPublicObjModule;

  /* Get the extern index from the extern definitions array */
  ASSERT(nExternIndex < ExternDefs.cnt);
  nExternIndex = ((struct TExternDef *)AtItem(&ExternDefs, nExternIndex))->nameIndex;

  /* Find this particular extern by nExternIndex */
  TRACE2("searching extern ID: %d, module ID: %d\n", nExternIndex, nObjModule);

  bExternFound = FALSE;
  offset = 0;  /* In case the extern is not resolved */
  for (i = 0; i < ExternsCollection.cnt; i++)
  {
    pExtern = AtItem(&ExternsCollection, i);
    if (pExtern->nObjFile != nObjModule)
      continue;
    if (pExtern->nExternIndex != nExternIndex)
      continue;
    if (pExtern->bResolved)
    {
      bExternFound = TRUE;
      pExternName = pExtern->psSymbolName;
      pPublic = pExtern->pResolved;
      nPublicObjModule = pPublic->nObjFile;
      break;
    }
  }

  if (bExternFound)
  {
    /* Prepare szExternModuleName */
    pObj = AtItem(&ObjFilesCollection, nPublicObjModule);
    nExternSeg = pExtern->nSegment;
    if (pObj->nModuleName != -1)
    {
      ASSERT(pObj->szLibName[0] != '\0');
      sprintf(szExternModuleName, "%s (%s)", GetModuleName(pObj->nModuleName), pObj->szLibName);
    }
    else
    {
      ASSERT(pObj->szLibName[0] == '\0');
      ASSERT(pObj->nObjPos == 0);
      strcpy(szExternModuleName, pObj->szObjName);
    }

    /* Check whether extern and public match same segment */
    i = pPublic->nSegment;
    if (i > O51_DSEG)  /* Allowed different data chunks */
      i = O51_DSEG;
    j =	pExtern->nSegment;
    if (j > O51_DSEG)  /* Allowed different data chunks */
      j = O51_DSEG;

    if (i != O51_NOSEG && j != O51_NOSEG)
      if (i != j)
        Error("Segment missmatch for extern %s in module %s",
          pExtern->psSymbolName, szExternModuleName);

    offset = pPublic->offset;
  }
  else
    pExternName = "UNRESOLVED";

  return (offset);
}

char *segnames[] =
{
  "noseg", "cseg", "iseg", "bseg", "xseg", "dseg"
};

/* ************************************************************************
   Function: cmpoffset
   Description:
     Call-back function to compare two CodeChunks offsets.
*/
static int cmpoffset(const void *p1, const void *p2)
{
  UINT o1, o2;

  o1 = ((struct CodeChunks *)p1)->offset;
  o2 = ((struct CodeChunks *)p2)->offset;
  if (o1 > o2)
    return (1);
  if (o1 < o2)
    return (-1);
  return (0);
}

/* ************************************************************************
   Function: ProcessCode
   Description:
     Reads a block of code from	a object module (InFile). Puts this particular
     module in the CodeChinksCollection.
   On exit:
     CodeChunk points at a block in heap containing this code block
     The block is inserted in the collection as well
*/
void ProcessCode(void)
{
  struct ObjFileData *pObj;

  /*
  len = blkLenght decreased by hdrsize and the size of the code
  offset (WORD)
  */
  UINT len = hdr.blkLength - (OBJ_BLOCK_HEADER_SIZE + sizeof(WORD));

  ChunkOffset = ReadWord();
  TRACE2("code block -> offset: %#x, size %d\n", ChunkOffset, len);
  CodeChunk = xmalloc(len + sizeof(struct CodeChunks));
  Read(&CodeChunk[sizeof(struct CodeChunks)], len);

  pObj = AtItem(&ObjFilesCollection, nObjModule);
  ((struct CodeChunks *)CodeChunk)->offset = ChunkOffset + pObj->CSegOffset;
  ((struct CodeChunks *)CodeChunk)->size = len;
  InsertItemSorted(&CodeChunksCollection, CodeChunk, cmpoffset);
}

/* ************************************************************************
   Function: ProcessFixup
   Description:
*/
void ProcessFixup(void)
{
  int count;
  TFixupItem f;
  int offset;
  BYTE *p;
  struct ObjFileData *pObj;
  int nDChunk;

  pObj = AtItem(&ObjFilesCollection, nObjModule);

  ASSERT(CodeChunk != NULL);

  count = ReadWord();
  #ifdef _DEBUG
  if (fDumpFixups)
  {
    TRACE1("Processing %d fixups\n", count);
    TRACE0("extr. l. offs  abs     target   fixup         gen. offset\n"
           "---------------------------------------------------------\n");
  }
  #endif

  while (count-- > 0)
  {
    f.target = ReadWord();
    f.fixType = ReadByte();
    f.srcType = ReadByte();
    f.srcData.segment = ReadWord();
    f.offset  = ReadWord();

    /*
    Evaluate the offset depending on whether the fixup is in the present
    frame or is external.
    */
    if (f.srcType == FIXUP_SRC_EXTERN)
    {
      #ifdef _DEBUG
      if (fDumpFixups)
        TRACE1("%-6c", '*');  /* Indicate external location */
      #endif
      offset = GetExternOffset(f.srcData.externIndex) + f.offset;
    }
    else
    {
      #ifdef _DEBUG
      if (fDumpFixups)
        TRACE1("%-6c", ' ');
      #endif
      ASSERT(f.srcType == FIXUP_SRC_FRAME);
      if (f.srcData.segment < 5)
        switch (f.srcData.segment)
        {
	  case O51_NOSEG:
	    offset = f.offset;
          case O51_CSEG:
	    offset = pObj->CSegOffset + f.offset;
	    break;
          case O51_ISEG:
	    offset = pObj->ISegOffset + f.offset;
            break;
          case O51_BSEG:
	    offset = pObj->BSegOffset + f.offset;
            break;
          case O51_XSEG:
	    offset = pObj->XSegOffset + f.offset;
            break;
        }
      else
      {
        /* Find the particular dseg chunk */
        nDChunk = (pObj->DChunkIndex + f.srcData.segment) - O51_DSEG;
	ASSERT(nDChunk < dchunks_cnt);
	ASSERT(dchunks[nDChunk].number == nDChunk);
        offset = dchunks[nDChunk].offset + f.offset;
      }
    }

    #ifdef _DEBUG
    if (fDumpFixups)
    {
      TRACE2("%#-6x   %#-6x  ", f.target, ChunkOffset + f.target);
      if (f.srcType == FIXUP_SRC_EXTERN)
      {
        if (nExternSeg >= 5)
          TRACE1("dseg(%02d) ", nExternSeg);
        else
          TRACE1("%s     ", segnames[nExternSeg]);
      }
      else
        if (f.srcData.segment >= 5)
          TRACE1("dseg(%02d) ", f.srcData.segment - 5);
        else
          TRACE1("%s     ", segnames[f.srcData.segment]);
    }
    #endif

    /* "p" points where to put the fixup */
    p = CodeChunk + f.target + sizeof(struct CodeChunks);

    switch (f.fixType)
    {
      case FIXUP_BYTELO:
        p[0] = offset;
	#ifdef _DEBUG
	if (fDumpFixups)
	  TRACE1("FIXUP_BYTELO  %#-6x", offset);
	#endif
        break;

      case FIXUP_BYTEHI:
        p[0] = offset >> 8;
	#ifdef _DEBUG
	if (fDumpFixups)
	  TRACE1("FIXUP_BYTEHI  %#-6x", offset);
	#endif
        break;

      case FIXUP_WORD:
        p[0] = offset >> 8;
        p[1] = offset;
	#ifdef _DEBUG
	if (fDumpFixups)
	  TRACE1("FIXUP_WORD    %#-6x", offset);
	#endif
        break;

      case FIXUP_AJMP:
        p[0] = (p[0] & ~0xE0) | ((offset >> 3) & 0xE0);
        p[1] = offset;
        if ((offset & 0xF800) != ((ChunkOffset + f.target + 2) & 0xF800))
          Fatal("Invalid 11 bit fixup");
	#ifdef _DEBUG
	if (fDumpFixups)
	  TRACE1("FIXUP_AJMP    %#-6x", p[0] + (p[1] << 8));
	#endif
        break;
    }
    #ifdef _DEBUG
    if (fDumpFixups)
    {
      if (f.srcType == FIXUP_SRC_EXTERN)
      {
          TRACE2(" %s: %s", pExternName, szExternModuleName);
      }
      TRACE0("\n");
    }
    #endif
  }

  #ifdef _DEBUG
  if (fDumpFixups)
    TRACE0("\n");
  #endif

  /* Indicate last code chunk processed */
  CodeChunk = NULL;
}

/* ************************************************************************
   Function: ProcessModule
   Description:
*/
void ProcessModule(void)
{
  #ifdef _DEBUG
  struct ObjFileData *pObj;
  #endif

  /*
  startpos is the position of the obj module in InFile. This is in
  case the objmodule is part from a library
  */
  startpos = ftell(InFile);

  strt.objSize = ReadDWord();
  strt.magic = ReadDWord();
  strt.verMaj = ReadByte();
  strt.verMin = ReadByte();

  if (strt.magic != MagicID)
    Fatal("Invalid object file (%s)", szInputFile);
  if (strt.verMaj * 256 + strt.verMin > OBJ_MAJ * 256 + OBJ_MIN)
    puts("Warning: Greater version object file");

  #ifdef _DEBUG
  /* Dump the segment offsets for this particular module */
  if (fDumpFixups)
  {
    pObj = AtItem(&ObjFilesCollection, nObjModule);
    TRACE3("cseg: %#lx, xseg %#lx, iseg %#x, ",
      pObj->CSegOffset, pObj->XSegOffset, pObj->ISegOffset);
    TRACE2("bseg: %#x dsegID_start: %#x\n", pObj->BSegOffset, pObj->DChunkIndex);
  }
  #endif

  CodeChunk = NULL;  /* No code chunk yet loaded */

  while ((pos = ftell(InFile)) - startpos < strt.objSize)
  {
    switch (ReadHdr())
    {
      case OBLK_EXTERN:
        LoadExterns();  /* Load the externs IDs (indexes) */
        break;
      case OBLK_CODE:
        ProcessCode();
	break;
      case OBLK_FIXUP:
        ProcessFixup();
	break;
      default:
        /*printf( "%04X: Skipping: block(%d), size(%d)\n", pos, hdr.blkType, hdr.blkLength );*/
        fseek(InFile, hdr.blkLength - OBJ_BLOCK_HEADER_SIZE, SEEK_CUR);
    }
  }
}

/* ************************************************************************
   Function: Link
   Description:
     Last frontier.
*/
void Link(void)
{
  struct ObjFileData *pObj;

  ASSERT(ObjFilesCollection.cnt != 0);
  ASSERT(OutputFile == OUTPUT_BIN || OutputFile == OUTPUT_HEX);
  ASSERT(szOutputFileName[0] != '\0');

  InitCollect(&CodeChunksCollection, 64);
  InitCollect(&ExternDefs, 64);

  TRACE1("\n%d obj modules to be processed\n", ObjFilesCollection.cnt);

  for (nObjModule = 0; nObjModule < ObjFilesCollection.cnt; nObjModule++)
  {
    pObj = AtItem(&ObjFilesCollection, nObjModule);
    bIsModule = TRUE;
    if (pObj->nModuleName == -1)
      bIsModule = FALSE;
    if (bIsModule)
    {
      ASSERT(pObj->szLibName[0] != '\0');
      strcpy(szInputFile, pObj->szLibName);
      strcpy(szModuleName, GetModuleName(pObj->nModuleName));
    }
    else
    {
      ASSERT(pObj->szLibName[0] == '\0');
      ASSERT(pObj->nObjPos == 0);
      strcpy(szInputFile, pObj->szObjName);
    }
    if ((InFile = fopen(szInputFile, "rb")) == NULL)
    {
      if (bIsModule)
        Fatal("Can't open obj module %s (lib: %s)", szModuleName, szInputFile);
      else
        Fatal("Can't open obj file %s", szInputFile);
    }

    if (bIsModule)
      TRACE2("Processing obj module %s (lib: %s)\n", szModuleName, szInputFile);
    else
      TRACE1("Processing obj file %s\n", szInputFile);

    /*
    Seek at the start position of the particular obj module
    when in a library.
    */
    if (bIsModule)
      fseek(InFile, pObj->nObjPos, SEEK_CUR);

    ProcessModule();
    FreeAll(&ExternDefs);

    fclose(InFile);
    if (bIsModule)
      TRACE2("Closing obj module %s (lib: %s)\n", szModuleName, szInputFile);
    else
      TRACE1("Closing obj file %s\n", szInputFile);
    InFile = NULL;
  }

  ShutDownCollect(&ExternDefs);
}

/* ************************************************************************
   Function: FreeSymobls
   Description:
*/
void FreeSymbols(void)
{
  int i;
  struct ExternDefData *e;
  struct PublicDefData *p;

  for (i = 0; i < ExternsCollection.cnt; i++)
  {
    e = AtItem(&ExternsCollection, i);
    xfree(e->psSymbolName);
  }

  for (i = 0; i < PublicsCollection.cnt; i++)
  {
    p = AtItem(&PublicsCollection, i);
    xfree(p->psSymbolName);
  }
}

