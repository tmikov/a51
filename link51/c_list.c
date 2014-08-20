/* clist.c */

#include "common/global.h"
#pragma hdrstop

#include "misc.h"
#include "c_list.h"

/*
  CList	is handy to collect data in consequtive way. The items
  could be freed only altogether. New item can be inserted only
  at the end of the list.
  The items are holded in chunks in allocated in the heap.
*/

/* ************************************************************************
   Function: Init_CList
   Description:
     Initializes a CList. Delta	is the thershold value to allocate new
     chunk from the heap.
*/
void Init_CList(struct CListDesc *p, UINT isize, UINT delta)
{
  ASSERT(p != NULL);
  ASSERT(delta != 0);
  ASSERT(isize != 0);

  p->delta = delta;
  p->isize = isize;
  p->stub.back = &p->stub;
  p->head = &p->stub;  /* CList is empty */
  p->cnt = 0;
};

/* ************************************************************************
   Function: InsertItem_CList
   Description:
     Inserts a item in the CList. The item contents is copied in the heap.
*/
void InsertItem_CList(struct CListDesc *p, void *item)
{
  struct CList *b;

  ASSERT(p != NULL);
  ASSERT(p->head != NULL);  /* InitCList should initilize p->head */
  ASSERT(item != NULL);

  if (p->cnt % p->delta == 0)
    /* It is time to allocate memory for the next chunk */
    b = xmalloc(p->delta * (sizeof(struct CList) + p->isize));
  else
  {
    /* Let b points to the succesive memory position */
    b = p->head + 1;  /* Skip CList header */
    (BYTE *)b += p->isize;  /* Skip the last stored item */
  }

  /* Move the item into the heap */
  memcpy(b + 1, item, p->isize);

  b->back = p->head;
  p->head = b;

  ++p->cnt;
}

/* ************************************************************************
   Function: FreeAll_CList
   Description:
     Disposes CList contents from the heap.
*/
void FreeAll_CList(struct CListDesc *p)
{
  void *x;

  ASSERT(p != NULL);
  ASSERT(p->head != NULL);  /* InitCList should initilize p->head */

  while (p->head != &p->stub)
  {
    x = p->head;
    p->head = p->head->back;
    p->cnt--;
    if (p->cnt % p->delta == 0)  /* It it time to free a chunk */
      xfree(x);
  };
};

/* ************************************************************************
   Function: GetHead_CList
   Description:
     Returns the head of CList.
*/
struct CList *GetHead_CList(struct CListDesc *p)
{
  return (p->head);
};

/* ************************************************************************
   Function: GetData_CList
   Description:
     Returns pointer to the data component of a	CList item.
*/
void *GetData_CList(struct CList *p)
{
  ++p;
  return (p);
};

/* ************************************************************************
   Function: EO_CList
   Description:
     Checks for End Of CList.
*/
BOOL EO_CList(struct CList *x)
{
  if (x->back == x)
    return (TRUE);
  return (FALSE);
};

