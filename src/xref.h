// from XREF.C

typedef struct TXrefItem
{
  TFileName * file;
  UINT        line;
  struct TXrefItem * prev;
} TXrefItem;

TXrefItem  * AddXref ( TFileName * file, UINT line, TXrefItem * prev );
void       FreeAllXref ( void );
