CC=gcc -Wall -Werror -Wextra

.PHONY: all s21_grep clean rebuild

all: s21_grep

s21_grep:
	$(CC) -c s21_grep.c
	$(CC) s21_grep.o -o s21_grep
	rm *.o

clean:
	rm s21_grep

rebuild:
	make clean
	make