#include "global.h"
#pragma hdrstop
#include "a51.h"
#include "a_tab.h"

TSegment   Segments[6];
UINT       CurSegment;
TSegment  _DSPTR * pCurSegment;

UINT OpCount;
UINT Using;     // using register bank XX
TExpr Op[3];
BOOL bGenerate; // TRUE for Pass2
// specifies whether we need to calculate the expression on this pass
BOOL bNeedConst; 

TExpr EmptyExpr = { NULL };

const char * pszSegmentNames[] =
{
  "noseg",
  "CSEG",
  "ISEG",
  "BSEG",
  "XSEG",
  "DSEG",
};


///////////////////////////////////////////////////////////////////////////
// Local functions

//--------------------------------------------------------------------------
// Name         SegReset
//
// Description  Reset the current address for all segments
//--------------------------------------------------------------------------
static void SegReset ( void )
{
  int i;
  for( i = 0; i < _countof( Segments ); ++i )
    Segments[i].curAddress = 0;
};

static BOOL CheckAddressFor ( UINT seg, ULONG addr )
{
  if (seg != O51_NOSEG && addr > (ULONG)Segments[seg].maxByte + 1)
  {
    PrintError1( ERR_INV_SEG_ADDR );
    return FALSE;
  }
  else
    return TRUE;
};

static BOOL CheckAddress ( ULONG addr )
{
  return CheckAddressFor( CurSegment, addr );
};

//--------------------------------------------------------------------------
// Name         GetConstValue
//
// Description  Simply check if we know the value of an expression, generate
//              error if we don't, and return the value.
//--------------------------------------------------------------------------
BOOL GetConstValue ( const TExpr * e, UINT * res )
{
  if (e->rType != RELOC_CONST)
  {
    // print the proper error message
    if (e->rType != RELOC_UNDEF)
    {
      if (e->rIdent)
        PrintError( ERR_NO_RELOC, e->rIdent->szName );
      else
        PrintError( ERR_RELOC_EXPR );
    }
  }
  else
  {
    *res = e->uValue & 0xFFFF;
    return TRUE;
  }
  return FALSE;
};

//--------------------------------------------------------------------------
// Name         GetBitValue
//
// Description  Same as GetConstValue but check if it is in range 0 - 7
//--------------------------------------------------------------------------
static BOOL GetBitValue ( const TExpr * e, UINT * res )
{
  if (GetConstValue( e, res ))
  {
    if (*res > 7)
    {
      PrintError( ERR_INV_BIT_NUM );
      *res = 0;
    }
    else
      return TRUE;
  }
  return FALSE;
};

//--------------------------------------------------------------------------
// Name         CheckSegment
//
// Description  Checks if we are in a segment and generates an error message
//              if we are not
//--------------------------------------------------------------------------
BOOL CheckSegment ( void )
{
  if (CurSegment == O51_NOSEG)
  {
    PrintError1( ERR_NO_SEG );
    return FALSE;
  }
  else
    return TRUE;
};

//--------------------------------------------------------------------------
// Name         BindObj
//
// Description  If we are in a code segment sets the object position to the
//              current address
//--------------------------------------------------------------------------
static void BindObj ( void )
{
  if (bMakeObj && CurSegment == O51_CSEG)
    Obj_StartAt( pCurSegment->curAddress );
};


#ifdef A_DEBUG
static void DumpExpr ( const TExpr * e )
{
  static const char * relocs[] =
  {
    "",
    "Const ",
    "Frame ",
    "Extern",
  };

  if (e->rIdent)
    printf( "%s+", e->rIdent->szName );
  printf( "%04Xh %s %s", e->uValue, pszSegmentNames[e->seg], relocs[e->rType] );
};

static void DumpSymbol ( const TIdent * pIdent )
{
  printf( "%-16s\t%c ", pIdent->szName, pIdent->isPublic ? 'P' : ' ' );
  DumpExpr( &pIdent->e );
  printf( " code(%d) %s(%u)\n",
          pIdent->code,
          pIdent->defFile ? pIdent->defFile->name : "",
          pIdent->defFile ? pIdent->defLine : 0 );
};

static void DumpSymbols ( void )
{
  TIdent * pIdent;

  for( pIdent = pFirstIdent; pIdent; pIdent = pIdent->pNextIdent )
  {
    if (pIdent->code == SYM_SYMBOL)
      DumpSymbol( pIdent );
  }
};
#endif

///////////////////////////////////////////////////////////////////////////
// Global functions

void Asm_Init ( void )
{
  memset( Segments, 0, sizeof( Segments ) );

  Segments[O51_NOSEG].maxByte = 0xFFFF;
  Segments[O51_CSEG].maxByte = 0xFFFF;
  Segments[O51_ISEG].maxByte = 0xFF;
  Segments[O51_BSEG].maxByte = 0xFF;
  Segments[O51_XSEG].maxByte = 0xFFFF;
  Segments[O51_DSEG].maxByte = 0xFF;

  CurSegment = O51_NOSEG;
  pCurSegment = NULL;
  Using = 0;
};

void Asm_Done ( void )
{
};

static void PrintSegmentStatistics ( void )
{
  int i;

  fprintf( ListFile, "\r\n\r\nSegment\t\tsize\r\n-----------------------------\r\n" );
  for ( i = 1; i < 6; ++i )
    fprintf( ListFile, "%s\t\t%5u (%04Xh)\r\n",
             pszSegmentNames[i], Segments[i].size, Segments[i].size );
};

static void PrintSymbol ( const TIdent * pIdent, UINT maxNameLen )
{
  static const char * segNames[] =
  {
    "",
    "code",
    "idata",
    "bit",
    "xdata",
    "data",
  };

  fprintf( ListFile, "%-*s  %04Xh %-5s  %s: %u\r\n",
          maxNameLen, 
          pIdent->szName, pIdent->e.uValue, segNames[pIdent->e.seg],
          pIdent->defFile->name , pIdent->defLine );
};

static int CDECL CmpXrefs ( const void * _a, const void * _b )
{
  TXrefItem * a = *(TXrefItem **)_a;
  TXrefItem * b = *(TXrefItem **)_b;
  int res;

  if (a->file != b->file)
    return strcmp( a->file->name, b->file->name );

  if (a->line < b->line)
    return -1;
  if (a->line > b->line)
    return 1;
  return 0;
};

static void PrintSymbolWithXref ( const TIdent * pIdent, UINT maxNameLen )
{
  TXrefItem * * xrefs;  // array of all xref-s for an ident
  TXrefItem * xref;
  UINT count, i;
  char buf[128];
  int len;
  BOOL haveName;
  static UINT maxFileLen = 0;
  const  lineLen = 72;

  // The first time find longest file name
  if (maxFileLen == 0)
  {
    TFileName * file;

    maxFileLen = 8; // minimal length for a file name
    for ( file = (TFileName *)FileNames.Flink;
          !EndOfList( &FileNames, file );
          file = (TFileName *)file->link.Flink )
    {
      if ((len = strlen( file->name )) > maxFileLen)
        maxFileLen = len;
    }

    if (maxFileLen > 32)
      maxFileLen = 32;
  }

  // count the entries
  count = 0;
  for ( xref = pIdent->lastXref; xref; xref = xref->prev )
    ++count;

  xrefs = xmalloc( sizeof( TXrefItem * ) * count );

  // copy the xrefs
  i = 0;
  for ( xref = pIdent->lastXref; xref; xref = xref->prev )
    xrefs[i++] = xref;

  // sort them
  qsort( xrefs, count, sizeof( xrefs[0] ), CmpXrefs );

  // Print the name of the symbol
  fprintf( ListFile, "%-*s  ", maxNameLen, pIdent->szName );

  i = 0;
  xref = xrefs[i];

  while (i < count)
  {
    if (i)   // if the is not the first file for this symbol
      fprintf( ListFile, "%*s  ", maxNameLen, "" );  // fill with empty spaces

    if (xref->file == pIdent->defFile)
      fprintf( ListFile, "#%-*s  ", maxFileLen, xref->file->name );
    else
      fprintf( ListFile, "%-*s   ", maxFileLen, xref->file->name );

    len = 0;
    haveName = TRUE;
    do
    {
      if (len)
      {
        if (len >= lineLen - maxNameLen - maxFileLen - 5)
        {
          buf[len] = 0;
          fprintf( ListFile, "%*s%s\r\n",
                   haveName ? 0 : maxNameLen + 2 + maxFileLen + 3, "", buf );
          len = 0;
          haveName = FALSE;
        }
        else
          buf[len++] = ' ';
      }
      else
        if (!haveName)
          fprintf( ListFile, "%*s", maxNameLen + 2 + maxFileLen + 3, "" );

      if (xref->file == pIdent->defFile && xref->line == pIdent->defLine)
        buf[len++] = '#';
      ltoa( xref->line, buf + len, 10 );
      len += strlen( buf + len );

      if (++i == count)
        break;
      xref = xrefs[i];
    }
    while (xref->file == xrefs[i-1]->file);

    buf[len] = 0;
    fprintf( ListFile, "%*s%s\r\n", haveName ? 0 : maxNameLen + 2 + maxFileLen + 3,
             "", buf );
  }

  xfree( xrefs );
};


//--------------------------------------------------------------------------
// Name         CmpIdentsByValue
//
// Description  Compare two symbols by segment and then by address
//--------------------------------------------------------------------------
static int CDECL CmpIdentsByValue ( const void * _a, const void * _b )
{
  const TIdent * a = *((const TIdent * const *)_a);
  const TIdent * b = *((const TIdent * const *)_b);

  // first compare the segments
  if (a->e.seg < b->e.seg)
    return -1;
  if (a->e.seg > b->e.seg)
    return 1;

  // For the same segment compare the values
  if (a->e.uValue < b->e.uValue)
    return -1;
  if (a->e.uValue > b->e.uValue)
    return 1;
  return 0;
};

static int CDECL CmpIdentsByName ( const void * a, const void * b )
{
  return strcmp( (*(TIdent **)a)->szName, (*(TIdent **)b)->szName );
};

static void PrintLstSymbols ( void )
{
  UINT count;  // total number of symbols to be printed
  UINT i;
  UINT maxNameLen;  // Longest symbol len
  TIdent * pIdent;
  TIdent * * symbols;

  // count the symbols

  count = 0;
  for( pIdent = pFirstIdent; pIdent; pIdent = pIdent->pNextIdent )
  {
    if (pIdent->code == SYM_SYMBOL && pIdent->e.rType != RELOC_UNDEF)
      ++count;
  }

  if (count > (UINT_MAX / sizeof( TIdent *)) - 16 )
  {
    PrintErrorStr( ERR_WARN0, "Not enough memory to print the symbol table" );
    return;
  }

  if (count == 0)
    return;

  // Allocate a buffer to sort them
  symbols = xmalloc( sizeof( TIdent * ) * count );

  // move the pointers to the buffer
  i = 0;
  maxNameLen = 8;  // minimal length of a ident name
  for( pIdent = pFirstIdent; pIdent; pIdent = pIdent->pNextIdent )
  {
    if (pIdent->code == SYM_SYMBOL && pIdent->e.rType != RELOC_UNDEF)
    {
      UINT len;

      symbols[i++] = pIdent;
      if ((len = strlen( pIdent->szName )) > maxNameLen)
        maxNameLen = len;
    }
  }

  if (maxNameLen > 32) // too long idents will not look very nice...
    maxNameLen = 32;

  if (bMakeLST)
  {
    UINT lastSeg = 0xFFFF;

    // Sort the symbols by value
    qsort( symbols, count, sizeof( symbols[0] ), CmpIdentsByValue );

    for ( i = 0; i < count; ++i )
    {
      // if a new segment : print the header
      if (symbols[i]->e.seg != lastSeg)
      {
        lastSeg = symbols[i]->e.seg;

        fprintf( ListFile, "\r\n\r\nSymbols in %s\r\n", pszSegmentNames[lastSeg] );
        fprintf( ListFile, "\r\n%-*s  %04s\r\n",
                maxNameLen, "Name", "Value" );
        fprintf( ListFile, "---------------------------------------------------------------------\r\n" );
      }

      fprintf( ListFile, "%-*s  %04Xh\r\n",
              maxNameLen, 
              symbols[i]->szName, symbols[i]->e.uValue );
    }
  }

  // sort the symbols by name
  qsort( symbols, count, sizeof( symbols[0] ), CmpIdentsByName );

  if (bMakeLST)
  {
    fprintf( ListFile, "\r\n\r\nList of all symbols by name\r\n" );
    fprintf( ListFile, "\r\n%-*s  %04s %-5s  %-12s\r\n",
            maxNameLen, "Name", "Value", "Type", "File" );
    fprintf( ListFile, "---------------------------------------------------------------------\r\n" );

    // print the symbols
    for ( i = 0; i < count; ++i )
      PrintSymbol( symbols[i], maxNameLen );
    fprintf( ListFile, "\r\n\r\n" );
  }

  if (bMakeXref)
  {
    BOOL hasTitle;  // TRUE after we put the title to the warnings

    if (count)
      fprintf( ListFile, "Cross reference of all symbols sorted by name\r\n\r\n" );

    for ( i = 0; i < count; ++i )
      PrintSymbolWithXref( symbols[i], maxNameLen );
    fprintf( ListFile, "\r\n\r\n" );

    // Print the unused symbols
    hasTitle = FALSE;
    for ( i = 0; i < count; ++i )
    {
      TXrefItem * xref = symbols[i]->lastXref;

      // check if the symbol has only one reference and it is where the
      // symbol has been defined

      if (xref->prev == NULL && xref->file == symbols[i]->defFile &&
          xref->line == symbols[i]->defLine)
      {
        if (!hasTitle)
        {
          fprintf( ListFile, "** Warning: Following symbols were defined but never used:\r\n" );
          hasTitle = TRUE;
          fprintf( ListFile, "\r\n%-*s  %04s %-5s  %-12s\r\n",
                  maxNameLen, "Name", "Value", "Type", "File" );
          fprintf( ListFile, "---------------------------------------------------------------------\r\n" );
        }
        PrintSymbol( symbols[i], maxNameLen );
      }
    }

    if (hasTitle)
      fprintf( ListFile, "\r\n\r\n" );
  }

  xfree( symbols );
};

void Pass1 ( void )
{
  bGenerate = FALSE;
  StartSeg( O51_CSEG ); // we are in CSEG by default
  yyparse();

#ifdef A_DEBUG
  if (bDumpSymb)
    DumpSymbols();
#endif
};

void Pass2 ( void )
{
#if YYDEBUG
  yydebug = FALSE;      // no need for the second pass
#endif
  SegReset();
  StartSeg( O51_CSEG ); // we are in CSEG by default
  Obj_OpenFile();
  Obj_PutNames();
  Obj_PutSegments();
  Obj_PutExterns();
  Obj_PutPublics();
  
  Scan_Reset();
  bGenerate = TRUE;
  yyparse();

  if (bMakeLST)
    PrintSegmentStatistics();
  if (bMakeLST || bMakeXref)
    PrintLstSymbols();

  Obj_CloseFile();
};

void DefineSymbol ( TIdent * pIdent, BYTE rlcType, UINT offset, UINT seg,
                    TIdent * rIdent, TFileData * pData )
{
  pIdent->code        = SYM_SYMBOL;
  pIdent->e.rType     = rlcType;
  pIdent->e.uValue    = offset;
  pIdent->e.seg       = seg;
  pIdent->e.rIdent    = rIdent;

  if (pData && !pIdent->defFile)
  {
    pIdent->defLine = pData->CurLine;
    pIdent->defFile = pData->pName;
  }
};

///////////////////////////////////////////////////////////////////////////
// Functions called from ASM.Y

//--------------------------------------------------------------------------
// Name         MakeIdent
//
// Description  Make an expression of an ident
//--------------------------------------------------------------------------
void MakeIdent ( TExpr * res, const TIdent * pIdent )
{
  if (bGenerate && pIdent->e.rType == RELOC_UNDEF)
    PrintError( ERR_NOT_DEFINED, pIdent->szName );
  *res = pIdent->e;
   res->rIdent = pIdent->e.rIdent ? pIdent->e.rIdent : (TIdent *)pIdent;
};

//--------------------------------------------------------------------------
// Name         GetCurPos
//
// Description  Return the current offset in the current segment
//--------------------------------------------------------------------------
UINT GetCurPos ( void )
{
  return pCurSegment ? pCurSegment->curAddress : 0;
};

//--------------------------------------------------------------------------
// Name         LOGICAL
//
// Description  Convert '0' and '!= 0' to 0 and 0xFFFF (to make it compliant)
//--------------------------------------------------------------------------
static UINT LOGICAL ( UINT x )
{
  return x ? 0xFFFF : 0;
};

//--------------------------------------------------------------------------
// Name         Expr
//
// Description
//--------------------------------------------------------------------------
void Expr ( TExpr * res, const TExpr * e1, const TExpr * e2, int op )
{
  UINT  val1, val2;

  ASSERT( res );
  ASSERT( e1 );

  *res = EmptyExpr;
  res->rType = RELOC_CONST;

  // If one of the operands is not defined => do not calculate
  // (we transfer the identifier to generate more reasonable errors)
  if (e1 && e1->rType == RELOC_UNDEF)
  {
    res->rIdent = e1->rIdent;
    res->rType = RELOC_UNDEF;
    return;
  }
  if (e2 && e2->rType == RELOC_UNDEF)
  {
    res->rIdent = e2->rIdent;
    res->rType = RELOC_UNDEF;
    return;
  }

  switch (op)
  {
//-------------------------------------------------------------------------
    case LEX_HIGH: 
      res->seg = e1->seg;
      if (e1->lo)
      {
        PrintWarning( WARN_HIGH_LOW );
        res->rType = RELOC_CONST;
      }
      else
      if (bNeedConst || IS_CONST( e1 ))
      {
        if (GetConstValue( e1, &val1 ))
        {
          res->uValue = val1 >> 8;
          res->rType = RELOC_CONST;
        }
      }
      else
      {
        res->hi = TRUE;
        res->rIdent = e1->rIdent;
        res->uValue = e1->uValue;
        res->rType = e1->rType;
      }
      break;

//-------------------------------------------------------------------------
    case LEX_LOW:
      res->seg = e1->seg;
      if (e1->hi)
      {
        PrintWarning( WARN_LOW_HIGH );
        res->rType = RELOC_CONST;
      }
      else
      if (bNeedConst || IS_CONST( e1 ))
      {
        if (GetConstValue( e1, &val1 ))
        {
          res->uValue = val1 & 0xFF;
          res->rType = RELOC_CONST;
        }
      }
      else
      {
        res->lo = TRUE;
        res->rIdent = e1->rIdent;
        res->uValue = e1->uValue;
        res->rType = e1->rType;
      }
      break;

//-------------------------------------------------------------------------
#define UN_OP( expr )                                      \
        res->seg = e1->seg;                                \
        if (GetConstValue( e1, &val1 ))                    \
        {                                                  \
          res->uValue = (expr) & 0xFFFF;                   \
          res->rType = RELOC_CONST;                        \
        }                                                  \
        break

      case LEX_NOT:
        UN_OP( ~val1 );
      case UMINUS:
        UN_OP( -val1 );

#undef UN_OP
//-------------------------------------------------------------------------
      case '.':
        Expr( res, e1, e2, '+' );
        res->seg = O51_BSEG;
        return;

//-------------------------------------------------------------------------
#define DIV_OP( op )        \
        ASSERT( e2 );       \
        if (GetConstValue( e1, &val1 ) && GetConstValue( e2, &val2 ))       \
        {                                                                    \
          if (val2 == 0)                                                     \
            PrintError( ERR_DIV_ZERO );                                      \
          else                                                               \
          {                                                                  \
            res->uValue = val1 op val2;                                            \
            res->rType = RELOC_CONST;                                        \
          }                                                                  \
        }                                                                    \
        break;

      case '/':
        DIV_OP( / );
      case LEX_MOD:
        DIV_OP( % );

#undef DIV_OP
//-------------------------------------------------------------------------
#define ARITM_OP( op )  \
        if (GetConstValue( e1, &val1 ) && GetConstValue( e2, &val2 ))        \
        {                                                                    \
          res->uValue = (val1 op val2) & 0xFFFF;                             \
          res->rType = RELOC_CONST;                                          \
        };                                                                   \
        break;

      case '*':
        ARITM_OP( * );
      case LEX_SHL:
        ARITM_OP( << );
      case LEX_SHR:
        ARITM_OP( >> );
      case LEX_OR:
        ARITM_OP( | );
      case LEX_XOR:
        ARITM_OP( ^ );
      case LEX_AND:
        ARITM_OP( & );
//-------------------------------------------------------------------------
#define CMPOP( op )                                                     \
  ASSERT( e2 );                                                         \
  if (e1->rType != e2->rType || e1->rType == RELOC_EXTERN)            \
    PrintError( ERR_INV_RELOC_EXPR );                                 \
  else                                                                \
  if (e1->rType == RELOC_CONST)                                       \
  {                                                                   \
    res->uValue = LOGICAL( e1->uValue op e2->uValue );                \
    res->rType = RELOC_CONST;                                         \
  }                                                                   \
  else                                                                \
  {                                                                   \
    ASSERT( e1->rType == RELOC_FRAME );                               \
    if (e1->seg != e2->seg)                                           \
      PrintError( ERR_INV_RELOC_EXPR );                               \
    else                                                              \
    {                                                                 \
      res->uValue = LOGICAL( e1->uValue op e2->uValue );              \
      res->rType = RELOC_CONST;                                       \
    }                                                                 \
  }                                                                   \
  break

      case '=':
        CMPOP( == );
      case LEX_NE:
        CMPOP( != );
      case '<':
        CMPOP( < );
      case LEX_LE:
        CMPOP( <= );
      case '>':
        CMPOP( > );
      case LEX_GE:
        CMPOP( >= );

#undef CMPOP
//-------------------------------------------------------------------------
      case '+':
        ASSERT( e2 );
        // if only one operand has a segment type => assign it to the result
        if (e1->seg == O51_NOSEG)
          res->seg = e2->seg;
        else
        if (e2->seg == O51_NOSEG)
          res->seg = e1->seg;

        res->uValue = e1->uValue + e2->uValue & 0xFFFF;

        if (e1->lo || e1->hi || e2->lo || e2->hi)
          PrintError( ERR_INV_RELOC_EXPR );
        else
        if (e1->rType != RELOC_CONST && e2->rType != RELOC_CONST)
          PrintError( ERR_ADD_RELOC );
        else
        if (e1->rType == RELOC_CONST)
        {
          res->rIdent = e2->rIdent;
          res->rType  = e2->rType;
        }
        else
        {
          res->rIdent = e1->rIdent;
          res->rType  = e1->rType;
        }
        break;

//-------------------------------------------------------------------------
      case '-':
        ASSERT( e2 );
        // if only one operand has a segment type => assign it to the result
        if (e1->seg == O51_NOSEG)
          res->seg = e2->seg;
        else
        if (e2->seg == O51_NOSEG)
          res->seg = e1->seg;

        if (e1->lo || e1->hi || e2->lo || e2->hi)
          PrintError( ERR_INV_RELOC_EXPR );
        else
        {
          res->uValue = e1->uValue - e2->uValue & 0xFFFF;

          if (e2->rType == RELOC_EXTERN)
            PrintError( ERR_SUB_EXTERN );  // can't subtract extern
          else
          switch ((e1->rType << 4) + e2->rType)
          {
            case (RELOC_CONST << 4) + RELOC_FRAME:
              PrintWarning( WARN_SUB_CONST_REL );
              res->rType = RELOC_CONST;
              break;

            case (RELOC_EXTERN << 4) + RELOC_CONST:
            case (RELOC_FRAME << 4)  + RELOC_CONST:
              res->rType = e1->rType;
              res->rIdent = e1->rIdent;
              break;

            case (RELOC_FRAME << 4)  + RELOC_FRAME:
              if (e1->seg != e2->seg)
                PrintError( ERR_SUB_DIFF_SEG );
              else
                res->rType = RELOC_CONST;
              break;

            case (RELOC_CONST << 4)  + RELOC_CONST:
              res->rType = RELOC_CONST;
              break;

            default:
              PrintError( ERR_SUB_RELOC );
              break;
          }
        }
        break;

      default:
        ASSERT( 0 );
  }
};

//--------------------------------------------------------------------------
// Name         UseExpr
//
// Description  Marks the ident in the expression as used
//--------------------------------------------------------------------------
static void UseExpr ( const TExpr * expr )
{
  if (expr->rIdent)
    expr->rIdent->bUsed = TRUE;
};

void PutOp ( TExpr * expr )
{
  UseExpr( expr );

  if (OpCount < 3)
    Op[OpCount] = *expr;

  if (OpCount == 3)
    PrintError( ERR_MANY_OPS );
  ++OpCount;
};

void Compile ( UINT iCode )
{
  if (CurSegment != O51_CSEG)
    PrintError( ERR_NEED_CSEG );
  else
  {
    // fill the rest of operands with OP_EMPTY
    UINT i;
    for( i = OpCount; i < 3; ++i )
      Op[i].addr = OP_EMPTY;
    CompileCmd( iCode - SYM_FIRST_INSTR );
  }
};

//--------------------------------------------------------------------------
// Name         Advance
//
// Description  Advamces the current position int the segment _len_ bytes
//--------------------------------------------------------------------------
void Advance ( UINT len )
{
  ULONG res;

  if (!CheckSegment())
    return;

  res = (ULONG)pCurSegment->curAddress + len;
  if (CheckAddress( res ))
  {
    pCurSegment->curAddress = res;
    if (pCurSegment->curAddress > pCurSegment->size)
      pCurSegment->size = pCurSegment->curAddress;
  }
};

//--------------------------------------------------------------------------
// Name         UseSpace
//
// Description  Like Advance() but also generates in the object file
//--------------------------------------------------------------------------
void UseSpace ( UINT size )
{
  if (size)
  {
    Advance( size );
    if (bMakeObj && CurSegment == O51_CSEG)
      Obj_UseSpace( size );
  }
};

void StartSeg ( UINT seg )
{
  ASSERT( seg < _countof( Segments ) && seg != 0 );
  CurSegment = seg;
  pCurSegment = Segments + seg;
  BindObj();
};

void StartSegAt ( UINT seg, UINT at )
{
  StartSeg( seg );
  if (CheckAddress( at ))
  {
    pCurSegment->curAddress = at;
    BindObj();
  }
};

void Org ( const TExpr * e )
{
  if (!CheckSegment())
    return;

  if (e->rType == RELOC_EXTERN)
    PrintError( ERR_RELOC_EXPR );
  else
  if (e->rType == RELOC_FRAME && e->seg != CurSegment)
    PrintError( ERR_RELOC_EXPR );
  else
    StartSegAt( CurSegment, e->uValue );
};

static UINT CombineSegs ( UINT s1, UINT s2 )
{
  return s1 ? s1 : s1 | s2;
};

/////////////////////////////////////////////////////////////////////////////
// Define various symbols during pass 1
// Just exit during pass 2

void Public ( TIdent * pIdent )
{
  if (bGenerate)        // only for Pass 1
    return;

  if (pIdent->e.rType == RELOC_EXTERN)
    PrintError1( ERR_PUB_EXT_CONFL, pIdent->szName );
  else
    pIdent->isPublic = TRUE;
};

void Extern ( TIdent * pIdent, UINT seg )
{
  if (bGenerate)        // only for Pass 1
    return;

  if (pIdent->e.rType != RELOC_UNDEF)
    PrintError1( ERR_SYM_REDEF, pIdent->szName );
  else
    DefineSymbol( pIdent, RELOC_EXTERN, 0, seg, NULL, IncludeTop );
};

//--------------------------------------------------------------------------
// Name         Define
//
// Description  Define a symbol equate in segment seg.
//              seg & 0x8000 means SET
//--------------------------------------------------------------------------
void Define ( TIdent * pIdent, UINT segment, const TExpr * expr )
{
  BYTE relocType;
  TExpr e;

  if (bGenerate)        // only for Pass 1
    return;

  ASSERT( expr->addr == OP_EMPTY );

  if (expr->rType == RELOC_UNDEF)
  {
    if (expr->rIdent)
      PrintError1( ERR_UNKNOWN_VALUE, expr->rIdent->szName );
    else
      PrintError1( ERR_UNKNOWN_VALUE, pIdent->szName );
    // Though the symbols is undefined we define it to avoid further errors
    e = EmptyExpr;
    e.rType = RELOC_CONST;
    expr = &e;
  }

  if ((segment & 0x8000) == 0 && pIdent->e.rType != RELOC_UNDEF)
  {
    PrintError1( ERR_SYM_REDEF, pIdent->szName );
    return;
  }

  segment &= ~0x8000u;
  if (expr->seg != segment && expr->seg != O51_NOSEG && segment != O51_NOSEG)
    PrintWarning1( WARN_SEG_CONFL );

  segment = CombineSegs( segment, expr->seg );

  if (expr->rType == RELOC_CONST && segment != O51_NOSEG)
    relocType = RELOC_FRAME;
  else
    relocType = expr->rType;

  if (relocType == RELOC_CONST && !CheckAddressFor( segment, expr->uValue ))
     return;

  DefineSymbol( pIdent, relocType, expr->uValue, segment,
                relocType == RELOC_EXTERN ? expr->rIdent : NULL,
                IncludeTop );
};

void PutLabel ( TIdent * pIdent )
{
  if (bGenerate)        // only for Pass 1
    return;

  if (!CheckSegment())
    return;
  if (pIdent->e.rType != RELOC_UNDEF)
    PrintError1( ERR_SYM_REDEF, pIdent->szName );
  else
    DefineSymbol( pIdent, RELOC_FRAME, pCurSegment->curAddress, CurSegment,
                  NULL, IncludeTop );
};

void GenDBString ( void )
{
  if (CurSegment == O51_CSEG)
  {
    if (bGenerate)
    {
      // put it in the object file
      Obj_Reserve( LexStringLen );
      Obj_PutCodeBytes( LexString, LexStringLen );
    }  
    Advance( LexStringLen );       // move the current address
  }
  // else we've already generated an error
};

void GenDB ( const TExpr * expr )
{
  UseExpr( expr );
  if (CurSegment == O51_CSEG)
  {
    if (bGenerate)
    {
      BYTE b = (BYTE)expr->uValue;

      // put it in the object file
      Obj_Reserve( 1 );
      GenFixUp( 0, expr, FIXUP_BYTELO );
      Obj_PutCodeBytes( &b, 1 );
    }  
    Advance( 1 );       // move the current address
  }
  // else we've already generated an error
};

void GenDW ( const TExpr * expr )
{
  UseExpr( expr );
  if (CurSegment == O51_CSEG)
  {
    if (bGenerate)
    {
      BYTE b[2];

      b[0] = (BYTE)(expr->uValue >> 8);
      b[1] = (BYTE)expr->uValue;

      // put it in the object file
      Obj_Reserve( 2 );
      GenFixUp( 0, expr, FIXUP_WORD );
      Obj_PutCodeBytes( b, 2 );
    }  
    Advance( 2 );       // move the current address
  }
  // else we've already generated an error
};
