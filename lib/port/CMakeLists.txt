# lib/port
set (files "")

if (NOT HAVE_DIRENT)
  set(files ${files} dirent.c)
endif (NOT HAVE_DIRENT)
if (NOT HAVE_STRUPR)
set(files ${files} strupr.c)
endif (NOT HAVE_STRUPR)
if (NOT HAVE__STRICMP)
set(files ${files} stricmp.c)
endif (NOT HAVE__STRICMP)
if (NOT HAVE_ULTOA)
set(files ${files} ltoa.c)
endif (NOT HAVE_ULTOA)

add_library(port STATIC ${files})
