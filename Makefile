CC = gcc
CFLAGS = -std=c17 -ggdb -D_POSIX_C_SOURCE=200809L -pedantic-errors -Wall -Wextra
OBJ_FILES = ./build/main.o ./build/httplib.o ./build/linkedlist.o ./build/response.o ./build/request_parser.o

httpc: $(OBJ_FILES)
	$(CC) $(CFLAGS) -o httpc $(OBJ_FILES)

test: httplib.o httplib_test.c
	$(CC) $(CFLAGS) -o test httplib_test.c ./build/httplib.o

./build/main.o: main.c httplib.h response/response.h
	$(CC) $(CFLAGS) -c main.c -o ./build/main.o

./build/httplib.o: httplib.c httplib.h
	$(CC) $(CFLAGS) -c httplib.c -o ./build/httplib.o

./build/request_parser.o: request_parser/request_parser.c request_parser/request_parser.h
	$(CC) $(CFLAGS) -c request_parser/request_parser.c -o ./build/request_parser.o

./build/linkedlist.o: linkedlist/linkedlist.c linkedlist/linkedlist.h
	$(CC) $(CFLAGS) -c linkedlist/linkedlist.c -o ./build/linkedlist.o

./build/response.o: response/response.c response/response.h
	$(CC) $(CFLAGS) -c response/response.c -o ./build/response.o

# Valgrind memory check
valgrind-check: httpc
	valgrind --leak-check=yes --track-origins=yes ./httpc

# Cleanup compiled files
clean:
	rm -f httpc test $(OBJ_FILES)
	rm -f **/*.o **/*.h.gch
	rm -rf ./docs

# Generate documentation
docs:
	doxygen Doxyfile

