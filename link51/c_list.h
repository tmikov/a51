/* clist.h */

struct CList
{
  struct CList *back;
};

struct CListDesc
{
  struct CList stub;
  struct CList *head;
  UINT cnt;
  UINT isize;  /* item size */
  UINT delta;
};

void Init_CList(struct CListDesc *p, UINT isize, UINT delta);
void InsertItem_CList(struct CListDesc *p, void *item);
void FreeAll_CList(struct CListDesc *p);
struct CList *GetHead_CList(struct CListDesc *p);
void *GetData_CList(struct CList *p);
BOOL EO_CList(struct CList *x);

