#include <ctype.h>

char * strlwr ( char * s )
{
  char * res = s;
  for ( ;*s; ++s )
    *s = tolower( *s );
  return res;
};