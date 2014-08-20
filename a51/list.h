//
//  Doubly-linked list manipulation routines.  Implemented as macros
//  but logically these are procedures.
//
// Originally contained in NTDDK.H
// Borrowed && edited by Ceco
//

#ifndef LIST_H
#define LIST_H

#if !defined(_WINNT_)                //WINNT.H defines these structures
typedef struct LIST_ENTRY
{
  struct LIST_ENTRY * Flink, * Blink;
} LIST_ENTRY, * PLIST_ENTRY;

typedef struct SINGLE_LIST_ENTRY
{
  struct SINGLE_LIST_ENTRY * Next;
} SINGLE_LIST_ENTRY, * PSINGLE_LIST_ENTRY;
#endif

typedef struct SLIST_ENTRY
{
  struct SLIST_ENTRY * pNext;
} SLIST_ENTRY, * PSLIST_ENTRY;

typedef struct SLIST_ROOT
{
  PSLIST_ENTRY pFirst, pLast;
} SLIST_ROOT, * PSLIST_ROOT;


#define GET_LIST_ITEM( type, link, entry )  \
  ((type *)((BYTE *)(entry) - offsetof( type, link )))


//
//  void
//  InitializeListHead(
//      PLIST_ENTRY ListHead
//      );
//

#define InitializeListHead(ListHead) (\
    (ListHead)->Flink = (ListHead)->Blink = (ListHead))

//
//  BOOL
//  IsListEmpty(
//      PLIST_ENTRY ListHead
//      );
//

#define IsListEmpty(ListHead) \
    ((ListHead)->Flink == (ListHead))

//
//  BOOL
//  EndOfList(
//      PLIST_ENTRY ListHead,
//      PLIST_ENTRY Entry
//      );
//
#define EndOfList(ListHead, Entry) \
  ((PLIST_ENTRY)(Entry) == (ListHead))

//
//  PLIST_ENTRY
//  RemoveHeadList(
//      PLIST_ENTRY ListHead
//      );
//

#define RemoveHeadList(ListHead) \
    (ListHead)->Flink;\
    {RemoveEntryList((ListHead)->Flink)}

//
//  PLIST_ENTRY
//  RemoveTailList(
//      PLIST_ENTRY ListHead
//      );
//

#define RemoveTailList(ListHead) \
    (ListHead)->Blink;\
    {RemoveEntryList((ListHead)->Blink)}

//
//  void
//  RemoveEntryList(
//      PLIST_ENTRY Entry
//      );
//

#define RemoveEntryList(Entry) {\
    PLIST_ENTRY _EX_Blink;\
    PLIST_ENTRY _EX_Flink;\
    _EX_Flink = (Entry)->Flink;\
    _EX_Blink = (Entry)->Blink;\
    _EX_Blink->Flink = _EX_Flink;\
    _EX_Flink->Blink = _EX_Blink;\
    }

//
//  void
//  InsertTailList(
//      PLIST_ENTRY ListHead,
//      PLIST_ENTRY Entry
//      );
//

#define InsertTailList(ListHead,Entry) {\
    PLIST_ENTRY _EX_Blink;\
    PLIST_ENTRY _EX_ListHead;\
    _EX_ListHead = (ListHead);\
    _EX_Blink = _EX_ListHead->Blink;\
    (Entry)->Flink = _EX_ListHead;\
    (Entry)->Blink = _EX_Blink;\
    _EX_Blink->Flink = (Entry);\
    _EX_ListHead->Blink = (Entry);\
    }

//
//  void
//  InsertHeadList(
//      PLIST_ENTRY ListHead,
//      PLIST_ENTRY Entry
//      );
//

#define InsertHeadList(ListHead,Entry) {\
    PLIST_ENTRY _EX_Flink;\
    PLIST_ENTRY _EX_ListHead;\
    _EX_ListHead = (ListHead);\
    _EX_Flink = _EX_ListHead->Flink;\
    (Entry)->Flink = _EX_Flink;\
    (Entry)->Blink = _EX_ListHead;\
    _EX_Flink->Blink = (Entry);\
    _EX_ListHead->Flink = (Entry);\
    }

//
//
//  PSINGLE_LIST_ENTRY
//  PopEntryList(
//      PSINGLE_LIST_ENTRY ListHead
//      );
//

#define PopEntryList(ListHead) \
    (ListHead)->Next;\
    {\
        PSINGLE_LIST_ENTRY FirstEntry;\
        FirstEntry = (ListHead)->Next;\
        if (FirstEntry != NULL) {     \
            (ListHead)->Next = FirstEntry->Next;\
        }                             \
    }


//
//  void
//  PushEntryList(
//      PSINGLE_LIST_ENTRY ListHead,
//      PSINGLE_LIST_ENTRY Entry
//      );
//

#define PushEntryList(ListHead,Entry) \
    (Entry)->Next = (ListHead)->Next; \
    (ListHead)->Next = (Entry)


//
//  void
//  InitListRoot(
//      PSLIST_ROOT listRoot
//      );
//

#define InitSListRoot( listRoot ) \
  ((listRoot)->pFirst = (listRoot)->pLast = NULL)

//
//  BOOL
//  IsSListEmpty(
//      PSLIST_ROOT listRoot
//      );
//

#define IsSListEmpty( listRoot ) \
  ((listRoot)->pFirst == NULL)


//
//  void
//  AddTailSList(
//      PSLIST_ROOT  listRoot,
//      PSLIST_ENTRY entry
//      );
//

#define AddTailSList( listRoot, entry )   \
  do {                                    \
    if ((listRoot)->pLast)                \
      (listRoot)->pLast->pNext = (entry); \
    else                                  \
      (listRoot)->pFirst = (entry);       \
    (listRoot)->pLast = (entry);          \
    (entry)->pNext = NULL;                \
  } while (0)


//
//  PLIST_ENTRY
//  RemoveHeadSList(
//      PSLIST_ROOT  listRoot
//      );
//

#define RemoveHeadSList( listRoot )       \
  (listRoot)->pFirst;                     \
  {                                       \
    if (((listRoot)->pFirst = (listRoot)->pFirst->pNext) == NULL)  \
      (listRoot)->pLast = NULL;                                    \
  }


#endif // LIST_H

