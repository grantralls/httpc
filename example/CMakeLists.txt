# Create a library from the source files
add_executable(run_server main.c)

target_link_libraries(run_server PRIVATE common_properties)
target_link_libraries(run_server PRIVATE Httpc)
