#ifndef SUDOKU_HPP_
#define SUDOKU_HPP_

#include "indices.hpp"

#include <algorithm>
#include <array>
#include <bitset>
#include <functional>
#include <iomanip>
#include <iostream>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>

class Sudoku {
 public:
  enum class house { row, column, box };

  explicit Sudoku(const std::vector<int> &puzzle) : _puzzle{puzzle} {};

  // check
  void check_puzzle() const;

  // initialize candidates
  void initialize_candidates();

  // singles
  bool solve_naked_singles();
  bool solve_hidden_singles();

  void solve();

  bool prune_hidden_subsets();
  bool prune_hidden_subsets(house tag);
  bool prune_locked_claiming_candidates();
  bool prune_locked_pointing_candidates();

  void print_puzzle() const;
  static void print_puzzle(const std::vector<int> &vec);
  void print_unknown() const;
  static void print_candidates(const std::string &str,
                               const std::vector<std::vector<int>> &vec);
  void print_singles() const;

  void print_readout() const;

 private:
  static constexpr int UNKNOWN = 0;      ///< code for unknown cell value
  static constexpr int PUZZLE_SIZE = 81; ///< 81 cells in the puzzle

  std::vector<int> _puzzle; ///< the puzzle
  std::bitset<PUZZLE_SIZE> _unknown{};
  std::vector<std::vector<int>> _candidates{PUZZLE_SIZE};
  std::array<int, 81> _singles{};

  int _iterations{};
  std::vector<std::vector<int>> _puzzles;
  int _naked_singles{};
  int _hidden_singles{};

  struct _candidates_pruned_by {
    int _claiming_locked{};
    int _pointing_locked{};
  };

  _candidates_pruned_by _candidates_pruned_by{};

  // check
  [[nodiscard]] bool puzzle_is_valid() const;
  [[nodiscard]] bool check_houses(house tag) const;

  // initialize candidates
  void find_unknown_indices();
  void initialize_candidates(int cell);

  // singles
  bool find_hidden_singles();
  bool find_hidden_singles(house tag);
  void update_candidates(int cell);

  // locked claiming
  bool prune_locked_claiming_candidates(house tag);
};

// begin - prune candidates ----------------------------------------------------

// hidden pair/triple/quad
// for each row, column, and box
// if there exists an identical pair of candidates in two cells, but nowhere
// else in the house, then all other candidates from those cells can be
// removed.

// pointing locked candidates
// https://hodoku.sourceforge.net/en/tech_intersections.php
// If in a block all candidates of a certain digit are confined to a row or
// column, that digit cannot appear outside of that block in that row or
// column.
// identify if a candidate only appears in one row or column of a box

inline bool Sudoku::prune_hidden_subsets() {
  bool row = prune_hidden_subsets(house::row);
  bool column = prune_hidden_subsets(house::column);
  bool box = prune_hidden_subsets(house::box);
  return row || column || box;
}

inline bool Sudoku::prune_hidden_subsets(const house tag) {
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

#endif // SUDOKU_HPP_
