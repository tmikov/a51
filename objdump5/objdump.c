/* objdump.c */

#include <stdlib.h>
#include <errno.h>
#include "common/global.h"
#include "a51/obj51.h"
#include "collect.h"

char *segments[6] = {"no seg", "cseg", "iseg", "bseg", "xseg", "dseg"};
char *fixuptypes[4] = {"bytelo", "bytehi", "word", "ajump"};

char fname[80] = {'\0'};
BOOL foptions = TRUE;  /* Whether options were procesed successfully */
FILE *f;
struct TObjStart objstart;
char ObjModNm[255];
BOOL gerror;  /* Not a file error */
DWORD fpos;
struct TObjBlockHeader BlkHdr;
struct Collect names;
struct Collect externs;
struct Collect publics;
struct Collect bcode;
struct Collect fixup;
BOOL fDump;

/* ************************************************************************
   Function: Read
   Created: 22nd of May, 1997; 18:22
   Description:
     Reads the required number of bytes from the file f
   Returns:
     TRUE -- the requested number of bytes was resolved
     FALSE -- DOS fails to read the requested number of bytes
*/
BOOL Read(void *buf, UINT len, FILE *f)
{
  return (fread(buf, 1, len, f) == len);
};

/* ************************************************************************
   Function: ReadByte
   Created: 22nd of May, 1997; 18:23
   Description:
     Read a byte from the file f
   Returns:
     TRUE -- the byte was read
     FALSE -- DOS fails to read a byte from the file
*/
BOOL ReadByte(BYTE *b, FILE *f)
{
  return (Read(b, 1, f));
};

/* ************************************************************************
   Function: ReadWord
   Created: 22nd of May, 1997; 18:24
   Description:
     Reads a word from the file f
   Returns:
     TRUE -- the word was read
     FALSE -- DOS fails to read a word from the file
*/
BOOL ReadWord(WORD *w, FILE *f)
{
  #if (INTEL_BYTE_ORDER)
  return (Read(w, 2, f));
  #else
  /*if (!ReadByte((BYTE *)w + 1, f))
    return (FALSE);
  if (!ReadByte((BYTE *)w, f))
    return (FALSE);
  return (TRUE);*/
  #endif
};

/* ************************************************************************
   Function: ReadDWord
   Created: 22nd of May, 1997; 18:29
   Description:
     Reads a dword from the file f
   Returns:
     TRUE -- the dword was read
     FALSE -- DOS fails to read a word from the file
*/
BOOL ReadDWord(DWORD *d, FILE *f)
{
  #if (INTEL_BYTE_ORDER)
  return (Read(d, 4, f));
  #else
  /*if (!ReadByte((BYTE *)d + 0, f))
    return (FALSE);
  if (!ReadByte((BYTE *)d + 1, f))
    return (FALSE);
  if (!ReadByte((BYTE *)d + 2, f))
    return (FALSE);
  if (!ReadByte((BYTE *)d + 3, f))
    return (FALSE);
  return (TRUE);*/
  #endif
};

/* ************************************************************************
   Function: dump
   Created: 11th of April, 1997; 10:51
   Description:
     Dumps an array from the memory on stdout
*/
void dump(void *buf, WORD sz)
{
  WORD i;

  if (fDump) {
    printf("  %04x: ", fpos);
    for (i = 0; i < sz; i++)
      printf("%02x ", ((BYTE *)buf)[i]);
    printf("\n");
  };
};

/* ************************************************************************
   Function: DumpCode
   Created: 23rd of April, 1997; 9:06
   Description:
     Dumps a code block from memory on stdout
*/
void DumpCode(void *buf_, WORD sz)
{
  UINT i;
  void *buf2;
  WORD sz2;
  WORD sz3;
  BYTE *buf = (BYTE *)buf_;

  sz3 = sz;
  while (sz > 0) {
    printf("  %04x: ", sz3 - sz);
    buf2 = buf;
    sz2 = sz;
    for (i = 0; i < 16 && sz > 0; i++, sz--, buf++)
      printf("%02x ", *buf);
    buf = buf2;
    sz = sz2;
    if (i < 16)
      for (; i < 16; i++)
        printf("%2c ", ' ');
    printf("    ");
    for (i = 0; i < 16 && sz > 0; i++, sz--, buf++)
      if (*buf < 0x20)
        printf(".");
      else
        printf("%c", *buf);
    printf("\n");
  };

};

/* ************************************************************************
   Function: ReadObjStartStruct
   Created: 23rd of May, 1997; 16:50
   Description:
     Reads a TObjStart structure block from a file
   Returns:
     TRUE -- the structure was read
     FALSE -- DOS fails to read the structure from the file
*/
BOOL ReadObjStartStruct(struct TObjStart *s, FILE *f)
{
  if (!ReadDWord(&s->objSize, f))
    return (FALSE);
  if (!ReadDWord(&s->magic, f))
    return (FALSE);
  if (!ReadByte(&s->verMaj, f))
    return (FALSE);
  if (!ReadByte(&s->verMin, f))
    return (FALSE);
  return (TRUE);
};

/* ************************************************************************
   Function: ReadObjBlockHeaderStruct
   Created: 23rd of May, 1997; 17:02
   Description:
     Reads a TObjBlockHeader structure block from a file
   Returns:
     TRUE -- the structure was read
     FALSE -- DOS fails to read the structure from the file
*/
BOOL ReadObjBlockHeaderStruct(struct TObjBlockHeader *s, FILE *f)
{
  if (!ReadByte(&s->blkType, f))
    return (FALSE);
  if (!ReadWord(&s->blkLength, f))
    return (FALSE);
  return (TRUE);
};

/* ************************************************************************
   Function: ReadObjStart
   Created: 12th of April, 1997; 16:20
   Description:
     Reads object start bytes.
     Checks for valid MagicID as well
   On exit:
     TRUE - the start was reas and ID is OK
     FALSE - the read fails either by the file system or ID is invalid
*/
BOOL ReadObjStart(void)
{
  fpos = ftell(f);
  if (fpos == (DWORD)-1L)
    return (FALSE);
  if (!ReadObjStartStruct(&objstart, f))
    return (FALSE);
  if (objstart.magic != MagicID) {
    dump(&objstart, sizeof(TObjStart));
    printf("error: Bad ID\n");
    gerror = TRUE;
    return (FALSE);
  };
  return (TRUE);
};

/* ************************************************************************
   Function: ShowObjStart
   Created: 12th of April, 1997; 16:24
   Description:
     Displays object file start block
*/
void ShowObjStart(void)
{
  printf("%04ld: START SECTION\n", fpos);
  printf("  file size: %lu\n  obj format version: %d.%02d\n",
    objstart.objSize, objstart.verMaj, objstart.verMin);
  dump(&objstart, sizeof(TObjStart));
};

/* ************************************************************************
   Function: ReadBlkHdr
   Created: 12th of April, 1997; 17:05
   Description:
     Reads a block header
   On exit:
     FALSE - fails to read a header
     TRUE - read OK
     BlkHdr - the block header
*/
BOOL ReadBlkHdr(void)
{
  fpos = ftell(f);
  if (fpos == (DWORD)-1L)
    return (FALSE);
  if (!ReadObjBlockHeaderStruct(&BlkHdr, f))
    return (FALSE);
  printf("\n%04lx: ", fpos);
  switch (BlkHdr.blkType) {
    case OBLK_MODNAME:
      printf("BLOCK MODULE NAME");
      break;
    case OBLK_NAMES:
      printf("BLOCK NAMES");
      break;
    case OBLK_EXTERN:
      printf("BLOCK EXTERNAL DEFINITIONS");
      break;
    case OBLK_PUBLIC:
      printf("BLOCK PUBLIC DEFINITIONS");
      break;
    case OBLK_COMMENT:
      printf("BLOCK COMMENT");
      break;
    case OBLK_SEGMENTS:
      printf("BLOCK SEGMENTS");
      break;
    case OBLK_CODE:
      printf("BLOCK CODE");
      break;
    case OBLK_FIXUP:
      printf("BLOCK FIXUP");
      break;
    default:
      printf("BLOCK UNKNOWN");
      break;
  };
  printf(", size: %d bytes\n", BlkHdr.blkLength);
  dump(&BlkHdr, sizeof(struct TObjBlockHeader));
  return (TRUE);
};

/* ************************************************************************
   Function: ReadModNm
   Created: 11th of April, 1997; 9:58
   Description:
     Reads the module name block
*/
BOOL ReadModNm(void)
{
  if (BlkHdr.blkType != OBLK_MODNAME) {
    printf("error: module name block expected\n");
    fail:
    gerror = TRUE;  /* This is not a file error */
    return (FALSE);
  };

  if (BlkHdr.blkLength > 255) {
    printf("error in module name block: name length is great that 255 characters\n");
    goto fail;
  };

  fpos = ftell(f);
  if (fpos == (DWORD)-1L)
    return (FALSE);
  /* Read the module name */
  BlkHdr.blkLength -= OBJ_BLOCK_HEADER_SIZE;
  if (!Read(&ObjModNm, BlkHdr.blkLength, f))
    return (FALSE);

  return (TRUE);
};

/* ************************************************************************
   Function: ShowModNm
   Created: 11th of April, 1997; 10:11
   Description:
     Displays the module name on stdout
*/
void ShowModNm(void)
{
  printf("  MODULE NAME: %s\n", ObjModNm);
  dump(&ObjModNm, BlkHdr.blkLength);
};

/* ************************************************************************
   Function: ReadString
   Created: 23rd of April, 1997; 9:06
   Description:
     Reads a string from a file
   On exit:
     FALSE - reading fails
     TRUE - reading OK
*/
BOOL ReadString(char *b)
{
  char c;

  do {
    if (!Read(&c, sizeof(char), f))
      return (FALSE);
    *b = c;
    b++;
  } while (c != '\0');
  return (TRUE);
};

/* ************************************************************************
   Function: ReadNames
   Created: 11th of April, 1997; 13:50
   Description:
     Reads the names array from the object file
*/
BOOL ReadNames(void)
{
  WORD s;
  UINT l;
  char aname[255];
  char *p;

  /* Read the count */
  if (!ReadWord(&s, f))
    return (FALSE);
  printf("  names: %d\n", s);
  dump(&s, sizeof(WORD));
  while (s != 0) {
    fpos = ftell(f);
    if (fpos == (DWORD)-1L)
      return (FALSE);
    if (!ReadString(aname))
      return (FALSE);
    l = strlen(aname) + 1;
    p = malloc(l);
    if (p == NULL) {
      memerror:
      printf("error: out of memory\n");
      gerror = TRUE;
      return (FALSE);
    };
    strcpy(p, aname);
    if (!InsertItem(&names, p))
      goto memerror;
    printf("  %u: %s\n", names.cnt - 1, aname);
    dump(aname, l);
    s --;
  };
  return (TRUE);
};

/* ************************************************************************
   Function: ReadTExternDef
   Created: 2th of Jule, 1997; 6:09
   Description:
     Reads a TExternDef structure from a file
   On exit:
     FALSE - reading fails
     TRUE - reading OK
*/
BOOL ReadTExternDef(struct TExternDef *d, FILE *f)
{
  if (!ReadWord(&d->nameIndex, f))
    return (FALSE);
  if (!ReadByte(&d->segment, f))
    return (FALSE);
  return (TRUE);
};

/* ************************************************************************
   Function: ReadExterns
   Created: 2th of Jule, 1997; 6:09
   Description:
     Read the external definitions section from a object file
*/
BOOL ReadExterns(void)
{
  struct TExternDef t;
  void *p;
  WORD s;

  /* Read the count */
  if (!ReadWord(&s, f))
    return (FALSE);
  printf("  externs: %d\n", s);
  dump(&s, sizeof(WORD));
  while (s != 0) {
    fpos = ftell(f);
    if (fpos == (DWORD)-1L)
      return (FALSE);
    if (!ReadTExternDef(&t, f))
      return (FALSE);
    p = malloc(sizeof(struct TExternDef));
    if (p == NULL) {
      memerror:
      printf("error: out of memory\n");
      gerror:
      gerror = TRUE;
      return (FALSE);
    };
    memcpy(p, &t, sizeof(struct TExternDef));
    if (!InsertItem(&externs, p))
      goto memerror;
    if (t.nameIndex >= names.cnt) {
      dump(&t, sizeof(struct TExternDef));
      printf("error: invalid extern reference index\n");
      goto gerror;
    };
    if (t.segment > 6) {
      dump(&t, sizeof(struct TExternDef));
      printf("error: invalid extern reference segment\n");
      goto gerror;
    };
    printf("  %s (%u), %s (%u)\n", AtItem(&names, t.nameIndex), t.nameIndex,
      segments[t.segment], t.segment);
    dump(&t, sizeof(struct TExternDef));
    s --;
  };
  return (TRUE);
};


/* ************************************************************************
   Function: ReadTPublicDef
   Created: 2th of Jule, 1997; 6:09
   Description:
     Reads a TPublicDef structure from a file
   On exit:
     FALSE - reading fails
     TRUE - reading OK
*/
BOOL ReadTPublicDef(struct TPublicDef *d, FILE *f)
{
  if (!ReadWord(&d->nameIndex, f))
    return (FALSE);
  if (!ReadByte(&d->segment, f))
    return (FALSE);
  if (!ReadWord(&d->offset, f))
    return (FALSE);
  return (TRUE);
};

/* ************************************************************************
   Function: ReadPublics
   Created: 23rd of April, 1997; 9:07
   Description:
     Reads the public definitions section from a object file
*/
BOOL ReadPublics(void)
{
  struct TPublicDef t;
  void *p;
  WORD s;

  /* Read the count */
  if (!ReadWord(&s, f))
    return (FALSE);
  printf("  publics: %d\n", s);
  dump(&s, sizeof(WORD));
  while (s != 0) {
    fpos = ftell(f);
    if (fpos == (DWORD)-1L)
      return (FALSE);
    if (!ReadTPublicDef(&t, f))
      return (FALSE);
    p = malloc(sizeof(struct TPublicDef));
    if (p == NULL) {
      memerror:
      printf("error: out of memory\n");
      gerror:
      gerror = TRUE;
      return (FALSE);
    };
    memcpy(p, &t, sizeof(struct TPublicDef));
    if (!InsertItem(&publics, p))
      goto memerror;
    if (t.nameIndex >= names.cnt) {
      dump(&t, sizeof(struct TPublicDef));
      printf("error: invalid extern reference index\n");
      goto gerror;
    };
    if (t.segment > 6) {
      dump(&t, sizeof(struct TPublicDef));
      printf("error: invalid extern reference segment\n");
      goto gerror;
    };
    printf("  %s (%u), %s (%u), offset: %04x\n", AtItem(&names, t.nameIndex), t.nameIndex,
      segments[t.segment], t.segment, t.offset);
    dump(&t, sizeof(struct TPublicDef));
    s --;
  };
  return (TRUE);
};

/* ************************************************************************
   Function: ReadSegments
   Created: 23rd of April, 1997; 9:07
   Description:
     Reads the segments sizes sections from a object file
     Reads the data chunks sizes section as well
   On exit:
     FALSE - reading fails
     TRUE - reading OK
*/
BOOL ReadSegments(void)
{
  WORD s;
  WORD x;

  fpos = ftell(f);
  if (fpos == (DWORD)-1L)
    return (FALSE);
  if (!ReadWord(&s, f))
    return (FALSE);
  printf("  cseg size: %u\n", s);
  dump(&s, sizeof(WORD));

  fpos = ftell(f);
  if (fpos == (DWORD)-1L)
    return (FALSE);
  if (!ReadWord(&s, f))
    return (FALSE);
  printf("  bseg size: %u\n", s);
  dump(&s, sizeof(WORD));

  fpos = ftell(f);
  if (fpos == (DWORD)-1L)
    return (FALSE);
  if (!ReadWord(&s, f))
    return (FALSE);
  printf("  iseg size: %u\n", s);
  dump(&s, sizeof(WORD));

  fpos = ftell(f);
  if (fpos == (DWORD)-1L)
    return (FALSE);
  if (!ReadWord(&s, f))
    return (FALSE);
  printf("  xseg size: %u\n", s);
  dump(&s, sizeof(WORD));

  fpos = ftell(f);
  if (fpos == (DWORD)-1L)
    return (FALSE);
  if (!ReadWord(&s, f))
    return (FALSE);
  printf("  dseg chunks: %u\n", s);
  dump(&s, sizeof(WORD));

  while (s > 0) {
    fpos = ftell(f);
    if (fpos == (DWORD)-1L)
      return (FALSE);
    if (!ReadWord(&x, f))
      return (FALSE);
    printf("  data chunk of %u bytes\n", x);
    dump(&x, sizeof(WORD));
    s--;
  };
  return (TRUE);
};

/* ************************************************************************
   Function: ReadCode
   Created: 23rd of April, 1997; 9:07
   Description:
     Reads a code block section from a object file
*/
BOOL ReadCode(void)
{
  WORD s;
  void *b;
  UINT l;

  fpos = ftell(f);
  if (fpos == (DWORD)-1L)
    return (FALSE);
  if (!ReadWord(&s, f))
    return (FALSE);
  printf("  offset: %u\n", s);
  dump(&s, sizeof(WORD));

  l = BlkHdr.blkLength - OBJ_BLOCK_HEADER_SIZE - sizeof(WORD);
  b = malloc(l);
  if (b == NULL) {
    memerror:
    printf("error: out of memory\n");
    gerror = TRUE;
    return (FALSE);
  };

  fpos = ftell(f);
  if (fpos == (DWORD)-1L)
    return (FALSE);

  if (!Read(b, l, f))
    return (FALSE);
  DumpCode(b, l);

  if (!InsertItem(&bcode, b))
    goto memerror;

  return (TRUE);
};

/* ************************************************************************
   Function: ReadTFixupItem
   Created: 2th of Jule, 1997; 6:09
   Description:
     Reads a TFixupItem structure from a file
   On exit:
     FALSE - reading fails
     TRUE - reading OK
*/
BOOL ReadTFixupItem(struct TFixupItem *d, FILE *f)
{
  if (!ReadWord(&d->target, f))
    return (FALSE);
  if (!ReadByte(&d->fixType, f))
    return (FALSE);
  if (!ReadByte(&d->srcType, f))
    return (FALSE);
  if (!ReadWord(&d->srcData.segment, f))
    return (FALSE);
  if (!ReadWord(&d->offset, f))
    return (FALSE);
  return (TRUE);
};

/* ************************************************************************
   Function: ReadFixup
   Created: 23rd of April, 1997; 9:07
   Description:
     Reads code and data fixups section from a object file
*/
BOOL ReadFixup(void)
{
  WORD s;
  struct TFixupItem *b;
  struct TExternDef *e;

  fpos = ftell(f);
  if (fpos == (DWORD)-1L)
    return (FALSE);
  if (!ReadWord(&s, f))
    return (FALSE);
  printf("  count: %u\n", s);
  dump(&s, sizeof(WORD));

  while (s > 0) {
    b = malloc(sizeof(struct TFixupItem));
    if (b == NULL) {
      memerror:
      printf("error: out of memory\n");
      gerror = TRUE;
      return (FALSE);
    };
    fpos = ftell(f);
    if (fpos == (DWORD)-1L)
      return (FALSE);

    if (!ReadTFixupItem(b, f))
      return (FALSE);

    printf("  target: %04x, offset: %04x, type: %s, ",
      b->target, b->offset, fixuptypes[b->fixType]);
    if (b->srcType == FIXUP_SRC_FRAME) {
      if (b->srcData.segment < 5)
        printf("from: %s\n", segments[b->srcData.segment]);
      else
        printf("from: dseg chunk %d\n", b->srcData.segment - O51_DSEG);
    }
    else {  /* if FIXUP_SRC_FRAME */
     /* Check for valid external reference index */
     if (b->srcData.externIndex > externs.cnt) {
       printf("error: Invalid extern reference\n");
       dump(b, sizeof(struct TFixupItem));
       gerror = TRUE;
       return (FALSE);
     };
     e = AtItem(&externs, b->srcData.externIndex);
     printf("from: extrn seg: %s, name: %s\n", segments[e->segment],
       AtItem(&names, e->nameIndex));
    };

    dump(b, sizeof(struct TFixupItem));

    if (!InsertItem(&fixup, b))
      goto memerror;
    s--;
  };

  return (TRUE);
};

/* ************************************************************************
   Function: ProcessOption
   Created: 23rd of April, 1997; 9:26
   Description:
*/
void ProcessOption(char *option)
{
  int l;

  if (option[0] != '/' && option[0] != '-') {
    /* This is a filename */
    strcpy(fname, option);
    if (strchr(fname, '.') == NULL)
      strcat(fname, ".obj");
    return;
  };

  strupr(option);
  l = strlen(option);
  switch (option[1]) {
    case 'D':
      if (l != 2) {
	badopt:
        printf("Invalid option %s\n", option);
	foptions = FALSE;
	return;
      };
      fDump = TRUE;
      break;
    default:
      goto badopt;
  };
};

/* ************************************************************************
   Function: main
   Created: 2th of Jule, 1997; 6:13
   Description:
*/
int main(int argc, char *argv[])
{
  BOOL c;
  DWORD fpos;
  UINT i;

  gerror = FALSE;
  fDump = FALSE;

  printf("ObjDump51 Version 0.1\n");

  for (i = 1; i < argc; i++)
    ProcessOption(argv[i]);

  if (!foptions)
    return (EXIT_FAILURE);

  if (fname[0] == '\0') {
    printf("usage: objdump [-d] file[.obj]\n"
           "  -d  Detailed dump\n");
    return (EXIT_FAILURE);
  };

  f = fopen(fname, "rb");
  if (f == NULL) {
    showerror:
    if (!gerror)
      printf("objdump error: %s\n", sys_errlist[errno]);
    return (EXIT_FAILURE);
  };

  if (!ReadObjStart())
    goto showerror;
  ShowObjStart();

  if (!ReadBlkHdr())
    goto showerror;
  ReadModNm();
  ShowModNm();

  if (!InitCollect(&names, 64)) {
    printf("error: out of memory\n");
    gerror = TRUE;
    goto showerror;
  };

  if (!InitCollect(&externs, 64)) {
    printf("error: out of memory\n");
    gerror = TRUE;
    goto showerror;
  };

  if (!InitCollect(&publics, 64)) {
    printf("error: out of memory\n");
    gerror = TRUE;
    goto showerror;
  };

  if (!InitCollect(&bcode, 64)) {
    printf("error: out of memory\n");
    gerror = TRUE;
    goto showerror;
  };

  if (!InitCollect(&fixup, 64)) {
    printf("error: out of memory\n");
    gerror = TRUE;
    goto showerror;
  };

  /* Variable blocks part */
  c = TRUE;
  while (c) {
    if (!ReadBlkHdr())
      goto showerror;
    switch (BlkHdr.blkType) {
      case OBLK_NAMES:
        if (!ReadNames())
          goto showerror;
        break;
      case OBLK_EXTERN:
	if (!ReadExterns())
	  goto showerror;
        break;
      case OBLK_PUBLIC:
        if (!ReadPublics())
	  goto showerror;
	break;
      case OBLK_SEGMENTS:
	if (!ReadSegments())
	  goto showerror;
	break;
      case OBLK_CODE:
        if (!ReadCode())
	  goto showerror;
	break;
      case OBLK_FIXUP:
        if (!ReadFixup())
	  goto showerror;
	break;
      default:
        printf("  unrecognized block of %d bytes, skipped\n", BlkHdr.blkLength);
	fseek(f, BlkHdr.blkLength - sizeof(struct TObjBlockHeader), SEEK_CUR);
	break;
    };
    fpos = ftell(f);
    if (fpos == (DWORD)-1L)
      return (FALSE);
    if (fpos == objstart.objSize)
      c = FALSE;
  };

  fclose(f);
  return (EXIT_SUCCESS);
};

