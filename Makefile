build:
	gcc -std=c17 -pedantic-errors main.c
docs:
	doxygen Doxyfile
