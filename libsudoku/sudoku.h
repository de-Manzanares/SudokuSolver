#pragma once

#include <stdint.h>

typedef uint8_t num_t;

extern const num_t HOUSE_SZ;
extern const num_t PUZZLE_SZ;

extern const num_t rcs[9][9];
extern const num_t boxes[9][9];

struct Sudoku {
  num_t cells[81];
  num_t solved[81];
  num_t candidates[81][9];
};

num_t sudoku_cell_val(const struct Sudoku *s, num_t cell);
num_t sudoku_cell_is_solved(const struct Sudoku *s, num_t cell);
void sudoku_cell_set_solved(struct Sudoku *s, num_t cell);

num_t sudoku_cell_get_row(num_t cell);
num_t sudoku_cell_get_column(num_t cell);

/* todo we can do better */
num_t sudoku_cell_get_box(const num_t cell);

void sudoku_init(struct Sudoku *s);
uint8_t sudoku_load(struct Sudoku *s, const char *cs);
uint8_t sudoku_check_form(const struct Sudoku *s);
void sudoku_init_solved(struct Sudoku *s);
void sudoku_init_candidates(struct Sudoku *s);

num_t sudoku_solve_singles_naked(struct Sudoku *s);

uint8_t sudoku_is_solved(const struct Sudoku *s);

void sudoku_print_candidates(const struct Sudoku *s);
void sudoku_print_puzzle(const struct Sudoku *s);
