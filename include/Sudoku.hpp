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

  // initialize candidatess
  void initialize_candidates();

  bool prune_hidden_subsets();

  bool prune_hidden_subsets(house tag);
  bool prune_claiming_locked_candidates();
  bool prune_claiming_locked_candidates(house tag);
  bool prune_pointing_locked_candidates();

  // singles
  bool solve_naked_singles();
  bool solve_hidden_singles();

  void solve();

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

  bool _sc = true; ///< we have found at least one single this iteration
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

inline bool Sudoku::prune_claiming_locked_candidates() {
  const bool row = prune_claiming_locked_candidates(house::row);
  const bool column = prune_claiming_locked_candidates(house::column);
  return row || column;
}

// claiming locked candidates
// https://hodoku.sourceforge.net/en/tech_intersections.php
// If in a row (or column) all candidates of a certain digit are confined to
// one block, that candidate that be eliminated from all other cells in that
// block.
inline bool Sudoku::prune_claiming_locked_candidates(const house tag) {
  bool got_one = false;
  const auto &houses = tag == house::row ? indices::rows : indices::columns;

  for (int i = 0; i < 9; ++i) { // for each house of the houses

    std::vector<std::vector<int>> intersections(3);

    for (int j = 0; j < 9; ++j) { // for each cell in the house

      // place the candidates in the corresponding intersection
      if (j < 3) {
        for (const auto candidate : _candidates[houses[i][j]]) {
          intersections[0].push_back(candidate); // first box
        }
      } else if (j < 6) {
        for (const auto candidate : _candidates[houses[i][j]]) {
          intersections[1].push_back(candidate); // second box
        }
      } else {
        for (const auto candidate : _candidates[houses[i][j]]) {
          intersections[2].push_back(candidate); // third box
        }
      }
    }

    // find the claimed candidates

    // count the candidate frequency
    std::vector<int> frequency(10);
    for (const auto &intersection : intersections) {
      for (const auto candidate : intersection) {
        ++frequency[candidate];
      }
    }

    // if row/colum frequency == intersection frequency, candidate is claimed
    std::vector<std::vector<int>> claimed_candidates(3);
    for (int k = 0; k < 3; ++k) {
      std::bitset<10> claimed{};
      for (auto candidate : intersections[k]) {
        if (frequency[candidate] == std::count(intersections[k].begin(),
                                               intersections[k].end(),
                                               candidate)) {
          claimed.set(candidate);
        }
      }
      for (int l = 0; l < 10; ++l) {
        if (claimed[l]) {
          claimed_candidates[k].push_back(l);
        }
      }
    }

    // remove the claimed candidate from cells in the box outside the
    // intersection

    for (int k = 0; k < 3; ++k) {           // for each intersection
      if (!claimed_candidates[k].empty()) { // if there are claimed candidates
                                            // for each claimed candidate
        for (const auto claimed : claimed_candidates[k]) {
          std::function<int()> choose_box;
          if (tag == house::row) {
            choose_box = [&i, &k]() {
              if (i < 3) {
                return k;
              }
              if (i < 6) {
                return 3 + k;
              }
              if (i < 9) {
                return 6 + k;
              }
            };
          }
          if (tag == house::column) {
            choose_box = [&i, &k]() {
              if (i < 3) {
                return k * 3;
              }
              if (i < 6) {
                return k * 3 + 1;
              }
              if (i < 9) {
                return k * 3 + 2;
              }
            };
          }
          // for each cell in the box
          for (const auto cell : indices::boxes[choose_box()]) {
            // if the cell is outside the intersection
            if (std::find(houses[i].begin(), houses[i].end(), cell) ==
                houses[i].end()) {
              // and contains a claimed candidate
              // remove the claimed candidate from that cell
              if (std::find(_candidates[cell].begin(), _candidates[cell].end(),
                            claimed) != _candidates[cell].end()) {
                got_one = true;
                std::cout << "eliminate candidate by locked claiming : "
                          << std::setw(2) << cell << " " << claimed << '\n';
                ++_candidates_pruned_by._claiming_locked;
                _candidates[cell].erase(std::remove(_candidates[cell].begin(),
                                                    _candidates[cell].end(),
                                                    claimed),
                                        _candidates[cell].end());
              }
            }
          }
        }
      }
    }
  }
  return got_one;
}

inline bool Sudoku::prune_pointing_locked_candidates() {
  bool got_one = false;
  for (int i = 0; i < 9; ++i) {   // for each box
    for (int j = 0; j < 2; ++j) { // for both rows (0) and columns (1)

      std::vector<std::vector<int>> intersections(3);

      // place candidates in the corresponding intersection

      // for each of the intersections (rows or columns)
      for (int k = 0; k < 3; ++k) {
        for (int l = 0; l < 3; ++l) { // for each cell in that intersection
          for (const auto candidate :
               _candidates[indices::subranges[i][j][k]->at(l)]) {
            intersections[k].push_back(candidate);
          }
        }
      }

      // find the pointing candidates

      // count the candidate frequency
      std::vector<int> frequency(10);
      for (const auto &intersection : intersections) {
        for (const auto candidate : intersection) {
          ++frequency[candidate];
        }
      }

      // if box frequency == intersection frequency, candidate is pointing
      std::vector<std::vector<int>> pointing_candidates(3);
      for (int k = 0; k < 3; ++k) {
        std::bitset<10> pointing{};
        for (auto candidate : intersections[k]) {
          if (frequency[candidate] == std::count(intersections[k].begin(),
                                                 intersections[k].end(),
                                                 candidate)) {
            pointing.set(candidate);
          }
        }
        for (int l = 0; l < 10; ++l) {
          if (pointing[l]) {
            pointing_candidates[k].push_back(l);
          }
        }
      }

      // remove the pointing candidates from the row/column outside the box

      // i : box
      // j : 0 -> rows, 1 -> columns
      // k : first, second, third row/column intersecting the given box
      // l : first, second, third cell of the intersecting row/column
      for (int k = 0; k < 3; ++k) { // for each intersection
                                    // if there are pointing candidates
        if (!pointing_candidates[k].empty()) {
          // for each pointing candidate
          for (const auto candidate : pointing_candidates[k]) {
            if (j == 0) { // rows
              auto choose_row = [&i, &k]() {
                if (i < 3) {
                  return k;
                }
                if (3 <= i && i < 6) {
                  return 3 + k;
                }
                if (i >= 6) {
                  return 6 + k;
                }
              };
              // for each cell in the row
              for (const auto cell : indices::rows[choose_row()]) {
                // if the cell is not in the box
                if (std::find(indices::subranges[i][j][k]->begin(),
                              indices::subranges[i][j][k]->end(),
                              cell) == indices::subranges[i][j][k]->end()) {
                  // eliminate instances of the pointing candidate in that cell
                  for (auto it = _candidates[cell].begin();
                       it != _candidates[cell].end();) {
                    if (*it == candidate) {
                      std::cout << "eliminate candidate by locked pointing : "
                                << std::setw(2) << cell << " " << candidate
                                << '\n';
                      ++_candidates_pruned_by._pointing_locked;
                      got_one = true;
                      it = _candidates[cell].erase(it);
                    } else {
                      ++it;
                    }
                  }
                }
              }
            } else if (j == 1) { // columns
              auto choose_column = [&i, &k]() {
                if (i == 0 || i == 3 || i == 6) {
                  return k;
                }
                if (i == 1 || i == 4 || i == 7) {
                  return 3 + k;
                }
                if (i == 2 || i == 5 || i == 8) {
                  return 6 + k;
                }
              };
              // for each cell in the row
              for (const auto cell : indices::columns[choose_column()]) {
                // if the cell is not in the box
                if (std::find(indices::subranges[i][j][k]->begin(),
                              indices::subranges[i][j][k]->end(),
                              cell) == indices::subranges[i][j][k]->end()) {
                  // eliminate instances of the pointing candidate in that cell
                  for (auto it = _candidates[cell].begin();
                       it != _candidates[cell].end();) {
                    if (*it == candidate) {
                      std::cout << "eliminate candidate by locked pointing : "
                                << std::setw(2) << cell << " " << candidate
                                << '\n';
                      ++_candidates_pruned_by._pointing_locked;
                      got_one = true;
                      it = _candidates[cell].erase(it);
                    } else {
                      ++it;
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  return got_one;
}

// end - prune candidates ------------------------------------------------------
#endif // SUDOKU_HPP_
