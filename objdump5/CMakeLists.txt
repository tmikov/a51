set(files objdump.c collect.c)

add_executable(objdump5 ${files})
target_link_libraries(objdump5 port common)
