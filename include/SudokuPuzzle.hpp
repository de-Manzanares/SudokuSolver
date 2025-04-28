#ifndef SUDOKU_SOLVER_INCLUDE_SUDOKU_PUZZLE_HPP_
#define SUDOKU_SOLVER_INCLUDE_SUDOKU_PUZZLE_HPP_

#include <Indices.hpp>

#include <algorithm>
#include <array>
#include <bitset>
#include <iostream>
#include <stdexcept>
#include <vector>

class SudokuPuzzle {
 public:
  explicit SudokuPuzzle(std::string_view strv);

  static constexpr int EMPTY = 0;        /// code for empty/unsolved cell
  static constexpr int PUZZLE_SIZE = 81; /// 81 cells in the puzzle

  std::vector<int> puzzle;           /// the puzzle
  std::bitset<PUZZLE_SIZE> unsolved; /// empty/unsolved cells

  /// candidates[cell] = {all candidates for that cell}
  std::vector<std::vector<int>> candidates{PUZZLE_SIZE};

  /// the location and values of hidden singles
  std::array<int, PUZZLE_SIZE> singles{};

 private:
  static void check_size(std::string_view strv);
  static char find_empty_cell_character(std::string_view strv);
  static void check_contents(std::string_view strv, char empty_cell);
  void initialize_puzzle(std::string_view strv, char empty_cell);
  void initialize_unsolved();
  void initialize_candidates();
  void initialize_candidates(int cell);
};

inline SudokuPuzzle::SudokuPuzzle(std::string_view strv) {
  check_size(strv);
  const char empty_cell = find_empty_cell_character(strv);
  check_contents(strv, empty_cell);
  initialize_puzzle(strv, empty_cell);
  initialize_unsolved();
  initialize_candidates();
}

inline void SudokuPuzzle::check_size(std::string_view strv) {
  if (strv.size() != PUZZLE_SIZE) {
    throw std::invalid_argument(
        "SudokuPuzzle::SudokuPuzzle() requires 81 characters");
  }
}

inline char SudokuPuzzle::find_empty_cell_character(std::string_view strv) {
  char empty_cell{};
  const char *const it =
      std::find_if_not(strv.cbegin(), strv.cend(),
                       [](const auto x) { return '1' <= x && x <= '9'; });
  if (it == strv.end()) {
    empty_cell = '1'; // no empty cell symbol
  } else {
    empty_cell = *it;
  }
  return empty_cell;
}

inline void SudokuPuzzle::check_contents(std::string_view strv,
                                         char empty_cell) {
  if (!std::all_of(strv.cbegin(), strv.cend(), [empty_cell](const auto x) {
        return std::isdigit(x) || x == empty_cell;
      })) {
    throw std::invalid_argument(
        "SudokuPuzzle::SudokuPuzzle() initializer may contain characters "
        "[1-9] and another character representing empty cells; empty cells "
        "can be represented by any character that is not [1-9]");
  }
}

inline void SudokuPuzzle::initialize_puzzle(std::string_view strv,
                                            const char empty_cell) {
  puzzle.reserve(PUZZLE_SIZE);
  for (const auto ch : strv) {
    if (ch == empty_cell) {
      puzzle.push_back(EMPTY);
    } else {
      puzzle.push_back(ch - '0');
    }
  }
}

inline void SudokuPuzzle::initialize_unsolved() {
  unsolved.reset();
  for (int i = 0; i < PUZZLE_SIZE; ++i) {
    if (puzzle[i] == EMPTY) {
      unsolved.set(i);
    }
  }
}

inline void SudokuPuzzle::initialize_candidates() {
  for (int i = 0; i < PUZZLE_SIZE; ++i) {
    if (unsolved[i]) {
      initialize_candidates(i);
    }
  }
}

inline void SudokuPuzzle::initialize_candidates(const int cell) {
  std::bitset<10> seen;
  for (int i = 0; i < 3; ++i) {
    for (const auto index :
         // NOLINTBEGIN(*-pro-bounds-constant-array-index)
         i == 2   ? Indices::boxes[Indices::associations[cell][i]]
         : i == 1 ? Indices::columns[Indices::associations[cell][i]]
                  : Indices::rows[Indices::associations[cell][i]])
    // NOLINTEND(*-pro-bounds-constant-array-index)
    {
      seen.set(puzzle[index]);
    }
  }
  for (int i = 1; i < 10; ++i) {
    if (!seen[i]) {
      candidates[cell].push_back(i);
    }
  }
}

#endif // SUDOKU_SOLVER_INCLUDE_SUDOKU_PUZZLE_HPP_
