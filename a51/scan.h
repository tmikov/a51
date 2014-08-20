#define LINE_BUF_SIZE           8192

typedef struct TFileName
{
  LIST_ENTRY link;
  char       name[1];
} TFileName;

typedef struct TFileData
{
  TFileName * pName;
  FILE * File;
  UINT   CurLine;

  UCHAR  LineBuf[LINE_BUF_SIZE];
  UINT   CurPos;
  UINT   BufUsed;

  struct TFileData * Prev;
} TFileData;

extern TFileData * IncludeTop; 
extern UCHAR _DSPTR * LastLineStr;
extern int    InclNest;
extern BOOL   bLstGen;
extern LIST_ENTRY     FileNames;     


