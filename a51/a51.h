#include "list.h"
#include "misc.h"
#include "scan.h"
#include "xref.h"
#include "a51/obj51.h"
#include "ident.h"
#include "lex.h"
#include "err.h"
#include "asm.h"
#include "genobj.h"
#include "cmdtab.h"

extern int  NumErrors;
extern int  NumWarnings;
extern BOOL bAnyErrors;

extern BOOL bCaseSensitive;
extern int  MaxErrors;
extern int  MaxWarnings;
extern BOOL bMakeObj;
extern BOOL bOptCALL;
extern BOOL bWarnings;
extern BOOL bMakeLST;
extern BOOL bMakeXref;
extern BOOL bNoMod51;

extern char szInputFile[];
extern char szOutputFile[];
extern char szListFile[];
extern char szBareInputName[];

extern FILE * ListFile;
extern int  ListLine; 

extern TCmdDef Instructions[];

// debug flags
#ifdef A_DEBUG
extern BOOL bTraceGen;
extern BOOL bDumpSymb;
#endif

// from IDTAB.C

extern TIdent * pFirstIdent;
extern TIdent * pLastIdent;

void IdentTable_Init ( void );
void IdentTable_Done ( void );
TIdent * AllocTIdent ( UINT len, const UCHAR * name, UINT hash );
void FreeTIdent ( TIdent * pIdent );
TIdent * FindIdent ( const UCHAR * szName, UINT hashVal );
void AddIdent ( TIdent * pIdent );
void RemoveIdent ( TIdent * pIdent );

// from SCAN.C
BOOL AddToIncludePath ( const char * iPath );
void OpenInput ( const char * szFileName );
int  GetLex ( void );
const char * GetCurFileInfo ( int * curLine, UCHAR ** lastLine );
BOOL Scan_Init ( void );
void Scan_Done ( void );
void Scan_Reset ( void );
void GetIncludeName ( void );
void GetToClosingBrace ( void );
void Include ( void );
void DoIf ( BOOL expr );
void DoElse ( void );
void DoEndif ( void );

// from AMAIN.C
void FatalError ( const char * str, ... );

//////// externs from YACC

#if YYDEBUG == 1
extern int yydebug;
#endif

int yyparse ( void );
void yyerror ( char * msg );

