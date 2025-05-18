#include "sudoku.h"

#include <stdio.h>

int main(int argc, char **argv) {
  struct Sudoku sudoku;
  sudoku_init(&sudoku);
  // clang-format off
  if (sudoku_load(&sudoku, "200070038000006070300040600008020700100000006007030400004080009060400000910060002") != 0) {
    return 1;
  }
  // clang-format on
  if (sudoku_check_form(&sudoku) != 0) {
    return 1;
  }
  sudoku_candidates_init(&sudoku);
  // sudoku_print_candidates(&sudoku);
  return 0;
}
