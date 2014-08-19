cmake_minimum_required (VERSION 2.6)
project(a51)

include(CheckFunctionExists)

check_function_exists(strupr HAVE_STRUPR)
check_function_exists(stricmp HAVE_STRICMP)
check_function_exists(_stricmp HAVE__STRICMP)
check_function_exists(ultoa HAVE_ULTOA)
check_function_exists(ltoa HAVE_LTOA)
check_function_exists(itoa HAVE_ITOA)

configure_file(
  "${PROJECT_SOURCE_DIR}/config.h.in"
  "${PROJECT_BINARY_DIR}/config.h"
)
include_directories("${PROJECT_BINARY_DIR}")

add_executable(gentbl gentbl.c)
add_executable(genup genup.c)
add_executable(generr generr.c)

add_custom_command(
  OUTPUT ${PROJECT_BINARY_DIR}/ctbl.inc
  COMMAND gentbl > ${PROJECT_BINARY_DIR}/ctbl.inc
  DEPENDS gentbl
)
add_custom_command(
  OUTPUT ${PROJECT_BINARY_DIR}/uptab.inc
  COMMAND genup > ${PROJECT_BINARY_DIR}/uptab.inc
  DEPENDS genup
)
add_custom_command(
  OUTPUT ${PROJECT_BINARY_DIR}/errmsg.inc
  COMMAND generr < ${PROJECT_SOURCE_DIR}/errmsg.txt > ${PROJECT_BINARY_DIR}/errmsg.inc
  DEPENDS generr
)

set(files 
  amain.c  asm.c  a_tab.c  cmdtab.c  err.c  ident.c
  idtab.c  minilex.c  scan.c  genobj.c xref.c
  misc.c ltoa.c debug.c findf.c
  ${PROJECT_BINARY_DIR}/ctbl.inc
  ${PROJECT_BINARY_DIR}/uptab.inc
  ${PROJECT_BINARY_DIR}/errmsg.inc
)

if (NOT HAVE_STRUPR)
set(files ${files} strupr.c)
endif (NOT HAVE_STRUPR)
if (NOT HAVE__STRICMP)
set(files ${files} stricmp.c)
endif (NOT HAVE__STRICMP)
if (NOT HAVE_ULTOA)
set(files ${files} ltoa.c)
endif (NOT HAVE_ULTOA)

add_executable(a51 ${files})
