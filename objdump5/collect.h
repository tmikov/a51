/* collect.h */

#ifndef COLLECT
#define COLLECT

#include "global.h"

struct Collect {
  void **items;
  UINT cnt;
  WORD delta;
  UINT limit;
};

BOOL InitCollect(struct Collect *p, WORD delta);
BOOL InsertItem(struct Collect *p, void *i);
void *AtItem(struct Collect *p,	UINT number);
void FreeAll(struct Collect *p);
#endif

