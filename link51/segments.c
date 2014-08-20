/* segments.c */

/*
Loads and calculates the absolute segment addresses
*/

#include "common/global.h"
#pragma hdrstop

#include "link51.h"

struct DataChunks dchunks[256];  /* 256 separate data chunks maximum */
int dchunks_cnt;
int LastData;

static TObjStart strt;
static long startpos, pos;
static char szInputFile[FILENAME_MAX];
static char szModuleName[FILENAME_MAX];
static BOOL bIsModule;
static int nObjModule;
static long GlobalCSegOffset;  /* cseg fixup offset for the current module */
static long GlobalXSegOffset;  /* xseg fixup offset for the current module */
static int GlobalISegOffset;  /* iseg fixup offset for the current module */
static int GlobalBSegOffset;  /* bseg fixup offset for the current module */

/* ************************************************************************
   Function: PrepareDSeg
   Description:
     Calc address for each of the dseg chunks.
*/
void PrepareDSeg(void)
{
  int DSegOffset;
  int dseg_chunk;
  int dchunk_size;

  if (bSmartDSeg)
  {
    DSegOffset = 0;
    dseg_chunk = 0;
    LastData = -1;

    TRACE1("\n%d dseg chunks for processing\n", dchunks_cnt);

    /* Fill from 0x7 up to the bseg -- 0x20 */
    if (GlobalBSegOffset != 0)  /* Only if there're bseg variables */
    {
      while (dseg_chunk < dchunks_cnt &&
        DSegOffset + (dchunk_size = dchunks[dseg_chunk].size) < 0x20)
      {
        TRACE3("Processing dseg chunk %d with %d bytes of size\n"
          "plased at offset %#x\n",
          dchunks[dseg_chunk].number, dchunk_size, DSegOffset);
        dchunks[dseg_chunk].offset = DSegOffset;
        DSegOffset += dchunk_size;
        dseg_chunk++;
      }

      /* Notice that GlobalBSegOffset stands for bseg size as well */
      LastData = 0x20 + (GlobalBSegOffset + 8) / 8;
    }
    else
      LastData = DSegOffset;

    TRACE2("bseg size = %#x bits (%#x bytes)\n", GlobalBSegOffset,
      (GlobalBSegOffset + 8) / 8);
    DSegOffset = LastData;
    TRACE1("Continuing from offset %#x\n", DSegOffset);
    while (dseg_chunk < dchunks_cnt)
    {
      dchunk_size = dchunks[dseg_chunk].size;
      TRACE3("Processing dseg chunk %d with %d bytes of size\n"
        "plased at offset %#x\n",
        dchunks[dseg_chunk].number, dchunk_size, DSegOffset);
      dchunks[dseg_chunk].offset = DSegOffset;
      DSegOffset += dchunk_size;
      if (DSegOffset > 0xff)
        Fatal("dseg overflow");
      dseg_chunk++;
    }
  }
  else
  {
    DSegOffset = 0;
    for (dseg_chunk = 0; dseg_chunk < dchunks_cnt; dseg_chunk++)
    {
      dchunk_size = dchunks[dseg_chunk].size;
      dchunks[dseg_chunk].offset = DSegOffset;
      DSegOffset += dchunk_size;
      if (DSegOffset > 0xff)
        Fatal("dseg overflow");
    }
  }
  LastData = DSegOffset;
  TRACE1("Stack to start from %#x\n", LastData);
}

/* ************************************************************************
   Function: LoadSegments
   Description:
     Loads segments info for the particular obj module
*/
static void LoadSegments(void)
{
  struct ObjFileData *pObj;
  int i;
  long CSegSize;
  long XSegSize;
  int ISegSize;
  int BSegSize;
  #ifdef _DEBUG
  char ModuleName[FILENAME_MAX];
  #endif

  pObj = AtItem(&ObjFilesCollection, nObjModule);

  ASSERT(pObj->CSegOffset == -1);
  ASSERT(pObj->XSegOffset == -1);
  ASSERT(pObj->ISegOffset == -1);
  ASSERT(pObj->BSegOffset == -1);
  ASSERT(pObj->DChunkIndex == -1);

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

  pObj->CSegOffset = GlobalCSegOffset;
  pObj->XSegOffset = GlobalXSegOffset;
  pObj->ISegOffset = GlobalISegOffset;
  pObj->BSegOffset = GlobalBSegOffset;
  pObj->DChunkIndex = dchunks_cnt;

  TRACE3("cseg_ofs: %#lx, xseg_ofs: %#lx, iseg_ofs: %#x, ",
    GlobalCSegOffset, GlobalXSegOffset, GlobalISegOffset);
  TRACE1("bseg_ofs: %#x\n", GlobalBSegOffset);

  while ((pos = ftell(InFile)) - startpos < strt.objSize)
  {
    switch (ReadHdr())
    {
      case OBLK_SEGMENTS:
        CSegSize = ReadWord();
        BSegSize = ReadWord();
        ISegSize = ReadWord();
        XSegSize = ReadWord();

        TRACE3("cseg_size: %#lx, xseg_size: %#lx, iseg_size: %#x, ",
          CSegSize, XSegSize, ISegSize);
        TRACE1("bseg_size: %#x\n", BSegSize);

	GlobalCSegOffset += CSegSize;
	GlobalBSegOffset += BSegSize;
	GlobalISegOffset += ISegSize;
	GlobalXSegOffset += XSegSize;

        i = ReadWord();  /* Num dchunks */
	TRACE1("%d dchunks\n", i);
	ASSERT(i >= 0);
        for (; i; --i)
        {
          dchunks[dchunks_cnt].size = ReadWord();
          TRACE1("%d ", dchunks[dchunks_cnt].size);
          dchunks[dchunks_cnt].offset = 0;
          dchunks[dchunks_cnt].number = dchunks_cnt;
          ++dchunks_cnt;
        }
	TRACE0("\n");
        break;
      #ifdef _DEBUG
      case OBLK_MODNAME:
        ReadString(ModuleName);
        if (bIsModule)
	  ASSERT(strcmp(ModuleName, szModuleName) == 0);
	break;
      #endif
      default:
        /*TRACE3("%04X: Skipping: block(%d), size(%d)\n", pos, hdr.blkType, hdr.blkLength);*/
        fseek(InFile, hdr.blkLength - OBJ_BLOCK_HEADER_SIZE, SEEK_CUR);
    }
  }
}

/* ************************************************************************
   Function: ProcessSegments
   Description:
     Laods and calculates segments start positions for all
     the obj modules scheduled for linking.
*/
void ProcessSegments(void)
{
  struct ObjFileData *pObj;

  GlobalCSegOffset = 0;
  GlobalXSegOffset = 0;
  GlobalISegOffset = 0;
  GlobalBSegOffset = 0;
  dchunks_cnt = 0;

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

    LoadSegments();

    fclose(InFile);
    if (bIsModule)
      TRACE2("Closing obj module %s (lib: %s)\n", szModuleName, szInputFile);
    else
      TRACE1("Closing obj file %s\n", szInputFile);
    InFile = NULL;
  }

  PrepareDSeg();
}

/* ************************************************************************
   Function: CalcPublics
   Description:
     Calculates the absolute offsets of all the public symbols
     depending on their segments
*/
void CalcPublics(void)
{
  int i;
  struct PublicDefData *p;
  struct ObjFileData *pObj;
  int nSeg;
  long offset;
  int nDChunk;
  struct ExternDefData Key;
  struct PublicDefData *pLastData;

  /* Put __last_data offset */
  Key.psSymbolName = LASTDATANAME;
  ASSERT(SearchItem(&PublicsCollection, &Key, &i, cmpsymbols));
  pLastData = AtItem(&PublicsCollection, i);
  pLastData->offset = LastData;

  for (i = 0; i < PublicsCollection.cnt; i++)
  {
    p = AtItem(&PublicsCollection, i);
    pObj = AtItem(&ObjFilesCollection, p->nObjFile);
    nSeg = p->nSegment;
    switch (p->nSegment)
    {
      case O51_NOSEG:
        offset = p->offset;
        break;
      case O51_CSEG:
        offset = pObj->CSegOffset + p->offset;
        break;
      case O51_ISEG:
        offset = pObj->ISegOffset + p->offset;
        break;
      case O51_BSEG:
        offset = pObj->BSegOffset + p->offset;
        break;
      case O51_XSEG:
	offset = pObj->XSegOffset + p->offset;
        break;
      default:
        if (p != pLastData)
	{
          nDChunk = (pObj->DChunkIndex + p->nSegment) - O51_DSEG;
	  nSeg += pObj->DChunkIndex;
	  ASSERT(nDChunk < dchunks_cnt);
	  ASSERT(dchunks[nDChunk].number == nDChunk);
          offset = dchunks[nDChunk].offset + p->offset;
	}
	else
	  offset = pLastData->offset;
        break;
    }
    p->offset = offset;
  }
}

