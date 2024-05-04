#include "s21_cat.h"

void error_fun(char flag) {
  printf("cat: illegal option -- %c\n", flag);
  printf("usage: cat [-belnstuv] [file ...]\n");
  exit(EXIT_FAILURE);
}

void error_fun_gnu(char **argv, int i) {
  printf("cat: illegal option -- %s\n", argv[i]);
  printf("usage: cat [-belnstuv] [file ...]\n");
  exit(EXIT_FAILURE);
}

void set_flag(char flag, option *opt) {
  switch (flag) {
    case 'b':
      opt->b = 1;
      break;
    case 'e':
      opt->e = 1;
      opt->v = 1;
      break;
    case 'n':
      opt->n = 1;
      break;
    case 's':
      opt->s = 1;
      break;
    case 't':
      opt->t = 1;
      opt->v = 1;
      break;
    case 'v':
      opt->v = 1;
      break;
    case 'T':
      opt->t = 1;
      break;
    case 'E':
      opt->e = 1;
      break;
    default:
      error_fun(flag);
  }
}

void parser_flag(char **argv, int i, option *opt) {
  for (size_t i1 = 1; i1 < strlen(argv[i]); i1++) {
    set_flag(argv[i][i1], opt);
  }
}

void parser_flag_GNU(char **argv, int i, option *opt) {
  if (strcmp(argv[i], "--number-nonblank") == 0) {
    set_flag('b', opt);
  } else if (strcmp(argv[i], "--number") == 0) {
    set_flag('n', opt);
  } else if (strcmp(argv[i], "--squeeze-blank") == 0) {
    set_flag('s', opt);
  } else {
    error_fun_gnu(argv, i);
  }
}

void find_flag(int argc, char **argv, option *opt, int *index) {
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-' && argv[i][1] != '-') {
      parser_flag(argv, i, opt);
    } else if (argv[i][0] == '-' && argv[i][1] == '-') {
      parser_flag_GNU(argv, i, opt);
    } else {
      *index = i;
      break;
    }
  }
}

void process_c(int c, int *prev, option opt, int *count, int *empty_line) {
  if (opt.s == 0 || *prev != '\n' || c != '\n' || !(*empty_line)) {
    if (*prev == '\n' && c == '\n') {
      *empty_line = 1;
    } else {
      *empty_line = 0;
    }
    if (((opt.n == 1 && opt.b == 0) || (opt.b == 1 && c != '\n')) &&
        *prev == '\n') {
      printf("%6d\t", ++*count);
    }
    if (opt.e == 1 && c == '\n') {
      if (opt.b == 1 && *prev == '\n') {
        printf("      	$");
      } else {
        printf("$");
      }
    }
    if (opt.t == 1 && c == '\t') {
      printf("^I");
      *prev = c;
      return;
    }
    if (opt.v == 1) {
      if (c > 127 && c < 160) printf("M-^");
      if ((c < 32 && c != '\n' && c != '\t') || c == 127) printf("^");
      if ((c < 32 || (c > 126 && c < 160)) && c != '\n' && c != '\t')
        c = c > 126 ? c - 128 + 64 : c + 64;
    }
    printf("%c", c);
  }
  *prev = c;
}

void file_reader(char *filename, option opt) {
  FILE *file = fopen(filename, "r");
  if (file != NULL) {
    int cur;
    int count = 0;
    int prev = '\n';
    int empty_line = 0;
    while ((cur = fgetc(file)) != EOF) {
      process_c(cur, &prev, opt, &count, &empty_line);
    }
  } else {
    printf("cat: %s: No such file or directory", filename);
    exit(EXIT_FAILURE);
  }
  fclose(file);
}

void reader(int argc, char **argv, option *opt, int count) {
  for (int i = count; i < argc; i++) {
    file_reader(argv[i], *opt);
  }
}