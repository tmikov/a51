// ident codes

#define SYM_SYMBOL        0
#define SYM_SFR           1
// from 32 to 127 inclusive are the character codes
#define SYM_FIRST_INSTR 128
#define SYM_LAST_INSTR  255  // up to .. : instruction codes
#define SYM_LAST_RES    511  // up to .. : reserved words codes


#define RELOC_UNDEF     0
#define RELOC_CONST     1
#define RELOC_FRAME     2
#define RELOC_EXTERN    3

struct TIdent;

typedef struct TExpr
{
  struct TIdent * rIdent;   // relocatable identifier
  UINT     uValue;      
  UINT     addr : 4;        // addressation
  UINT     seg  : 3;
  UINT     lo   : 1;
  UINT     hi   : 1;
  UINT     rType : 2;
} TExpr;

#define IS_CONST( e )   ((e)->rType == RELOC_CONST)

typedef struct TIdent
{
  char * szName;
  UINT   HashVal;

  UINT   code;           // code: code of cmd, directive, etc.

  TExpr  e;              // the value of the symbol
  UINT   isPublic : 1;   // a public symbol
  UINT   bNoCase  : 1;   // TRUE if the ident is always case insensitive
  UINT   bUsed    : 1;   // TRUE if the ident has been REALLY used

  UINT   nameIndex;      // index of the name if it extern or public
  UINT   externIndex;    // index in the extern table

  UINT   defLine;        // line where the ident was first defined
  TFileName * defFile;   // file where the ident was last defined
  TXrefItem * lastXref;

  struct TIdent * pNextHash;
  struct TIdent * pNextIdent;
} TIdent;

extern TIdent * pLexSymbol;

void Ident_Init ( void );
void Ident_Done ( void );
int IdentGetLex ( BOOL process );
void AfterPrimaryControls ( void );
