%expect 16  // 2  - LEX_STRING in DB and in expression
            // 14 - the directives in prim_ctrls and in program
%{
#include "global.h"
#include "a51.h"

#if YYDEBUG
#  define YYERROR_VERBOSE         1
#endif
#define YYSTDERR                  stdout

static void CheckCodeSeg ( void );
static UINT StringToWord ( void );

int yylex ( void );

%}

%union
{
  UINT     uValue;
  TIdent * pIdent;
  TExpr    expr;
}

%token LEX_IF LEX_ELSE LEX_ENDIF LEX_IFDEF LEX_IFNDEF

%token LEX_A LEX_C LEX_DPTR LEX_PC LEX_AB
%token <pIdent> LEX_DOLLAR

%token LEX_ORG LEX_USING LEX_END LEX_AT LEX_CSEG
%token LEX_DSEG LEX_ISEG LEX_BSEG LEX_XSEG LEX_EQU LEX_SET
%token LEX_CODE LEX_DATA LEX_IDATA LEX_BIT LEX_XDATA
%token LEX_DB LEX_DW LEX_DS LEX_DBIT
%token LEX_NOMOD51 LEX_NOPAGING LEX_PAGELENGTH LEX_NOSYMBOLS
%token LEX_DEBUG  LEX_PAGEWIDTH LEX_NOTABS LEX_XREF LEX_TITLE LEX_NAME LEX_DATE
%token LEX_INCLUDE LEX_EJECT
%token LEX_NOLIST LEX_LIST

%token LEX_EXTRN LEX_PUBLIC

%left LEX_OR LEX_XOR LEX_AND
%left LEX_NE LEX_LE LEX_GE '>' '<' '='
%left '+' '-'
%left LEX_SHL LEX_SHR
%left '*' '/' LEX_MOD
%nonassoc '.'
%right UPLUS UMINUS LEX_NOT LEX_HIGH LEX_LOW
%nonassoc LEX_NUMBER LEX_IDENT

%token <pIdent> LEX_IDENT
%token <uValue> LEX_NUMBER
%token <uValue> LEX_INSTR
%token <uValue> LEX_RN LEX_ARN
%token LEX_STRING 

%type <expr>   op_expr expr e1 const_expr
%type <uValue> seg_name  def_qual seg_ident instr

%%

file
  : prim_ctrls { AfterPrimaryControls() } program
  ;

prim_ctrls
  : prim_ctrls prim_ctrl 
  | /* empty */
  ;

prim_ctrl
  : directive '\n'
  | '\n'
  ;

program
  : program line
  | /* empty */
  ;

line
  : '\n'
  | label line
  | definition   '\n'
  | instruction
  | pseudo_instr '\n'
  | directive    '\n'
  | LEX_INCLUDE  '('  { GetIncludeName(); yyclearin; } ')' '\n'
    { GenListingLine(); Include(); }
  | LEX_IF const_expr  '\n'     { DoIf( $2.uValue != 0 ) }
  | LEX_IFDEF LEX_IDENT '\n'    { DoIf( $2->e.rType != RELOC_UNDEF ) }
  | LEX_IFNDEF LEX_IDENT '\n'   { DoIf( $2->e.rType == RELOC_UNDEF ) }
  | LEX_ELSE  '\n'    { DoElse() }                            
  | LEX_ENDIF '\n'    { DoEndif() }

  // error handling
  | error        '\n' { yyerrok; PrintError( ERR_SYNTAX ); }
  ;

label
  : LEX_IDENT ':'      { PutLabel( $1 ) } 
  ;

  // assembler directives
directive
  : LEX_NOMOD51  { bNoMod51 = TRUE } 
  | unsup_dir    { PrintWarning( WARN_UNSUP_DIRECTIVE ) }
  ;

// unsupported directives
unsup_dir
  : LEX_NOPAGING      
  | LEX_PAGELENGTH '(' const_expr ')'  
  | LEX_PAGEWIDTH  '(' const_expr ')'   
  | LEX_NOSYMBOLS    
  | LEX_EJECT        
  | LEX_NOLIST       
  | LEX_LIST         
  | LEX_DEBUG        
  | LEX_NOTABS       
  | LEX_XREF         
  | LEX_TITLE      '(' { GetToClosingBrace(); yyclearin; } ')'
  | LEX_DATE       '(' { GetToClosingBrace(); yyclearin; } ')'
  ;


pseudo_instr
  : LEX_ORG    expr         { Org( &$2 ) }
  | LEX_USING  const_expr
    {
      if ($2.uValue > 3)
        PrintError( ERR_INV_USING );
      else
        Using = $2.uValue;
    }
  | LEX_END                      { YYACCEPT } // TODO: warn if in include!!!
  | seg_name LEX_AT expr         { StartSeg( $1 ); Org( &$3 ) }
  | seg_name                     { StartSeg( $1 ) }

  | LEX_EXTRN extrn_list
  | LEX_PUBLIC public_list
  | LEX_NAME   LEX_IDENT             { PrintWarning( WARN_UNSUP_DIRECTIVE ) }
  ;

gen_ds
  : const_expr         { UseSpace( $1.uValue ) }
  ;

gen_dbit
  : const_expr
    {
      if (CurSegment != O51_BSEG)
        PrintError( ERR_NOT_IN_BSEG );
      else
      {
        UseSpace( $1.uValue );
      }
    }
  ;

public_list
  : public_def
  | public_list ',' public_def
  ;

public_def
  : LEX_IDENT  { Public( $1 ) }
  ;

extrn_list
  : extrn_def
  | extrn_list ',' extrn_def
  ;

extrn_def
  : seg_ident '(' LEX_IDENT ')'      { Extern( $3, $1 ) }
  | LEX_IDENT                        { Extern( $1, O51_NOSEG ) }
  ;

seg_name
  : LEX_CSEG  { $$ = O51_CSEG }
  | LEX_DSEG  { $$ = O51_DSEG }
  | LEX_ISEG  { $$ = O51_ISEG }
  | LEX_BSEG  { $$ = O51_BSEG }
  | LEX_XSEG  { $$ = O51_XSEG }
  ;

// a definition e.g (X equ 10)
definition
  : LEX_IDENT def_qual expr
    {
      // Warning on common error: "extern data(x)"
      if (_stricmp( $1->szName, "extern" ) == 0)
        PrintWarning( WARN_EXTERN_IDENT );
      Define( $1, $2, &$3 )
    }
  | LEX_IDENT { PutLabel( $1 ) } def_data
  | def_data
  ;

def_data
  : LEX_DB { CheckCodeSeg() } db_list
  | LEX_DW { CheckCodeSeg() } dw_list 
  | LEX_DS   gen_ds
  | LEX_DBIT gen_dbit
  ;

db_list
  : db_val
  | db_list ',' db_val
  ;

db_val
  : expr        { GenDB( &$1 );   }
  | LEX_STRING  { GenDBString();  }
  ;

dw_list
  : dw_val
  | dw_list ',' dw_val
  ;

dw_val
  : expr        { GenDW( &$1 );   }
  ;

seg_ident
  : LEX_CODE      { $$ = O51_CSEG }
  | LEX_DATA      { $$ = O51_DSEG }
  | LEX_IDATA     { $$ = O51_ISEG }
  | LEX_BIT       { $$ = O51_BSEG }
  | LEX_XDATA     { $$ = O51_XSEG }
  ;

// a qualifier for a definition
def_qual
  : LEX_EQU       { $$ = O51_NOSEG }
  | LEX_SET       { $$ = O51_NOSEG | 0x8000 }
  | seg_ident
  ;

instruction
  : instr ops '\n' { Compile( $1 ) }

  // error handling
  | instr error '\n'
    { PrintError( ERR_INV_OP ) }
  | LEX_IDENT
    {
      PrintError( ERR_INV_INSTR, $1->szName );
      OpCount = 0
    }
    ops '\n' // anyway parse the operands
  ;

instr
  : LEX_INSTR { OpCount = 0 }
  ;

ops
  : /* empty */
  | ops_tail
  ;

ops_tail
  : op          
  | op ',' ops_tail
  ;

op
  : op_expr      { PutOp( &$1 ) }
  ;

// an expression whose value must be known at pass 1
const_expr
  : { bNeedConst = TRUE } e1
    {
      UINT dummy;
      $$ = GetConstValue( &$2, &dummy ) ? $2 : EmptyExpr;
    } 
  ;

// an expression in an operand   
op_expr
  : LEX_RN
    { $$ = EmptyExpr; $$.addr = OP_RN; $$.uValue = $1 }
  | '@' LEX_RN
    {
      $$ = EmptyExpr;
      $$.addr = OP_AT_R0;
      $$.uValue = $2 & 1;
      if ($2 > 1)
        PrintError( ERR_INV_INDEX_REG );
    }
  | '@' LEX_A '+' LEX_DPTR
    { $$ = EmptyExpr; $$.addr = OP_AT_A_DPTR }
  | '@' LEX_A '+' LEX_PC
    { $$ = EmptyExpr; $$.addr = OP_AT_A_PC }
  | '@' LEX_DPTR
    { $$ = EmptyExpr; $$.addr = OP_AT_DPTR }
  | LEX_DPTR
    { $$ = EmptyExpr; $$.addr = OP_DPTR }
  | LEX_A
    { $$ = EmptyExpr; $$.addr = OP_A }
  | LEX_C
    { $$ = EmptyExpr; $$.addr = OP_C }
  | LEX_AB
    { $$ = EmptyExpr; $$.addr = OP_AB }
  | '#' expr           { $$ = $2; $$.addr = OP_IMM; }
  | '/' expr           { $$ = $2; $$.addr = OP_NOT_BIT; $$.seg = O51_BSEG; }
  | expr               { $$ = $1; $$.addr = OP_DIR; }
  ;

expr
  : { bNeedConst = FALSE; } e1 { $$ = $2 }
  ;

// NOTE: bNeedConst must have a valid value any time before we call
// e1
e1
  : e1 LEX_OR e1           { Expr( &$$, &$1, &$3, LEX_OR ); }
  | e1 LEX_XOR e1          { Expr( &$$, &$1, &$3, LEX_XOR ); }
  | e1 LEX_AND e1          { Expr( &$$, &$1, &$3, LEX_AND ); }

  | e1 '=' e1              { Expr( &$$, &$1, &$3, '=' ); }
  | e1 LEX_NE e1           { Expr( &$$, &$1, &$3, LEX_NE ); }
  | e1 '<' e1              { Expr( &$$, &$1, &$3, '<' ); }
  | e1 LEX_LE e1           { Expr( &$$, &$1, &$3, LEX_LE ); }
  | e1 '>' e1              { Expr( &$$, &$1, &$3, '>' ); }
  | e1 LEX_GE e1           { Expr( &$$, &$1, &$3, LEX_GE ); }

  | e1 '+' e1              { Expr( &$$, &$1, &$3, '+' ); }
  | e1 '-' e1              { Expr( &$$, &$1, &$3, '-' ); }

  | e1 LEX_SHL e1          { Expr( &$$, &$1, &$3, LEX_SHL ); }
  | e1 LEX_SHR e1          { Expr( &$$, &$1, &$3, LEX_SHR ); }

  | e1 '*' e1              { Expr( &$$, &$1, &$3, '*' ); }
  | e1 '/' e1              { Expr( &$$, &$1, &$3, '/' ); }
  | e1 LEX_MOD e1          { Expr( &$$, &$1, &$3, LEX_MOD ); }

  | e1 '.' e1              { Expr( &$$, &$1, &$3, '.' ); }

  | '+' e1  %prec UPLUS    { $$ = $2; }
  | '-' e1  %prec UMINUS   { Expr( &$$, &$2, NULL, UMINUS ); }
  | LEX_NOT e1             { Expr( &$$, &$2, NULL, LEX_NOT ); }
  | LEX_HIGH e1            { Expr( &$$, &$2, NULL, LEX_HIGH ); }
  | LEX_LOW e1             { Expr( &$$, &$2, NULL, LEX_LOW ); }

  | '(' e1 ')'             { $$ = $2 }

  | LEX_NUMBER             { $$ = EmptyExpr; $$.uValue = $1 & 0xFFFF; $$.rType = RELOC_CONST }
  | LEX_STRING             { $$ = EmptyExpr; $$.uValue = StringToWord(); $$.rType = RELOC_CONST }
  | LEX_IDENT              { MakeIdent( &$$, $1 ); }
  | LEX_ARN
    { $$ = EmptyExpr; $$.uValue = $1 + (Using << 3); $$.seg = O51_DSEG; $$.rType = RELOC_CONST }
  | LEX_DOLLAR
    {
      $$ = EmptyExpr;
      $$.uValue = GetCurPos();
      $$.seg = CurSegment;
      $$.rType = RELOC_FRAME;
    }     
  ;

%%

// This function is useless
void yyerror ( char * msg )
{
  PrintErrorStr( ERR_ERROR, msg );
};

int yylex ( void )
{
  int lex = GetLex();

  switch (lex)
  {
    case LEX_NUMBER:
      yylval.uValue = LexNumber;
      break;

    // return the code of the instruction
    case LEX_INSTR:
      ASSERT( pLexSymbol );
      yylval.uValue = pLexSymbol->code;
      break;

    // return the number of the register
    case LEX_ARN:
    case LEX_RN:
      yylval.uValue = pLexSymbol->e.uValue;
      break;

    case LEX_IDENT:
      yylval.pIdent = pLexSymbol;
      if (bGenerate && bMakeXref)
      {
        pLexSymbol->lastXref =
          AddXref( IncludeTop->pName, IncludeTop->CurLine, pLexSymbol->lastXref );
      }
      break;

    default:
      if (pLexSymbol)
        yylval.pIdent = pLexSymbol;
      break;
  }

  return lex;
};

static void CheckCodeSeg ( void )
{
  if (CurSegment != O51_CSEG)
    PrintError( ERR_NOT_IN_CSEG );
};

static UINT StringToWord ( void )
{
  // The string must be the same in the code.
  // The ordering of a word generated: HI LO, so the first byte of
  // the string is high, the second is low

  if (LexStringLen == 2)
    return (LexString[0] << 8) + LexString[1];
  if (LexStringLen == 1)
    return LexString[0];

  PrintError( ERR_INV_NUM_CONST );
  return 0;
};


