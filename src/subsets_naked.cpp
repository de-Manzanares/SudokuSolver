#include "Sudoku.hpp"

#include <algorithm>
#include <functional>
#include <iostream>
#include <map>
#include <set>
#include <unordered_map>

bool Sudoku::prune_naked_subsets(const int n) {
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
      for (const auto &[values, cells] : subsets) {
        // for every cell in the house
        for (const auto cell : house) {
          // excluding the cells containing the naked subset
          if (std::find(cells.begin(), cells.end(), cell) == cells.end()) {
            auto it =
                std::remove_if(_candidates[cell].begin(),
                               _candidates[cell].end(), [&values](auto x) {
                                 return std::find(values.begin(), values.end(),
                                                  x) != values.end();
                               });
            if (it != _candidates[cell].end()) {
              got_one = true;

              std::cout << "eliminate candidate by naked subset (cell, vals) : "
                        << std::setw(2) << cell << ", ";
              for (auto itt = it; itt != _candidates[cell].end(); ++itt) {
                std::cout << std::setw(2) << *itt << ' ';
                ++_candidates_pruned_by._subset_naked;
              }
              std::cout << '\n';

              _candidates[cell].erase(it, _candidates[cell].end());
            }
          }
        }
      }
    }
  }
  return got_one;
}
