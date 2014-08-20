/* map.c */

/*
Generates a map file of how the link has been done.
*/

#include "common/global.h"
#pragma hdrstop

#include "link51.h"

static char szModuleName[FILENAME_MAX];

/* ************************************************************************
   Function: ComposeObjModuleName
   Description:
*/
void ComposeObjModuleName(int nObjModule)
{
  struct ObjFileData *pObj;

  pObj = AtItem(&ObjFilesCollection, nObjModule);

  if (pObj->nModuleName != -1)
  {
    ASSERT(pObj->szLibName[0] != '\0');
    sprintf(szModuleName, "%s (%s)", GetModuleName(pObj->nModuleName), pObj->szLibName);
  }
  else
  {
    ASSERT(pObj->szLibName[0] == '\0');
    ASSERT(pObj->nObjPos == 0);
    strcpy(szModuleName, pObj->szObjName);
  }
}

/* ************************************************************************
   Function: GenerateMap
   Description:
     Generates map file.
*/
void GenerateMap(void)
{
  struct PublicDefData *p;
  int i;
  struct ObjFileData *pObj;

  if ((OutFile = fopen(szMapFileName, "wb")) == NULL)
    Fatal("Can't open map file (%s)", szMapFileName);
  TRACE1("Map file (%s) sucessfully created\n", szMapFileName);

  fprintf(OutFile,
	  "SEGMENT LOCATIONS\n\n"
          "cseg xseg iseg bseg dseg module\n"
          "-------------------------------\n");

  for (i = 0; i < ObjFilesCollection.cnt; i++)
  {
    ComposeObjModuleName(i);
    pObj = AtItem(&ObjFilesCollection, i);

    fprintf(OutFile, "%04lx %04lx %02x   %02x   %02x   %s\n",
      pObj->CSegOffset, pObj->XSegOffset, pObj->ISegOffset, pObj->BSegOffset,
      dchunks[pObj->DChunkIndex].offset, szModuleName);
  }

  fprintf(OutFile, "\n\nSYMBOLS BY NAME\n\n"
	  "symbol                    segment loc   module\n"
	  "------------------------------------------------\n");

  for (i = 0; i < PublicsCollection.cnt; i++)
  {
    p = AtItem(&PublicsCollection, i);
    ComposeObjModuleName(p->nObjFile);

    fprintf(OutFile, "%-25s %-7s %04x  %s\n",
      p->psSymbolName, segnames[p->nSegment], p->offset, szModuleName);
  }

  fclose(OutFile);
  TRACE1("Closing the map file %s\n", szMapFileName);
  OutFile = NULL;
}

