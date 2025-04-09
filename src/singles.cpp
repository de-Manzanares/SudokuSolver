#include "Indices.hpp"
#include "Sudoku.hpp"
#include <algorithm>

bool Sudoku::solve_naked_singles() {
  bool got_one = false;
  for (int i = 0; i < PUZZLE_SIZE; ++i) {
    if (_candidates[i].size() == 1) {
      _puzzle[i] = _candidates[i][0];
      _unknown.reset(i);
      update_candidates(i);
      got_one = true;
      ++_naked_singles;
    }
  }
  return got_one;
}

bool Sudoku::solve_hidden_singles() {
  const bool got_one = find_hidden_singles();
  if (got_one) {
    for (int i = 0; i < PUZZLE_SIZE; ++i) {
      if (_singles[i] != 0) {
        _puzzle[i] = _singles[i];
        _singles[i] = 0;
        _unknown.reset(i);
        update_candidates(i);
        ++_hidden_singles;
      }
    }
  }
  return got_one;
}

void Sudoku::update_candidates(const int cell) {
  _candidates[cell].clear();
  const int val = _puzzle[cell];
  for (int i = 0; i < 3; ++i) {
    for (const auto index :
         i == 2   ? Indices::boxes[Indices::associations[cell][i]]
         : i == 1 ? Indices::columns[Indices::associations[cell][i]]
                  : Indices::rows[Indices::associations[cell][i]]) {
      _candidates[index].erase(std::remove(_candidates[index].begin(),
                                           _candidates[index].end(), val),
                               _candidates[index].end());
    }
  }
}

bool Sudoku::find_hidden_singles() {
  const bool row = find_hidden_singles(House::row);
  const bool column = find_hidden_singles(House::column);
  const bool box = find_hidden_singles(House::box);
  return row || column || box;
}

bool Sudoku::find_hidden_singles(const House tag) {
  bool got_one = false;
  const auto &houses = tag == House::row      ? Indices::rows
                       : tag == House::column ? Indices::columns
                                              : Indices::boxes;
  for (const auto &house : houses) {
    std::vector<int> frequency(10);
    for (const auto cell : house) {
      for (const auto candidate : _candidates[cell]) {
        ++frequency[candidate];
      }
    }
    for (int val = 1; val < 10; ++val) {
      if (frequency[val] == 1) {
        for (const auto index : house) {
          for (const auto candidate : _candidates[index]) {
            if (candidate == val) {
              _singles[index] = val;
              got_one = true;
              goto found;
            }
          }
        }
      }
    found:;
    }
  }
  return got_one;
}
