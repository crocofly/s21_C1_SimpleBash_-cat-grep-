CC=gcc -Wall -Werror -Wextra

.PHONY: all s21_cat clean rebuild

all: s21_cat

s21_cat:
	$(CC) -c once.c
	$(CC) once.o -o s21_cat
	rm *.o

clean:
	rm s21_cat

rebuild:
	make clean
	make