#include "s21_grep.h"

int main(int argc, char **argv) {
  option opt = parser_flags(argc, argv);
  file_reader(argc, argv, &opt);
  return 0;
}