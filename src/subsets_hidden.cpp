#include "Sudoku.hpp"

#include <algorithm>
#include <functional>
#include <iostream>
#include <set>
#include <unordered_map>
#include <unordered_set>

// hidden pair/triple/quad
// for each row, column, and box
// if there exists an identical pair of candidates in two cells, but nowhere
// else in the house, then all other candidates from those cells can be
// removed.

bool Sudoku::prune_hidden_subsets() {
  const bool row = prune_hidden_subsets(house::row);
  const bool column = prune_hidden_subsets(house::column);
  const bool box = prune_hidden_subsets(house::box);
  return row || column || box;
}

bool Sudoku::prune_hidden_subsets(const house tag) {
  bool got_one = false;
  auto &houses = tag == house::row      ? indices::rows
                 : tag == house::column ? indices::columns
                                        : indices::boxes;
  for (const auto &house : houses) {
    // todo use a vector instead of a map
    // std::vector<std::vector<int>> candidates_to_cells(10);
    std::unordered_map<int, std::vector<int>> candidates_to_cells;
    // for each candidate, find in which cells of the house it resides
    for (int val = 1; val < 10; ++val) {
      for (const auto index : house) {
        for (const auto candidate : _candidates[index]) {
          if (val == candidate) {
            candidates_to_cells[val].push_back(index);
          }
        }
      }
    }
    // now we can search for the pairs/triples/quads in the row
    std::unordered_map<int, std::vector<int>> pairs;
    std::unordered_map<int, std::vector<int>> triples;
    std::unordered_map<int, std::vector<int>> quads;
    for (const auto &[val, cells] : candidates_to_cells) {
      if (cells.size() == 2) {
        pairs[val] = cells;
      }
      if (1 < cells.size() && cells.size() < 4) {
        triples[val] = cells;
      }
      if (1 < cells.size() && cells.size() < 5) {
        quads[val] = cells;
      }
    }
    if (pairs.size() == 2) {
      // calculate the union of the cells
      std::set<int> cells;
      for (auto it = pairs.begin(); it != pairs.end(); ++it) {
        for (auto iit = it->second.begin(); iit != it->second.end(); ++iit) {
          cells.insert(*iit);
        }
      }
      if (cells.size() == 2) {
        std::set<int> values;
        for (auto &[value, idxs] : pairs) {
          values.insert(value);
        }
        for (const auto cell : cells) {
          if (!got_one) {
            if (std::any_of(_candidates[cell].begin(), _candidates[cell].end(),
                            [&values](const auto c) {
                              return values.find(c) == values.end();
                            })) {
              got_one = true;
            }
          }
          if (got_one) {
            _candidates[cell].erase(
                std::remove_if(_candidates[cell].begin(),
                               _candidates[cell].end(),
                               [&values](auto x) {
                                 return std::find(values.begin(), values.end(),
                                                  x) == values.end();
                               }),
                _candidates[cell].end());
          }
        }
        if (got_one) {
          std::cout << "hidden pairs: ";
          for (const auto cell : cells) {
            std::cout << std::setw(2) << cell << ' ';
          }
          std::cout << '\n';
        }
      }
    }
    if (triples.size() == 3) {
      // calculate the union of the cells
      std::set<int> cells;
      for (auto it = triples.begin(); it != triples.end(); ++it) {
        for (auto iit = it->second.begin(); iit != it->second.end(); ++iit) {
          cells.insert(*iit);
        }
      }
      if (cells.size() == 3) {
        std::set<int> values;
        for (auto &[value, idxs] : triples) {
          values.insert(value);
        }
        for (const auto cell : cells) {
          _candidates[cell].erase(
              std::remove_if(_candidates[cell].begin(), _candidates[cell].end(),
                             [&values](auto x) {
                               return std::find(values.begin(), values.end(),
                                                x) == values.end();
                             }),
              _candidates[cell].end());
        }
      }
    }
    if (quads.size() == 4) {
      // calculate the union of the cells
      std::set<int> cells;
      for (auto it = quads.begin(); it != quads.end(); ++it) {
        for (auto iit = it->second.begin(); iit != it->second.end(); ++iit) {
          cells.insert(*iit);
        }
      }
      if (cells.size() == 4) {
        std::set<int> values;
        for (auto &[value, idxs] : quads) {
          values.insert(value);
        }
        for (const auto cell : cells) {
          _candidates[cell].erase(
              std::remove_if(_candidates[cell].begin(), _candidates[cell].end(),
                             [&values](auto x) {
                               return std::find(values.begin(), values.end(),
                                                x) == values.end();
                             }),
              _candidates[cell].end());
        }
      }
    }
  }
  return got_one;
}
