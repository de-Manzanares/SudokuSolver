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

inline num_t sudoku_cell_get_row(const num_t cell);
inline num_t sudoku_cell_get_column(const num_t cell);

/* todo we can do better */
inline num_t sudoku_cell_get_box(const num_t cell);

void sudoku_init(struct Sudoku *s);
num_t sudoku_load(struct Sudoku *s, const char *cs);
num_t sudoku_check_form(const struct Sudoku *s);
void sudoku_solved_init(struct Sudoku *s);
void sudoku_candidates_init(struct Sudoku *s);

void sudoku_print_candidates(struct Sudoku *s);