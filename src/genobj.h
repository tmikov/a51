void Obj_Init ( void );
void Obj_Done ( void );
void Obj_OpenFile ( void );
void Obj_CloseFile ( void );
void Obj_PutNames ( void );
void Obj_PutSegments ( void );
void Obj_PutExterns ( void );
void Obj_PutPublics ( void );
void Obj_StartAt ( UINT codeAddr );
void Obj_UseSpace ( UINT len );
void Obj_Reserve ( UINT len );
void Obj_PutCodeBytes ( const BYTE * buf, UINT len );
void Obj_PutFixUp ( int offset, UINT fixType, UINT srcType, UINT srcData,
                    UINT dOffset );

extern FILE * ObjF;

