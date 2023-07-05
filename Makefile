CC=gcc -Wall -Werror -Wextra

.PHONY: all s21_cat s21_grep clean rebuild

all: s21_cat s21_grep

s21_cat:
	$(CC) -c cat/once.c
	$(CC) once.o -o cat/s21_cat
	rm *.o

s21_grep:
	$(CC) -c grep/s21_grep.c
	$(CC) s21_grep.o -o grep/s21_grep
	rm *.o

clean:
	rm cat/s21_cat
	rm grep/s21_grep

rebuild:
	make clean
	make

style:
	cp ../materials/linters/.clang-format ../../src
	clang-format -n cat/once.c -style=google
	clang-format -i cat/once.c -style=google
	clang-format -n grep/s21_grep.c -style=google
	clang-format -i grep/s21_grep.c -style=google
	rm .clang-format