#include "Sudoku.hpp"

#include <algorithm>
#include <functional>
#include <iostream>
#include <map>
#include <set>

bool Sudoku::prune_naked_pair(const std::size_t n) {
  bool got_one = false;
  const auto house_type =
      std::vector{&indices::rows, &indices::columns, &indices::boxes};

  for (const auto &houses : house_type) {
    for (const auto &house : *houses) {

      // find which cells have n candidates
      std::vector<int> size_n;
      size_n.reserve(9);
      for (const int cell : house) {
        if (_candidates[cell].size() == n) {
          size_n.emplace_back(cell);
        }
      }

      // find if n cells share identical candidates
      std::map<std::vector<int>, std::vector<int>> subsets;
      for (const auto cell : size_n) {
        subsets[_candidates[cell]].emplace_back(cell);
      }
      for (auto it = subsets.begin(); it != subsets.end();) {
        if (it->second.size() != n) {
          it = subsets.erase(it);
        } else {
          ++it;
        }
      }
      for (const auto &subset : subsets) {
        // for every cell in the house
        for (const auto cell : house) {
          // excluding the cells containing the naked subset
          if (std::find(subset.second.begin(), subset.second.end(), cell) ==
              subset.second.end()) {
            auto it = std::remove_if(
                _candidates[cell].begin(), _candidates[cell].end(),
                [&subset](auto x) {
                  return std::find(subset.first.begin(), subset.first.end(),
                                   x) != subset.first.end();
                });
            if (it != _candidates[cell].end()) {
              got_one = true;

              // std::cout << "eliminate candidate by naked subset (cell, vals)
              // : "
              //           << std::setw(2) << cell << ", ";
              // for (auto itt = it; itt != _candidates[cell].end(); ++itt) {
              //   std::cout << std::setw(2) << *itt << ' ';
              //   ++_candidates_pruned_by._subset_naked;
              // }
              // std::cout << '\n';

              _candidates[cell].erase(it, _candidates[cell].end());
            }
          }
        }
      }
    }
  }
  return got_one;
}

bool Sudoku::prune_naked_subset(const std::size_t n) {
  bool got_one = false;
  const auto house_type =
      std::vector{&indices::rows, &indices::columns, &indices::boxes};

  for (const auto &houses : house_type) {
    for (const auto &house : *houses) {

      // find which cells have n candidates
      std::vector<int> size_n;
      size_n.reserve(9);
      for (const int cell : house) {
        if (2 <= _candidates[cell].size() && _candidates[cell].size() <= n) {
          size_n.emplace_back(cell);
        }
      }

      // of those, find which combination of n candidates yields a union of
      // cells of size n
      std::map<int, std::vector<int>> val_to_cells;
      for (const auto cell : size_n) {
        for (const auto candidate : _candidates[cell]) {
          val_to_cells[candidate].emplace_back(cell);
        }
      }

      std::vector<bool> select(val_to_cells.size(), false);
      if (val_to_cells.size() >= n) {
        std::fill(std::prev(select.end(), n), select.end(), true);
      }
      std::set<int> cells;
      do {
        cells.clear();
        for (int i = 0; i < val_to_cells.size(); ++i) {
          if (select[i]) {
            for (const auto cell : std::next(val_to_cells.begin(), i)->second) {
              cells.insert(cell);
            }
          }
        }
        if (cells.size() == n) {
          break;
        }
      } while (std::next_permutation(select.begin(), select.end()));

      std::set<int> subset_vals;

      if (cells.size() == n) {
        for (const auto cell : cells) {
          for (const auto candidate : _candidates[cell]) {
            subset_vals.insert(candidate);
          }
        }
      }

      if (subset_vals.size() == n) {

        for (const auto cell : house) {
          if (cells.find(cell) == cells.end()) {
            auto it = std::remove_if(
                _candidates[cell].begin(), _candidates[cell].end(),
                [&subset_vals](auto x) {
                  return std::find(subset_vals.begin(), subset_vals.end(), x) !=
                         subset_vals.end();
                });
            if (it != _candidates[cell].end()) {
              got_one = true;
              _candidates[cell].erase(it, _candidates[cell].end());
            }
          }
        }
      }
    }
  }
  return got_one;
}
