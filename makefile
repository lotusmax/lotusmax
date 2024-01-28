CC = gcc
CFLAGS = -Wall -Werror -std=c11 -pedantic

s21_cat: s21_cat.c
	$(CC) $(CFLAGS) -o s21_cat s21_cat.c

style:
	clang-format -style=Google -i *.c

valgrind:
	valgrind --tool=memcheck --leak-check=yes ./s21_cat -benst txt2.txt

diff: s21_cat
	./s21_cat -benst txt2.txt > s21_cat_output.txt
	cat -benst txt2.txt > cat_output.txt
	diff s21_cat_output.txt cat_output.txt

clean:
	rm -f s21_cat s21_cat_output.txt cat_output.txt

