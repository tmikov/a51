// misc.h

extern jmp_buf  ErrJmpBuf;  // used on exit on fatal errors

void    ErrorJump ( int value );
void  * xrealloc ( void * p, UINT size );

#ifdef HEAP_DBG

void  * _dbg_xmalloc ( UINT size, const char * name, UINT line );
void  * _dbg__xmalloc ( UINT size, const char * name, UINT line );
void    _dbg_xfree ( void * p, const char * name, UINT line );
UCHAR * _dbg_AllocString ( UINT len, const UCHAR * str, const char * name, UINT line );
void    _dbg_print_heap ( const char * msg );
void    _dbg_validate_heap_ptr_size ( const void * p, UINT size,
                                      const char * name, UINT line );

#  define xfree( p )           _dbg_xfree( p, __FILE__, __LINE__ )
#  define xmalloc( s )         _dbg_xmalloc( s, __FILE__, __LINE__ )
#  define _xmalloc( s )        _dbg__xmalloc( s, __FILE__, __LINE__ )
#  define AllocString( l, s )  _dbg_AllocString( l, s, __FILE__, __LINE__ )
#  define VALIDATE_HEAP_PTR_SIZE( p, size ) \
     _dbg_validate_heap_ptr_size( p, size, __FILE__, __LINE__ )
#  define VALIDATE_HEAP_PTR( p )  VALIDATE_HEAP_PTR_SIZE( p, 0 )

#else

#  define xfree( p )           free( p )
#  define _xmalloc( s )        malloc( s )
void  * xmalloc ( UINT size );
UCHAR * AllocString ( UINT len, const UCHAR * str );

#define VALIDATE_HEAP_PTR( p )
#define VALIDATE_HEAP_PTR_SIZE( p, size )

#endif

#define FreeString( str )  xfree( str )

