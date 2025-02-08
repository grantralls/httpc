CC=gcc
CFLAGS=-std=c17 -pedantic-errors -Wall -Wextra

httpc: main.o httplib.o
	$(CC) -o httpc main.o httplib.o request_parser.o
test: httplib.o httplib_test.c
	$(CC) $(CFLAGS) -o test httplib_test.c httplib.o
main.o: main.c
	$(CC) $(CFLAGS) -c main.c
httplib.o: httplib.h httplib.c request_parser.h request_parser.c
	$(CC) $(CFLAGS) -c request_parser.c request_parser.h httplib.h httplib.c
valgrind-check: httpc
	valgrind --leak-check=yes --track-origins=yes ./httpc
clean:
	rm httpc main.o httplib.o request_parser.o httplib.h.gch
	rm -rf ./docs
docs:
	doxygen Doxyfile
