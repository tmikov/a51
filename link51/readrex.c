/* readrex.c */

#include "global.h"
#pragma	hdrstop

#include "link51.h"
#include "err.h"
#include "obj51.h"

FILE *InFile = NULL;
struct TObjBlockHeader hdr;

void Read(void *buf, UINT len)
{
  if (fread(buf, 1, len, InFile) != len)
    Fatal("Error reading source");
};

BYTE ReadByte(void)
{
  BYTE b;
  Read(&b, 1);
  return b;
};

WORD ReadWord(void)
{
  return ReadByte() + (ReadByte() << 8);
};

DWORD ReadDWord(void)
{
  return
    (DWORD)ReadByte() +
    ((DWORD)ReadByte() << 8) + 
    ((DWORD)ReadByte() << 16) + 
    ((DWORD)ReadByte() << 24);
};

UINT ReadHdr(void)
{
  hdr.blkType = ReadByte();
  hdr.blkLength = ReadWord();
  return hdr.blkType;
};

