/*
  genobj.c: Generate an object file

  It turned out that we cannot turn off the structure alignment for all
  compilers. So we can't read & write the whole structures as defined
  in OBJ51.H. Also: we always store the values in INTEL byte order,
  so we must write the words & dwords in a silly way if !INTEL_BYTE_ORDER.
  Note that the program will work on INTEL byte ordered machines even if
  INTEL_BYTE_ORDER is not defined, but slower.
*/

#include "global.h"
#pragma hdrstop
#include "a51.h"

#define CODE_BUF_SIZE  4096
#define MAX_FIXUPS     1024

FILE * ObjF = NULL;

////////////////////////////////////////////////////////////////////////////
// Local data

static UINT NumExterns; // number of extern symbols in the module
static UINT NumPublics; // number of public symbols in the module

static BYTE * pCodeBuf;
static UINT CodeBufAddr;  // address of the start of the buffer
static UINT CodeUsed;   // space in the code buffer used
static UINT CodeSize;   // current size of the code buffer

static TFixupItem FixupBuf[MAX_FIXUPS];
static UINT       FixupCount;

////////////////////////////////////////////////////////////////////////////
// Local functions

static void Obj_Put ( const void * buf, UINT len )
{
  if (fwrite( buf, 1, len, ObjF ) != len)
    FatalError( "Error writing to output file" );
};

static void Obj_PutByte ( BYTE b )
{
  Obj_Put( &b, 1 );
};

static void Obj_PutWord ( WORD w )
{
#if (!INTEL_BYTE_ORDER)
  Obj_PutByte( w & 0xFF );
  Obj_PutByte( (w >> 8) & 0xFF );
#else
  Obj_Put( &w, 2 );
#endif
};

static void Obj_PutDWord ( DWORD d )
{
#if (!INTEL_BYTE_ORDER)
  Obj_PutByte( d & 0xFF );
  Obj_PutByte( (d >> 8) & 0xFF );
  Obj_PutByte( (d >> 16) & 0xFF );
  Obj_PutByte( (d >> 24) & 0xFF );
#else
  Obj_Put( &d, 4 );
#endif
};

static long Obj_PutHeader ( UINT type, UINT len )
{
  long            pos = ftell( ObjF );
  Obj_PutByte( type );
  Obj_PutWord( len + OBJ_BLOCK_HEADER_SIZE );
  return pos;
};

static void Obj_PutModName ( const char * modName )
{
  UINT len = strlen( modName ) + 1;

  Obj_PutHeader( OBLK_MODNAME, len );
  Obj_Put( modName, len );
};

static void Obj_StartFile ( void )
{
  fseek( ObjF, 0, SEEK_SET );
  Obj_PutDWord( 0 );
  Obj_PutDWord( MagicID );
  Obj_PutByte( OBJ_MAJ );
  Obj_PutByte( OBJ_MIN );
  Obj_PutModName( szBareInputName );
};

static long Obj_Seek ( long newPos )
{
  long oldPos = ftell( ObjF );
  fseek( ObjF, newPos, SEEK_SET ); 
  return oldPos;
};

static void Obj_EndFile ( void )
{
  DWORD size = Obj_Seek( 0 );
  Obj_PutDWord( size );
  Obj_Seek( size );
};

//--------------------------------------------------------------------------
// Name         FlushCodeBuf
//
// Description  Stores the current code buffer in the file.
//              Frees the current buffer.
//--------------------------------------------------------------------------
static void FlushCodeBuf ( void )
{
  if (pCodeBuf == NULL)
    return;

  // store the code block
  Obj_PutHeader( OBLK_CODE, sizeof( WORD ) + CodeUsed );
  Obj_PutWord( (WORD)CodeBufAddr );
  Obj_Put( pCodeBuf, CodeUsed );

  // store the fixup block (if any)
  if (FixupCount)
  {
    UINT i;
    Obj_PutHeader( OBLK_FIXUP, sizeof( WORD ) + FixupCount * OBJ_FIXUP_ITEM_SIZE );  
    Obj_PutWord( (WORD)FixupCount );
    for( i = 0; i < FixupCount; ++i )
    {
      Obj_PutWord( FixupBuf[i].target );
      Obj_PutByte( FixupBuf[i].fixType );
      Obj_PutByte( FixupBuf[i].srcType );
      Obj_PutWord( FixupBuf[i].srcData.segment );
      Obj_PutWord( FixupBuf[i].offset );
    }
  }

  CodeBufAddr += CodeUsed;

  FixupCount =
  CodeSize =
  CodeUsed = 0;
  xfree( pCodeBuf );
  pCodeBuf = NULL;
};

//////////////////////////////////////////////////////////////////////
// Global functions

void Obj_OpenFile ( void )
{
  if (!bMakeObj)
    return;

  if ((ObjF = fopen( szOutputFile, "wb" )) == NULL)
    FatalError( "Error opening output file '%s'", szOutputFile );
  else
    setvbuf( ObjF, NULL, _IOFBF, 8192 );
  Obj_StartFile();
};

void Obj_CloseFile ( void )
{
  if (!bMakeObj)
    return;

  FlushCodeBuf();

  Obj_EndFile();
  fclose( ObjF );
  ObjF = NULL;
};

//--------------------------------------------------------------------------
// Name         Obj_PutNames
//
// Description  Traverse the identifier list and generate a name for each
//              extern or public symbol
// NOTE:        Stores the number of externs in NumExterns
//              and the number of publics in NumPublics
//--------------------------------------------------------------------------
void Obj_PutNames ( void )
{
  TIdent * curIdent;
  long pos;
  LONG size = 0;
  UINT count = 0;

  if (!bMakeObj)
    return;

  // store dummy header and save the pos so we can correct the header later
  pos = Obj_PutHeader( OBLK_NAMES, 0 );
  Obj_PutWord( 0 );

  NumExterns =
  NumPublics = 0;
  for( curIdent = pFirstIdent; curIdent; curIdent = curIdent->pNextIdent )
  {
    if (curIdent->e.rType == RELOC_EXTERN || curIdent->isPublic)
    {
      UINT len;

      if (curIdent->e.rType == RELOC_EXTERN)
      {
        if (curIdent->bUsed)
        {
          curIdent->externIndex = NumExterns++;
          if (NumExterns == 0xFFFF)
            FatalError( "Too many extern definitions" );
        }
      }
      else
      {
        ++NumPublics;
        if (NumExterns == 0xFFFF)
          FatalError( "Too many public definitions" );
      }

      curIdent->nameIndex = count++;

      // check for word overflow
      if (count == 0)
        FatalError( "Too many symbols" );

      // put the string
      len = strlen( curIdent->szName ) + 1;
      if ((size += len) > 0xFFFF)
        FatalError( "Too long strings" );
      Obj_Put( curIdent->szName, len );
    }
  }

  // go back to header
  pos = Obj_Seek( pos );
  Obj_PutHeader( OBLK_NAMES, size + 2 );
  Obj_PutWord( count );
  Obj_Seek( pos ); // go to the end
};

void Obj_PutSegments ( void )
{
  if (!bMakeObj)
    return;

  Obj_PutHeader( OBLK_SEGMENTS,
                 Segments[O51_DSEG].size ?
                 OBJ_BLOCK_SEGMENTS_SIZE : OBJ_BLOCK_SEGMENTS_SIZE - 2 );
  Obj_PutWord( Segments[O51_CSEG].size );
  Obj_PutWord( Segments[O51_BSEG].size );
  Obj_PutWord( Segments[O51_ISEG].size );
  Obj_PutWord( Segments[O51_XSEG].size );

  if (Segments[O51_DSEG].size)  // if there is DSEG data at all...
  {
    Obj_PutWord( 1 );
    Obj_PutWord( Segments[O51_DSEG].size );
  }
  else
    Obj_PutWord( 0 );
};

//--------------------------------------------------------------------------
// Name         Obj_PutExterns
//
// Description  Generates the extern definitions.
//              Uses NumExterns
//--------------------------------------------------------------------------
void Obj_PutExterns ( void )
{
  UINT count;
  TIdent * pIdent;

  if (!bMakeObj)
    return;
  if (!NumExterns) // important (see we have NumExterns - 1 below)
    return;

  Obj_PutHeader( OBLK_EXTERN, 2 + NumExterns * OBJ_EXTERN_DEF_SIZE );

  count = NumExterns;
  Obj_PutWord( NumExterns );

  for( pIdent = pFirstIdent; pIdent; pIdent = pIdent->pNextIdent )
  {
    if (pIdent->e.rType == RELOC_EXTERN && pIdent->bUsed)
    {
      Obj_PutWord( pIdent->nameIndex );
      Obj_PutByte( pIdent->e.seg );

      if (--count == 0)
        break;
    }
  }
};

//--------------------------------------------------------------------------
// Name         Obj_PutPublics
//
// Description  Generates the extern definitions.
//              Uses NumPublics
//--------------------------------------------------------------------------
void Obj_PutPublics ( void )
{
  UINT count;
  TIdent * pIdent;

  if (!bMakeObj)
    return;

  if (!NumPublics) // important (see we have NumPublics - 1 below)
    return;

  Obj_PutHeader( OBLK_PUBLIC, 2 + NumPublics * OBJ_PUBLIC_DEF_SIZE );

  count = NumPublics;
  Obj_PutWord( NumPublics );

  for( pIdent = pFirstIdent; pIdent; pIdent = pIdent->pNextIdent )
  {
    if (pIdent->isPublic)
    {
      if (pIdent->e.rType == RELOC_UNDEF)
        PrintError( ERR_SYM_UNDEF, pIdent->szName );
      else
      {
        Obj_PutWord( pIdent->nameIndex );
        Obj_PutByte( pIdent->e.seg );
        Obj_PutWord( pIdent->e.uValue );
      }

      if (--count == 0)
        break;
    }
  }
};

void Obj_StartAt ( UINT codeAddr )
{
  FlushCodeBuf();
  CodeBufAddr = codeAddr;
};

void Obj_UseSpace ( UINT len )
{
  if (len)
    Obj_StartAt( CodeBufAddr + CodeUsed + len );
};

//--------------------------------------------------------------------------
// Name         Obj_Reserve
//
// Description  Make sure we can generate _len_ bytes of code in the current
//              code block. If we can't allocates a new code block.
//              Exactly the same number of bytes MUST be generated with
//              Obj_PutCodeBytes before another call to Obj_Reserve.
//--------------------------------------------------------------------------
void Obj_Reserve ( UINT len )
{
  if (!bMakeObj)
    return;

  if (len == 0)
  {
    TRACE0( "Obj_Reserve( 0 ) called\n" );
    return;
  }

  // if the buffer overflows (if we have no buffer CodeSize will be 0)
  if (CodeUsed + len > CodeSize)
  {
    FlushCodeBuf();     // save the previous buffer (if any)

    CodeSize = len > CODE_BUF_SIZE ? len : CODE_BUF_SIZE;
    pCodeBuf = xmalloc( CodeSize );
    CodeUsed = 0;
  }
  // now we are prepared to insert at least len bytes in the buffer
};

//--------------------------------------------------------------------------
// Name         Obj_PutCodeBytes
//
// Description  Generates code bytes int current code block.
//              Exactly _len_ number of bytes must have been reserved with
//              Obj_Reserve.
//--------------------------------------------------------------------------
void Obj_PutCodeBytes ( const BYTE * buf, UINT len )
{
  if (!bMakeObj)
    return;

  ASSERT( len + CodeUsed <= CodeSize );

  memcpy( pCodeBuf + CodeUsed, buf, len );
  CodeUsed += len;
};

//--------------------------------------------------------------------------
// Name         Obj_PutFixUp
//
// Description  Generate a fixup for address CodeUsed + offset.
//              if srcType == FIXUP_SRC_EXTERN srcData is the extern index
//              if srcType == FIXUP_SRC_FRAME  srcData is the frame index
//--------------------------------------------------------------------------
void Obj_PutFixUp ( int offset, UINT fixType, UINT srcType, UINT srcData,
                    UINT dOffset )
{
  if (!bMakeObj)
    return;

  ASSERT( (long)CodeUsed + offset >= 0 );
  ASSERT( CodeUsed + offset < CodeSize );

  if (FixupCount == MAX_FIXUPS)
    PrintFatal( FATAL_MANY_FIXUPS );

  FixupBuf[FixupCount].target  = CodeUsed + offset;
  FixupBuf[FixupCount].fixType = fixType;
  FixupBuf[FixupCount].srcType = srcType;
  if (srcType == FIXUP_SRC_EXTERN)
    FixupBuf[FixupCount].srcData.externIndex = srcData;
  else
    FixupBuf[FixupCount].srcData.segment = srcData;
  FixupBuf[FixupCount].offset = dOffset;

  ++FixupCount;
};

void Obj_Init ( void )
{
  ObjF = NULL;
  NumExterns = NumPublics = 0;

  CodeBufAddr = 
  FixupCount =
  CodeUsed = 
  CodeSize = 0;
  pCodeBuf = NULL;
};

void Obj_Done ( void )
{
  xfree( pCodeBuf );
  ASSERT( ObjF == NULL );
};
