#include "global.h"
#include "findf.h"
#ifdef HAVE_DIRENT
#include <dirent.h>
#else
#include "dirent.h"
#endif

struct FF_DAT
{
  DIR *  dir;
  int    mode;
  int    dirPathLen;
  char * pszPattern; 
  char   szDirPath[1];
};

int case_sensitive_match = FALSE;

//--------------------------------------------------------------------------
// Name         match_wildarg
//
// Description  Matches wildcard agains a string. Returns TRUE if match ok.
//              The variable case_sensitive_match controls whether the
//              strings are compared exactly.
//--------------------------------------------------------------------------
int match_wildarg ( const char * s, const char * p )
{
  for (;;++p,++s)
  {
    if (*p == '?')
    {
      if (*s == 0)
        return FALSE;
    }
    else
    if (*p == '*')
    {
      // skip consequtive '*'
      do
        ++p;
      while (*p == '*');

      // Fast check the common case of "...*"
      if (*p == 0)
        return TRUE;

      // Try all other characters in s
      for ( ; *s; ++s )
      {
        if (match_wildarg( s, p ))
          return TRUE;
      }
      return FALSE;
    }
    else
    {
      if (case_sensitive_match)
      {
        if (*p != *s)
          return FALSE;
      }
      else
      {
        if (tolower(*p) != tolower(*s))
          return FALSE;
      }

      if (*p == 0)
        return TRUE;
    }
  }
};

//--------------------------------------------------------------------------
// Name         find_open
//
// Description  Initiates a find procedure and returns a find handle.
//              Returns NULL on errors, but not if there are no files
//              matching the pattern.
//              Consequent find_file will find the files with matching
//              _pattern_ and _mode_.
//              _mode_ is a combination of FFIND_xxx flags. All files
//              that match at least one of the flags will be found.
//
// NOTE: Unlike the Win32 functions doesn't search for the first file!
// Unlike Microsoft, we do not have two separate functions to do one thing.
// (Microsoft just don't get it, do they?)
//--------------------------------------------------------------------------
FF_DAT * find_open ( const char * pattern, int mode )
{
  FF_DAT     * pDat;
  DIR        * dir;
  int        flag;
  const char * wild;
  int        wildLen;
  char       * dirPath;
  int        dirLen;

  // Validate the input parameters
  //
  if (pattern == NULL || mode == 0)
  {
    errno = EINVAL;
    return NULL;
  }

  // Split the pattern into dir and pattern
  //
  flag = 0;
  for ( wild = strchr( pattern, 0 ); wild > pattern; --wild )
  {
#if defined(MSDOS) || defined(WIN32)
    if (wild[-1] == '/' || wild[-1] == '\\' || wild[-1] == ':')
#else
    if (wild[-1] == '/')
#endif
    {
      flag = 1;
      break;
    }
  }

  // Open the directory
  //  
  if (flag)  // if have found a directory in the string
  {
    dirLen = wild - pattern;
    if ((dirPath = alloca( dirLen + 1 )) == NULL)
    {
      errno = ENOMEM;
      return NULL;
    }

    memcpy( dirPath, pattern, dirLen );
    dirPath[dirLen] = 0;
  }
  else  // no dir => use current dir
  {
#ifdef MSDOS
    dirPath = ".\\";
#else
    dirPath = "./";
#endif
    dirLen = 0;
  }

  if ((dir = opendir( dirPath )) == NULL)
    return NULL; // error opening dir. opendir() have set errno already

  // Allocate and initialize a FF_DAT structure
  //
  wildLen = strlen( wild );

  if ((pDat = malloc( sizeof( FF_DAT ) - 1 + dirLen + wildLen + 2 )) == NULL)
  {
    closedir( dir );
    errno = ENOMEM;
    return NULL;
  }

  pDat->dir = dir;
  pDat->mode = mode;
  pDat->dirPathLen = dirLen;
  memcpy( pDat->szDirPath, dirPath, dirLen + 1 );
  pDat->pszPattern = pDat->szDirPath + dirLen + 1;
  memcpy( pDat->pszPattern, wild, wildLen + 1 );

  return pDat;
};

//--------------------------------------------------------------------------
// Name         find_file
//
// Description  Finds the next file. Returns 0 if fins a file.
//              Returns -1 on error or if no more files. If no more files
//              sets errno to ENOENT. In other cases sets errno according to
//              the error.
//
// NOTE: This function is used to find the first file and the following files. 
//--------------------------------------------------------------------------
int find_file ( FF_DAT * pDat, struct findfilestruct * res )
{
  struct dirent * de;
  char * filePath;
  int    nameLen;

  ASSERT( pDat );

  errno = ENOENT;

  while (de = readdir( pDat->dir ))
  {
    if (match_wildarg( de->d_name, pDat->pszPattern ))
    {
      nameLen = strlen( de->d_name );
      if ((filePath = alloca( pDat->dirPathLen + nameLen + 1 )) == NULL)
      {
        errno = ENOMEM;
        return -1;
      }

      memcpy( filePath, pDat->szDirPath, pDat->dirPathLen );
      memcpy( filePath + pDat->dirPathLen, de->d_name, nameLen + 1 );

#ifdef __DJGPP__
      {
        int statres;
        unsigned short save_djstat_flags = _djstat_flags;
        _djstat_flags =   _STAT_INODE |
                          _STAT_EXEC_EXT |
                          _STAT_EXEC_MAGIC |
                          _STAT_DIRSIZE |
                          _STAT_ROOT_TIME |
                          _STAT_WRITEBIT;
        statres = stat( filePath, &res->st );
        _djstat_flags = save_djstat_flags;
        if (statres)
          return -1;
      }
#else
      if (stat( filePath, &res->st ))
        return -1;        // some error while performing stat
#endif
      /*
        Check the attributes, using whatever is possible. The
        problem is that some systems define S_IFREG as an exclusion of
        all the rest flags(S_IFREG == 0). Hopefully, such systems have
        the macro S_ISREG() defined...
      */
      res->mode = 0;

      if (pDat->mode & FFIND_FILES)
      {
#ifdef S_ISREG
        if (S_ISREG( res->st.st_mode ))
#elif S_IFREG != 0
        if (res->st.st_mode & S_IFREG)
#else // if no other way, assume all non-dirs are files
        if ((res->st.st_mode & S_IFDIR) == 0)
#endif
        {
          res->mode = FFIND_FILES;
        }
      }

      if (pDat->mode & FFIND_DIRS)
      {
#ifdef S_ISDIR
        if (S_ISDIR( res->st.st_mode ))
#else
        if (res->st.st_mode & S_IFDIR)
#endif
        {
          res->mode = FFIND_DIRS;
        }
      }

      if (res->mode != 0)
      {
        memcpy( res->filename, filePath + pDat->dirPathLen, nameLen + 1 );
        return 0;
      }
    }
  }

  return -1;
};

//--------------------------------------------------------------------------
// Name         find_close
//
// Description  Closes the find handle
//--------------------------------------------------------------------------
void find_close ( FF_DAT * pDat )
{
  ASSERT( pDat );

  closedir( pDat->dir );
  free( pDat );
};

/*
int main ( int argc, char ** argv )
{
  const char * wild;
  FF_DAT * dat;

  if (argc == 1)
    wild = "*";
  else
    wild = argv[1];

  if (dat = find_open( wild, FFIND_DIRS | FFIND_FILES ))
  {
    struct findfilestruct fs;

    while (find_file( dat, &fs ) == 0)
      printf( "%s\t%ld\n", fs.filename, fs.st.st_size );

    find_close( dat );
  }

  return 0;
};
*/
