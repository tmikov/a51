set(files
  link51.c err.c collect.c readrex.c
  process.c symbols.c segments.c output.c map.c
)

add_executable(link51 ${files})
target_link_libraries(link51 port common)
