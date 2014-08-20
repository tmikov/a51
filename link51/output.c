/* output.c */

/*
Generates final output file
*/

#include "common/global.h"
#pragma hdrstop

#include "link51.h"

FILE *OutFile = NULL;

/* ************************************************************************
   Function: DumpCodeCollect
   Description:
     Dumps the code chunks collection
*/
static void DumpCodeCollect(void)
{
  UINT i;

  TRACE0("Code chunks in sorted order\n");
  for (i = 0; i < CodeChunksCollection.cnt; i++)
    TRACE2("chunk %d, offset %#x\n", i,
      ((struct CodeChunks *)AtItem(&CodeChunksCollection, i))->offset);
}

/* ************************************************************************
   Function: PackCodeCollect
   Description:
     Joins adjecent code block from the CodeChunksCollection
*/
static void PackCodeCollect(void)
{
  int i;
  struct CodeChunks *p;
  struct CodeChunks *pnext;
  int newsize;
  struct CodeChunks *ncode;

  for (i = 0; i < CodeChunksCollection.cnt - 1;)
  {
    p = AtItem(&CodeChunksCollection, i);
    pnext = AtItem(&CodeChunksCollection, i + 1);

    if (p->offset + p->size >= pnext->offset)  /* Adjecent code */
    {
      UINT end1, end2;
      end1 = p->offset + p->size;
      end2 = pnext->offset + pnext->size;

      newsize = (end2 > end1 ? end2 : end1) - p->offset;

      ncode = xmalloc(newsize + sizeof(struct CodeChunks));
      memcpy(ncode + 1, p + 1, p->size);
      memcpy((BYTE *)(ncode + 1) + pnext->offset - p->offset, pnext + 1, pnext->size );
      ncode->offset = p->offset;
      ncode->size = newsize;
      TRACE3("Joined block %#x (%d bytes) with %#x\n", p->offset, p->size, pnext->offset);
      AtRemoveItem(&CodeChunksCollection, i);
      AtRemoveItem(&CodeChunksCollection, i);
      AtInsertItem(&CodeChunksCollection, ncode, i);
      xfree(pnext);
      xfree(p);
    }
    else
      ++i;
  }
}

/* ************************************************************************
   Function: OpenOutput
   Description:
*/
void OpenOutput(void)
{
  if ((OutFile = fopen(szOutputFileName, "wb")) == NULL)
    Fatal("Can't open target file (%s)", szOutputFileName);
  TRACE1("Output file (%s) sucessfully created\n", szOutputFileName);
}

/* ************************************************************************
   Function: fwrite2
   Description:
*/
void fwrite2(const void *ptr, size_t size, size_t n, FILE *stream)
{
  if (fwrite(ptr, size, n, stream) != n)
    Fatal("Can't write target file (%s)\n", szOutputFileName);
}

/* ************************************************************************
   Function: GenBin
   Description:
*/
void GenBin(void)
{
  UINT i;
  UINT j;
  UINT curoffset;
  struct CodeChunks *p;

  TRACE0("Generate bin output file\n");
  DumpCodeCollect();

  curoffset = 0;
  for (i = 0; i < CodeChunksCollection.cnt; ++i)
  {
    p = AtItem(&CodeChunksCollection, i);
    for (j = 0; j < p->offset - curoffset; ++j)
      fwrite2(&FillValue, sizeof(BYTE), 1, OutFile);
    fwrite2(p + 1, sizeof(BYTE), p->size, OutFile);
    curoffset = p->offset + p->size;  /* Mark the end of the block */
  }
}

/* ************************************************************************
   Function: GenChunkHex
   Description:
*/
static void GenChunkHex(struct CodeChunks *c)
{
  UINT i;
  UINT nlines;
  UINT b;
  UINT lend;
  UINT j;
  BYTE csum;
  BYTE *p;
  BYTE co;

  nlines = (c->size + 15) / 16;
  /* Separate the code to have 16 bytes	at a line */
  for (i = 0; i < nlines; i++)  /* Loop for each line */
  {
    fputc(':', OutFile);  /* Starting ':' */

    b = i * 16;
    lend = (i + 1) * 16;
    if (lend >= c->size)
      lend = c->size;
    fprintf(OutFile, "%02X", lend - b);  /* Number of bytes at this line */
    csum = lend - b;

    /* Loading address */
    fprintf(OutFile, "%04X", b + c->offset);
    csum += ((c->offset + b) & 0xFF) + (((c->offset + b) >> 8) & 0xFF);

    /* Data record ID */
    fprintf(OutFile, "00");

    /* Data */
    lend -= b;
    p = (BYTE *)(c + 1) + b;
    for (j = 0; j < lend; j++)
    {
      co = *(p + j);
      fprintf(OutFile, "%02X", co);
      csum += co;
    }

    /* Check sum */
    fprintf(OutFile, "%02X\r\n", (BYTE)(-csum) );
  }

}

/* ************************************************************************
   Function: GenHex
   Description:
*/
void GenHex(void)
{
  UINT i;

  TRACE0("Generate hex output file\n");
  DumpCodeCollect();
  PackCodeCollect();
  DumpCodeCollect();

  for (i = 0; i < CodeChunksCollection.cnt; i++)
    GenChunkHex((struct CodeChunks *)AtItem(&CodeChunksCollection, i));
  fprintf( OutFile, ":00000001FF\r\n" );
}

/* ************************************************************************
   Function: DoneOutput
   Description:
*/
void DoneOutput(void)
{
  fclose(OutFile);
  TRACE1("Closing the output file %s\n", szOutputFileName);
  OutFile = NULL;

  FreeAll(&CodeChunksCollection);
  ShutDownCollect(&CodeChunksCollection);
}

