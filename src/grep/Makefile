CC=gcc
CFLAGS=-Wall -Wextra -Werror
SOURCES_GREP=s21_grep.c s21_grep_funs.c

all: s21_grep

s21_grep:
	$(CC) $(CFLAGS) $(SOURCES_GREP) -o s21_grep

clean:
	rm -f s21_grep

rebuild: clean all