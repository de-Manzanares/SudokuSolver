#include "Sudoku.hpp"

bool Sudoku::solve_naked_singles() {
  bool got_one = false;
  for (int i = 0; i < PUZZLE_SIZE; ++i) {
    if (_candidates[i].size() == 1) {
      got_one = true;
      ++_naked_singles;
      _puzzle[i] = _candidates[i][0];
      _unknown.reset(i);
      update_candidates(i);
      std::cout << "naked single : " << std::setw(2) << i << " " << _puzzle[i]
                << '\n';
    }
  }
  return got_one;
}

bool Sudoku::solve_hidden_singles() {
  const bool got_one = find_hidden_singles();
  if (got_one) {
    for (int i = 0; i < PUZZLE_SIZE; ++i) {
      if (_singles[i] != 0) {
        ++_hidden_singles;
        _puzzle[i] = _singles[i];
        _singles[i] = 0;
        _unknown.reset(i);
        update_candidates(i);
        std::cout << "hidden single : " << std::setw(2) << i << " "
                  << _puzzle[i] << '\n';
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
         i == 2   ? indices::boxes[indices::associations[cell][i]]
         : i == 1 ? indices::columns[indices::associations[cell][i]]
                  : indices::rows[indices::associations[cell][i]]) {
      _candidates[index].erase(std::remove(_candidates[index].begin(),
                                           _candidates[index].end(), val),
                               _candidates[index].end());
    }
  }
}

bool Sudoku::find_hidden_singles() {
  const bool row = find_hidden_singles(house::row);
  const bool column = find_hidden_singles(house::column);
  const bool box = find_hidden_singles(house::box);
  return row || column || box;
}

bool Sudoku::find_hidden_singles(const house tag) {
  bool got_one = false;
  const auto &houses = tag == house::row      ? indices::rows
                       : tag == house::column ? indices::columns
                                              : indices::boxes;
  for (const auto &house : houses) {
    std::vector<int> frequency(10);
    for (const auto cell : house) {
      for (const auto candidate : _candidates[cell]) {
        frequency[candidate]++;
      }
    }
    for (int j = 1; j < 10; ++j) {
      if (frequency[j] == 1) {
        for (const auto index : house) {
          for (const auto candidate : _candidates[index]) {
            if (candidate == j) {
              _singles[index] = j;
              got_one = true;
            }
          }
        }
      }
    }
  }
  return got_one;
}
