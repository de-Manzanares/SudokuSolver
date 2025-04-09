#include "Indices.hpp"
#include "Sudoku.hpp"

#include <algorithm>
#include <functional>
#include <iostream>
#include <set>

bool Sudoku::prune_naked_subset(const std::size_t n) {
  bool got_one = false;
  const auto house_type =
      std::vector{&Indices::rows, &Indices::columns, &Indices::boxes};

  for (const auto &houses : house_type) {
    for (const auto &house : *houses) {

      // find which cells have at least 2 but not more than n candidates
      std::vector<int> good_cells;
      good_cells.reserve(9);
      for (const int cell : house) {
        if (2 <= _candidates[cell].size() && _candidates[cell].size() <= n) {
          good_cells.emplace_back(cell);
        }
      }

      // find which combination of those cells has a union of n unique
      // candidates
      std::set<int> cells;            // the combination
      std::set<int> union_candidates; // to hold the union of candidates
      std::vector cell_mask(good_cells.size(), false);
      if (good_cells.size() >= n) {
        std::fill(std::prev(cell_mask.end(), n), cell_mask.end(), true);

        do {
          cells.clear();
          union_candidates.clear();
          for (std::size_t i = 0; i < cell_mask.size(); ++i) {
            if (cell_mask[i]) {
              cells.insert(good_cells[i]);
            }
          }
          for (const auto cell : cells) {
            for (const auto candidate : _candidates[cell]) {
              union_candidates.insert(candidate);
            }
          }
          if (union_candidates.size() == n) {
            break;
          }
        } while (std::next_permutation(cell_mask.begin(), cell_mask.end()));
      }

      if (union_candidates.size() == n) { // we found one!
        for (const auto cell : house) {
          if (cells.find(cell) == cells.end()) {
            auto it = std::remove_if(
                _candidates[cell].begin(), _candidates[cell].end(),
                [&union_candidates](auto x) {
                  return std::find(union_candidates.begin(),
                                   union_candidates.end(),
                                   x) != union_candidates.end();
                });
            if (it != _candidates[cell].end()) {
              got_one = true; // there are candidates to be pruned
              _candidates[cell].erase(it, _candidates[cell].end());
            }
          }
        }
      }
    }
  }
  return got_one;
}
