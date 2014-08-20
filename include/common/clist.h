//
//  Doubly-linked list manipulation routines.  Implemented as macros
//  but logically these are procedures.
//
// Originally contained in NTDDK.H
// Borrowed && edited by Ceco
//

#ifndef CLIST_H
#define CLIST_H

typedef struct TListEntry
{
  struct TListEntry * Flink, * Blink;
} TListEntry, * PListEntry;

#define TListRoot TListEntry

typedef struct TSingleListEntry
{
  struct TSingleListEntry * Next;
} TSingleListEntry, * PSingleListEntry;

typedef struct TSingleListRoot
{
  TSingleListEntry * pFirst, * pLast;
} TSingleListRoot, * PSingleListRoot;


#define GET_LIST_ITEM( type, link, entry )  \
  ((type *)((BYTE *)(entry) - offsetof( type, link )))


//
//  void
//  INITIALIZE_LIST_HEAD
//      PListEntry ListHead
//      );
//

#define INITIALIZE_LIST_HEAD(ListHead) (\
    (ListHead)->Flink = (ListHead)->Blink = (ListHead))

//
//  BOOLEAN
//  IS_LIST_EMPTY(
//      PListEntry ListHead
//      );
//

#define IS_LIST_EMPTY(ListHead) \
    ((ListHead)->Flink == (ListHead))

//
//  BOOLEAN
//  END_OF_LIST(
//      PListEntry ListHead,
//      PListEntry Entry
//      );
//
#define END_OF_LIST(ListHead, Entry) \
  ((PListEntry)(Entry) == (ListHead))

//
//  PListEntry
//  REMOVE_HEAD_LIST(
//      PListEntry ListHead
//      );
//

#define REMOVE_HEAD_LIST(ListHead) \
    (ListHead)->Flink;\
    {REMOVE_ENTRY_LIST((ListHead)->Flink)}

//
//  PListEntry
//  REMOVE_TAIL_LIST(
//      PListEntry ListHead
//      );
//

#define REMOVE_TAIL_LIST(ListHead) \
    (ListHead)->Blink;\
    {REMOVE_ENTRY_LIST((ListHead)->Blink)}

//
//  void
//  REMOVE_ENTRY_LIST(
//      PListEntry Entry
//      );
//

#define REMOVE_ENTRY_LIST(Entry) {\
    PListEntry _EX_Blink;\
    PListEntry _EX_Flink;\
    _EX_Flink = (Entry)->Flink;\
    _EX_Blink = (Entry)->Blink;\
    _EX_Blink->Flink = _EX_Flink;\
    _EX_Flink->Blink = _EX_Blink;\
    }

//
//  void
//  INSERT_TAIL_LIST(
//      PListEntry ListHead,
//      PListEntry Entry
//      );
//

#define INSERT_TAIL_LIST(ListHead,Entry) {\
    PListEntry _EX_Blink;\
    PListEntry _EX_ListHead;\
    _EX_ListHead = (ListHead);\
    _EX_Blink = _EX_ListHead->Blink;\
    (Entry)->Flink = _EX_ListHead;\
    (Entry)->Blink = _EX_Blink;\
    _EX_Blink->Flink = (Entry);\
    _EX_ListHead->Blink = (Entry);\
    }

//
//  void
//  INSERT_HEAD_LIST(
//      PListEntry ListHead,
//      PListEntry Entry
//      );
//

#define INSERT_HEAD_LIST(ListHead,Entry) {\
    PListEntry _EX_Flink;\
    PListEntry _EX_ListHead;\
    _EX_ListHead = (ListHead);\
    _EX_Flink = _EX_ListHead->Flink;\
    (Entry)->Flink = _EX_Flink;\
    (Entry)->Blink = _EX_ListHead;\
    _EX_Flink->Blink = (Entry);\
    _EX_ListHead->Flink = (Entry);\
    }

//
//
//  PSingleListEntry
//  POP_ENTRY_LIST(
//      PSingleListEntry ListHead
//      );
//

#define POP_ENTRY_LIST(ListHead) \
    (ListHead)->Next;\
    {\
        PSingleListEntry FirstEntry;\
        FirstEntry = (ListHead)->Next;\
        if (FirstEntry != NULL) {     \
            (ListHead)->Next = FirstEntry->Next;\
        }                             \
    }


//
//  void
//  PUSH_ENTRY_LIST(
//      PSingleListEntry ListHead,
//      PSingleListEntry Entry
//      );
//

#define PUSH_ENTRY_LIST(ListHead,Entry) \
    (Entry)->Next = (ListHead)->Next; \
    (ListHead)->Next = (Entry)


//
//  void
//  INIT_SLIST_ROOT(
//      PSLIST_ROOT listRoot
//      );
//

#define INIT_SLIST_ROOT( listRoot ) \
  ((listRoot)->pFirst = (listRoot)->pLast = NULL)

//
//  BOOLEAN
//  IS_SLIST_EMPTY(
//      PSLIST_ROOT listRoot
//      );
//

#define IS_SLIST_EMPTY( listRoot ) \
  ((listRoot)->pFirst == NULL)


//
//  void
//  ADD_TAIL_SLIST(
//      PSLIST_ROOT  listRoot,
//      PSTListEntry entry
//      );
//

#define ADD_TAIL_SLIST( listRoot, entry )   \
  do {                                    \
    if ((listRoot)->pLast)                \
      (listRoot)->pLast->pNext = (entry); \
    else                                  \
      (listRoot)->pFirst = (entry);       \
    (listRoot)->pLast = (entry);          \
    (entry)->pNext = NULL;                \
  } while (0)


//
//  PListEntry
//  REMOVE_HEAD_SLIST(
//      PSLIST_ROOT  listRoot
//      );
//

#define REMOVE_HEAD_SLIST( listRoot )     \
  (listRoot)->pFirst;                     \
  {                                       \
    if (((listRoot)->pFirst = (listRoot)->pFirst->pNext) == NULL)  \
      (listRoot)->pLast = NULL;                                    \
  }

#ifdef __cplusplus

template <class Listed>
class CListRoot : public TListEntry
{
  TListEntry m_root;
public:
  void Initialize ( void )
    { INITIALIZE_LIST_HEAD( &m_root ); };
  BOOLEAN IsEmpty ( void ) const
    { return IS_LIST_EMPTY( &m_root ); };
  BOOLEAN IsAtEnd ( Listed * entry ) const
    { return END_OF_LIST( &m_root, entry ); };
  Listed * RemoveHead ( void )
    { Listed * pRes = (Listed *)REMOVE_HEAD_LIST( &m_root ); return pRes; };
  Listed * RemoveTail ( void )
    { Listed * pRes = (Listed *)REMOVE_TAIL_LIST( &m_root ); return pRes; };
  void InsertTail ( Listed * entry )
    { INSERT_TAIL_LIST( &m_root, entry ); };
  void InsertHead ( Listed * entry )
    { INSERT_HEAD_LIST( &m_root, entry ); };

  Listed * Head ( void ) const
    { return (Listed *)m_root.Flink; };
  Listed * Tail ( void ) const
    { return (Listed *)m_root.Blink; };
};

//
// All list items must inherit public from this class
// e.g: "class CItem : public CListEntry<CItem>"
//
template <class Listed>
class CListEntry : public TListEntry
{
public:
  Listed * Prev ( void )         { return (Listed *)Flink; };
  Listed * Next ( void )         { return (Listed *)Blink; };
  void     RemoveListEntry ( void )  { REMOVE_ENTRY_LIST( this ); };

  friend class CListRoot<Listed>;
};

#endif // __cplusplus

#endif // CLIST_H


