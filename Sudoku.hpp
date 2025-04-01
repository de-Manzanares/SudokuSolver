#ifndef SUDOKU_HPP_
#define SUDOKU_HPP_

#include <algorithm>
#include <array>
#include <bitset>
#include <functional>
#include <iostream>
#include <list>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>

// begin INDICES ---------------------------------------------------------------

static constexpr std::array<int, 3> intersection(const std::array<int, 9> &a,
                                                 const std::array<int, 9> &b) {
  std::array<int, 3> result{0, 0, 0};
  int idx = 0;
  for (const int i : a) {
    for (const int j : b) {
      if (i == j) {
        result[idx++] = i;
      }
    }
  }
  return result;
}

static constexpr std::array<std::array<int, 9>, 9> initialize_rows() {
  std::array<std::array<int, 9>, 9> rows{};
  for (int i = 0; i < 9; i++) {
    for (int j = 0; j < 9; j++) {
      rows[i][j] = i * 9 + j;
    }
  }
  return rows;
}

static constexpr std::array<std::array<int, 9>, 9> initialize_columns() {
  std::array<std::array<int, 9>, 9> rows{};
  for (int i = 0; i < 9; i++) {
    for (int j = 0; j < 9; j++) {
      rows[i][j] = i + 9 * j;
    }
  }
  return rows;
}

static constexpr std::array<std::array<int, 9>, 9> initialize_boxes() {
  std::array<std::array<int, 9>, 9> boxes{};
  for (int i = 0; i < 9; i++) {
    const int brow = i / 3;
    const int bcol = i % 3;
    for (int y = 0; y < 3; ++y) {
      for (int x = 0; x < 3; ++x) {
        boxes[i][3 * y + x] = 9 * (3 * brow + y) + (3 * bcol + x);
      }
    }
  }
  return boxes;
}

static constexpr std::array<std::array<int, 3>, 81> initialize_associations() {
  std::array<std::array<int, 3>, 81> associations{};
  for (int i = 0; i < 81; i++) {
    const int row = i / 9;
    const int col = i % 9;
    const int box = (row / 3) * 3 + (col / 3);
    associations[i] = {row, col, box};
  }
  return associations;
}

struct indices {
  static constexpr auto rows = initialize_rows();
  static constexpr auto columns = initialize_columns();
  static constexpr auto boxes = initialize_boxes();
  static constexpr auto associations = initialize_associations();

  using subrange = std::array<int, 3>;
  static constexpr subrange box0row0 = intersection(boxes[0], rows[0]);
  static constexpr subrange box0row1 = intersection(boxes[0], rows[1]);
  static constexpr subrange box0row2 = intersection(boxes[0], rows[2]);
  static constexpr subrange box0column0 = intersection(boxes[0], columns[0]);
  static constexpr subrange box0column1 = intersection(boxes[0], columns[1]);
  static constexpr subrange box0column2 = intersection(boxes[0], columns[2]);
  //
  static constexpr subrange box1row0 = intersection(boxes[1], rows[0]);
  static constexpr subrange box1row1 = intersection(boxes[1], rows[1]);
  static constexpr subrange box1row2 = intersection(boxes[1], rows[2]);
  static constexpr subrange box1column3 = intersection(boxes[1], columns[3]);
  static constexpr subrange box1column4 = intersection(boxes[1], columns[4]);
  static constexpr subrange box1column5 = intersection(boxes[1], columns[5]);
  //
  static constexpr subrange box2row0 = intersection(boxes[2], rows[0]);
  static constexpr subrange box2row1 = intersection(boxes[2], rows[1]);
  static constexpr subrange box2row2 = intersection(boxes[2], rows[2]);
  static constexpr subrange box2column6 = intersection(boxes[2], columns[6]);
  static constexpr subrange box2column7 = intersection(boxes[2], columns[7]);
  static constexpr subrange box2column8 = intersection(boxes[2], columns[8]);
  //
  static constexpr subrange box3row3 = intersection(boxes[3], rows[3]);
  static constexpr subrange box3row4 = intersection(boxes[3], rows[4]);
  static constexpr subrange box3row5 = intersection(boxes[3], rows[5]);
  static constexpr subrange box3column0 = intersection(boxes[3], columns[0]);
  static constexpr subrange box3column1 = intersection(boxes[3], columns[1]);
  static constexpr subrange box3column2 = intersection(boxes[3], columns[2]);
  //
  static constexpr subrange box4row3 = intersection(boxes[4], rows[3]);
  static constexpr subrange box4row4 = intersection(boxes[4], rows[4]);
  static constexpr subrange box4row5 = intersection(boxes[4], rows[5]);
  static constexpr subrange box4column3 = intersection(boxes[4], columns[3]);
  static constexpr subrange box4column4 = intersection(boxes[4], columns[4]);
  static constexpr subrange box4column5 = intersection(boxes[4], columns[5]);
  //
  static constexpr subrange box5row3 = intersection(boxes[5], rows[3]);
  static constexpr subrange box5row4 = intersection(boxes[5], rows[4]);
  static constexpr subrange box5row5 = intersection(boxes[5], rows[5]);
  static constexpr subrange box5column6 = intersection(boxes[5], columns[6]);
  static constexpr subrange box5column7 = intersection(boxes[5], columns[7]);
  static constexpr subrange box5column8 = intersection(boxes[5], columns[8]);
  //
  static constexpr subrange box6row6 = intersection(boxes[6], rows[6]);
  static constexpr subrange box6row7 = intersection(boxes[6], rows[7]);
  static constexpr subrange box6row8 = intersection(boxes[6], rows[8]);
  static constexpr subrange box6column0 = intersection(boxes[6], columns[0]);
  static constexpr subrange box6column1 = intersection(boxes[6], columns[1]);
  static constexpr subrange box6column2 = intersection(boxes[6], columns[2]);
  //
  static constexpr subrange box7row6 = intersection(boxes[7], rows[6]);
  static constexpr subrange box7row7 = intersection(boxes[7], rows[7]);
  static constexpr subrange box7row8 = intersection(boxes[7], rows[8]);
  static constexpr subrange box7column3 = intersection(boxes[7], columns[3]);
  static constexpr subrange box7column4 = intersection(boxes[7], columns[4]);
  static constexpr subrange box7column5 = intersection(boxes[7], columns[5]);
  //
  static constexpr subrange box8row6 = intersection(boxes[8], rows[6]);
  static constexpr subrange box8row7 = intersection(boxes[8], rows[7]);
  static constexpr subrange box8row8 = intersection(boxes[8], rows[8]);
  static constexpr subrange box8column6 = intersection(boxes[8], columns[6]);
  static constexpr subrange box8column7 = intersection(boxes[8], columns[7]);
  static constexpr subrange box8column8 = intersection(boxes[8], columns[8]);

  static constexpr std::array<std::array<std::array<const subrange *, 3>, 2>, 9>
      subranges = {{{{{&box0row0, &box0row1, &box0row2},
                      {&box0column0, &box0column1, &box0column2}}},
                    {{{&box1row0, &box1row1, &box1row2},
                      {&box1column3, &box1column4, &box1column5}}},
                    {{{&box2row0, &box2row1, &box2row2},
                      {&box2column6, &box2column7, &box2column8}}},
                    {{{&box3row3, &box3row4, &box3row5},
                      {&box3column0, &box3column1, &box3column2}}},
                    {{{&box4row3, &box4row4, &box4row5},
                      {&box4column3, &box4column4, &box4column5}}},
                    {{{&box5row3, &box5row4, &box5row5},
                      {&box5column6, &box5column7, &box5column8}}},
                    {{{&box6row6, &box6row7, &box6row8},
                      {&box6column0, &box6column1, &box6column2}}},
                    {{{&box7row6, &box7row7, &box7row8},
                      {&box7column3, &box7column4, &box7column5}}},
                    {{{&box8row6, &box8row7, &box8row8},
                      {&box8column6, &box8column7, &box8column8}}}}};
};

// end INDICES -----------------------------------------------------------------

class Sudoku {
 public:
  enum class house { row, column, box };

  explicit Sudoku(const std::vector<int> &puzzle) : _puzzle{puzzle} {};

  void check_puzzle() const;
  [[nodiscard]] bool puzzle_is_valid() const;
  [[nodiscard]] bool check_houses(house tag) const;

  [[nodiscard]] bool check_validity(int value, int val_index) const;

  void find_unknown_indices();
  void find_candidates();

  void prune_hidden_subsets(house tag);
  void prune_claiming_locked_candidates(house tag);
  void prune_pointing_locked_candidates();

  void reset_singles();
  void find_implicit_singles(house tag);
  void solve_explicit_singles();
  void solve_implicit_singles();

  void solve();

  void print_puzzle() const;
  static void print_puzzle(const std::vector<int> &vec);
  void print_unknown();
  static void print_candidates(const std::string &str,
                               const std::vector<std::vector<int>> &vec);
  void print_singles() const;

  void print_readout() const;

 private:
  static constexpr int UNKNOWN = 0;      ///< code for unknown cell value
  static constexpr int PUZZLE_SIZE = 81; ///< 81 cells in the puzzle

  std::vector<int> _puzzle; ///< the puzzle
  std::bitset<PUZZLE_SIZE> _unknown{};
  std::vector<std::vector<int>> _candidates;
  std::array<int, 81> _singles{};

  bool _sc = true; ///< we have found at least one single this iteration
  int _iterations{};
  std::vector<std::vector<int>> _puzzles;
};

inline void Sudoku::solve() {
  find_unknown_indices();

  while (_sc) {
    _sc = false;
    _puzzles.push_back(_puzzle);

    if (_unknown.none()) {
      break; // exit if the puzzle is solved
    }

    find_candidates();

    print_unknown();
    print_candidates("Candidates:\n", _candidates);

    prune_hidden_subsets(house::row);
    prune_hidden_subsets(house::column);
    prune_hidden_subsets(house::box);

    prune_claiming_locked_candidates(house::row);
    prune_claiming_locked_candidates(house::column);

    prune_pointing_locked_candidates();

    print_candidates("Pruned Candidates:\n", _candidates);

    reset_singles();
    find_implicit_singles(house::row);
    find_implicit_singles(house::column);
    find_implicit_singles(house::box);
    print_singles();

    solve_implicit_singles();

    solve_explicit_singles();

    print_puzzle();
    ++_iterations;
    check_puzzle();
  }
}

// begin checking validity -----------------------------------------------------

inline void Sudoku::check_puzzle() const {
  try {
    if (_puzzle.size() != PUZZLE_SIZE || !puzzle_is_valid()) {
      throw std::runtime_error("puzzle is malformed");
    }
  } catch (const std::exception &e) {
    std::cerr << e.what() << '\n';
    std::exit(EXIT_FAILURE);
  }
}

inline bool Sudoku::puzzle_is_valid() const {
  return check_houses(house::row) && check_houses(house::column) &&
         check_houses(house::box);
}

inline bool Sudoku::check_houses(const house tag) const {
  auto &houses = tag == house::row      ? indices::rows
                 : tag == house::column ? indices::columns
                                        : indices::boxes;
  for (const auto &house : houses) {
    std::bitset<10> seen;
    for (const auto index : house) {
      if (_puzzle[index] != 0) {
        if (seen[_puzzle[index]]) {
          return false;
        }
        seen.set(_puzzle[index]);
      }
    }
  }
  return true;
}

/**
 * @brief Is this value valid at this index?
 * @details Used to create a list of candidate values for each empty square.
 * Check the row, column, and box of the given index for the given value.
 * @param value a candidate value
 * @param val_index the cell in which it might be placed
 * @return True - the candidate value is valid
 * False - the candidate value is invalid
 */
inline bool Sudoku::check_validity(const int value, const int val_index) const {
  for (int i = 0; i < 3; ++i) {
    std::bitset<10> seen;
    seen.set(value);
    for (const auto index :
         i == 2   ? indices::boxes[indices::associations[val_index][i]]
         : i == 1 ? indices::columns[indices::associations[val_index][i]]
                  : indices::rows[indices::associations[val_index][i]]) {
      if (_puzzle[index] != 0) {
        if (seen[_puzzle[index]]) {
          return false;
        }
        seen.set(_puzzle[index]);
      }
    }
  }
  return true;
}

// end checking validity -------------------------------------------------------

// begin - create candidates ---------------------------------------------------

inline void Sudoku::find_unknown_indices() {
  _unknown.reset();
  for (int i = 0; i < PUZZLE_SIZE; ++i) {
    if (_puzzle[i] == UNKNOWN) {
      _unknown.set(i);
    }
  }
}

inline void Sudoku::find_candidates() {
  _candidates.clear();
  _candidates.resize(PUZZLE_SIZE);
  for (int i = 0; i < PUZZLE_SIZE; ++i) {
    if (_unknown[i]) {
      for (int j = 1; j < 10; ++j) {
        if (check_validity(j, i)) {
          _candidates[i].push_back(j);
        }
      }
    }
  }
}

// end - create candidates -----------------------------------------------------

// begin - prune candidates ----------------------------------------------------

// hidden pair/triple/quad
// for each row, column, and box
// if there exists an identical pair of candidates in two cells, but nowhere
// else in the house, then all other candidates from those cells can be
// removed.

// claiming locked candidates
// https://hodoku.sourceforge.net/en/tech_intersections.php
// If in a row (or column) all candidates of a certain digit are confined to
// one block, that candidate that be eliminated from all other cells in that
// block.
// identify if a candidate only appears in one box or a row or column

// pointing locked candidates
// https://hodoku.sourceforge.net/en/tech_intersections.php
// If in a block all candidates of a certain digit are confined to a row or
// column, that digit cannot appear outside of that block in that row or
// column.
// identify if a candidate only appears in one row or column of a box

inline void Sudoku::prune_hidden_subsets(const house tag) {
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
}

inline void Sudoku::prune_claiming_locked_candidates(const house tag) {
  const auto &houses = tag == house::row ? indices::rows : indices::columns;
  for (int i = 0; i < 9; ++i) {
    std::vector<std::vector<int>> set(3); // to hold candidates from each box
    for (int j = 0; j < 9; ++j) {
      if (j < 3) {
        for (const auto candidate : _candidates[houses[i][j]]) {
          set[0].push_back(candidate); // first box
        }
      } else if (j < 6) {
        for (const auto candidate : _candidates[houses[i][j]]) {
          set[1].push_back(candidate); // second box
        }
      } else {
        for (const auto candidate : _candidates[houses[i][j]]) {
          set[2].push_back(candidate); // third box
        }
      }
    }
    // find candidates that are in one box but not the others
    std::map<int, int> frequency;
    for (const auto &list : set) {
      for (const auto val : list) {
        frequency[val]++;
      }
    }

    std::vector<std::vector<int>> unique_candidates(3);
    for (int k = 0; k < 3; ++k) {
      std::unordered_set<int> unique;
      for (auto val : set[k]) {
        if (frequency[val] == std::count(set[k].begin(), set[k].end(), val)) {
          unique.insert(val);
        }
      }
      unique_candidates[k].assign(unique.begin(), unique.end());
    }

    // remove duplicates from lists
    for (auto &list : unique_candidates) {
      std::sort(list.begin(), list.end());
      list.erase(std::unique(list.begin(), list.end()), list.end());
    }

    // if all instances of a candidate in a row are confined to a given block,
    // then that candidate may be removed from all other cells in that block
    for (int k = 0; k < 3; ++k) {
      if (!unique_candidates[k].empty()) {
        for (const auto val : unique_candidates[k]) {
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
          for (const auto index : indices::boxes[choose_box()]) {
            if (std::find(houses[i].begin(), houses[i].end(), index) ==
                houses[i].end()) {
              _candidates[index].erase(std::remove(_candidates[index].begin(),
                                                   _candidates[index].end(),
                                                   val),
                                       _candidates[index].end());
            }
          }
        }
      }
    }
  }
}

inline void Sudoku::prune_pointing_locked_candidates() {
  for (int i = 0; i < 9; ++i) {   // intersections
    for (int j = 0; j < 2; ++j) { // 0: rows, 1: columns
      std::vector<std::vector<int>> set(3);
      for (int k = 0; k < 3; ++k) {   // the indices of the intersection
        for (int l = 0; l < 3; ++l) { // index
          for (const auto candidate :
               _candidates[indices::subranges[i][j][k]->at(l)]) {
            set[k].push_back(candidate);
          }
        }
      }
      // find values that are in one subset but not the others
      std::map<int, int> frequency;
      for (const auto &list : set) {
        for (const auto value : list) {
          frequency[value]++;
        }
      }

      std::vector<std::vector<int>> unique_candidates(3);
      for (int k = 0; k < 3; ++k) {
        std::unordered_set<int> unique;
        for (auto val : set[k]) {
          if (frequency[val] == std::count(set[k].begin(), set[k].end(), val)) {
            unique.insert(val);
          }
        }
        unique_candidates[k].assign(unique.begin(), unique.end());
      }

      // remove duplicate values
      for (auto &list : unique_candidates) {
        std::sort(list.begin(), list.end());
        list.erase(std::unique(list.begin(), list.end()), list.end());
      }
      // i : box
      // j : 0 -> rows, 1 -> columns
      // k : first, second, third row/column in the given box
      // l : first, second, third index of the given row/column
      for (int k = 0; k < 3; ++k) {
        if (!unique_candidates[k].empty()) {
          for (const auto val : unique_candidates[k]) {
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
              for (const auto index : indices::rows[choose_row()]) {
                if (std::find(indices::subranges[i][j][k]->begin(),
                              indices::subranges[i][j][k]->end(),
                              index) == indices::subranges[i][j][k]->end()) {
                  for (auto it = _candidates[index].begin();
                       it != _candidates[index].end();) {
                    if (*it == val) {
                      it = _candidates[index].erase(it);
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
              for (const auto index : indices::columns[choose_column()]) {
                if (std::find(indices::subranges[i][j][k]->begin(),
                              indices::subranges[i][j][k]->end(),
                              index) == indices::subranges[i][j][k]->end()) {
                  for (auto it = _candidates[index].begin();
                       it != _candidates[index].end();) {
                    if (*it == val) {
                      it = _candidates[index].erase(it);
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
}

// end - prune candidates ------------------------------------------------------

inline void Sudoku::reset_singles() {
  std::cout << "Finding hidden singles ...\n\n";
  _singles.fill(0);
}

inline void Sudoku::find_implicit_singles(const house tag) {
  const auto &houses = tag == house::row      ? indices::rows
                       : tag == house::column ? indices::columns
                                              : indices::boxes;
  const auto label = tag == house::row      ? "row"
                     : tag == house::column ? "col"
                                            : "box";
  for (int i = 0; i < houses.size(); ++i) {
    std::vector<int> candidate_occurrences(10);
    for (const auto index : houses[i]) {
      for (const auto candidate : _candidates[index]) {
        candidate_occurrences[candidate]++;
      }
    }
    for (int j = 1; j < 10; ++j) {
      if (candidate_occurrences[j] == 1) {
        for (const auto index : houses[i]) {
          for (const auto candidate : _candidates[index]) {
            if (candidate == j) {
              _singles[index] = j;
            }
          }
        }
      }
    }
    std::cout << label << i << " occurrences: ";
    for (const int j : candidate_occurrences) {
      std::cout << ' ' << j << ' ';
    }
    std::cout << '\n';
  }
  std::cout << '\n';
}

inline void Sudoku::solve_explicit_singles() {
  bool printed_message = false;
  for (int i = 0; i < _candidates.size(); ++i) {
    if (_candidates[i].size() == 1) {
      _puzzle[i] = _candidates[i][0];
      _candidates[i].clear();
      _unknown.reset(i);
      if (!printed_message) {
        std::cout << "Applying naked singles ...\n\n";
        printed_message = true;
      }
    }
  }
}

inline void Sudoku::solve_implicit_singles() {
  bool printed_message = false;
  for (int i = 0; i < PUZZLE_SIZE; ++i) {
    if (_singles[i] != 0) {
      _puzzle[i] = _singles[i];
      _singles[i] = 0;
      _unknown.reset(i);
      _sc = true;
      if (!printed_message) {
        std::cout << "Applying hidden singles ...\n\n";
        printed_message = true;
      }
    }
  }
}

// begin - printing ------------------------------------------------------------

inline void Sudoku::print_puzzle() const {
  for (int i = 0; i < _puzzle.size(); ++i) {
    if (_puzzle[i] != 0) {
      std::cout << ' ' << _puzzle[i] << ' ';
    } else {
      std::cout << " . ";
    }
    if ((i + 1) % 9 == 0) {
      std::cout << '\n';
    }
  }
  std::cout << '\n';
}

inline void Sudoku::print_puzzle(const std::vector<int> &vec) {
  for (int i = 0; i < vec.size(); ++i) {
    if (vec[i] != 0) {
      std::cout << ' ' << vec[i] << ' ';
    } else {
      std::cout << " . ";
    }
    if ((i + 1) % 9 == 0) {
      std::cout << '\n';
    }
  }
  std::cout << '\n';
}

inline void Sudoku::print_unknown() {
  std::cout << "Unknown Cells:\n";
  for (int i = 0; i < PUZZLE_SIZE; ++i) {
    if (_unknown[i]) {
      std::cout << i << " ";
    }
  }
  std::cout << "\n\n";
}

inline void Sudoku::print_candidates(const std::string &str,
                                     const std::vector<std::vector<int>> &vec) {
  std::cout << str;
  for (int i = 0; i < vec.size(); ++i) {
    if (!vec[i].empty()) {
      std::cout << "Cell " << i << " : ";
      for (const int j : vec[i]) {
        std::cout << ' ' << j << ' ';
      }
      std::cout << '\n';
    }
  }
  std::cout << '\n';
}

inline void Sudoku::print_singles() const {
  bool printed_message = false;
  for (int i = 0; i < PUZZLE_SIZE; ++i) {
    if (_singles[i] != 0) {
      if (!printed_message) {
        std::cout << "Hidden singles:\n";
        printed_message = true;
      }
      std::cout << "Cell " << i << " : " << _singles[i] << '\n';
    }
  }
  std::cout << '\n';
}

inline void Sudoku::print_readout() const {
  for (const auto &puzzle : _puzzles) {
    print_puzzle(puzzle);
  }
  std::cout << "Applied " << _iterations
            << " iterations of deductive logic.\n\n";
}

#endif // SUDOKU_HPP_
