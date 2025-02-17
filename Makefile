CC=gcc
CFLAGS=-std=c17 -ggdb -D_POSIX_C_SOURCE=200809L -pedantic-errors -Wall -Wextra

httpc: main.o httplib.o linkedlist.o response.o
	$(CC) -o httpc main.o httplib.o response.o
test: httplib.o httplib_test.c
	$(CC) $(CFLAGS) -o test httplib_test.c httplib.o
main.o: main.c httplib.o response.o
	$(CC) $(CFLAGS) -c main.c httplib.o response.o
httplib.o: httplib.h httplib.c request_parser.o linkedlist.o
	$(CC) $(CFLAGS) -c httplib.h httplib.c request_parser.o linkedlist.o
request_parser.o: ./request_parser/request_parser.h ./request_parser/request_parser.c 
	$(CC) $(CFLAGS) -c ./request_parser/request_parser.c ./request_parser/request_parser.h
linkedlist.o: ./linkedlist/linkedlist.c ./linkedlist/linkedlist.h
	$(CC) $(CFLAGS) -c ./linkedlist/linkedlist.c ./linkedlist/linkedlist.h
response.o: ./response/response.c ./response/response.h
	$(CC) $(CFLAGS) -c ./response/response.c ./response/response.h
valgrind-check: httpc
	valgrind --leak-check=yes --track-origins=yes ./httpc
clean:
	rm -f httpc **/*.o **/*.h.gch
	rm -rf ./docs
docs:
	doxygen Doxyfile
