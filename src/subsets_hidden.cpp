#include "Indices.hpp"
#include "Sudoku.hpp"

#include <algorithm>
#include <functional>
#include <iostream>
#include <map>
#include <set>
#include <unordered_map>

bool Sudoku::prune_hidden_subsets(const size_t n) {
  bool got_one = false;
  const auto house_type =
      std::vector{&Indices::rows, &Indices::columns, &Indices::boxes};

  for (const auto &houses : house_type) {
    for (const auto &house : *houses) {
      std::unordered_map<int, std::vector<int>> candidates_to_cells;
      // for each candidate, find in which cells of the house it resides
      for (int val = 1; val < 10; ++val) {
        for (const auto cell : house) {
          for (const auto candidate : _candidates[cell]) {
            if (val == candidate) {
              candidates_to_cells[val].push_back(cell);
            }
          }
        }
      }
      // now we can search for the pairs/triples/quads in the house
      std::unordered_map<int, std::vector<int>> val_to_cells;
      for (const auto &[val, cells] : candidates_to_cells) {
        if ((n == 2 && cells.size() == 2) ||
            (n == 3 && (1 < cells.size() && cells.size() < 4)) ||
            (n == 4 && (1 < cells.size() && cells.size() < 5))) {
          val_to_cells[val] = cells;
        }
      }

      std::map<std::vector<int>, std::size_t> cells_frequency;
      for (const auto &[val, cells] : val_to_cells) {
        ++cells_frequency[cells];
      }

      for (auto it = cells_frequency.begin(); it != cells_frequency.end();) {
        if (it->second != n) {
          it = cells_frequency.erase(it);
        } else {
          ++it;
        }
      }

      for (const auto &[fcells, freq] : cells_frequency) {
        for (auto it = val_to_cells.begin(); it != val_to_cells.end();) {
          if (fcells != it->second) {
            it = val_to_cells.erase(it);
          } else {
            ++it;
          }
        }
      }

      if (val_to_cells.size() == n) {
        // calculate the union of the cells
        std::set<int> cells;
        for (const auto &[val, lcells] : val_to_cells) {
          std::copy(lcells.begin(), lcells.end(),
                    std::inserter(cells, cells.begin()));
        }

        if (cells.size() == n) {
          std::set<int> values;
          for (auto &[value, cells] : val_to_cells) {
            values.insert(value);
          }
          for (const auto cell : cells) {
            auto it =
                std::remove_if(_candidates[cell].begin(),
                               _candidates[cell].end(), [&values](auto x) {
                                 return std::find(values.begin(), values.end(),
                                                  x) == values.end();
                               });
            if (it != _candidates[cell].end()) {
              got_one = true;

              // std::cout
              //     << "eliminate candidate by hidden subset (cell, vals) : "
              //     << std::setw(2) << cell << ", ";
              // for (auto itt = it; itt != _candidates[cell].end(); ++itt) {
              //   std::cout << std::setw(2) << *itt << ' ';
              //   ++_candidates_pruned_by._subset_hidden;
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
