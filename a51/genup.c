/*
  uptab: generate the upcase table
*/
#include <stdio.h>
#include <ctype.h>

int main ( void )
{
  int i;
  printf( "static UCHAR UpTable[128] =\n{\n" );
  for( i = 0; i < 128; ++i )
  {
    if (i < 32 || i > 127 || i == '\'' || i == '\\')
      printf( "  %d,\n", i );
    else
      printf( "  '%c',\n", toupper( i ) );
  }
  printf( "};\n" );

  return 0;
};
