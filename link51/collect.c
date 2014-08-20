/* collect.c */

#include "common/global.h"
#pragma hdrstop

#include "collect.h"
#include "err.h"
#include "common/misc.h"

/* ************************************************************************
   Function: InitCollect
   Description:
     Initializes a collection of pointers.
     Delta parameter describes by how much to increace the array chunk
*/
void InitCollect(struct Collect *p, UINT delta)
{
  ASSERT(p != NULL);
  ASSERT(delta != 0);

  p->delta = delta * sizeof(void *);
  p->items = xmalloc(p->delta);
  p->limit = p->delta;
  p->cnt = 0;
}

/* ************************************************************************
   Function: ExpandCollection
   Description:
     Expands a collection by size of delta
*/
void ExpandCollection(struct Collect *p)
{
  void *t;

  /* Expand the pointers array by size of delta */
  t = xmalloc(p->limit + p->delta);
  memcpy(t, p->items, p->limit);
  p->limit += p->delta;
  xfree(p->items);
  p->items = t;
}

/* ************************************************************************
   Function: InsertItem
   Description:
     Add a pointer to collection of pointers
*/
void InsertItem(struct Collect *p, const void *i)
{
  ASSERT(p != NULL);
  ASSERT(i != NULL);
  /*VALIDATE_HEAP_PTR(p->items);*/

  if (p->cnt * sizeof(void *) >= p->limit)
    ExpandCollection(p);
  p->items[p->cnt] = (void *)i;  /* Add	the item at the end of the pointers array */
  p->cnt++;
}

/* ************************************************************************
   Function: AtInsertItem
   Description:
     Inserts an item at a specific position
*/
void AtInsertItem(struct Collect *p, const void *i, UINT index)
{
  ASSERT(p != NULL);
  ASSERT(i != NULL);
  /*VALIDATE_HEAP_PTR(p->items);*/
  ASSERT(index <= p->cnt);

  if (index == p->cnt)
  {
    InsertItem(p, i);
    return;
  }

  if (p->cnt * sizeof(void *) >= p->limit)
    ExpandCollection(p);

  /* Make room to insert a pointer */
  memmove(&((char **)p->items)[index + 1], &((char **)p->items)[index],
    (p->cnt - index) * sizeof(char **));

  p->items[index] = (void *)i;  /* unconst */
  p->cnt++;
}

/* ************************************************************************
   Function: AtRemoveItem
   Description:
     Removes an item from a specific position
*/
void AtRemoveItem(struct Collect *p, UINT index)
{
  ASSERT(p != NULL);
  /*VALIDATE_HEAP_PTR(p->items);*/
  ASSERT(index <= p->cnt);

  /* Remove the pointer from position index */
  memmove(&((char **)p->items)[index], &((char **)p->items)[index + 1],
    (p->cnt - (index + 1)) * sizeof(char **));

  --p->cnt;
}

/* ************************************************************************
   Function: SearchItem
   Description:
     Searches for an item in the collection
*/
BOOL SearchItem(const struct Collect *p, const void *Key, int *Index,
  int (*fcmp)(const void*, const void*))
{
  int l, h, i, c;
  BOOL f;

  f = FALSE;

  l = 0;
  h = p->cnt - 1;
  while (l <= h)
  {
    i = (l + h) / 2;
    c = (fcmp)(p->items[i], Key);
    if (c < 0)
      l = i + 1;
    else
    {
      h = i - 1;
      if (c == 0)
      {
        f = TRUE;
	l = i;
      }
    }
  }
  *Index = l;
  return (f);
}

/* ************************************************************************
   Function: InsertItemSorted
   Description:
     Inserts new item into a sorted collection.
*/
void InsertItemSorted(struct Collect *p, const void *item,
  int (*fcmp)(const void*, const void*))
{
  int i;

  if (!SearchItem(p, item, &i, fcmp))
    AtInsertItem(p, item, i);
  else
    InsertItem(p, item);
}

/* ************************************************************************
   Function: AtItem
   Description:
     Give me the value of item x
*/
void *AtItem(const struct Collect *p, UINT number)
{
  ASSERT(p != NULL);
  ASSERT(number < p->cnt);
  /*VALIDATE_HEAP_PTR(p->items);*/
  /*VALIDATE_HEAP_PTR(p->items[number]);*/

  return (p->items[number]);
}

/* ************************************************************************
   Function: FreeAll
   Description:
     Disposes all the items from the heap
*/
void FreeAll(struct Collect *p)
{
  UINT i;

  ASSERT(p != NULL);
  /*VALIDATE_HEAP_PTR(p->items);*/

  for (i = 0; i < p->cnt; ++i)
    xfree(p->items[i]);

  p->cnt = 0;
}

/* ************************************************************************
   Function: ShutDownCollect
   Description:
     Disposes the collection array from the heap
*/
void ShutDownCollect(struct Collect *p)
{
  ASSERT(p->cnt == 0);

  xfree(p->items);
}

