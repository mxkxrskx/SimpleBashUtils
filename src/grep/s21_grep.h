#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct options {
  int e, i, v, c, l, n, h, s, f, o;
  char pattern[1024];
  int pattern_len;

} option;

//Парсер на флаги
option parser_flags(int argc, char **argv);
void set_flag(int flag, option *opt);

//Считываение файла и вывод всех нужных строк
void file_reader(int argc, char **argv, option *opt);
void process_file(option *opt, char *filename);
void process_line(regex_t re, FILE *file, option *opt, char *filename);
void process_flags(option *opt, regex_t *re, int count_line, char *line,
                   char *filename);
void pattern_append(option *opt, char *pattern);
void reg_from_file(option *opt, char *filename);
void reg_match_from_line(regex_t *re, char *line);