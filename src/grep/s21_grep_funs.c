#include "s21_grep.h"
#define REG_MATCH 0

void set_flag(int flag, option *opt) {
  switch (flag) {
    case 'e':
      opt->e = 1;
      pattern_append(opt, optarg);
      break;
    case 'i':
      opt->i = REG_ICASE;
      break;
    case 'v':
      opt->v = 1;
      opt->o = 0;
      break;
    case 'c':
      opt->c = 1;
      break;
    case 'l':
      opt->l = 1;
      break;
    case 'n':
      opt->n = 1;
      break;
    case 's':
      opt->s = 1;
      break;
    case 'h':
      opt->h = 1;
      break;
    case 'f':
      opt->f = 1;
      reg_from_file(opt, optarg);
      break;
    case 'o':
      opt->o = 1;
      break;
  }
}

void reg_from_file(option *opt, char *filename) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    printf("File doesnt open");
    exit(EXIT_FAILURE);
  } else {
    char *line = NULL;
    size_t len = 0;
    for (int read = getline(&line, &len, file); read != -1;
         read = getline(&line, &len, file)) {
      if (line[read - 1] == '\n') {
        line[read - 1] = '\0';
      }
      pattern_append(opt, line);
    }
    free(line);
  }
  fclose(file);
}

void reg_match_from_line(regex_t *re, char *line) {
  regmatch_t match;
  int offset = 0;
  for (int res = regexec(re, line + offset, 1, &match, 0); res == 0;
       offset += match.rm_eo, res = regexec(re, line + offset, 1, &match, 0)) {
    char *start = line + offset + match.rm_so;
    for (int i = 0; i < match.rm_eo - match.rm_so; i++) {
      putchar(start[i]);
    }
    putchar('\n');
  }
}

void pattern_append(option *opt, char *pattern) {
  if (opt->pattern_len != 0) {
    strcat(opt->pattern, "|");
    opt->pattern_len++;
  }
  if (strcmp(pattern, "|") == 0) {
    char word[3] = "\\|\0";
    opt->pattern_len += sprintf(opt->pattern + opt->pattern_len, "(%s)", word);
  } else {
    opt->pattern_len +=
        sprintf(opt->pattern + opt->pattern_len, "(%s)", pattern);
  }
}

option parser_flags(int argc, char **argv) {
  option opt = {0};
  int flag;
  while ((flag = getopt_long(argc, argv, "e:ivclnhsf:o", NULL, NULL)) != -1) {
    set_flag(flag, &opt);
  }
  if (opt.pattern_len == 0) {
    pattern_append(&opt, argv[optind++]);
  }
  if (argc - optind == 1) {
    opt.h = 1;
  }
  return opt;
}

void print_line(char *line) {
  int len = strlen(line);
  for (int i = 0; i < len; i++) {
    putchar(line[i]);
  }
  if (line[len - 1] != '\n') putchar('\n');
}

void process_flags(option *opt, regex_t *re, int count_line, char *line,
                   char *filename) {
  if (!opt->c && !opt->l) {
    if (!opt->h) printf("%s:", filename);
    if (opt->n) printf("%d:", count_line);
    if (opt->o && opt->v == 0) {
      reg_match_from_line(re, line);
    } else {
      print_line(line);
    }
  }
}

void process_line(regex_t re, FILE *file, option *opt, char *filename) {
  char *line = NULL;
  size_t len = 0;
  int read = 0;
  int count = 0;
  int count_line = 1;
  while ((read = getline(&line, &len, file)) != -1) {
    int comp = regexec(&re, line, 0, NULL, 0);
    if ((comp == REG_MATCH && opt->v == REG_MATCH) ||
        (comp == REG_NOMATCH && opt->v)) {
      process_flags(opt, &re, count_line, line, filename);
      count++;
    }
    count_line++;
  }
  if (opt->c) {
    if (!opt->h) printf("%s:", filename);
    if (opt->l && count > 0) count = 1;
    printf("%d\n", count);
  }
  if (opt->l && count > 0) {
    printf("%s\n", filename);
  }
  free(line);
}

void process_file(option *opt, char *filename) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    if (!(opt->s)) {
      printf("grep: %s: No such file or directory\n", filename);
    }
  } else {
    regex_t re;
    if (regcomp(&re, opt->pattern, REG_EXTENDED | opt->i)) {
      printf("ERROR");
      exit(EXIT_FAILURE);
    }
    process_line(re, file, opt, filename);
    regfree(&re);
  }
  fclose(file);
}

void file_reader(int argc, char **argv, option *opt) {
  for (int i = optind; i < argc; i++) {
    process_file(opt, argv[i]);
  }
}