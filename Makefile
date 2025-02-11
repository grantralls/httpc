CC=gcc
CFLAGS=-std=c17 -ggdb -D_POSIX_C_SOURCE=200809L -pedantic-errors -Wall -Wextra

httpc: main.o httplib.o linkedlist.o
	$(CC) -o httpc main.o httplib.o request_parser.o linkedlist.o
test: httplib.o httplib_test.c
	$(CC) $(CFLAGS) -o test httplib_test.c httplib.o
main.o: main.c
	$(CC) $(CFLAGS) -c main.c
httplib.o: httplib.h httplib.c request_parser.h request_parser.c
	$(CC) $(CFLAGS) -c request_parser.c request_parser.h httplib.h httplib.c
linkedlist.o: linkedlist.c linkedlist.h
	$(CC) $(CFLAGS) -c linkedlist.c linkedlist.h
valgrind-check: httpc
	valgrind --leak-check=yes --track-origins=yes ./httpc
clean:
	rm -f httpc main.o httplib.o request_parser.o httplib.h.gch linkedlist.o linkedlist.h.gch request_parser.h.gch
	rm -rf ./docs
docs:
	doxygen Doxyfile
