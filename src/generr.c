/*
  generr: generates the error message array.
*/

#include "global.h"

int numDefs = 0;
char defs[1024][32];

int main ( void )
{
  char outbuf[512];
  char buf[512];
  char * p, * e;
  int i;

  printf( "#ifdef ERR_C\n\n" );
  printf( "static const char _DSPTR * ErrMsg[] =\n{\n" );
  while (p = gets( buf ))
  {
    // skip the leading spaces
    while (isspace( *p ))
      ++p;

    // skip the line if comment
    if (p[0] == '/' && p[1] == '/')
      continue;

    // skip the empty or invalid lines
    if (!isalpha( *p ) && *p != '_')
      continue;

    ASSERT( numDefs < _countof( defs ) );

    i = 0;
    do
    {
      defs[numDefs][i++] = *p++;
    }
    while (isalnum( *p ) || *p == '_');
    defs[numDefs][i] = 0;
    ++numDefs;

    // skip the spaces
    while (isspace( *p ))
      ++p;

    // if a string does not follow ignore the line
    if (*p != '"')
    {
ignore:
      --numDefs;
      continue;
    }

    e = strchr( p + 1, '"' );
    if (e == NULL)
      goto ignore;

    e[1] = 0;

    sprintf( outbuf, "/* %3d */ %s,", numDefs - 1, p );
    while (strlen( outbuf ) < 65)
      strcat( outbuf, " " );
    printf( "%s // %s\n", outbuf, defs[numDefs-1] );
  }

  printf( "};\n\n" );
  printf( "#endif // ERR_C\n\n" );

  for( i = 0; i < numDefs; ++i )
  {
    sprintf( outbuf, "#define %s", defs[i] );
    while (strlen( outbuf ) < 41)
      strcat( outbuf, " " );
    printf( "%s%3d\n", outbuf, i );
  }

  return 0;
};
                                                                                
