#include "common/global.h"
#pragma hdrstop
#include "a51.h"

/////////////////////////////////////////////////////////////////////////
// String allocation & deallocation.
//
// Later I will implement a faster local heap for the strings.
// Because I'm lazy I use the global heap.

char * AllocString ( int len, const char * str )
{
  return memcpy( xmalloc( len + 1 ), str, len + 1 );
};

void FreeString ( char * str )
{
  free( str );
};

