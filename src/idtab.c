#include "global.h"
#pragma hdrstop
#include "a51.h"

//////////////////////////////////////////////////////////////////////////
// Global data

TIdent * pFirstIdent,  // keep track of all idents here
       * pLastIdent;

//////////////////////////////////////////////////////////////////////////
// Local data

#define HASH_TABLE_SIZE       8192
#define HASH_TABLE_MASK     0x1FFF

struct THashElem
{
  TIdent * pLast;
};

static struct THashElem HashTable[HASH_TABLE_SIZE];

#ifdef HASH_DEBUG
static UINT HashCol = 0; // number of hash collisions
#endif

////////////////////////////////////////////////////////////////////////
// Global functions

// Hash support ////////////////////////////////////////////////////////

//--------------------------------------------------------------------------
// Name         Init_IdentTable
//
// Description  Prepare the hash table for use
//--------------------------------------------------------------------------
void IdentTable_Init ( void )
{
// Here fix a bug in Borland C++ 5.0 for DOS (sizeof( HashTable ) == 32768)
  memset( HashTable, 0, sizeof( HashTable ) - 1 );
// to avoid the integer overflow we clear the last element by hand:
  memset( &HashTable[_countof( HashTable ) - 1], 0, sizeof( HashTable[0] ) );

  pFirstIdent = pLastIdent = NULL;
};

//--------------------------------------------------------------------------
// Name         Done_IdentTable
//
// Description  Free all idents
//--------------------------------------------------------------------------
void IdentTable_Done ( void )
{
#ifdef HASH_DEBUG
  UINT hashUsed, longCol, numIdents;
  UINT hash;

  // make some hash table statistics
  numIdents =
  longCol  =
  hashUsed = 0;
  for( hash = 0; hash < _countof( HashTable ); ++hash )
  {
    TIdent * p;
    UINT   c;

    hashUsed += HashTable[hash].pLast != NULL;
    // count collision length
    for( c = 0, p = HashTable[hash].pLast; p; ++numIdents, ++c, p = p->pNextHash );
    if (c > longCol)
      longCol = c;
  }

  TRACE0( "\n\n--------- Hash table statistics ---------\n" );
  TRACE1( "Hash table collisions on add: %u\n", HashCol );
  TRACE1( "Hash table size             : %u\n", _countof( HashTable ) );
  TRACE1( "Hash table used slots       : %u\n", hashUsed );
  TRACE1( "Number of idents            : %u\n", numIdents );
  TRACE1( "Longest hash collision      : %u\n", longCol );
#endif

  while (pFirstIdent)
  {
    TIdent * p = pFirstIdent->pNextIdent;
    FreeTIdent( pFirstIdent );
    pFirstIdent = p;
  }
};

//--------------------------------------------------------------------------
// Name         AllocTIdent
//
// Description  Allocate a TIdent structure and fill it with
//              (Allocates a string for the name)
//--------------------------------------------------------------------------
TIdent * AllocTIdent ( UINT len, const UCHAR * name, UINT hash )
{
  TIdent * pIdent;

  pIdent = xmalloc( sizeof( TIdent ) );
  memset( pIdent, 0, sizeof( *pIdent ) );
  pIdent->szName = AllocString( len, (char *)name );
  pIdent->HashVal = hash;

  // insert it in the list of idents
  if (pLastIdent)
    pLastIdent->pNextIdent = pIdent;
  else
    pFirstIdent = pIdent;
  pLastIdent = pIdent;

  return pIdent;
};

//--------------------------------------------------------------------------
// Name         FreeTIdent
//
// Description  Deallocate a TIdent structure allocated with AllocTIdent.
//              Frees the string allocated for the name also.
//--------------------------------------------------------------------------
void FreeTIdent ( TIdent * pIdent )
{
  if (pIdent)
  {
    FreeString( pIdent->szName );
    xfree( pIdent );
  }
};

//--------------------------------------------------------------------------
// Name         FindIdent
//
// Description  ....
//--------------------------------------------------------------------------
TIdent * FindIdent ( const UCHAR * szName, UINT hashVal )
{
  TIdent * pIdent;
  // If any identifiers with that hash...
  if (pIdent = HashTable[hashVal & HASH_TABLE_MASK].pLast)
  {
    // find the one with the exact match
    do
    {
      if (pIdent->HashVal == hashVal && strcmp( pIdent->szName, (char *)szName ) == 0)
        return pIdent;
    }
    while (pIdent = pIdent->pNextHash);
  }
  return pIdent;
};

//--------------------------------------------------------------------------
// Name         AddIdent
//
// Description  ...
//--------------------------------------------------------------------------
void AddIdent ( TIdent * pIdent )
{
  pIdent->pNextHash = HashTable[pIdent->HashVal & HASH_TABLE_MASK].pLast;
  HashTable[pIdent->HashVal & HASH_TABLE_MASK].pLast = pIdent;
#ifdef HASH_DEBUG
  // increase the collision counter if another ident with same hash
  HashCol += pIdent->pNextHash != NULL;
#endif
};

//--------------------------------------------------------------------------
// Name         RemoveIdent
//
// Description  Removes an ident from the hash table (but does not free
//              the structure).
//              MUST be the last ident of the chain.
//--------------------------------------------------------------------------
void RemoveIdent ( TIdent * pIdent )
{
  if (pIdent)
  {
    ASSERT( HashTable[pIdent->HashVal & HASH_TABLE_MASK].pLast == pIdent );
    HashTable[pIdent->HashVal & HASH_TABLE_MASK].pLast = pIdent->pNextHash;
  }
};

