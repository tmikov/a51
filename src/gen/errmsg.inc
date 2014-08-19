#ifdef ERR_C

static const char _DSPTR * ErrMsg[] =
{
/*   0 */ "'%s' is a constant",                                   // ERR_CONST
/*   1 */ "'%s' is external",                                     // ERR_EXTERN
/*   2 */ "'%s' is from different segment",                       // ERR_DIFF_SEG
/*   3 */ "'%s' not defined",                                     // ERR_NOT_DEFINED
/*   4 */ "Allowed only in CSEG",                                 // ERR_NOT_IN_CSEG
/*   5 */ "Allowed only in BSEG",                                 // ERR_NOT_IN_BSEG
/*   6 */ "Can't add relocatable symbols",                        // ERR_ADD_RELOC
/*   7 */ "Can't subtract external symbols",                      // ERR_SUB_EXTERN
/*   8 */ "Can't subtract relocatable symbols",                   // ERR_SUB_RELOC
/*   9 */ "Can't subtract symbols from different segments",       // ERR_SUB_DIFF_SEG
/*  10 */ "Division by zero",                                     // ERR_DIV_ZERO
/*  11 */ "Illegal addressing mode for the instruction",          // ERR_INV_ADDR_MODE
/*  12 */ "Invalid 11-bit expression",                            // ERR_INV_11
/*  13 */ "Invalid address for segment",                          // ERR_INV_SEG_ADDR
/*  14 */ "Invalid binary number",                                // ERR_INV_BIN_NUM
/*  15 */ "Invalid bit number",                                   // ERR_INV_BIT_NUM
/*  16 */ "Invalid decimal number",                               // ERR_INV_DEC_NUM
/*  17 */ "Invalid index register",                               // ERR_INV_INDEX_REG
/*  18 */ "Invalid instruction '%s'",                             // ERR_INV_INSTR
/*  19 */ "Invalid numeric constant",                             // ERR_INV_NUM_CONST
/*  20 */ "Invalid octal number",                                 // ERR_INV_OCT_NUM
/*  21 */ "Invalid operand",                                      // ERR_INV_OP
/*  22 */ "Invalid relocatable expression",                       // ERR_INV_RELOC_EXPR
/*  23 */ "invalid value for 'Using'",                            // ERR_INV_USING
/*  24 */ "Line too long",                                        // ERR_LINE_LONG
/*  25 */ "Need CSEG for instructions",                           // ERR_NEED_CSEG
/*  26 */ "No segment defined",                                   // ERR_NO_SEG
/*  27 */ "Offset too large",                                     // ERR_LARGE_OFS
/*  28 */ "public/extrn conflict for '%s'",                       // ERR_PUB_EXT_CONFL
/*  29 */ "Symbol '%s' already defined",                          // ERR_SYM_REDEF
/*  30 */ "Symbol '%s' is not defined",                           // ERR_SYM_UNDEF
/*  31 */ "Syntax error",                                         // ERR_SYNTAX
/*  32 */ "Too many operands",                                    // ERR_MANY_OPS
/*  33 */ "Unclosed conditionals on EOF",                         // ERR_OPEN_COND
/*  34 */ "Unmatched ELSE",                                       // ERR_INV_ELSE
/*  35 */ "Unmatched ENDIF",                                      // ERR_INV_ENDIF
/*  36 */ "Unterminated string constant",                         // ERR_OPEN_STRING
/*  37 */ "Value of '%s' is unknown",                             // ERR_UNKNOWN_VALUE
/*  38 */ "Need a constant. '%s' is relocatable",                 // ERR_NO_RELOC
/*  39 */ "Need a constant. The expression is relocatable",       // ERR_RELOC_EXPR
/*  40 */ "This directive must precede all other statements",     // ERR_PRIMARY_CTRL
/*  41 */ "Operand has invalid segment",                          // ERR_BAD_SEGMENT
/*  42 */ "Need a constant",                                      // ERR_NEED_CONST
/*  43 */ "Can't open include file: '%s'",                        // FATAL_OPEN_INCLUDE
/*  44 */ "Error reading file",                                   // FATAL_READ_FILE
/*  45 */ "Out of memory",                                        // FATAL_NO_MEMORY
/*  46 */ "Too many error messages",                              // FATAL_MANY_ERRS
/*  47 */ "Too many fixups",                                      // FATAL_MANY_FIXUPS
/*  48 */ "Too many nested conditionals",                         // FATAL_MANY_NEST_COND
/*  49 */ "Too many warning messages",                            // FATAL_MANY_WARNS
/*  50 */ "HIGH LOW x = 0",                                       // WARN_HIGH_LOW
/*  51 */ "Identifier too long. Truncated",                       // WARN_LONG_IDENT
/*  52 */ "LOW HIGH x = 0",                                       // WARN_LOW_HIGH
/*  53 */ "Segment conflict",                                     // WARN_SEG_CONFL
/*  54 */ "WORD value truncated to BYTE. May lose info",          // WARN_TRUNC_WORD
/*  55 */ "Unsupported directive. Ignored",                       // WARN_UNSUP_DIRECTIVE
/*  56 */ "Subtracting relocatable from constant",                // WARN_SUB_CONST_REL
/*  57 */ "identifier 'extern' used. Possible mismatch with 'extrn'", // WARN_EXTERN_IDENT
};

#endif // ERR_C

#define ERR_CONST                          0
#define ERR_EXTERN                         1
#define ERR_DIFF_SEG                       2
#define ERR_NOT_DEFINED                    3
#define ERR_NOT_IN_CSEG                    4
#define ERR_NOT_IN_BSEG                    5
#define ERR_ADD_RELOC                      6
#define ERR_SUB_EXTERN                     7
#define ERR_SUB_RELOC                      8
#define ERR_SUB_DIFF_SEG                   9
#define ERR_DIV_ZERO                      10
#define ERR_INV_ADDR_MODE                 11
#define ERR_INV_11                        12
#define ERR_INV_SEG_ADDR                  13
#define ERR_INV_BIN_NUM                   14
#define ERR_INV_BIT_NUM                   15
#define ERR_INV_DEC_NUM                   16
#define ERR_INV_INDEX_REG                 17
#define ERR_INV_INSTR                     18
#define ERR_INV_NUM_CONST                 19
#define ERR_INV_OCT_NUM                   20
#define ERR_INV_OP                        21
#define ERR_INV_RELOC_EXPR                22
#define ERR_INV_USING                     23
#define ERR_LINE_LONG                     24
#define ERR_NEED_CSEG                     25
#define ERR_NO_SEG                        26
#define ERR_LARGE_OFS                     27
#define ERR_PUB_EXT_CONFL                 28
#define ERR_SYM_REDEF                     29
#define ERR_SYM_UNDEF                     30
#define ERR_SYNTAX                        31
#define ERR_MANY_OPS                      32
#define ERR_OPEN_COND                     33
#define ERR_INV_ELSE                      34
#define ERR_INV_ENDIF                     35
#define ERR_OPEN_STRING                   36
#define ERR_UNKNOWN_VALUE                 37
#define ERR_NO_RELOC                      38
#define ERR_RELOC_EXPR                    39
#define ERR_PRIMARY_CTRL                  40
#define ERR_BAD_SEGMENT                   41
#define ERR_NEED_CONST                    42
#define FATAL_OPEN_INCLUDE                43
#define FATAL_READ_FILE                   44
#define FATAL_NO_MEMORY                   45
#define FATAL_MANY_ERRS                   46
#define FATAL_MANY_FIXUPS                 47
#define FATAL_MANY_NEST_COND              48
#define FATAL_MANY_WARNS                  49
#define WARN_HIGH_LOW                     50
#define WARN_LONG_IDENT                   51
#define WARN_LOW_HIGH                     52
#define WARN_SEG_CONFL                    53
#define WARN_TRUNC_WORD                   54
#define WARN_UNSUP_DIRECTIVE              55
#define WARN_SUB_CONST_REL                56
#define WARN_EXTERN_IDENT                 57
