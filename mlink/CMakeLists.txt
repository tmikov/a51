project(mlink)
add_executable(mlink mlink.c genbin.c debug.c)
target_link_libraries(mlink port)