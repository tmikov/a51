#pragma hdrstop

#if defined(WIN32) || defined(_WIN32)
                        // we need some functions for pointer validation
#  define USE_WINDOWS   // tell global.h we need WINDOWS.H
#endif

#include "global.h"
#include "clist.h"
#include "misc.h"

/*
  If HEAP_DBG is defined the heap checking is activated.
  HEAP_DBG must be set to one of the following values:
    0 - full heap checking ( slow xfree() )
    1 - fast xfree()
*/

// This function must be provided by the user. It is called when we run
// out of memory. In most implementations it should abort the program
//
extern void OutOfMemory( void );

//////////////////////////////////////////////////////////////////////////
// Global data

jmp_buf  ErrJmpBuf;  // used on exit on fatal errors


//--------------------------------------------------------------------------
// Name         ErrorJump
//
// Description  Called when we want to exit on fatal error
//--------------------------------------------------------------------------
void ErrorJump ( int value )
{
  longjmp( ErrJmpBuf, value );
};

////////////////////// Memory services ///////////////////////////////////

#ifdef HEAP_DBG

#define FREE_FILL_VALUE    0xCC             // int 3 for x86
#define MALLOC_FILL_VALUE  0xAA
#define END_MARK_LEN    4

typedef struct dbg_head
{
  TListEntry  link;
  unsigned    size;
  unsigned    line;
  char        file[12];
  unsigned    checkSum;      // ~sum( of bytes before checkSum )
  char        sgn[4];
} dbg_head;

static const char * heapErrMsg[] =
{
  "OK",                                          // 0
  "Can't access block",                          // 1
  "Start signature is bad",                      // 2
  "End signature is bad",                        // 3
  "Block not in heap list",                      // 4
  "Heap is corrupt",                             // 5
  "Heap list is corrupt",                        // 6
  "Bad check sum of block header",               // 7
};

static TListEntry heap = { &heap, &heap };

extern void (*_dbg_heap_abort_proc)(void) = NULL;

long _dbg_UsedHeapSize    = 0;
long _dbg_MaxUsedHeapSize = 0;
long _dbg_UsedBlocksCount = 0;
long _dbg_MaxUsedBlocksCount = 0;
long _dbg_xmallocCount    = 0;
long _dbg_xfreeCount      = 0;
long _dbg_xreallocCount   = 0;

//--------------------------------------------------------------------------
// Name         AbortProg
//
// Description  Aborts the program if a fatal error is encountered. Flushes
//              all file buffers.
//--------------------------------------------------------------------------
static void AbortProg ( void )
{
  if (_dbg_heap_abort_proc)
    _dbg_heap_abort_proc();
  flushall();
  abort();
};

//--------------------------------------------------------------------------
// Name         CalcCheckSum
//
// Description  Calculate the check sum of the header
//--------------------------------------------------------------------------
static unsigned CalcCheckSum ( dbg_head * p )
{
  unsigned sum, cnt;
  BYTE * pb;

  pb = (BYTE *)p;
  cnt = offsetof( dbg_head, checkSum );
  sum = 0;
  do
  {
    sum += *pb++;
  }
  while (--cnt);

  return ~sum;
};

//--------------------------------------------------------------------------
// Name         IsValidBlockPrim
//
// Description  Checks if a heap block is valid
// Returns:
//      0 - ok
//      1 - can't access block (protected mode violation under Win32)
//      2 - Start signature is bad
//      3 - End signature is bad
//--------------------------------------------------------------------------
static int IsValidBlockPrim ( dbg_head * p )
{
  if (p == NULL)
    return 1;

#if defined(WIN32)
  //
  // We are able to make some more validations under WIN32
  //
  if (IsBadReadPtr( p, sizeof( dbg_head ) ) ||
      IsBadReadPtr( p+1, p->size + END_MARK_LEN ))
  {
    return 1;
  }
#endif
  if (CalcCheckSum( p ) != p->checkSum)
    return 7;

  if (p->sgn[0] != 'D' ||
      p->sgn[1] != 'B' ||
      p->sgn[2] != 'G' ||
      p->sgn[3] != 'H')
  {
    return 2;
  }
  if (((BYTE *)(p + 1))[p->size]   != 'e' ||
      ((BYTE *)(p + 1))[p->size+1] != 'n' ||
      ((BYTE *)(p + 1))[p->size+2] != 'd' ||
      ((BYTE *)(p + 1))[p->size+3] != 'b')
  {
    return 3;
  }

  return 0;
};

//--------------------------------------------------------------------------
// Name         _dbg_validate_heap
//
// Description  Validates the entire heap.
//--------------------------------------------------------------------------
void _dbg_validate_heap ( const char * file, unsigned line )
{
  TListEntry * curP;
  int err;
  long blkCount;

  blkCount = 0;
  for ( curP = heap.Flink; !END_OF_LIST( &heap, curP ); curP = curP->Flink )
  {
    ++blkCount;

    // Check if there are too many blocks
    //
    if (blkCount > _dbg_UsedBlocksCount)
    {
      printf( "\n\n!!! %s(%u): VALIDATE_HEAP() failed; %s\n\n", file, line,
              heapErrMsg[6] );
      AbortProg();
    }

    if (err = IsValidBlockPrim( (dbg_head *)curP ))
    {
      printf( "\n\n!!! %s(%u): VALIDATE_HEAP(%p) failed; %s\n\n", file, line,
              curP + 1, heapErrMsg[err] );
      AbortProg();
    }
  }

  // Check if there are too few blocks
  //
  if (blkCount != _dbg_UsedBlocksCount)
  {
    printf( "\n\n!!! %s(%u): VALIDATE_HEAP() failed; %s\n\n", file, line,
            heapErrMsg[6] );
    AbortProg();
  }
}

//--------------------------------------------------------------------------
// Name         IsValidBlock
//
// Description  Checks if a heap block is valid
// Returns:
//      0 - ok
//      1 - can't access block (protected mode violation under Win32)
//      2 - Start signature is bad
//      3 - End signature is bad
//      4 - Invalid block (Block is not on heap list)
//      5 - Heap is corrupt
//      etc... for complete list of errors see heapErrMsg[]
//--------------------------------------------------------------------------
static int IsValidBlock ( dbg_head * p )
{
  PListEntry curP;
  long blkCount;

  if (p == NULL)
    return 1;

  // Iterate through the block list until we reach p
  //
  blkCount = 0;
  for ( curP = heap.Flink; ; curP = curP->Flink )
  {
    if (END_OF_LIST( &heap, curP )) 
      return 4;                     // p is not in the list !

    ++blkCount;
    if (blkCount > _dbg_UsedBlocksCount)
      return 6;                     // the list is corrupt

    if (curP == &p->link)
      break;                        // we found the block

    if (IsValidBlockPrim( (dbg_head *)curP ) != 0)
      return 5;                     // The current block is damaged
  }

  return IsValidBlockPrim( p );     // validate p
};

void _dbg_validate_heap_ptr_size ( const void * ptr, unsigned size,
                                   const char * file, unsigned line )
{
  dbg_head * p;
  int  err;

  p = (dbg_head *)ptr - 1;
  if (err = IsValidBlock( p ))
  {
    printf( "\n\n!!! %s(%u): VALIDATE_HEAP_PTR(%p) failed:%s\n\n", file, line,
            ptr, heapErrMsg[err] );
    AbortProg();
  }
  if (size != 0 && p->size != size)
  {
    printf( "\n\n!!! %s(%u): VALIDATE_HEAP_PTR(%p) size mismatch.\n"
            "size = %u, block size = %u\n\n", file, line, ptr, size, p->size );
    AbortProg();
  }
};


void _dbg_print_heap ( const char * msg )
{
  dbg_head * p;
  int err;

  printf( msg );
  printf( "Used size:   %ld\n"
          "Peak size:   %ld\n"
          "Used blocks: %ld\n"
          "Peak count:  %ld\n"
          "xmalloc():   %ld\n"
          "xfree():     %ld\n"
          "xrealloc():  %ld\n",
         _dbg_UsedHeapSize,
         _dbg_MaxUsedHeapSize,
         _dbg_UsedBlocksCount,
         _dbg_MaxUsedBlocksCount,
         _dbg_xmallocCount,
         _dbg_xfreeCount,
         _dbg_xreallocCount );

  for ( p = (dbg_head *)heap.Flink;
        !END_OF_LIST( &heap, p ); p = (dbg_head *)p->link.Flink )
  {
    if (err = IsValidBlockPrim( p ))
    {
      printf( "*** INVALID BLOCK at %p:%s\n", p, heapErrMsg[err] );
      AbortProg();
    }
      
    printf( "%p  %7u :%s(%d)\n", p + 1, p->size, p->file, p->line );
  }
};

//--------------------------------------------------------------------------
// Name         InsertBlock
//
// Description  Inserts a block in the heap list.
//              Sets its checkSum and the checkSum of the prev. one
//--------------------------------------------------------------------------
static void InsertBlock ( dbg_head * p )
{
  INSERT_TAIL_LIST( &heap, &p->link );
  p->checkSum = CalcCheckSum( p );

  if (!END_OF_LIST( &heap, p->link.Blink ))
    ((dbg_head *)p->link.Blink)->checkSum = CalcCheckSum( (dbg_head *)p->link.Blink );
};

//--------------------------------------------------------------------------
// Name         RemoveBlock
//
// Description  Removes a block from the heap list.
//              Sets the check sum of the prev and next blocks
//--------------------------------------------------------------------------
static void RemoveBlock ( dbg_head * p )
{
  REMOVE_ENTRY_LIST( &p->link );

  if (!END_OF_LIST( &heap, p->link.Blink ))
    ((dbg_head *)p->link.Blink)->checkSum = CalcCheckSum( (dbg_head *)p->link.Blink );
  if (!END_OF_LIST( &heap, p->link.Flink ))
    ((dbg_head *)p->link.Flink)->checkSum = CalcCheckSum( (dbg_head *)p->link.Flink );
};

//--------------------------------------------------------------------------
// Name         SetMarks
//
// Description  Sets the start and end safety marks of the block 
//--------------------------------------------------------------------------
static void SetMarks ( dbg_head * p )
{
  p->sgn[0] = 'D';
  p->sgn[1] = 'B';
  p->sgn[2] = 'G';
  p->sgn[3] = 'H';

  // Mark end of heap block
  ((BYTE *)(p + 1))[p->size]   = 'e';
  ((BYTE *)(p + 1))[p->size+1] = 'n';
  ((BYTE *)(p + 1))[p->size+2] = 'd';
  ((BYTE *)(p + 1))[p->size+3] = 'b';
};

//--------------------------------------------------------------------------
// Name         CopyFileName
//
// Description  Copies the file name to p->file. If the file name is too
//              long copies the last characters of the name. This is because
//              the first characters are likely to be directories and we
//              are not interested in them.
//--------------------------------------------------------------------------
static void CopyFileName ( dbg_head * p, const char * file )
{
  int l;

  l = strlen( file );
  if (l < sizeof( p->file ))
    memcpy( p->file, file, l + 1 );
  else
    memcpy( p->file, file + l - sizeof( p->file ) + 1, sizeof( p->file ) );
};

//--------------------------------------------------------------------------
// Name         _dbg__xmalloc
//
// Description  Allocates a block of memory in the debug heap.
//		Returns NULL if no memory
//--------------------------------------------------------------------------
void * _dbg__xmalloc ( unsigned size, const char * file, unsigned line )
{
  dbg_head * p;

  ++_dbg_xmallocCount;

  if ((p = malloc( size + sizeof( dbg_head ) + END_MARK_LEN )) == NULL)
    return NULL;

  if ((_dbg_UsedHeapSize += size) > _dbg_MaxUsedHeapSize)
    _dbg_MaxUsedHeapSize = _dbg_UsedHeapSize;

  if (++_dbg_UsedBlocksCount > _dbg_MaxUsedBlocksCount)
    _dbg_MaxUsedBlocksCount = _dbg_UsedBlocksCount;

  p->size = size;
  p->line = line;
  CopyFileName( p, file );
  SetMarks( p );
  InsertBlock( p );

  memset( p + 1, MALLOC_FILL_VALUE, size );

  return p + 1;
};

//--------------------------------------------------------------------------
// Name         _dbg_xmalloc
//
// Description  Allocates a block of memory in the debug heap.
//		If no memory calls OutOfMemory() (possibly aborts the program)
//--------------------------------------------------------------------------
void * _dbg_xmalloc ( unsigned size, const char * file, unsigned line )
{
  void * p;

  if ((p = _dbg__xmalloc( size, file, line )) == NULL)
    OutOfMemory();
  else
    VALIDATE_HEAP_PTR_SIZE( p, size );

  return p;
};

void _dbg_xfree ( void * toFree, const char * file, unsigned line )
{
  dbg_head * p;
  int err;

  if (toFree == NULL)
    return;

  ++_dbg_xfreeCount;

  p = (dbg_head *)toFree - 1;

#if HEAP_DBG == 1
  if (err = IsValidBlockPrim( p ))
#else
  if (err = IsValidBlock( p ))
#endif
  {
    printf( "\n\n!!! %s(%u): xfree() with invalid block:%s\n\n", file, line,
            heapErrMsg[err] );
    AbortProg();
  }

  _dbg_UsedHeapSize -= p->size;
  --_dbg_UsedBlocksCount;

  RemoveBlock( p );

  memset( p, FREE_FILL_VALUE, p->size + sizeof( dbg_head ) + END_MARK_LEN );

  free( p );
};

//--------------------------------------------------------------------------
// Name         xrealloc
//
// Description  Changes the size of a prealocated block in heap.
//              Abides to ANSI C standart for realloc()
//		Calls OutOfMemory() if no memory.
//--------------------------------------------------------------------------
void * xrealloc ( void * blk, unsigned new_size )
{
  dbg_head * p;
  dbg_head * new_p;
  unsigned  temp;

  // new_size is 0 => just free the block
  //
  if (new_size == 0)
  {
    xfree( blk );
    return NULL;
  }

  // blk is NULL => allocate a new block
  //
  if (blk == NULL)
    return xmalloc( new_size );

  ++_dbg_xreallocCount;

  p = (dbg_head *)blk - 1;
  if (temp = IsValidBlock( p ))
  {
    printf( "\n\n!!!: xrealloc() with invalid block:%s\n\n", heapErrMsg[temp] );
    AbortProg();
  }

  temp = p->size;   // preserve the old size of the block

  if (temp == new_size) // Are we changing the size of the block?
    return blk;     // no change in size : exit

  RemoveBlock( p ); // detach block from valid list

  // Destroy marks
  //
  memset( p->sgn, FREE_FILL_VALUE, sizeof( p->sgn ) );
  memset( (BYTE *)(p + 1) + p->size, FREE_FILL_VALUE, END_MARK_LEN );

  if ((new_p = realloc( p, new_size + sizeof( dbg_head ) + END_MARK_LEN )) == NULL)
  {
    /* In most cases OutOfMemory() will abort the program.
    In case it doesn't we must abide to the ANSI specification of realloc():
    If realloc() fails it does not free the old block. So, we must restore
    the situation as it was before xrealloc() was called */

    SetMarks( p );
    InsertBlock( p );

    OutOfMemory();

    return NULL; 
  }

  _dbg_UsedHeapSize -= temp; // subtract old size
  if ((_dbg_UsedHeapSize += new_size) > _dbg_MaxUsedHeapSize)
    _dbg_MaxUsedHeapSize = _dbg_UsedHeapSize;

  new_p->size = new_size;
  SetMarks( new_p );
  InsertBlock( new_p );

  return new_p + 1;
};

#else

void * xmalloc ( unsigned size )
{
  void * p;
  if ((p = _xmalloc( size )) == NULL)
    OutOfMemory();
  return p;
};

void * xrealloc ( void * p, unsigned size )
{
  if (size == 0)
  {
    free( p );
    return NULL;
  }
  if ((p = realloc( p, size )) == NULL)
    OutOfMemory();
  return p;
};

#endif

/////////////////////////////// String support ///////////////////////////

#ifdef HEAP_DBG

UCHAR * _dbg_AllocString ( unsigned len, const UCHAR * str,
                           const char * file, unsigned line )
{
  UCHAR * p = _dbg_xmalloc( len + 1, file, line );
  return memcpy( p, str, len + 1 );
};

#else

UCHAR * AllocString ( unsigned len, const UCHAR * str )
{
  UCHAR * p = xmalloc( len + 1 );
  return memcpy( p, str, len + 1 );
};

#endif

UCHAR * DupString ( const UCHAR * str )
{
  UCHAR * res;
  int len;

  if (res = xmalloc( len = strlen( str ) + 1 ))
    memcpy( res, str, len );
  return res;
};

