CC = gcc
CFLAGS = -std=c17 -O0 -ggdb3 -D_POSIX_C_SOURCE=200809L -pedantic-errors -Wall -Wextra
OBJ_FILES = ./build/httplib.o ./build/linkedlist.o ./build/response.o ./build/request_parser.o ./build/tree.o

httpc: $(OBJ_FILES)
	ar rcs ./build/libhttpc.a $(OBJ_FILES)

test: httplib_test.c httpc ./test/main.o
	$(CC) $(CFLAGS) -L ./build -o run-tests httplib_test.c -l:libhttpc.a
	$(CC) $(CFLAGS) -L ./build -o run-test-server ./test/main.c -l:libhttpc.a

./test/main.o: ./test/main.c httplib.h response/response.h
	$(CC) $(CFLAGS) -c ./test/main.c -o ./test/main.o

./build/httplib.o: httplib.c httplib.h
	$(CC) $(CFLAGS) -c httplib.c -o ./build/httplib.o

./build/request_parser.o: request_parser/request_parser.c request_parser/request_parser.h
	$(CC) $(CFLAGS) -c request_parser/request_parser.c -o ./build/request_parser.o

./build/linkedlist.o: linkedlist/linkedlist.c linkedlist/linkedlist.h
	$(CC) $(CFLAGS) -c linkedlist/linkedlist.c -o ./build/linkedlist.o

./build/response.o: response/response.c response/response.h
	$(CC) $(CFLAGS) -c response/response.c -o ./build/response.o

./build/tree.o: ./uri-tree/tree.c ./uri-tree/tree.h  
	$(CC) $(CFLAGS) -c ./uri-tree/tree.c -o ./build/tree.o


# Valgrind memory check
valgrind-check: httpc
	valgrind --leak-check=yes --track-origins=yes ./httpc

# Cleanup compiled files
clean:
	rm -f httpc $(OBJ_FILES)
	rm -f **/*.o **/*.h.gch
	rm -rf ./docs
	rm -rf build/*
	rm -f ./run-tests
	rm -f ./run-test-server

# Generate documentation
docs:
	doxygen Doxyfile

