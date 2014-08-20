#include "common/global.h"
#pragma hdrstop
#include "a51.h"

static BYTE codes[4];

#ifdef A_DEBUG
static const char * opStr[] =
{
  "OP_EMPTY",
  "OP_RN", 
  "OP_DIR",
  "OP_IMM", 
  "OP_AT_R0",
  "OP_DPTR",      
  "OP_AB",
  "OP_A",
  "OP_AT_DPTR",   
  "OP_AT_A_DPTR", 
  "OP_AT_A_PC",   
  "OP_NOT_BIT",
  "OP_C",
};

static void PrintInstr ( const char * instr, UINT len )
{
  UINT i;

  printf( "%s(%d), %04X(%d):\t%s\t", IncludeTop->pName->name, IncludeTop->CurLine,
          pCurSegment->curAddress, len, instr );

  for( i = 0; i < OpCount; ++i )
  {
    if (i)
      putchar( ',' );
    printf( opStr[Op[i].addr] );
    if (Op[i].uValue || Op[i].rIdent)
    {
      printf( "(%04Xh", Op[i].uValue );
      if (Op[i].rIdent)
        printf( ",'%s'", Op[i].rIdent->szName );
      putchar( ')' );
    }
  }
  putchar( '\n' );
};

#endif

// include the fast operand table
//--------------------------------------------------------------------------
// Name         FindAddr
//
// Description  Find an the addressing structure of current instruction
//              depending on its operands. Returns NULL if no apropriate
//              addressing structure.
//--------------------------------------------------------------------------
static const TAddrDef _DSPTR * FindAddr ( int cmdIndex )
{
  int i;
  const TAddrDef _DSPTR * addr;
  const _DSPTR TCmdDef * cmd = Instructions + cmdIndex;

  for( addr = cmd->Addr, i = cmd->NumAddr; --i >= 0; ++addr )
    if ( (addr->Op[0] & OP_MASK) == Op[0].addr &&
         (addr->Op[1] & OP_MASK) == Op[1].addr &&
         (addr->Op[2] & OP_MASK) == Op[2].addr )
    {
      return addr;
    }
  return NULL;
};

static void GenDirect ( int offset, UINT uVal, UINT fixType )
{
  switch (fixType)
  {
    case FIXUP_BYTELO:
      codes[offset] = uVal;
      break;

    case FIXUP_WORD:
      codes[offset] = uVal >> 8;
      codes[offset + 1] = uVal;
      break;

    case FIXUP_AJMP:
      codes[offset] |= (uVal >> 3) & 0xE0;
      codes[offset + 1] = uVal;
      break;

    default:
      ASSERT( 0 );
  }
};

void GenFixUp ( int offset, const TExpr * expr, UINT fixType )
{
  UINT fixupSrc;
  UINT srcData;

  if (expr->rType == RELOC_UNDEF)
    return;

  if (expr->rType == RELOC_CONST)
  {
    GenDirect( offset, expr->uValue, fixType );
    return;
  }

#ifdef _DEBUG
  if (expr->rIdent)
    ASSERT( expr->rIdent->e.rType != RELOC_UNDEF );
#endif

  if (expr->rType == RELOC_EXTERN)
  {
    ASSERT( expr->rIdent );
    fixupSrc = FIXUP_SRC_EXTERN;
    srcData  = expr->rIdent->externIndex;
  }
  else
  {
    ASSERT( expr->seg != O51_NOSEG );
    fixupSrc = FIXUP_SRC_FRAME;
    srcData  = expr->seg;
  }

  switch (fixType)
  {
    case FIXUP_BYTELO:
      if (expr->hi)
        fixType = FIXUP_BYTEHI;
      Obj_PutFixUp( offset, fixType, fixupSrc, srcData, expr->uValue );
      break;

    case FIXUP_WORD:
      if (expr->lo)
        Obj_PutFixUp( offset + 1, FIXUP_BYTELO, fixupSrc, srcData, expr->uValue );
      else
      if (expr->hi)
        Obj_PutFixUp( offset + 1, FIXUP_BYTEHI, fixupSrc, srcData, expr->uValue );
      else
        Obj_PutFixUp( offset, FIXUP_WORD, fixupSrc, srcData, expr->uValue );
      break;

    case FIXUP_AJMP:
      if (expr->lo || expr->hi)
        PrintError( ERR_INV_11 );
      else
        Obj_PutFixUp( offset, FIXUP_AJMP, fixupSrc, srcData, expr->uValue );
      break;

    default:
      ASSERT( 0 );
  }
};

//--------------------------------------------------------------------------
// Name         CheckOffset
//
// Description  Check if the target is suitable for a short jump
//--------------------------------------------------------------------------
static BOOL CheckOffset ( const TExpr * e, UINT len )
{
  UINT addrVal;

  if (e->rType == RELOC_EXTERN || e->rType == RELOC_UNDEF)
    return FALSE;
  if (e->rType == RELOC_FRAME && e->seg != CurSegment)
    return FALSE;
  if (e->rType == RELOC_CONST)
    return FALSE;

  // optimize only backward jumps
  if (e->uValue <= pCurSegment->curAddress)
    addrVal = e->uValue - pCurSegment->curAddress - len & 0xFFFF;
  else
    return FALSE; 

  // check if the offset is signed byte
  return (addrVal & 0xFF80) == 0 || (addrVal & 0xFF80) == 0xFF80;
};

//--------------------------------------------------------------------------
// Name         GetOffset
//
// Description  Calc the offset for a short jump
//--------------------------------------------------------------------------
static UINT GetOffset ( const TExpr * e, UINT len )
{
  UINT addrVal;

  if (e->rType == RELOC_EXTERN)
  {
    PrintError( ERR_EXTERN, e->rIdent->szName );
    return FALSE;
  }
  if (e->rType == RELOC_FRAME && e->seg != CurSegment)
  {
    PrintError( ERR_DIFF_SEG, e->rIdent->szName );
    return FALSE;
  }
  if (e->rType == RELOC_CONST)
  {
    PrintError( ERR_CONST, "expression");
    return FALSE;
  }

  addrVal = e->uValue;
  addrVal -= pCurSegment->curAddress + len;

  return addrVal;
};

//--------------------------------------------------------------------------
// Name         GetByte
//
// Description  Returns the lo byte of an expression. Generates an error
//              if the HI BYTE is != 0, 0xFF
//--------------------------------------------------------------------------
static BYTE GetByte ( UINT val )
{
  if ((val & 0xFF00) != 0 && (val & 0xFF00) != 0xFF00)
    PrintWarning( WARN_TRUNC_WORD );
  return val;
};

//--------------------------------------------------------------------------
// Name         CheckAJMP
//
// Description  Check if a target is suitable for a ACALL.
//              Checks only if bOptCALL is TRUE
//--------------------------------------------------------------------------
static BOOL CheckACALL ( const TExpr * e )
{
  if (!bOptCALL)
    return FALSE;

  // We can't have 11 bit addressing of an extern value
  // or a value from different segment
  if (e->rType == RELOC_UNDEF ||
      e->rType == RELOC_EXTERN ||
      e->rType == RELOC_FRAME && e->seg != CurSegment)
    return FALSE;

  // we optimize only backward calls to addresses with HIGH 5 bits the same
  return e->uValue <= pCurSegment->curAddress &&
         (e->uValue & 0xF800) == ((pCurSegment->curAddress + 2) & 0xF800);
};



static void Generate ( const TAddrDef _DSPTR * addr )
{
  UINT i;

  Obj_Reserve( addr->Len );

  memset( codes, 0, sizeof( codes ) );
  codes[0] = addr->OpCode;

  for( i = 0; i < 3; ++i )
  {
    // a fast way to check for valid segment of the operand
    static BYTE validSegs[6][4] =
    {
      // OPSEG_NO  OPSEG_DATA  OPSEG_BIT  OPSEG_CODE
      {   TRUE,     TRUE,       TRUE,      TRUE }, // O51_NOSEG
      {   TRUE,    FALSE,      FALSE,      TRUE }, // O51_CSEG
      {   TRUE,     TRUE,      FALSE,     FALSE }, // O51_ISEG
      {   TRUE,    FALSE,       TRUE,     FALSE }, // O51_BSEG
      {   TRUE,    FALSE,      FALSE,     FALSE }, // O51_XSEG
      {   TRUE,     TRUE,      FALSE,     FALSE }, // O51_DSEG
    };

    if (!validSegs[Op[i].seg][addr->Op[i] >> 4])
      PrintError( ERR_BAD_SEGMENT );

    switch (addr->OpAction[i])
    {
      UINT addrVal;

      case ACTION_NO:
        break;

      case ACTION_RN:
        codes[0] |= Op[i].uValue & 7;
        break;

      case ACTION_AT_R0:
        codes[0] |= Op[i].uValue & 1;
        break;

      case ACTION_VAL8_B2:
        GenFixUp( 1, &Op[i], FIXUP_BYTELO );
        break;
        
      case ACTION_VAL8_B3:
        GenFixUp( 2, &Op[i], FIXUP_BYTELO );
        break;
        
      case ACTION_VAL8_REL_B2:
      case ACTION_VAL8_REL_B3:
        addrVal = GetOffset( Op + i, addr->Len );

        //
        // Do nothing if the target is undefined
        //
        if (Op[i].rType != RELOC_UNDEF)
        {
          if ((addrVal & 0xFF80) != 0 && (addrVal & 0xFF80) != 0xFF80)
            PrintError( ERR_LARGE_OFS );
          else
            codes[addr->OpAction[i] == ACTION_VAL8_REL_B2 ? 1 : 2] = addrVal & 0xFF;
        }
        break;

      case ACTION_VAL16:
        GenFixUp( 1, &Op[i], FIXUP_WORD );
        break;

      case ACTION_VAL11:
        GenFixUp( 0, &Op[i], FIXUP_AJMP );
        break;
    }
  }

  Obj_PutCodeBytes( codes, addr->Len );
};

/////////////////////////////////////////////////////////////////////////
// Listing generation

/*
Format of the listing:

 1205: 2  1111  00 01 02 03     lab1:   jmp     lab1
                                                                                                
*/
static char LstBuf[64];
static const char NL[] = "\r\n";

static void GenLstStart ( void )
{
  ++ListLine;

  bLstGen = TRUE;
  if (InclNest)
    sprintf( LstBuf, "%5d:%2d", ListLine, InclNest );
  else
    sprintf( LstBuf, "%5d:  ", ListLine );
};

static void GenLstInstruction ( UINT len )
{
  UINT i;

  GenLstStart();

  sprintf( LstBuf + 8, "  %04X  ", pCurSegment->curAddress );
  for ( i = 0; i < len; ++i )
    sprintf( LstBuf + 16 + i * 3, "%02X ", codes[i] );

  memset( LstBuf + 16 + i * 3, ' ', (4 - i) * 3 );
  LstBuf[28] = '\t';
  fwrite( LstBuf, 1, 29, ListFile );

  fwrite( LastLineStr, 1, strlen( (char *)LastLineStr ), ListFile );
  fwrite( NL, 1, 2, ListFile );
};

void GenListingLine ( void )
{
  if (bGenerate && bMakeLST)
  {
    GenLstStart();
    LstBuf[8] = '\t';
    LstBuf[9] = '\t';
    LstBuf[10] = '\t';

    fwrite( LstBuf, 1, 11, ListFile );
    fwrite( LastLineStr, 1, strlen( (char *)LastLineStr ), ListFile );
    fwrite( NL, 1, 2, ListFile );
  }
};


//--------------------------------------------------------------------------
// Name         CompileCmd
//
// Description  if !bGenerate we don't care about Op[].uValue.
//              we only care about the addressation to calculate instruction
//              size.
//--------------------------------------------------------------------------
void CompileCmd ( int cmdIndex )
{
  const _DSPTR TAddrDef * addr;
  if ((addr = FindAddr( cmdIndex )) == NULL)
  {
    PrintError( ERR_INV_ADDR_MODE );
    return;
  }

  if (addr->Len == LEN_PATCH)
  {
    switch (addr->OpCode)
    {
      case PATCH_JMP:
        if (CheckOffset( Op, 2 ))
          ++addr;         // optimize to sjmp
        else
        if (CheckACALL( Op ))
          addr += 2;      // optimize to ajmp
        else
          addr += 3;      // make it a ljmp
        break;

      case PATCH_CALL:
        if (CheckACALL( Op ))
          ++addr;         // optimize to ACALL
        else
          addr += 2;      // make it a lcall
        break;

      default:
        ASSERT( 0 );
        return;
    }
  }

  // if in Pass 1 just increase the PC
  if (bGenerate)
  {
#ifdef A_DEBUG
    if (bTraceGen)
      PrintInstr( Instructions[cmdIndex].szName, addr->Len );
#endif
    Generate( addr );
    if (bMakeLST)
      GenLstInstruction( addr->Len );
    Advance( addr->Len );
  }
  else
  {
    UseSpace( addr->Len );
  }
};

