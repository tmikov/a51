/* collect.h */

#ifndef COLLECT
#define COLLECT

#include "common/global.h"

struct Collect {
  void **items;
  UINT cnt;
  UINT delta;
  UINT limit;
};

void InitCollect(struct Collect *p, UINT delta);
void InsertItem(struct Collect *p, const void *i);
void AtInsertItem(struct Collect *p, const void *i, UINT index);
void AtRemoveItem(struct Collect *p, UINT index);
void *AtItem(const struct Collect *p, UINT number);
BOOL SearchItem(const struct Collect *p, const void *Key, int *Index,
  int (*fcmp)(const void*, const void*));
void InsertItemSorted(struct Collect *p, const void *item,
  int (*fcmp)(const void*, const void*));
void FreeAll(struct Collect *p);
void ShutDownCollect(struct Collect *p);
#endif

