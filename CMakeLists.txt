cmake_minimum_required(VERSION 3.10)
project(Httpc)

# Create a library from the source files
add_library(Httpc STATIC src/httplib.c src/linkedlist.c src/request_parser.c src/response.c src/tree.c)

# Set include directories for users
target_include_directories(Httpc PUBLIC include)

# Set version information (optional)
set_target_properties(Httpc PROPERTIES VERSION 1.0.0 SOVERSION 1)
