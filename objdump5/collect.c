/* collect.c */

#include <malloc.h>
#include "common/global.h"
#include "collect.h"

/* ************************************************************************
   Function: InitCollect
   Created: 12th of April, 1997; 15:13
   Description:
     Initializes a collection of pointers.
     Delta parameter describes by how much to increasy the array chunk
*/
BOOL InitCollect(struct Collect *p, WORD delta)
{
  p->delta = delta * sizeof(void *);
  p->items = malloc(p->delta);
  if (p->items == NULL)
    return (FALSE);
  p->limit = p->delta;
  p->cnt = 0;
  return (TRUE);
};

/* ************************************************************************
   Function: InsertItem
   Created: 12th of April, 1997; 15:14
   Description:
     Add a pointer to collection of pointers
*/
BOOL InsertItem(struct Collect *p, void *i)
{
  void *t;

  if (p->cnt * sizeof(void *) >= p->limit) {
    /* if (realloc(p->items, p->limit + p->delta) == NULL)
      return (FALSE); */
    /* realloc makes problems on DJGPP */
    if ((t = (void *)malloc(p->limit += p->delta)) == NULL)
      return (FALSE);
    memcpy(t, p->items, p->limit);
    free(p->items);
    p->items = t;
  };
  p->items[p->cnt] = i;
  p->cnt++;
  return (TRUE);
};

/* ************************************************************************
   Function: AtItem
   Created: 12th of April, 1997; 15:14
   Description:
     Give me the value of item x
*/
void *AtItem(struct Collect *p,	UINT number)
{
  return (p->items[number]);
};

/* ************************************************************************
   Function: FreeAll
   Created: 12th of April, 1997; 15:14
   Description:
     Desposes all the items from the heap
*/
void FreeAll(struct Collect *p)
{
  UINT i;

  for (i = 0; i < p->cnt; i++)
    free(p->items[i]);
};

