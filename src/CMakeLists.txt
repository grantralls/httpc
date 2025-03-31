# Create a library from the source files
add_library(Httpc STATIC httplib.c linkedlist.c request_parser.c response.c tree.c)

# Set include directories for users
target_include_directories(Httpc PUBLIC ../include)

target_compile_options(Httpc PUBLIC -D_POSIX_C_SOURCE=200809L)
target_compile_options(Httpc PRIVATE -pedantic-errors)

target_link_libraries(Httpc PRIVATE common_properties)
