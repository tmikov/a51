/* link51.h */

#define OUTPUT_BIN 1
#define OUTPUT_HEX 2

#include "collect.h"
#include "c_list.h"
#include "common/misc.h"
#include "err.h"
#include "a51/obj51.h"

#define LASTDATALEN 11
#define LASTDATANAME "??LAST_DATA"

/* link51.c */
#ifdef _DEBUG
extern BOOL fDumpFixups;
extern char *segnames[];
#endif

extern BOOL bCaseSensitive;
extern BOOL bSmartDSeg;

/*
nModuleName should be checked to be determined whether this
is an obj file or obj module from a library.
*/
struct ObjFileData
{
  char szObjName[FILENAME_MAX];  /* Full object file name or name in library */
  char szLibName[FILENAME_MAX];  /* Library name if obj is a part of a library */
  long nObjPos;  /* Absolute position in the library (if is a part of a lib) */
  int nModuleName;  /* Index in ModuleNameCollection (-1) not a module */
  /* Offsets for various 8051 segments for this objmodule */
  long CSegOffset;
  long XSegOffset;
  int ISegOffset;
  int BSegOffset;
  int DChunkIndex;  /* Data chunks relative number in dhunks[] */
};

struct LibFileData
{
  char szLibName[FILENAME_MAX];
  BOOL bScheduled;  /* The library is scheduled for linking */
};

extern UINT OutputFile;
extern char szOutputFileName[FILENAME_MAX];
extern char szMapFileName[FILENAME_MAX];
extern BYTE FillValue;
extern struct Collect ObjFilesCollection;  /* of ObjFileData */
extern struct Collect LibFilesCollection;  /* of LibFileData */

struct ObjFileData *AllocObjFileData(const char *szObjName, const char *szLibName,
  long nObjPos, int nModuleName);


/* readrex.c */
extern FILE *InFile;
extern struct TObjBlockHeader hdr;

void Read(void *buf, UINT len);
BYTE ReadByte(void);
WORD ReadWord(void);
DWORD ReadDWord(void);
UINT ReadHdr(void);


/* symbols.c */
struct ExternDefData
{
  /* Don't reorder ! */
  char *psSymbolName;  /* Should be first! */
  int nSegment;
  int nObjFile;  /* Object file index, -1 for library */
  int nLibFile;  /* Lib file index, -1 for object file */
  long nLibPos;  /* Position in a library */
  int nLibModule;  /* Name index in ModuleNamesCollection (if from lib) */
  BOOL bModuleAttached;  /* When the obj module is scheduled for linking */

  int nExternIndex;  /* Index in the present obj module */
  BOOL bResolved;
  struct PublicDefData *pResolved;  /* Public that resolves the extern */
};

struct PublicDefData
{
  /* Don't reorder ! */
  char *psSymbolName;  /* Should be first! */
  int nSegment;
  int nObjFile;  /* Object file index, -1 for library */
  int nLibFile;  /* Lib file index, -1 for object file */
  int nLibPos;  /* Position in a library */
  int nLibModule;  /* Name index in ModuleNamesCollection (if from lib) */
  BOOL bModuleAttached;  /* When the obj module is scheduled for linking */

  WORD offset;
};

extern struct Collect ExternsCollection;
extern struct Collect PublicsCollection;
extern struct Collect ModuleNamesCollection;
extern struct Collect NamesCollection;	/* of strings (used by LoadNames) */

void ReadString(char *pStrBuf);
char *GetModuleName(int nLibModule);
int cmpsymbols(const void *s1, const void *s2);
void ProcessSymbols(void);


/* segments.c */
struct DataChunks
{
  int number;  /* Unique ID */
  int offset;
  int size;
};

extern struct DataChunks dchunks[];  /* 256 separate data chunks maximum */
extern int dchunks_cnt;
extern int LastData;  /* Stack can start from here */

void ProcessSegments(void);
void CalcPublics(void);


/* process.c */

extern char *segnames[];

struct CodeChunks
{
  UINT offset;
  UINT size;
};

/*
CodeChunksCollection is a sorted collection. The key is the offset of
the code chunks.
*/
extern struct Collect CodeChunksCollection;  /* of struct CodeChunks* */

void Link(void);
void FreeSymbols(void);

/* output.c */
extern FILE *OutFile;
void OpenOutput(void);
void GenBin(void);
void GenHex(void);
void DoneOutput(void);

/* map.c */
void GenerateMap(void);

