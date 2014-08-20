#include "common/global.h"
#pragma hdrstop
#include "a51.h"
#include "a_tab.h"

///////////////////////////////////////////////////////////////////////
// CT_xxxx : character type definitions
//
// To determine each char's type we use a table with these
// character attributes:

#define CT_DIGIT     1  // 0 - 9
#define CT_XDIGIT    2  // 0-9, a-f, A-F
#define CT_AF_LOW    4  // a-f
#define CT_AF_UP     8  // A-F
#define CT_SPACE    16  // 0x09 - 0x0D, 0x20
#define CT_ID_START 32  // letter, _, ?, $
#define CT_IDENT    64  // CT_ID_START | CT_DIGIT

#include "ctbl.inc"     // CharTable definition

#define TST_CHAR( c, x )  (CharTable[c] & (x))

#include "uptab.inc"    // UpTable definition

#define UPCASE( x )       (UpTable[(x) & 0x7F])

//////////////////////////////////////////////////////////////////////
// Global variables

UCHAR _DSPTR * CurCharPtr;       // pointer to current source char

UINT LexNumber;        // the number read by GetNumber

UCHAR * LexString = NULL;
UINT  LexStringLen;
UINT  LexIdentHash;
UINT  LexIdentHashUp; 

/*
 To speed up work with strings & idents and still avoid memory
 allocations in most cases I use a static buffer for smaller strings
 and idents. In fact this will be by far the most common case.
 If the string occasionally grows beyond the size of the static buffer
 we allocate a larger buffer in heap (that is eventually expanded).
 This buffer is freed each time MiniGetLex is called, so it is
 transparent to the caller.
*/
#define STRING_BUF_SIZE  512
#define STRING_BUF_GROW  512

#define MAX_IDENT_LEN    (STRING_BUF_SIZE - 1)  // reserve space for \0

UCHAR LexStringBuf[STRING_BUF_SIZE];
UCHAR LexIdentUp[STRING_BUF_SIZE];
static UINT  CurStringSize;


#if UINT_MAX > 0xFFFFu
#  define WORD_ROTL( x )  (((x & 0x8000u) >> 15) + ((x << 1) & 0xFFFFu))
#else
#  define WORD_ROTL( x )  _rotl( x, 1 )
#endif

// temporary macros
#define IS_IDENT_START( c ) (TST_CHAR( c, CT_ID_START ))
#define IS_IDENT( c )       (TST_CHAR( c, CT_IDENT ))

//////////////////////////////////////////////////////////////////////
// Local functions

//--------------------------------------------------------------------------
// Name         GrowStringBuffer
//
// Description  Expands the string buffer. 
//--------------------------------------------------------------------------
static void GrowStringBuffer ( void )
{
  UCHAR * newBlock;

  if (LexString == LexStringBuf)
  {
    // still in the static buffer, so just allocate new
    ASSERT( CurStringSize == STRING_BUF_SIZE );

    newBlock = xmalloc( STRING_BUF_SIZE + STRING_BUF_GROW );
    memcpy( newBlock, LexStringBuf, STRING_BUF_SIZE );
  }
  else
  {
    // already in heap buffer => reallocate it
    ASSERT( CurStringSize > STRING_BUF_SIZE );

    if (!(newBlock = realloc( LexString, CurStringSize + STRING_BUF_SIZE )))
      OutOfMemory();
  }

  LexString = newBlock;
  CurStringSize += STRING_BUF_GROW;
};

//--------------------------------------------------------------------------
// Name         AddToString
//
// Description  Add a character to current string growing it if necessary
//--------------------------------------------------------------------------
static void AddToString ( int ch )
{
  if (LexStringLen == CurStringSize)
    GrowStringBuffer();

  ASSERT( LexStringLen < CurStringSize );

  LexString[LexStringLen++] = (BYTE)ch;
};

//--------------------------------------------------------------------------
// Name: GetIdent
//
// Description  Get a A51 style ident from _curChar_ and store it in
//              LexIdent[].
//--------------------------------------------------------------------------
static int GetIdent ( void )
{
  UCHAR _DSPTR * curChar = CurCharPtr;
  int   curLen;

  LexIdentHash   =
  LexIdentHashUp = 0;
  curLen   = 0;

  do
  {
    if (curLen == MAX_IDENT_LEN)
    {
      PrintWarning( WARN_LONG_IDENT );
      do
      {
        ++curChar;
      }
      while (IS_IDENT( *curChar ));
    }
    else
    {
      int uCh = UPCASE( *curChar );

      // calculate the hash value
      LexIdentHash ^= *curChar;
      LexIdentHash = WORD_ROTL( LexIdentHash );

      // calculate the upcased hash value
      LexIdentHashUp ^= uCh;
      LexIdentHashUp = WORD_ROTL( LexIdentHashUp );

      LexIdent[curLen] = *curChar;
      LexIdentUp[curLen] = (BYTE)uCh;

      ++curLen;
      ++curChar;
    }
  }
  while (IS_IDENT( *curChar ));

  LexIdent[curLen] = 0; // finish the string
  LexIdentUp[curLen] = 0; // finish the up-string

  LexIdentHash += LexIdent[0];   // finish the hash value
  LexIdentHashUp += LexIdentUp[0];   // finish the up-hash value

  LexIdentLen = curLen; 
  CurCharPtr = curChar;

  return LEX_IDENT;
};

static UINT htol ( void )
{
  UINT res = 0;
  UCHAR _DSPTR * curChar = LexIdent; 

  // process a Hex number
  for(;; ++curChar)
  {
    if (TST_CHAR( *curChar, CT_DIGIT ))
      res = (res << 4) + *curChar - '0';
    else
    if (TST_CHAR( *curChar, CT_AF_UP ))
      res = (res << 4) + *curChar - 'A' + 10;
    else
    if (TST_CHAR( *curChar, CT_AF_LOW ))
      res = (res << 4) + *curChar - 'a' + 10;
    else
      break;
  }

  return res;
};

static UINT otol ( void )
{
  UINT res = 0;
  UCHAR _DSPTR * curChar = LexIdent; 

  // process a octal
  for(;*curChar; ++curChar)
  {
    if (!TST_CHAR( *curChar, CT_DIGIT ) || *curChar > '7')
    {
      PrintError( ERR_INV_OCT_NUM );
      return 0;
    }
    res = (res << 3) + *curChar - '0';
  }
  return res;
};

static UINT btol ( void )
{
  UINT res = 0;
  UCHAR _DSPTR * curChar = LexIdent; 

  // process a binary
  for(;*curChar; ++curChar)
  {
    if (*curChar > '1')
    {
      PrintError( ERR_INV_BIN_NUM );
      return 0;
    }
    res = (res << 1) + *curChar - '0';
  }
  return res;
};

static UINT dtol ( void )
{
  UINT res = 0;
  UCHAR _DSPTR * curChar = LexIdent; 

  // process a octal
  for(;*curChar; ++curChar)
  {
    if (!TST_CHAR( *curChar, CT_DIGIT ))
    {
      PrintError( ERR_INV_DEC_NUM );
      return 0;
    }
    res = (res * 10) + *curChar - '0';
  }
  return res;
};

//--------------------------------------------------------------------------
// Name         GetNumber
//
// Description  
//--------------------------------------------------------------------------
static int GetNumber ( void )
{
  int   curLen;
  BYTE _DSPTR * curChar = CurCharPtr;

  curLen = 0;
  do
  {
    if (curLen < 20)
    {
      LexIdent[curLen] = *curChar;
      ++curLen;
    }
    ++curChar;
  }
  while (TST_CHAR( *curChar, CT_XDIGIT ));
  LexIdent[curLen] = 0;

  if (*curChar == 'h' || *curChar == 'H')
  {
    ++curChar;
    LexNumber = htol();
  }
  else
  if (*curChar == 'q' || *curChar == 'Q' ||
      *curChar == 'o' || *curChar == 'O')
  {
    ++curChar;
    LexNumber = otol();
  }
  else
  if (LexIdent[curLen - 1] == 'b' || LexIdent[curLen - 1] == 'B')
  {
    LexIdent[curLen - 1] = 0;
    LexNumber = btol();
  }
  else
  if (LexIdent[curLen - 1] == 'd' || LexIdent[curLen - 1] == 'D')
  {
    LexIdent[curLen - 1] = 0;
    LexNumber = dtol();
  }
  else
    LexNumber = dtol();

  LexNumber &= 0xFFFF; // truncate to word

  CurCharPtr = curChar;
  return LEX_NUMBER;
};

//--------------------------------------------------------------------------
// Name         ProcessSymbol
//
// Description 
//--------------------------------------------------------------------------
static int ProcessSymbol ( void )
{
  UCHAR _DSPTR * curChar = CurCharPtr;

  if (curChar[0] == '>' && curChar[1] == '=')
  {
    CurCharPtr += 2;
    return LEX_GE;
  }
  if (curChar[0] == '<' && curChar[1] == '=')
  {
    CurCharPtr += 2;
    return LEX_LE;
  }
  if (curChar[0] == '<' && curChar[1] == '>')
  {
    CurCharPtr += 2;
    return LEX_NE;
  }
  ++CurCharPtr;
  return *curChar;
};

//--------------------------------------------------------------------------
// Name         LexFreeString
//
// Description  Frees the allocated string buffer (if it is allocated)
//--------------------------------------------------------------------------
static INLINE void LexFreeString ( void )
{
  ASSERT( LexString );
  if (LexString != LexStringBuf)
  {
    xfree( LexString );
    LexString = LexStringBuf;
  }
};

void MiniLex_Init ( void )
{
  LexString = LexStringBuf;
};

void MiniLex_Done ( void )
{
  LexFreeString();
};

UINT CalcStringHash ( const UCHAR * str )
{
  UINT value;
  const UCHAR * s = str;

  for( value = 0; *str; ++str )
  {
    value ^= *str;
    value = WORD_ROTL( value );
  }
  value += *s;

  return value;
};


int GetString ( void )
{
  int endCh = *CurCharPtr;

  ++CurCharPtr;
  LexStringLen = 0;
  CurStringSize = STRING_BUF_SIZE;

  while (*CurCharPtr != endCh)
  {
    if (*CurCharPtr == 0)
    {
      PrintError( ERR_OPEN_STRING );
      break;
    }
    AddToString( *CurCharPtr++ );
  }
  ++CurCharPtr; // skip the ending char

  return LEX_STRING;
};


//--------------------------------------------------------------------------
// Name         MiniGetLex
//
// Description  Get a token from the input string starting from CurCharPtr.
//              The input string must be zero-ended.
//              Returns the type of the token updating CurCharPtr as
//              necessary to point after the token.
//              Updates some global variables and buffers (defined above)
//              with additional info about the token.
//--------------------------------------------------------------------------
int MiniGetLex ( void )
{
  UCHAR _DSPTR * curChar = CurCharPtr;     // cache of CurCharPtr

  LexFreeString();      // free the last string

  while (TST_CHAR( *curChar, CT_SPACE ))  // skip blanks        
    ++curChar;

  CurCharPtr = curChar;                   // update CurCharPtr

  switch (*curChar)
  {
    case 0:
    case ';':
      return LEX_EOL;

    case '\'':
    case '"':
      return GetString();

    default:
      if (IS_IDENT_START( *curChar ))
        return GetIdent();
      if (TST_CHAR( *curChar, CT_DIGIT ))                     
        return GetNumber();
      return ProcessSymbol();
  }
};

//--------------------------------------------------------------------------
// Name         MiniGetLexSkip
//
// Description  MiniGetLex but when we are in skipping mode. In this case
//              we care only for the first toke. If it is identifier we
//              might proceed ....
//--------------------------------------------------------------------------
int MiniGetLexSkip ( void )
{
  UCHAR _DSPTR * curChar = CurCharPtr;     // cache of CurCharPtr

  LexFreeString();      // free the last string

  while (TST_CHAR( *curChar, CT_SPACE ))  // skip blanks        
    ++curChar;

  CurCharPtr = curChar;                   // update CurCharPtr

  return IS_IDENT_START( *curChar ) ? GetIdent() : LEX_EOL;
};
