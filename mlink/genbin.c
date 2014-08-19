#include "global.h"
#pragma hdrstop
#include "mlink.h"

#define BUF_SIZ     0xFF00

static BYTE * buffer;
static TObjStart strt;
static TObjBlockHeader hdr;
static UINT bufUsed;
static LONG pos;

static BYTE * lastCode;
static WORD   lastCodeOffset;

static void Read ( void * buf, UINT len )
{
  if (fread( buf, 1, len, fInFile ) != len)
    FatalError( "Error reading source" );
};

static BYTE ReadByte ( void )
{
  BYTE b;
  Read( &b, 1 );
  return b;
};

static WORD ReadWord ( void )
{
#if (INTEL_BYTE_ORDER)
  WORD w;
  Read( &w, 2 );
  return w;
#else
  return ReadByte() + (ReadByte() << 8);
#endif
};

static DWORD ReadDWord ( void )
{
#if (INTEL_BYTE_ORDER)
  DWORD d;
  Read( &d, 4 );
  return d;
#else
  return
    (DWORD)ReadByte() +
    ((DWORD)ReadByte() << 8) + 
    ((DWORD)ReadByte() << 16) + 
    ((DWORD)ReadByte() << 24);
#endif
};

static UINT ReadHdr ( void )
{
  hdr.blkType = ReadByte();
  hdr.blkLength = ReadWord();
  return hdr.blkType;
};

static void ProcessCode ( void )
{
  UINT len = hdr.blkLength - 5;

  lastCodeOffset = ReadWord();
  lastCode = buffer + lastCodeOffset;
  Read( lastCode, len );
  printf( "%04lX: Code at: %04Xh  Size: %d\n", pos, lastCodeOffset, len );

  if (lastCodeOffset + len > bufUsed)
    bufUsed = lastCodeOffset + len;
};

static void ProcessFixup ( void )
{
  UINT count;

  count = ReadWord();
  while (count-- > 0)
  {
    TFixupItem f;
    BYTE * p;

    f.target = ReadWord();
    f.fixType = ReadByte();
    f.srcType = ReadByte();
    f.srcData.segment = ReadWord();
    f.offset  = ReadWord();

    p = lastCode + f.target;

    switch (f.fixType)
    {
      case FIXUP_BYTELO:
        p[0] = f.offset;
        break;

      case FIXUP_BYTEHI:
        p[0] = f.offset >> 8;
        break;

      case FIXUP_WORD:
        p[0] = f.offset >> 8;
        p[1] = f.offset;
        break;

      case FIXUP_AJMP:
        p[0] = (p[0] & ~0xE0) | (f.offset >> 3) & 0xE0;
        p[1] = f.offset;
        if ((f.offset & 0xF800) != (lastCodeOffset + f.target + 2) & 0xF800)
          FatalError( "Invalid 11 bit fixup" );
        break;
    }
  }
};

void Process ( void )
{
  if ((buffer = malloc( BUF_SIZ )) == NULL)
    FatalError( "Out of memory" );
  bufUsed = 0;
  memset( buffer, 0xFF, BUF_SIZ );

  strt.objSize = ReadDWord();
  strt.magic = ReadDWord();
  strt.verMaj = ReadByte();
  strt.verMin = ReadByte();

  if (strt.magic != MagicID)
    FatalError( "Invalid object file" );
  if (strt.verMaj * 256 + strt.verMin > OBJ_MAJ * 256 + OBJ_MIN)
    puts( "Warning: Greater version object file" );

  lastCode = NULL;

  while ((pos = ftell( fInFile )) < strt.objSize)
  {
    UINT blk = ReadHdr();

    if (blk == OBLK_CODE)
      ProcessCode();
    else
    if (blk == OBLK_FIXUP)
      ProcessFixup();
    else
    {
      printf( "%04lX: Skipping: block(%d), size(%d)\n", pos, hdr.blkType, hdr.blkLength );
      fseek( fInFile, hdr.blkLength - OBJ_BLOCK_HEADER_SIZE, SEEK_CUR );
    }
  }

  fwrite( buffer, 1, bufUsed, fOutFile );
  free( buffer );
};

