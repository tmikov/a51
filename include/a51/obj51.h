/*
  Definitions for OBJ51 file format.
  Version 0.1

  Created by Ceco: Apr 9th 1997
*/

#ifndef OBJ51_H
#define OBJ51_H

/* This	ID right after the size of the obj */
#define MagicID  0x31353038L  // '8051'

#define OBJ_MAJ       0
#define OBJ_MIN       3

// types of blocks
#define OBLK_MODNAME  1
#define OBLK_NAMES    2
#define OBLK_EXTERN   3
#define OBLK_PUBLIC   4
#define OBLK_COMMENT  5
#define OBLK_SEGMENTS 6
#define OBLK_CODE     7
#define OBLK_FIXUP    8

// segment types
#define O51_NOSEG     0
#define O51_CSEG      1
#define O51_ISEG      2
#define O51_BSEG      3
#define O51_XSEG      4
#define O51_DSEG      5

// the beginning of teh object file
typedef struct TObjStart
{
  DWORD objSize; // size of the whole file
  DWORD magic;   // MagicID
  BYTE  verMaj;
  BYTE  verMin;
} TObjStart;

// header of an object block
typedef struct TObjBlockHeader
{
  BYTE blkType;
  WORD blkLength;  // size of the whole block (including this header)
} TObjBlockHeader;
#define OBJ_BLOCK_HEADER_SIZE  (2+1)

// folowed by these blocks:

// the name of the object module. Must be the first block
/* Length of the szName is max of 255 bytes */
typedef struct TBlkModName
{
  char szName[1];
} TBlkModName;

typedef struct TBlkNames
{
  WORD count;
  char names[1]; // each name ends with a '\0'
} TBlkNames;

typedef struct TExternDef
{
  WORD nameIndex;       // index in the global names list (TBlkNames)
  BYTE segment;
} TExternDef;
#define OBJ_EXTERN_DEF_SIZE (2+1)

typedef struct TPublicDef
{
  WORD nameIndex;
  BYTE segment;
  WORD offset;
} TPublicDef;
#define OBJ_PUBLIC_DEF_SIZE (2+1+2)

typedef struct TBlkExtern
{
  WORD count;
  TExternDef externs[1];
} TBlkExtern;

typedef struct TBlkPublic
{
  WORD count;
  TPublicDef publics[1];
} TBlkPublic;

typedef struct TBlkComment
{
  BYTE commentType;
  char szComment[1];
} TBlkComment;

typedef struct TBlkSegments
{
  WORD sizeCSeg;
  WORD sizeBSeg;
  WORD sizeISeg;
  WORD sizeXSeg;

  WORD numDataChunks;
  WORD sizeChunk[1];
} TBlkSegments;

#define OBJ_BLOCK_SEGMENTS_SIZE  (6*2)

typedef struct TBlkCode
{
  WORD offset;
  BYTE code[1];
} TBlkCode;

#define FIXUP_BYTELO  0
#define FIXUP_BYTEHI  1
#define FIXUP_WORD    2
#define FIXUP_AJMP    3

#define FIXUP_SRC_EXTERN 0
#define FIXUP_SRC_FRAME  1

/*
  The value in last_code_block[target] is ignored. The
  offset is contained in _offset_.
*/
typedef struct TFixupItem
{
  WORD target;  // address in previous BlkCode
  BYTE fixType;
  BYTE srcType;
  union
  {
    WORD segment;
    WORD externIndex;
  } srcData;
  WORD offset;  // added to the target address
} TFixupItem;

#define OBJ_FIXUP_ITEM_SIZE  8

typedef struct TBlkFixup
{
  WORD count;
  TFixupItem fixups[1];
} TBlkFixup;

#endif // OBJ51_H
