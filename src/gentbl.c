/*
  gentbl.c : generates the character classification table
*/

#include <stdio.h>
#include <ctype.h>

/*
#define CT_DIGIT     1  // 0 - 9
#define CT_XDIGIT    2  // 0-9, a-f, A-F
#define CT_AF_LOW    4  // a-f
#define CT_AF_UP     8  // A-F
#define CT_SPACE    16  // 0x09 - 0x0D, 0x20
#define CT_ID_START 32  // letter, _, ?, $
#define CT_IDENT    64  // CT_ID_START | CT_DIGIT
*/

#define PUT( c, x )  if (c) { if (has++) printf( " | " ); printf( x ); }


int main ( void )
{
  int i;
  int has;

  printf( "BYTE CharTable[256] =\n{\n" );

  for( i = 0; i < 256; ++i )
  {
    has = 0;

    printf( "  " );
    PUT( isdigit( i ), "CT_DIGIT" );
    PUT( isxdigit( i ), "CT_XDIGIT"  );
    PUT( (i >= 'a' && i <= 'f') , "CT_AF_LOW" );
    PUT( (i >= 'A' && i <= 'F') , "CT_AF_UP" );
    PUT( isspace( i ) , "CT_SPACE" );
    PUT( isalpha( i ) || i == '_' || i == '?' || i == '$', "CT_ID_START" );
    PUT( isalnum( i ) || i == '_' || i == '?' || i == '$', "CT_IDENT" );

    if (!has)
      printf( "0" );
    putchar( i == 255 ? ' ' : ',' );
    printf( " // '%c' = %d\n", i > 31 && i < 128 ? i : '.', i );
  }
  printf( "};\n" );
  return 0; // Signal MAKE that operation completed successfully
};
