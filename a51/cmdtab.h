/////////////////////////////////////////////////////////////////////
// definitions for assembler addressing tables

// masks for extracting the operand type and the desired segment
#define OP_MASK       0x0F
#define OP_SEG_MASK   0xF0

// values for the desired segments
#define OPSEG_NO      0x00
#define OPSEG_DATA    0x10
#define OPSEG_BIT     0x20
#define OPSEG_CODE    0x30

#define OP_EMPTY      0
#define OP_RN         1
#define OP_DIR        (2 | OPSEG_DATA)
#define OP_BIT        (2 | OPSEG_BIT)
#define OP_LAB        (2 | OPSEG_CODE)
#define OP_IMM        3
#define OP_AT_R0      4          
#define OP_DPTR       5          
#define OP_AB         6
#define OP_A          7
#define OP_AT_DPTR    8
#define OP_AT_A_DPTR  9
#define OP_AT_A_PC   10
#define OP_NOT_BIT   (11 | OPSEG_BIT)
#define OP_C         12


#define ACTION_NO           0
#define ACTION_RN           1
#define ACTION_AT_R0        2
#define ACTION_VAL8_B2      3
#define ACTION_VAL8_B3      4
#define ACTION_VAL8_REL_B2  5
#define ACTION_VAL8_REL_B3  6
#define ACTION_VAL16        7   // high byte first
#define ACTION_VAL11        8

// some aliases
#define ACTION_DIR          ACTION_VAL8_B2
#define ACTION_IMM          ACTION_VAL8_B2
#define ACTION_BIT          ACTION_VAL8_B2

// value stored in Len if we want to do special actions
#define LEN_PATCH           0
// codes stored TAddrDef.OpCode if Len is LEN_PATCH
#define PATCH_JMP           0
#define PATCH_CALL          1

typedef struct TAddrDef
{
  BYTE  Op[3];  // operands
  BYTE  Len;    // length of the instruction
  BYTE  OpCode;
  BYTE  OpAction[3];
} TAddrDef;

typedef struct TCmdDef
{
  const char      * szName;     // name of instruction
  const TAddrDef _DSPTR * Addr;
  UINT           NumAddr;       // number of possible addressations
} TCmdDef;

void CompileCmd ( int cmdIndex );
void GenFixUp ( int offset, const TExpr * expr, UINT fixType );
void GenListingLine ( void );


