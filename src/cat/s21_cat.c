#include "s21_cat.h"

int main(int argc, char **argv) {
  option opt = {0};
  int index = 0;
  find_flag(argc, argv, &opt, &index);
  reader(argc, argv, &opt, index);

  return 0;
}
