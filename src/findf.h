#ifndef FINDF_H
#define FINDF_H

#include <sys/stat.h>

typedef struct FF_DAT FF_DAT;

// mode passed to findopen and returned in findfilestruct by findfile
#define FFIND_FILES     1      
#define FFIND_DIRS      2

struct findfilestruct
{
  int    mode;
  char   filename[FILENAME_MAX+1];
  struct stat st;
};

#ifdef __cplusplus
extern "C"
#else
extern
#endif
int case_sensitive_match;


#ifdef __cplusplus
extern "C" {
#endif

int      match_wildarg ( const char * s, const char * p );
FF_DAT * find_open ( const char * pattern, int mode );
int      find_file ( FF_DAT * pDat, struct findfilestruct * res );
void     find_close ( FF_DAT * pDat );

#ifdef __cplusplus
};
#endif


#endif // FINDF_H

