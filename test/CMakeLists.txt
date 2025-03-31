# Next Steps
# Make adding a new test not suck, they *could* be generated with a for loop over the *_test.c files
# Or, make a function that takes a c file, and a name, that makes a executable
add_executable(linkedlist_test linkedlist_test.c)
target_compile_options(linkedlist_test PRIVATE -pedantic-errors)
target_link_libraries(linkedlist_test PRIVATE common_properties Httpc)
add_test(NAME linkedlist_test COMMAND linkedlist_test)

add_executable(request_parser_test request_parser_test.c)
target_compile_options(request_parser_test PRIVATE -pedantic-errors)
target_link_libraries(request_parser_test PRIVATE common_properties Httpc)
add_test(NAME request_parser_test COMMAND request_parser_test)
