#include "global.h"
#pragma hdrstop
#include "a51.h"

#define XREF_CHUNK_SIZE 1024

typedef struct TXrefChunk
{
  struct TXrefChunk * prevChunk;
  TXrefItem  items[XREF_CHUNK_SIZE];
} TXrefChunk;

TXrefChunk * lastChunk = NULL;
UINT         chunkIndex = XREF_CHUNK_SIZE; // first free item in last chunk
                                           // force a new chunk the first time


TXrefItem * AddXref ( TFileName * file, UINT line, TXrefItem * prev )
{
  if (chunkIndex == XREF_CHUNK_SIZE)
  {
    TXrefChunk * chunk;

    // Must allocate a new chunk
    chunkIndex = 0;
    chunk = xmalloc( sizeof( TXrefChunk ) );
    chunk->prevChunk = lastChunk;
    lastChunk = chunk;
  }

  lastChunk->items[chunkIndex].file = file;
  lastChunk->items[chunkIndex].line = line;
  lastChunk->items[chunkIndex].prev = prev;

  return &lastChunk->items[chunkIndex++];
};

void FreeAllXref ( void )
{
  while (lastChunk)
  {
    TXrefChunk * chunk = lastChunk->prevChunk;
    xfree( lastChunk );
    lastChunk = chunk;
  }
};
