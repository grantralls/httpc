CC=gcc-14
CFLAGS=-D_GNU_SOURCE -std=c17 -pedantic-errors -Wall -Wextra

httpc: main.o httplib.o
	$(CC) -o httpc main.o httplib.o
test: httplib.o httplib_test.c
	$(CC) $(CFLAGS) -o test httplib_test.c httplib.o
main.o: main.c
	$(CC) $(CFLAGS) -c main.c
httplib.o: httplib.h httplib.c
	$(CC) $(CFLAGS) -c httplib.h httplib.c
valgrind-check: httpc
	valgrind --leak-check=yes --track-origins=yes ./httpc
clean:
	rm httpc main.o httplib.o httplib.h.gch
	rm -rf ./docs
docs:
	doxygen Doxyfile
