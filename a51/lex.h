// --------- constant definitions for minilex

#define MAX_STRING_LEN 0x7FFF

/////////////////////////////////////////////////////////////
//  LEX_xxxx: LEX return codes

#define LEX_FAIL   -10    // internal lex alternative
#define LEX_EOL          '\n'
#define LEX_EOF          0


//////////////////////////////////////////////////////////////////////
// Global variables

// minilex variables
extern UCHAR _DSPTR * CurCharPtr;       // pointer to current source char

extern UINT LexNumber;        // the number read by GetNumber

extern UINT  LexStringLen;
extern UCHAR * LexString;
extern UCHAR LexStringBuf[];
extern UINT  LexIdentHash;     // hash value for the last ident
extern UINT  LexIdentHashUp;  // hash value for the upcased string

#define LexIdent     LexStringBuf  // alias for clarity
#define LexIdentLen  LexStringLen
extern UCHAR LexIdentUp[];         // upcased LexIdent

//////////////////////////////////////////////////////////////////////
// Global functions

// minilex functions
extern UINT CalcStringHash ( const UCHAR * str );
extern void MiniLex_Init ( void );
extern void MiniLex_Done ( void );
extern int MiniGetLex ( void );
extern int MiniGetLexSkip ( void );

