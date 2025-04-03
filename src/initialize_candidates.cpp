#include "Sudoku.hpp"

void Sudoku::initialize_candidates() {
  find_unknown_indices();
  _candidates.clear();
  _candidates.resize(PUZZLE_SIZE);
  for (int i = 0; i < PUZZLE_SIZE; ++i) {
    if (_unknown[i]) {
      initialize_candidates(i);
    }
  }
}

void Sudoku::find_unknown_indices() {
  _unknown.reset();
  for (int i = 0; i < PUZZLE_SIZE; ++i) {
    if (_puzzle[i] == UNKNOWN) {
      _unknown.set(i);
    }
  }
}

void Sudoku::initialize_candidates(const int cell) {
  // count every occurrence of a value for that cell's houses
  // the candidates are the complement of that set
  std::bitset<10> seen;
  for (int i = 0; i < 3; ++i) {
    for (const auto index :
         i == 2   ? indices::boxes[indices::associations[cell][i]]
         : i == 1 ? indices::columns[indices::associations[cell][i]]
                  : indices::rows[indices::associations[cell][i]]) {
      seen.set(_puzzle[index]);
    }
  }
  for (int i = 1; i < 10; ++i) {
    if (!seen[i]) {
      _candidates[cell].push_back(i);
    }
  }
}
