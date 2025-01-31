httpc: main.o
	gcc -ggdb -o httpc main.o
main.o: main.c
	gcc -ggdb -D_GNU_SOURCE -c -std=c17 -pedantic-errors main.c
valgrind-check: httpc
	valgrind --leak-check=yes --track-origins=yes ./httpc
clean:
	rm httpc main.o
docs:
	doxygen Doxyfile
