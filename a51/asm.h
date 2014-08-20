typedef struct TSegment
{
  UINT curAddress;
  UINT size;
  UINT maxByte;
} TSegment;

extern TSegment   Segments[6];
extern UINT       CurSegment;
extern TSegment _DSPTR * pCurSegment;

extern UINT OpCount;
extern UINT Using;   
extern TExpr Op[3];

extern TExpr EmptyExpr;
extern BOOL  bGenerate;
extern BOOL  bNeedConst;

void Asm_Init ( void );
void Asm_Done ( void );

void DefineSymbol ( TIdent * pIdent, BYTE rlcType, UINT offset, UINT seg,
                    TIdent * rIdent, TFileData * pData );
void PutOp ( TExpr * expr );
void Compile ( UINT iCode );

BOOL GetConstValue ( const TExpr * e, UINT * res );
void StartSeg ( UINT seg );
void StartSegAt ( UINT seg, UINT at );
void Org ( const TExpr * e );
void Advance ( UINT len );
void UseSpace ( UINT size );
void PutLabel ( TIdent * pIdent );
void Define ( TIdent * pIdent, UINT seg, const TExpr * expr );
void Extern ( TIdent * pIdent, UINT seg );
void Public ( TIdent * pIdent );

void MakeIdent ( TExpr * res, const TIdent * pIdent );
void Expr ( TExpr * res, const TExpr * e1, const TExpr * e2, int op );
UINT GetCurPos ( void );
void GenDBString ( void );
void GenDB ( const TExpr * expr );
void GenDW ( const TExpr * expr );

void Pass1 ( void );
void Pass2 ( void );


