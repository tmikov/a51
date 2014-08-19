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
include_directories("${PROJECT_SOURCE_DIR}")

set(files
  amain.c  asm.c  cmdtab.c  err.c  ident.c
  idtab.c  minilex.c  scan.c  genobj.c xref.c
  misc.c debug.c findf.c
)

option(GENERATE_SRC
       "Re-generate sources instead of using packaged ones" ON)

if (GENERATE_SRC)
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
    add_custom_command(
      OUTPUT ${PROJECT_BINARY_DIR}/a_tab.c ${PROJECT_BINARY_DIR}/a_tab.h
      COMMAND bison -d ${PROJECT_SOURCE_DIR}/a.y -o ${PROJECT_BINARY_DIR}/a_tab.c
      DEPENDS ${PROJECT_SOURCE_DIR}/a.y
    )

    set(files ${files}
      ${PROJECT_BINARY_DIR}/a_tab.c
      ${PROJECT_BINARY_DIR}/a_tab.h
      ${PROJECT_BINARY_DIR}/ctbl.inc
      ${PROJECT_BINARY_DIR}/uptab.inc
      ${PROJECT_BINARY_DIR}/errmsg.inc
    )
else(GENERATE_SRC)
    include_directories("${PROJECT_SOURCE_DIR}/gen")

    set(files ${files}
      ${PROJECT_SOURCE_DIR}/gen/a_tab.c
      ${PROJECT_SOURCE_DIR}/gen/a_tab.h
      ${PROJECT_SOURCE_DIR}/gen/ctbl.inc
      ${PROJECT_SOURCE_DIR}/gen/uptab.inc
      ${PROJECT_SOURCE_DIR}/gen/errmsg.inc
    )
endif(GENERATE_SRC)

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
