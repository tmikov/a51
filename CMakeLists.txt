cmake_minimum_required (VERSION 2.6)
project(a51)

include(CheckFunctionExists)
include(CheckIncludeFiles)
include(CheckTypeSize)

check_function_exists(strupr HAVE_STRUPR)
check_function_exists(strupr HAVE_STRLWR)
check_function_exists(stricmp HAVE_STRICMP)
check_function_exists(_stricmp HAVE__STRICMP)
check_function_exists(ultoa HAVE_ULTOA)
check_function_exists(ltoa HAVE_LTOA)
check_function_exists(itoa HAVE_ITOA)
check_include_files(dirent.h HAVE_DIRENT_H)
check_function_exists(opendir HAVE_OPENDIR)
check_type_size("int" _INT_SIZE)
check_type_size("long" _LONG_SIZE)

if (HAVE_DIRENT_H AND HAVE_OPENDIR)
  set(HAVE_DIRENT ON)
endif (HAVE_DIRENT_H AND HAVE_OPENDIR)

configure_file(
  "${PROJECT_SOURCE_DIR}/include/common/config.h.in"
  "${PROJECT_BINARY_DIR}/include/common/config.h"
)
include_directories("${PROJECT_BINARY_DIR}/include")
include_directories("${PROJECT_SOURCE_DIR}/include")

add_subdirectory(lib/port)
add_subdirectory(lib/common)
add_subdirectory(mlink)
add_subdirectory(a51)

