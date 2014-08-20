#define INSTR( name )  { #name, addr_##name, _countof( addr_##name ) }
#define ADDR( name )   static TAddrDef addr_##name[] =

ADDR( ADD )
{
  { { OP_A, OP_RN },     1, 0x28, { 0, ACTION_RN } },    // add A,Rn
  { { OP_A, OP_DIR },    2, 0x25, { 0, ACTION_DIR } },   // add A,dir
  { { OP_A, OP_AT_R0 },  1, 0x26, { 0, ACTION_AT_R0 } }, // add A,@Ri
  { { OP_A, OP_IMM },    2, 0x24, { 0, ACTION_IMM } },   // add A,#imm
};

ADDR( ADDC )
{
  { { OP_A, OP_RN },     1, 0x38, { 0, ACTION_RN } },    // addc A,Rn
  { { OP_A, OP_DIR },    2, 0x35, { 0, ACTION_DIR } },   // addc A,dir
  { { OP_A, OP_AT_R0 },  1, 0x36, { 0, ACTION_AT_R0 } }, // addc A,@Ri
  { { OP_A, OP_IMM },    2, 0x34, { 0, ACTION_IMM } },   // addc A,#imm
};

ADDR( SUBB )
{
  { { OP_A, OP_RN },     1, 0x98, { 0, ACTION_RN } },    // subb A,Rn
  { { OP_A, OP_DIR },    2, 0x95, { 0, ACTION_DIR } },   // subb A,dir
  { { OP_A, OP_AT_R0 },  1, 0x96, { 0, ACTION_AT_R0 } }, // subb A,@Ri
  { { OP_A, OP_IMM },    2, 0x94, { 0, ACTION_IMM } },   // subb A,#imm
};

ADDR( INC )
{
  { { OP_A },            1, 0x04, { 0 } },               // inc A
  { { OP_RN },           1, 0x08, { ACTION_RN } },       // inc Rn
  { { OP_DIR },          2, 0x05, { ACTION_DIR } },      // inc dir
  { { OP_AT_R0 },        1, 0x06, { ACTION_AT_R0 } },    // inc @R0
  { { OP_DPTR },         1, 0xA3, { 0 } },               // inc DPTR
};

ADDR( DEC )
{
  { { OP_A },            1, 0x14, { 0 } },               // dec A
  { { OP_RN },           1, 0x18, { ACTION_RN } },       // dec Rn
  { { OP_DIR },          2, 0x15, { ACTION_DIR } },      // dec dir
  { { OP_AT_R0 },        1, 0x16, { ACTION_AT_R0 } },    // dec @R0
};

ADDR( MUL )
{
  { { OP_AB },           1, 0xA4, { 0 } },               // mul AB
};

ADDR( DIV )
{
  { { OP_AB },           1, 0x84, { 0 } },               // div AB
};

ADDR( DA )
{
  { { OP_A },            1, 0xD4, { 0 } },               // da A
};

ADDR( ANL )
{
  { { OP_A, OP_RN },     1, 0x58, { 0, ACTION_RN } },    // anl A,Rn
  { { OP_A, OP_DIR },    2, 0x55, { 0, ACTION_DIR } },   // anl A,dir
  { { OP_A, OP_AT_R0 },  1, 0x56, { 0, ACTION_AT_R0 } }, // anl A,@Ri
  { { OP_A, OP_IMM },    2, 0x54, { 0, ACTION_IMM } },   // anl A,#imm
  { { OP_DIR, OP_A },    2, 0x52, { ACTION_DIR, 0 } },   // anl dir,A
  { { OP_DIR, OP_IMM },  3, 0x53, { ACTION_VAL8_B2, ACTION_VAL8_B3 } }, // anl dir,#imm

  { { OP_C, OP_BIT },    2, 0x82, { 0, ACTION_BIT } },   // anl C,bit
  { { OP_C, OP_NOT_BIT },2, 0xB0, { 0, ACTION_BIT } },   // anl C,/bit
};

ADDR( ORL )
{
  { { OP_A, OP_RN },     1, 0x48, { 0, ACTION_RN } },    // orl A,Rn
  { { OP_A, OP_DIR },    2, 0x45, { 0, ACTION_DIR } },   // orl A,dir
  { { OP_A, OP_AT_R0 },  1, 0x46, { 0, ACTION_AT_R0 } }, // orl A,@Ri
  { { OP_A, OP_IMM },    2, 0x44, { 0, ACTION_IMM } },   // orl A,#imm
  { { OP_DIR, OP_A },    2, 0x42, { ACTION_DIR, 0 } },   // orl dir,A
  { { OP_DIR, OP_IMM },  3, 0x43, { ACTION_VAL8_B2, ACTION_VAL8_B3 } }, // orl dir,#imm

  { { OP_C, OP_BIT },    2, 0x72, { 0, ACTION_BIT } },   // orl C,bit
  { { OP_C, OP_NOT_BIT },2, 0xA0, { 0, ACTION_BIT } },   // orl C,/bit
};

ADDR( XRL )
{
  { { OP_A, OP_RN },     1, 0x68, { 0, ACTION_RN } },    // xrl A,Rn
  { { OP_A, OP_DIR },    2, 0x65, { 0, ACTION_DIR } },   // xrl A,dir
  { { OP_A, OP_AT_R0 },  1, 0x66, { 0, ACTION_AT_R0 } }, // xrl A,@Ri
  { { OP_A, OP_IMM },    2, 0x64, { 0, ACTION_IMM } },   // xrl A,#imm
  { { OP_DIR, OP_A },    2, 0x62, { ACTION_DIR, 0 } },   // xrl dir,A
  { { OP_DIR, OP_IMM },  3, 0x63, { ACTION_VAL8_B2, ACTION_VAL8_B3 } }, // xrl dir,#imm
};

ADDR( CLR )
{
  { { OP_A },            1, 0xE4, { 0 } },               // clr A

  { { OP_C },            1, 0xC3, { 0 } },               // clr C
  { { OP_BIT },          2, 0xC2, { ACTION_BIT } },      // clr bit
};

ADDR( CPL )
{
  { { OP_A },            1, 0xF4, { 0 } },               // cpl A

  { { OP_C },            1, 0xB3, { 0 } },               // cpl C
  { { OP_BIT },          2, 0xB2, { ACTION_BIT } },      // cpl bit
};

ADDR( RL )
{
  { { OP_A },            1, 0x23, { 0 } },               // rl A
};

ADDR( RLC )
{
  { { OP_A },            1, 0x33, { 0 } },               // rlc A
};

ADDR( RR )
{
  { { OP_A },            1, 0x03, { 0 } },               // rr A
};

ADDR( RRC )
{
  { { OP_A },            1, 0x13, { 0 } },               // rrc A
};

ADDR( SWAP )
{
  { { OP_A },            1, 0xC4, { 0 } },               // swap A
};

ADDR( MOV )
{
  { { OP_A, OP_RN },     1, 0xE8, { 0, ACTION_RN } },    // mov A,Rn
  { { OP_A, OP_DIR },    2, 0xE5, { 0, ACTION_DIR } },   // mov A,dir
  { { OP_A, OP_AT_R0 },  1, 0xE6, { 0, ACTION_AT_R0 } }, // mov A,@Ri
  { { OP_A, OP_IMM },    2, 0x74, { 0, ACTION_IMM } },   // mov A,#imm
  { { OP_RN, OP_A },     1, 0xF8, { ACTION_RN, 0 } },    // mov Rn,A
  { { OP_RN, OP_DIR },   2, 0xA8, { ACTION_RN, ACTION_DIR }},// mov Rn,dir
  { { OP_RN, OP_IMM },   2, 0x78, { ACTION_RN, ACTION_IMM }},// mov Rn,#imm
  { { OP_DIR, OP_A },    2, 0xF5, { ACTION_DIR, 0 } },   // mov dir,A
  { { OP_DIR, OP_RN },   2, 0x88, { ACTION_DIR, ACTION_RN }}, // mov dir,Rn
  { { OP_DIR, OP_DIR },  3, 0x85, { ACTION_VAL8_B3, ACTION_VAL8_B2 }}, // mov dir,dir
  { { OP_DIR, OP_AT_R0 },2, 0x86, { ACTION_DIR, ACTION_AT_R0 }}, // mov dir,@R0
  { { OP_DIR, OP_IMM },  3, 0x75, { ACTION_VAL8_B2, ACTION_VAL8_B3 }}, // mov dir,#imm
  { { OP_AT_R0, OP_A },  1, 0xF6, { ACTION_AT_R0, 0 } }, // mov @R0,A
  { { OP_AT_R0, OP_DIR },2, 0xA6, { ACTION_AT_R0, ACTION_DIR }}, // mov @R0,dir
  { { OP_AT_R0, OP_IMM },2, 0x76, { ACTION_AT_R0, ACTION_IMM }}, // mov @R0,#imm
  { { OP_DPTR, OP_IMM }, 3, 0x90, { 0, ACTION_VAL16 } }, // mov DPTR,#imm

  { { OP_C, OP_BIT },    2, 0xA2, { 0, ACTION_BIT } },   // mov C,bit
  { { OP_BIT, OP_C },    2, 0x92, { ACTION_BIT, 0 } },   // mov bit,C
};

ADDR( MOVC )
{
  { {OP_A,OP_AT_A_DPTR}, 1, 0x93, { 0, 0 } },            // movc A,@A+DPTR
  { {OP_A,OP_AT_A_PC },  1, 0x83, { 0, 0 } },            // movc A,@A+PC
};

ADDR( MOVX )
{
  { { OP_A, OP_AT_R0 },  1, 0xE2, { 0, ACTION_AT_R0 } }, // movx A,@Ri
  { { OP_A, OP_AT_DPTR },1, 0xE0, { 0, 0 } },            // movx A,@DPTR
  { { OP_AT_R0, OP_A },  1, 0xF2, { ACTION_AT_R0, 0 } }, // movx @Ri,A
  { { OP_AT_DPTR, OP_A },1, 0xF0, { 0, 0 } },            // movx @DPTR,A
};

ADDR( PUSH )
{
  { { OP_DIR },          2, 0xC0, { ACTION_DIR } },      // push dir
};

ADDR( POP )
{
  { { OP_DIR },          2, 0xD0, { ACTION_DIR } },      // pop dir
};

ADDR( XCH )
{
  { { OP_A, OP_RN },     1, 0xC8, { 0, ACTION_RN } },    // xch A,Rn
  { { OP_A, OP_DIR },    2, 0xC5, { 0, ACTION_DIR } },   // xch A,dir
  { { OP_A, OP_AT_R0 },  1, 0xC6, { 0, ACTION_AT_R0 } }, // xch A,@Ri
};

ADDR( XCHD )
{
  { { OP_A, OP_AT_R0 },  1, 0xD6, { 0, ACTION_AT_R0 } }, // xchd A,@Ri
};

ADDR( SETB )
{
  { { OP_C },            1, 0xD3, { 0 } },               // setb C
  { { OP_BIT },          2, 0xD2, { ACTION_BIT } },      // setb bit
};

ADDR( RET )
{
  { { 0 },               1, 0x22, { 0 } },               // ret
};

ADDR( RETI )
{
  { { 0 },               1, 0x32, { 0 } },               // reti
};

ADDR( AJMP )
{
  { { OP_LAB },          2, 0x01, { ACTION_VAL11 } },    // ajmp lab
};

ADDR( LJMP )
{
  { { OP_LAB },          3, 0x02, { ACTION_VAL16 } },    // ljmp lab
};

ADDR( SJMP )
{
  { { OP_LAB },          2, 0x80, { ACTION_VAL8_REL_B2}},// sjmp lab
};

ADDR( JMP )
{
  { { OP_AT_A_DPTR },    1, 0x73, { 0 } },               // jmp @A+DPTR 
  { { OP_LAB },          LEN_PATCH, PATCH_JMP, { 0 } },  // jmp lab
  { { OP_LAB },          2, 0x80, { ACTION_VAL8_REL_B2}},// sjmp lab
  { { OP_LAB },          2, 0x01, { ACTION_VAL11 } },    // ajmp lab
  { { OP_LAB },          3, 0x02, { ACTION_VAL16 } },    // ljmp lab
};

ADDR( ACALL )
{
  { { OP_LAB },          2, 0x11, { ACTION_VAL11 } },    // acall lab
};

ADDR( LCALL )
{
  { { OP_LAB },          3, 0x12, { ACTION_VAL16 } },    // lcall lab
};

ADDR( CALL )
{
  { { OP_LAB },          LEN_PATCH, PATCH_CALL, { 0 } }, // call lab
  { { OP_LAB },          2, 0x11, { ACTION_VAL11 } },    // acall lab
  { { OP_LAB },          3, 0x12, { ACTION_VAL16 } },    // lcall lab
};

ADDR( JZ )
{
  { { OP_LAB },          2, 0x60, { ACTION_VAL8_REL_B2}},// jz lab
};

ADDR( JNZ )
{
  { { OP_LAB },          2, 0x70, { ACTION_VAL8_REL_B2}},// jnz lab
};

ADDR( JC )
{
  { { OP_LAB },          2, 0x40, { ACTION_VAL8_REL_B2}},// jc lab
};

ADDR( JNC )
{
  { { OP_LAB },          2, 0x50, { ACTION_VAL8_REL_B2}},// jnc lab
};

ADDR( JB )
{
  { { OP_BIT, OP_LAB },  3, 0x20, { ACTION_BIT, ACTION_VAL8_REL_B3 }}, // jb bit,lab
};

ADDR( JNB )
{
  { { OP_BIT, OP_LAB },  3, 0x30, { ACTION_BIT, ACTION_VAL8_REL_B3 }}, // jnb bit,lab
};

ADDR( JBC )
{
  { { OP_BIT, OP_LAB },  3, 0x10, { ACTION_BIT, ACTION_VAL8_REL_B3 }}, // jbc bit,lab
};

ADDR( CJNE )
{
  { { OP_A, OP_DIR, OP_LAB }, 3, 0xB5, { 0, ACTION_DIR, ACTION_VAL8_REL_B3 }}, // cjne A,dir,lab
  { { OP_A, OP_IMM, OP_LAB }, 3, 0xB4, { 0, ACTION_IMM, ACTION_VAL8_REL_B3 }}, // cjne A,#imm,lab
  { { OP_RN, OP_IMM, OP_LAB }, 3, 0xB8, { ACTION_RN, ACTION_IMM, ACTION_VAL8_REL_B3 }}, // cjne Rn,#imm,lab
  { { OP_AT_R0, OP_IMM, OP_LAB }, 3, 0xB6, { ACTION_AT_R0, ACTION_IMM, ACTION_VAL8_REL_B3 }}, // cjne @Ri,#imm,lab
};

ADDR( DJNZ )
{
  { { OP_RN, OP_LAB },   2, 0xD8, { ACTION_RN, ACTION_VAL8_REL_B2 } }, // djnz Rn,lab
  { { OP_DIR, OP_LAB },  3, 0xD5, { ACTION_DIR, ACTION_VAL8_REL_B3 } }, // djnz dir,lab
};

ADDR( NOP )
{
  { { 0 },               1, 0x00, { 0 } },               // nop
};

TCmdDef Instructions[] =
{
  INSTR( ADD ),
  INSTR( ADDC ),
  INSTR( SUBB ),
  INSTR( INC ),
  INSTR( DEC ),
  INSTR( MUL ),
  INSTR( DIV ),
  INSTR( DA ),
  INSTR( ANL ),
  INSTR( ORL ),
  INSTR( XRL ),
  INSTR( CLR ),
  INSTR( CPL ),
  INSTR( RL ),
  INSTR( RLC ),
  INSTR( RR ),
  INSTR( RRC ),
  INSTR( SWAP ),
  INSTR( MOV ),
  INSTR( MOVC ),
  INSTR( MOVX ),
  INSTR( PUSH ),
  INSTR( POP ),
  INSTR( XCH ),
  INSTR( XCHD ),
  INSTR( SETB ),

  INSTR( ACALL ),
  INSTR( LCALL ),
  INSTR( CALL ),
  INSTR( RET ),
  INSTR( RETI ),
  INSTR( AJMP ),
  INSTR( LJMP ),
  INSTR( SJMP ),
  INSTR( JMP ),
  INSTR( JZ ),
  INSTR( JNZ ),
  INSTR( JC ),
  INSTR( JNC ),
  INSTR( JB ),
  INSTR( JNB ),
  INSTR( JBC ),
  INSTR( CJNE ),
  INSTR( DJNZ ),
  INSTR( NOP ),
};

#undef ADDR
#undef INSTR
