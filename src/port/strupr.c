#include <ctype.h>

char * strupr ( char * s )
{
  char * res = s;
  for ( ;*s; ++s )
    *s = toupper( *s );
  return res;  
};