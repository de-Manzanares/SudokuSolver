#include "sudoku.h"

#include <stdio.h>

int main(int argc, char **argv) {
  struct Sudoku sudoku;
  sudoku_init(&sudoku);
  // clang-format off
  if (!sudoku_load(&sudoku, "290167003108000200000830491051000702080029130300006004872600000600280540000901020")) {
    return 1;
  }
  // clang-format on

  uint8_t naked_singles = 0;
  while (!sudoku_is_solved(&sudoku)) {
    naked_singles += sudoku_solve_singles_naked(&sudoku);
  }
  printf("\nSolved %d naked singles\n", naked_singles);
  sudoku_print_puzzle(&sudoku);
  return 0;
}
