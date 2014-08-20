set(files
  amain.c  asm.c  cmdtab.c  err.c  ident.c
  idtab.c  minilex.c  scan.c  genobj.c xref.c
  misc.c debug.c findf.c
)

option(GENERATE_SRC
       "Re-generate sources instead of using packaged ones" ON)

if (GENERATE_SRC)
    include_directories("${CMAKE_CURRENT_BINARY_DIR}")

    add_executable(gentbl gentbl.c)
    add_executable(genup genup.c)
    add_executable(generr generr.c)

    add_custom_command(
      OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/ctbl.inc
      COMMAND gentbl > ${CMAKE_CURRENT_BINARY_DIR}/ctbl.inc
      DEPENDS gentbl
    )
    add_custom_command(
      OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/uptab.inc
      COMMAND genup > ${CMAKE_CURRENT_BINARY_DIR}/uptab.inc
      DEPENDS genup
    )
    add_custom_command(
      OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/errmsg.inc
      COMMAND generr < ${CMAKE_CURRENT_SOURCE_DIR}/errmsg.txt > ${CMAKE_CURRENT_BINARY_DIR}/errmsg.inc
      DEPENDS generr
    )
    add_custom_command(
      OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/a_tab.c ${CMAKE_CURRENT_BINARY_DIR}/a_tab.h
      COMMAND bison -d ${CMAKE_CURRENT_SOURCE_DIR}/a.y -o ${CMAKE_CURRENT_BINARY_DIR}/a_tab.c
      DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/a.y
    )

    set(files ${files}
      ${CMAKE_CURRENT_BINARY_DIR}/a_tab.c
      ${CMAKE_CURRENT_BINARY_DIR}/a_tab.h
      ${CMAKE_CURRENT_BINARY_DIR}/ctbl.inc
      ${CMAKE_CURRENT_BINARY_DIR}/uptab.inc
      ${CMAKE_CURRENT_BINARY_DIR}/errmsg.inc
    )
else(GENERATE_SRC)
    include_directories("${CMAKE_CURRENT_SOURCE_DIR}/gen")

    set(files ${files}
      ${CMAKE_CURRENT_SOURCE_DIR}/gen/a_tab.c
      ${CMAKE_CURRENT_SOURCE_DIR}/gen/a_tab.h
      ${CMAKE_CURRENT_SOURCE_DIR}/gen/ctbl.inc
      ${CMAKE_CURRENT_SOURCE_DIR}/gen/uptab.inc
      ${CMAKE_CURRENT_SOURCE_DIR}/gen/errmsg.inc
    )
endif(GENERATE_SRC)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

add_executable(a51 ${files})
target_link_libraries(a51 port)
