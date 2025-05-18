#include "sudoku.h"

#include <stdio.h>

const num_t HOUSE_SZ = 9;   /* number of cells in house */
const num_t PUZZLE_SZ = 81; /* number of cells in puzzle */

const num_t rcs[9][9] = {
    // clang-format off
    {  0,  1,  2,  3,  4,  5,  6,  7,  8 },
    {  9, 10, 11, 12, 13, 14, 15, 16, 17 },
    { 18, 19, 20, 21, 22, 23, 24, 25, 26 },
    { 27, 28, 29, 30, 31, 32, 33, 34, 35 },
    { 36, 37, 38, 39, 40, 41, 42, 43, 44 },
    { 45, 46, 47, 48, 49, 50, 51, 52, 53 },
    { 54, 55, 56, 57, 58, 59, 60, 61, 62 },
    { 63, 64, 65, 66, 67, 68, 69, 70, 71 },
    { 72, 73, 74, 75, 76, 77, 78, 79, 80 },
    // clang-format on
};

const num_t boxes[9][9] = {
    // clang-format off
    {  0,  1,  2,  9, 10, 11, 18, 19, 20 },
    {  3,  4,  5, 12, 13, 14, 21, 22, 23 },
    {  6,  7,  8, 15, 16, 17, 24, 25, 26 },
    { 27, 28, 29, 36, 37, 38, 45, 46, 47 },
    { 30, 31, 32, 39, 40, 41, 48, 49, 50 },
    { 33, 34, 35, 42, 43, 44, 51, 52, 53 },
    { 54, 55, 56, 63, 64, 65, 72, 73, 74 },
    { 57, 58, 59, 66, 67, 68, 75, 76, 77 },
    { 60, 61, 62, 69, 70, 71, 78, 79, 80 },
    // clang-format on
};

void sudoku_init(struct Sudoku *s) {
  for (int i = 0; i < PUZZLE_SZ; i++) {
    s->cells[i] = 0;
    s->solved[i] = 0;
    for (int j = 0; j < HOUSE_SZ; j++) {
      s->candidates[i][j] = 0;
    }
  }
}

num_t sudoku_load(struct Sudoku *s, const char *cs) {
  num_t cs_len = 0;
  while (cs_len <= PUZZLE_SZ) {
    if (cs[cs_len] == '\0') {
      break;
    }
    ++cs_len;
  }
  if (cs_len < PUZZLE_SZ) {
    printf("sudoku_load() failed - input is too short - given "
           "%d/%d characters\n",
           cs_len, PUZZLE_SZ);
    return (num_t)-1;
  }
  if (cs_len > PUZZLE_SZ) {
    printf("sudoku_load() failed - input is too long - given %d/%d "
           "characters\n",
           cs_len, PUZZLE_SZ);
    return (num_t)-1;
  }
  for (num_t i = 0; i < PUZZLE_SZ; i++) {
    if ('0' <= cs[i] && cs[i] <= '9') {
      s->cells[i] = cs[i] - '0';
    } else {
      printf("sudoku_load() failed - input contains "
             "non-number\n");
      for (int j = 0; j <= i; ++j) {
        printf("%c", cs[j]);
      }
      putchar('\n');
      for (int j = 0; j < i; ++j) {
        putchar(' ');
      }
      printf("^\n");
      return (num_t)-1;
    }
  }
  return 0;
}

num_t sudoku_cell_val(const struct Sudoku *s, const num_t cell) {
  if (cell < PUZZLE_SZ) {
    return s->cells[cell];
  }
  return (num_t)-1;
}

num_t sudoku_cell_is_solved(const struct Sudoku *s, const num_t cell) {
  if (cell < PUZZLE_SZ) {
    return s->solved[cell] != 0;
  }
  return (num_t)-1;
}

void sudoku_cell_set_solved(struct Sudoku *s, const num_t cell) {
  if (cell < PUZZLE_SZ) {
    s->solved[cell] = 1;
  }
}

void sudoku_solved_init(struct Sudoku *s) {
  for (int i = 0; i < PUZZLE_SZ; ++i) {
    if (s->cells[i] != 0) {
      sudoku_cell_set_solved(s, i);
    }
  }
}

num_t sudoku_check_form(const struct Sudoku *s) {
  /* rows */
  for (num_t i = 0; i < HOUSE_SZ; ++i) {
    uint8_t seen[HOUSE_SZ + 1] = {0};
    for (num_t j = 0; j < HOUSE_SZ; ++j) {
      seen[sudoku_cell_val(s, rcs[i][j])]++; /* i , j */
      for (num_t k = 1; k <= HOUSE_SZ; ++k) {
        if (seen[k] > 1) {
          printf("puzzle is malformed - ");
          printf("%d appears %d times in row %d\n", k, seen[k], i + 1);
          return (num_t)-1;
        }
      }
    }
  }
  /* column */
  for (num_t i = 0; i < HOUSE_SZ; ++i) {
    uint8_t seen[HOUSE_SZ + 1] = {0};
    for (num_t j = 0; j < HOUSE_SZ; ++j) {
      seen[sudoku_cell_val(s, rcs[j][i])]++; /* j , i */
      for (num_t k = 1; k <= HOUSE_SZ; ++k) {
        if (seen[k] > 1) {
          printf("puzzle is malformed - ");
          printf("%d appears %d times in column "
                 "%d\n",
                 k, seen[k], i + 1);
          return (num_t)-1;
        }
      }
    }
  }
  /* box */
  for (num_t i = 0; i < HOUSE_SZ; ++i) {
    uint8_t seen[HOUSE_SZ + 1] = {0};
    for (num_t j = 0; j < HOUSE_SZ; ++j) {
      seen[sudoku_cell_val(s, boxes[i][j])]++;
      for (num_t k = 1; k <= HOUSE_SZ; ++k) {
        if (seen[k] > 1) {
          printf("puzzle is malformed - ");
          printf("%d appears %d times in box %d\n", k, seen[k], i + 1);
          return (num_t)-1;
        }
      }
    }
  }
  return 0;
}

num_t sudoku_cell_get_row(const num_t cell) { return cell / HOUSE_SZ; }
num_t sudoku_cell_get_column(const num_t cell) { return cell % HOUSE_SZ; }

/* todo we can do better */
num_t sudoku_cell_get_box(const num_t cell) {
  for (num_t i = 0; i < HOUSE_SZ; ++i) {
    for (num_t j = 0; j < HOUSE_SZ; ++j) {
      if (boxes[i][j] == cell) {
        return i;
      }
    }
  }
}

void sudoku_candidates_cell_init(struct Sudoku *s, const num_t cell) {
  const num_t row = sudoku_cell_get_row(cell);
  const num_t column = sudoku_cell_get_column(cell);
  const num_t box = sudoku_cell_get_box(cell);
  uint8_t seen[HOUSE_SZ + 1] = {0};
  for (num_t i = 0; i < HOUSE_SZ; ++i) {
    seen[sudoku_cell_val(s, rcs[row][i])]++;
  }
  for (num_t i = 0; i < HOUSE_SZ; ++i) {
    seen[sudoku_cell_val(s, rcs[i][column])]++;
  }
  for (num_t i = 0; i < HOUSE_SZ; ++i) {
    seen[sudoku_cell_val(s, boxes[box][i])]++;
  }
  for (num_t i = 0; i < HOUSE_SZ; ++i) {
    if (seen[i] == 0) {
      s->candidates[cell][i] = 1;
    }
  }
}

void sudoku_candidates_init(struct Sudoku *s) {
  // count all values seen in a cell's row, column, and box
  // the candidates for that cell are the complement of that set
  for (num_t i = 0; i < PUZZLE_SZ; ++i) {
    if (s->solved[i] == 0) {
      sudoku_candidates_cell_init(s, i);
    }
  }
}

void sudoku_print_candidates(struct Sudoku *s) {
  for (num_t i = 0; i < PUZZLE_SZ; ++i) {
    printf("cell %d: ", i);
    for (num_t j = 0; j < HOUSE_SZ; ++j) {
      if (s->candidates[i][j] == 1) {
        printf("%d ", j);
      }
    }
    putchar('\n');
  }
}
