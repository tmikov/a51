#include "global.h"
#pragma hdrstop
#include "a51.h"
#include "a_tab.h"

TIdent * pLexSymbol;

static struct
{
  const char * szName;
  UINT         lexValue;
} ResWords[] =
{
  { "A",     LEX_A },
  { "C",     LEX_C },
  { "DPTR",  LEX_DPTR },
  { "PC",    LEX_PC },
  { "AB",    LEX_AB },
  { "ORG",   LEX_ORG },
  { "USING", LEX_USING },
  { "END",   LEX_END },
  { "AT",    LEX_AT },
  { "CSEG",  LEX_CSEG },
  { "DSEG",  LEX_DSEG },
  { "ISEG",  LEX_ISEG },
  { "BSEG",  LEX_BSEG },   
  { "XSEG",  LEX_XSEG },
  { "EQU",   LEX_EQU },    
  { "SET",   LEX_SET },   
  { "CODE",  LEX_CODE },   
  { "DATA",  LEX_DATA },   
  { "IDATA", LEX_IDATA },	
  { "BIT",   LEX_BIT },   
  { "XDATA", LEX_XDATA },
  { "DB",    LEX_DB },   
  { "DW",    LEX_DW },   
  { "DS",    LEX_DS },
  { "DBIT",  LEX_DBIT },
  { "$NOMOD51",    LEX_NOMOD51 },
  { "$NOPAGING",   LEX_NOPAGING },
  { "$PAGELENGTH", LEX_PAGELENGTH },
  { "$NOSYMBOLS",  LEX_NOSYMBOLS },
  { "$INCLUDE",    LEX_INCLUDE },
  { "$EJECT",      LEX_EJECT },
  { "$NOLIST",     LEX_NOLIST },
  { "$LIST",       LEX_LIST },
  { "$DEBUG",      LEX_DEBUG },
  { "$PAGEWIDTH",  LEX_PAGEWIDTH },
  { "$NOTABS",     LEX_NOTABS },
  { "$XREF",       LEX_XREF },
  { "$TITLE",      LEX_TITLE },
  { "$DATE",       LEX_DATE },
  { "NAME",        LEX_NAME },
  { "OR",          LEX_OR },
  { "XOR",         LEX_XOR },
  { "AND",         LEX_AND },
  { "EQ",          '=' },
  { "NE",          LEX_NE },
  { "LT",          '<' },
  { "LE",          LEX_LE },
  { "GT",          '>' },
  { "GE",          LEX_GE },
  { "SHL",         LEX_SHL },
  { "SHR",         LEX_SHR },
  { "MOD",         LEX_MOD },
  { "NOT",         LEX_NOT },
  { "HIGH",        LEX_HIGH },
  { "LOW",         LEX_LOW },
  { "EXTRN",       LEX_EXTRN },
  { "PUBLIC",      LEX_PUBLIC },
  { "$",           LEX_DOLLAR },
  { "IF",          LEX_IF },
  { "ELSE",        LEX_ELSE },
  { "ENDIF",       LEX_ENDIF },
  { "IFDEF",       LEX_IFDEF },
  { "IFNDEF",      LEX_IFNDEF },
};

#include "cmdtab.inc"
#include "8051.inc"     // definitions for 8051's SFR

//--------------------------------------------------------------------------
// Name         AddSFR8051
//
// Description  Define the standart 8051 SFR-s. (All are case insensitive)
//--------------------------------------------------------------------------
static void AddSFR8051 ( void )
{
  int i;
  for ( i = 0; i < _countof( SFR8051 ); ++i )
  {
    TIdent * pIdent;

    pIdent = AllocTIdent( strlen( SFR8051[i].szName ), (UCHAR *)SFR8051[i].szName,
                          CalcStringHash( (UCHAR *)SFR8051[i].szName ) );
    AddIdent( pIdent );
    DefineSymbol( pIdent, RELOC_CONST, SFR8051[i].offset, SFR8051[i].segment, NULL, NULL );
    pIdent->code = SYM_SFR;
    pIdent->bNoCase = TRUE;
  }
};

//--------------------------------------------------------------------------
// Name         Ident_Init
//
// Description  Puts the predefined symbols in the hash table
//--------------------------------------------------------------------------
void Ident_Init ( void )
{
  int i;
  TIdent * pIdent;
  static UCHAR regName[] = "AR0";

  // add the reserved words
  for ( i = 0; i < _countof( ResWords ); ++i )
  {
    pIdent = AllocTIdent( strlen( ResWords[i].szName ), (UCHAR *)ResWords[i].szName,
                          CalcStringHash( (UCHAR *)ResWords[i].szName ) );
    pIdent->code = ResWords[i].lexValue;
    pIdent->bNoCase = TRUE;

    // the '$' is always defined
    if (pIdent->code == LEX_DOLLAR)
      pIdent->e.rType = RELOC_CONST;
    AddIdent( pIdent );
  }

  // add R0-7 AR0-7
  for( i = 0; i < 8; ++i )
  {
    regName[2] = (char)('0' + i); // build name 

    // define ARn
    pIdent = AllocTIdent( 3, regName, CalcStringHash( regName ) );
    pIdent->code = LEX_ARN;
    pIdent->e.uValue = i;
    pIdent->e.rType = RELOC_CONST;
    pIdent->bNoCase = TRUE;
    AddIdent( pIdent );

    // define Rn
    pIdent = AllocTIdent( 2, regName+1, CalcStringHash( regName+1 ) );
    pIdent->code = LEX_RN;
    pIdent->e.uValue = i;
    pIdent->bNoCase = TRUE;
    AddIdent( pIdent );
  }

  // Add the instructions
  for ( i = 0; i < _countof( Instructions ); ++i )
  {
    pIdent = AllocTIdent( strlen( Instructions[i].szName ),
                          (UCHAR *)Instructions[i].szName,
                          CalcStringHash( (UCHAR *)Instructions[i].szName ) );
    pIdent->code = i + SYM_FIRST_INSTR;
    pIdent->bNoCase = TRUE;
    AddIdent( pIdent );
  }
};

//--------------------------------------------------------------------------
// Name         AfterPrimaryControls
//
// Description  Called from ASM.Y after we have parsed all primary controls
//              Modifies the behaviuor of the asssembler before we have parsed
//              any real instructions.
//--------------------------------------------------------------------------
void AfterPrimaryControls ( void )
{
  if (!bNoMod51)
    AddSFR8051();
};

void Ident_Done ( void )
{
};

static int Lex;

//--------------------------------------------------------------------------
// Name         FindSymb
//
// Description  Search an identifier in the table. If the identifier has a
//              character or a reserved word code then Lex is set to that code.
//              For all instructions Lex is set to LEX_INSTR. For the remaining
//              cases Lex remains unchanged (that is LEX_IDENT)
//--------------------------------------------------------------------------
static BOOL FindSymb ( const UCHAR * str, UINT hash )
{
  if (pLexSymbol = FindIdent( str, hash ))
  {
    // if this symbol has a character code we set Lex = character
    //   this is for LT,GT,EQ that result to '<', '>','='
    if (pLexSymbol->code < 128 && pLexSymbol->code > 31)
      Lex = pLexSymbol->code;
    else
    if (pLexSymbol->code >= SYM_FIRST_INSTR)  
    {
      // if we found a instruction or a reserved word ...
      if (pLexSymbol->code <= SYM_LAST_INSTR)
        Lex = LEX_INSTR;
      else
      if (pLexSymbol->code <= SYM_LAST_RES)
        Lex = pLexSymbol->code;
    }
    return TRUE;
  }
  else
    return FALSE;
};

//--------------------------------------------------------------------------
// Name         IdentGetLex
//
// Description  Processes identifiers. Attempts to find each identifier in
//              the table and sets pLexSymbol. If the identifier is not in
//              the table inserts it.
//              For reserved words returns the code of the word.
//              For instructions returns LEX_INSTR.
//--------------------------------------------------------------------------
int IdentGetLex ( BOOL process )
{
  Lex = process ? MiniGetLex() : MiniGetLexSkip();

  if (Lex == LEX_IDENT)
  {
    if (bCaseSensitive)
    {
      // First try to find the ident as it is
      if (FindSymb( LexIdent, LexIdentHash ))
        return Lex;

/*
  The reserved words, instructions and predefined SFR-s are always case
  insensitive. (At least to maintain compatibility). So here we search
  for the ident upcased but assume we have found it only if it has the
  bNoCase flag set.
*/
      if (FindSymb( LexIdentUp, LexIdentHashUp ) && pLexSymbol->bNoCase)
        return Lex;
    }
    else
    {
      if (FindSymb( LexIdentUp, LexIdentHashUp ))
        return Lex;
    }
/*
 Must add the symbol. It will be created as empty and undefined. If we are
 in case insensitive mode => add the ident upcased.
*/

    if (bCaseSensitive)
      pLexSymbol = AllocTIdent( LexIdentLen, LexIdent, LexIdentHash );
    else
      pLexSymbol = AllocTIdent( LexIdentLen, LexIdentUp, LexIdentHashUp );
    AddIdent( pLexSymbol );
  }
  else
    pLexSymbol = NULL;

  return Lex;
};
