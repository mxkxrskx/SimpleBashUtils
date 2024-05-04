#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct options {
  int b;
  int t;
  int e;
  int n;
  int s;
  int v;
} option;

void error_fun(char flag);
void error_fun_gnu(char **argv, int i);

void set_flag(char flag, option *opt);
void parser_flag(char **argv, int i, option *opt);
void parser_flag_GNU(char **argv, int i, option *opt);
void find_flag(int argc, char **argv, option *opt, int *index);

void process_c(int c, int *prev, option opt, int *count, int *empty_line);
void file_reader(char *filename, option opt);
void reader(int argc, char **argv, option *opt, int index);