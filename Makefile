httpc: main.o httplib.o
	gcc -o httpc main.o httplib.o
test: httplib.o httplib_test.c
	gcc -o test httplib_test.c httplib.o
main.o: main.c
	gcc -c -D_GNU_SOURCE -std=c17 -pedantic-errors main.c
httplib.o: httplib.h httplib.c
	gcc -c -D_GNU_SOURCE -std=c17 -pedantic-errors httplib.h httplib.c
valgrind-check: httpc
	valgrind --leak-check=yes --track-origins=yes ./httpc
clean:
	rm httpc main.o httplib.o httplib.h.gch
	rm -rf ./docs
docs:
	doxygen Doxyfile
