/* t.c */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "global.h"
#include "obj51.h"

UINT CalcNamesSz(char *s[], UINT cnt)
{
  UINT l;
  UINT i;

  l = 0;
  for (i = 0; i < cnt; i++)
    l += strlen(s[i]) + 1;

  return (l);
};

void PutNames(char *s[], UINT cnt, FILE *f)
{
  UINT i;

  for (i = 0; i < cnt; i++)
    fwrite(s[i], 1, strlen(s[i]) + 1, f);
};

DWORD pascal filesize(FILE *stream)
/* Return file size in bytes */
{
  long curpos, length;

  curpos = ftell(stream);
  if (curpos == -1)
    return (-1);
  if (fseek(stream, 0L, SEEK_END))
    return (-1);
  length = ftell(stream);
  if (length == -1)
    return (-1);
  if (fseek(stream, curpos, SEEK_SET))
    return (-1);
  return(length);
};

int main(void)
{
  FILE *f;
  BYTE buf[100];
  struct TObjBlockHeader BlkHdr;
  const char modname[] = "test";
  char *a[] = {"label1", "label2", "label3"};

  f = fopen("test.obj", "wb");

  if (f == NULL) {
    showerror:
    printf("error: %s\n", sys_errlist[errno]);
    return (EXIT_FAILURE);
  };

  /* Preserve space to put the file size */
  fwrite(buf, 1, sizeof(DWORD), f);

  /* Put the MagicID */
  *(DWORD *)buf = MagicID;
  fwrite(buf, 1, sizeof(DWORD), f);

  /* Put the obj version */
  buf[0] = OBJ_MAJ;  /* Min */
  buf[1] = OBJ_MIN;  /* Maj */
  fwrite(buf, 1, sizeof(WORD), f);

  /* Block: module name */
  BlkHdr.blkType = OBLK_MODNAME;
  BlkHdr.blkLength = strlen(modname) + 1 + sizeof(struct TObjBlockHeader);
  fwrite(&BlkHdr, 1, sizeof(struct TObjBlockHeader), f);
  fwrite(modname, 1, BlkHdr.blkLength, f);

  /* Block: names */
  BlkHdr.blkType = OBLK_NAMES;
  BlkHdr.blkLength = CalcNamesSz(a, 3);
  fwrite(&BlkHdr, 1, sizeof(struct TObjBlockHeader), f);
  PutNames(a, 3, f);

  /* Get the file length */
  *(DWORD *)buf = filesize(f);
  fseek(f, 0, SEEK_SET);
  fwrite(buf, 1, sizeof(DWORD), f);

  fclose(f);
  return (EXIT_SUCCESS);
};

