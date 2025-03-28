#include <algorithm>
#include <array>
#include <bitset>
#include <cassert>
#include <iostream>
#include <list>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#define EASY_
#define MEDM_
#define HARD_
#define MAST_Y

#ifdef EASY_Y
std::vector<int> puzzle = {4, 8, 1, 3, 0, 0, 0, 7, 0, 5, 0, 7, 9, 8, 2, 0, 0,
                           0, 3, 0, 0, 1, 0, 0, 0, 6, 8, 0, 3, 0, 0, 0, 0, 1,
                           8, 5, 0, 9, 0, 0, 2, 5, 0, 0, 4, 0, 7, 0, 4, 0, 3,
                           0, 0, 9, 0, 0, 0, 2, 9, 0, 8, 0, 0, 6, 1, 2, 0, 0,
                           0, 4, 0, 0, 0, 0, 8, 0, 4, 7, 2, 1, 0};
#endif

#ifdef MEDM_Y
std::vector<int> puzzle = {0, 0, 0, 0, 0, 2, 8, 0, 6, 3, 0, 1, 0, 0, 0, 2, 0,
                           0, 0, 2, 0, 5, 0, 6, 0, 0, 0, 0, 0, 0, 3, 8, 0, 0,
                           0, 5, 0, 3, 0, 2, 0, 0, 0, 1, 0, 0, 0, 0, 0, 9, 0,
                           3, 0, 0, 0, 0, 9, 0, 6, 0, 5, 0, 0, 0, 4, 0, 0, 0,
                           0, 0, 0, 7, 1, 0, 0, 8, 0, 0, 0, 0, 0};
#endif

#ifdef HARD_Y
std::vector<int> puzzle = {2, 9, 1, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 3, 9, 0,
                           0, 0, 4, 0, 0, 0, 6, 0, 0, 0, 0, 6, 0, 2, 3, 0, 0,
                           9, 0, 0, 5, 7, 0, 6, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
                           5, 0, 0, 0, 0, 6, 0, 8, 0, 0, 0, 5, 0, 0, 0, 6, 0,
                           0, 8, 2, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0};
#endif

#ifdef MAST_Y
std::vector<int> puzzle = {
    3, 0, 0, 0, 0, 0, 7, 6, 0, 0, 0, 2, 0, 0, 3, 0, 0, 0, 5, 0, 8,
    0, 7, 2, 0, 1, 0, 0, 4, 0, 0, 3, 0, 1, 2, 0, 0, 0, 1, 0, 6, 0,
    0, 0, 5, 0, 2, 0, 0, 0, 0, 0, 3, 6, 1, 0, 0, 0, 0, 0, 0, 7, 0,
    0, 0, 0, 0, 2, 0, 0, 4, 0, 0, 5, 0, 0, 0, 4, 0, 0, 3,
};
#endif

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

bool check_validity(int value, int val_index);
bool check_validity(const std::array<std::array<int, 9>, 9> &scope);
bool is_valid();
void print_puzzle();
void print_puzzle(const std::vector<int> &vec);
int iterations{};
std::vector<std::vector<int>> puzzles;

int main() {
  print_puzzle();

  try {
    if (puzzle.size() != 81 || !is_valid()) {
      throw std::runtime_error("puzzle is malformed");
    }
  } catch (const std::exception &e) {
    std::cerr << e.what() << '\n';
    std::exit(EXIT_FAILURE);
  }

  bool exists_single_candidates = true;

  while (exists_single_candidates) {
    puzzles.push_back(puzzle);

    exists_single_candidates = false;
    // list unknown indices
    std::vector<int> unknown_indices{};
    unknown_indices.reserve(puzzle.size());
    for (int i = 0; i < puzzle.size(); ++i) {
      if (puzzle[i] == 0) {
        unknown_indices.push_back(i);
      }
    }
    if (unknown_indices.empty()) { // exit if the puzzle is solved
      break;
    }

    std::cout << "Unknown cells: ";
    for (const auto num : unknown_indices) {
      std::cout << num << ' ';
    }
    std::cout << '\n';

    // create list of candidates
    std::vector<std::vector<int>> candidates(81);
    for (const auto index : unknown_indices) {
      for (int i = 1; i < 10; ++i) {
        if (check_validity(i, index)) {
          candidates[index].push_back(i);
        }
      }
    }

    // print candidates
    std::cout << "\nCandidates:\n";
    for (int i = 0; i < candidates.size(); ++i) {
      if (!candidates[i].empty()) {
        std::cout << "Cell " << i << " : ";
        for (const int j : candidates[i]) {
          std::cout << ' ' << j << ' ';
        }
        std::cout << '\n';
      }
    }
    std::cout << '\n';

    // prune list of candidates

    // hidden pair/triple/quad
    // for each row, column, and box
    // if there exists an identical pair of candidates in two cells, but nowhere
    // else in the house, then all other candidates from those cells can be
    // removed.

    // rows
    for (int i = 0; i < 9; ++i) { // for each row
      std::unordered_map<int, std::vector<int>> candidates_to_cells;
      // for each candidate, find in which cells of the house it resides
      for (int val = 1; val < 10; ++val) {
        for (const auto index : indices::rows[i]) {
          for (const auto candidate : candidates[index]) {
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
        // compare every pair to every other pair
        if (pairs.begin()->second == std::next(pairs.begin())->second) {
          for (int j = 0; j < 2; ++j) {
            candidates[pairs.begin()->second[j]].erase(
                std::remove_if(candidates[pairs.begin()->second[j]].begin(),
                               candidates[pairs.begin()->second[j]].end(),
                               [&pairs](auto x) {
                                 return !(x == pairs.begin()->first ||
                                          x == std::next(pairs.begin())->first);
                               }),
                candidates[pairs.begin()->second[j]].end());
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
            candidates[cell].erase(
                std::remove_if(candidates[cell].begin(), candidates[cell].end(),
                               [&values](auto x) {
                                 return std::find(values.begin(), values.end(),
                                                  x) == values.end();
                               }),
                candidates[cell].end());
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
            candidates[cell].erase(
                std::remove_if(candidates[cell].begin(), candidates[cell].end(),
                               [&values](auto x) {
                                 return std::find(values.begin(), values.end(),
                                                  x) == values.end();
                               }),
                candidates[cell].end());
          }
        }
      }
    }
    // columns
    for (int i = 0; i < 9; ++i) { // for each column
      std::unordered_map<int, std::vector<int>> candidates_to_cells;
      // for each candidate, find in which cells of the house it resides
      for (int val = 1; val < 10; ++val) {
        for (const auto index : indices::columns[i]) {
          for (const auto candidate : candidates[index]) {
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
        // compare every pair to every other pair
        if (pairs.begin()->second == std::next(pairs.begin())->second) {
          for (int j = 0; j < 2; ++j) {
            candidates[pairs.begin()->second[j]].erase(
                std::remove_if(candidates[pairs.begin()->second[j]].begin(),
                               candidates[pairs.begin()->second[j]].end(),
                               [&pairs](auto x) {
                                 return !(x == pairs.begin()->first ||
                                          x == std::next(pairs.begin())->first);
                               }),
                candidates[pairs.begin()->second[j]].end());
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
            candidates[cell].erase(
                std::remove_if(candidates[cell].begin(), candidates[cell].end(),
                               [&values](auto x) {
                                 return std::find(values.begin(), values.end(),
                                                  x) == values.end();
                               }),
                candidates[cell].end());
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
            candidates[cell].erase(
                std::remove_if(candidates[cell].begin(), candidates[cell].end(),
                               [&values](auto x) {
                                 return std::find(values.begin(), values.end(),
                                                  x) == values.end();
                               }),
                candidates[cell].end());
          }
        }
      }
    }
    // boxes
    for (int i = 0; i < 9; ++i) { // for each box
      std::unordered_map<int, std::vector<int>> candidates_to_cells;
      // for each candidate, find in which cells of the house it resides
      for (int val = 1; val < 10; ++val) {
        for (const auto index : indices::boxes[i]) {
          for (const auto candidate : candidates[index]) {
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
        // compare every pair to every other pair
        if (pairs.begin()->second == std::next(pairs.begin())->second) {
          for (int j = 0; j < 2; ++j) {
            candidates[pairs.begin()->second[j]].erase(
                std::remove_if(candidates[pairs.begin()->second[j]].begin(),
                               candidates[pairs.begin()->second[j]].end(),
                               [&pairs](auto x) {
                                 return !(x == pairs.begin()->first ||
                                          x == std::next(pairs.begin())->first);
                               }),
                candidates[pairs.begin()->second[j]].end());
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
            candidates[cell].erase(
                std::remove_if(candidates[cell].begin(), candidates[cell].end(),
                               [&values](auto x) {
                                 return std::find(values.begin(), values.end(),
                                                  x) == values.end();
                               }),
                candidates[cell].end());
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
            candidates[cell].erase(
                std::remove_if(candidates[cell].begin(), candidates[cell].end(),
                               [&values](auto x) {
                                 return std::find(values.begin(), values.end(),
                                                  x) == values.end();
                               }),
                candidates[cell].end());
          }
        }
      }
    }

    // claiming locked candidates
    // https://hodoku.sourceforge.net/en/tech_intersections.php
    // If in a row (or column) all candidates of a certain digit are confined to
    // one block, that candidate that be eliminated from all other cells in that
    // block.
    // identify if a candidate only appears in one box or a row or column

    // rows
    for (int i = 0; i < 9; ++i) {           // for each row
      std::vector<std::vector<int>> set(3); // to hold candidates from each box
      for (int j = 0; j < 9; ++j) {         // for each index in the row
        if (j < 3) {
          for (const auto candidate : candidates[indices::rows[i].at(j)]) {
            set[0].push_back(candidate); // first box
          }
        } else if (j < 6) {
          for (const auto candidate : candidates[indices::rows[i].at(j)]) {
            set[1].push_back(candidate); // second box
          }
        } else {
          for (const auto candidate : candidates[indices::rows[i].at(j)]) {
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
            auto choose_box = [&i, &k]() {
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
            for (const auto index : indices::boxes[choose_box()]) {
              if (std::find(indices::rows[i].begin(), indices::rows[i].end(),
                            index) == indices::rows[i].end()) {
                candidates[index].erase(std::remove(candidates[index].begin(),
                                                    candidates[index].end(),
                                                    val),
                                        candidates[index].end());
              }
            }
          }
        }
      }
    }
    // columns
    for (int i = 0; i < 9; ++i) {           // for each column
      std::vector<std::vector<int>> set(3); // to hold candidates from each box
      for (int j = 0; j < 9; ++j) {         // for each index in the column
        if (j < 3) {
          for (const auto candidate : candidates[indices::columns[i].at(j)]) {
            set[0].push_back(candidate); // first box
          }
        } else if (j < 6) {
          for (const auto candidate : candidates[indices::columns[i].at(j)]) {
            set[1].push_back(candidate); // second box
          }
        } else {
          for (const auto candidate : candidates[indices::columns[i].at(j)]) {
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

      // if all instances of a candidate in a column are confined to a given
      // block, then that candidate may be removed from all other cells in that
      // block
      for (int k = 0; k < 3; ++k) {
        if (!unique_candidates[k].empty()) {
          for (const auto val : unique_candidates[k]) {
            auto choose_box = [&i, &k]() {
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
            for (const auto index : indices::boxes[choose_box()]) {
              if (std::find(indices::columns[i].begin(),
                            indices::columns[i].end(),
                            index) == indices::columns[i].end()) {
                candidates[index].erase(std::remove(candidates[index].begin(),
                                                    candidates[index].end(),
                                                    val),
                                        candidates[index].end());
              }
            }
          }
        }
      }
    }

    // pointing locked candidates
    // https://hodoku.sourceforge.net/en/tech_intersections.php
    // If in a block all candidates of a certain digit are confined to a row or
    // column, that digit cannot appear outside of that block in that row or
    // column.
    // identify if a candidate only appears in one row or column of a box

    for (int i = 0; i < 9; ++i) {   // intersections
      for (int j = 0; j < 2; ++j) { // 0: rows, 1: columns
        std::vector<std::vector<int>> set(3);
        for (int k = 0; k < 3; ++k) {   // the indices of the intersection
          for (int l = 0; l < 3; ++l) { // index
            for (const auto candidate :
                 candidates[indices::subranges[i][j][k]->at(l)]) {
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
            if (frequency[val] ==
                std::count(set[k].begin(), set[k].end(), val)) {
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
                    for (auto it = candidates[index].begin();
                         it != candidates[index].end();) {
                      if (*it == val) {
                        it = candidates[index].erase(it);
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
                    for (auto it = candidates[index].begin();
                         it != candidates[index].end();) {
                      if (*it == val) {
                        it = candidates[index].erase(it);
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

    // print candidates
    std::cout << "\nPruned Candidates:\n";
    for (int i = 0; i < candidates.size(); ++i) {
      if (!candidates[i].empty()) {
        std::cout << "Cell " << i << " : ";
        for (const int j : candidates[i]) {
          std::cout << ' ' << j << ' ';
        }
        std::cout << '\n';
      }
    }
    std::cout << '\n';

    // for each of the cells that has only one candidate, update the puzzle with
    // that value
    bool printed_message = false;
    for (int i = 0; i < candidates.size(); ++i) {
      if (candidates[i].size() == 1) {
        puzzle[i] = candidates[i][0];
        if (!printed_message) {
          std::cout << "Applying explicit singles ...\n\n";
          printed_message = true;
        }
      }
    }

    // check for implicit single candidates
    std::vector<int> singles(puzzle.size());

    // check rows
    for (int i = 0; i < indices::rows.size(); ++i) {
      std::vector<int> candidate_occurrences(10);
      for (const auto index : indices::rows[i]) {
        for (const auto candidate : candidates[index]) {
          candidate_occurrences[candidate]++;
        }
      }
      for (int j = 1; j < 10; ++j) {
        if (candidate_occurrences[j] == 1) {
          exists_single_candidates = true;
          for (const auto index : indices::rows[i]) {
            for (const auto candidate : candidates[index]) {
              if (candidate == j) {
                singles[index] = j;
              }
            }
          }
        }
      }
      std::cout << "row " << i << " occurrences: ";
      for (const int j : candidate_occurrences) {
        std::cout << ' ' << j << ' ';
      }
      std::cout << '\n';
    }
    std::cout << '\n';

    // check columns
    for (int i = 0; i < indices::columns.size(); ++i) {
      std::vector<int> candidate_occurrences(10);
      for (const auto index : indices::columns[i]) {
        for (const auto candidate : candidates[index]) {
          candidate_occurrences[candidate]++;
        }
      }
      for (int j = 1; j < 10; ++j) {
        if (candidate_occurrences[j] == 1) {
          exists_single_candidates = true;
          for (const auto index : indices::columns[i]) {
            for (const auto candidate : candidates[index]) {
              if (candidate == j) {
                singles[index] = j;
              }
            }
          }
        }
      }
      std::cout << "column " << i << " occurrences: ";
      for (const int j : candidate_occurrences) {
        std::cout << ' ' << j << ' ';
      }
      std::cout << '\n';
    }
    std::cout << '\n';

    // check boxes
    for (int i = 0; i < indices::boxes.size(); ++i) {
      std::vector<int> candidate_occurrences(10);
      for (const auto index : indices::boxes[i]) {
        for (const auto candidate : candidates[index]) {
          candidate_occurrences[candidate]++;
        }
      }
      for (int j = 1; j < 10; ++j) {
        if (candidate_occurrences[j] == 1) {
          exists_single_candidates = true;
          for (const auto index : indices::boxes[i]) {
            for (const auto candidate : candidates[index]) {
              if (candidate == j) {
                singles[index] = j;
              }
            }
          }
        }
      }
      std::cout << "box " << i << " occurrences: ";
      for (const int j : candidate_occurrences) {
        std::cout << ' ' << j << ' ';
      }
      std::cout << '\n';
    }
    std::cout << '\n';

    std::cout << "Explicit or Implicit single candidates:\n";
    for (int i = 0; i < singles.size(); ++i) {
      if (singles[i] != 0) {
        std::cout << "Cell " << i << " : " << singles[i] << '\n';
      }
    }
    std::cout << '\n';

    // for each of the cells that has an implied candidate, update the puzzle
    // with that value
    for (int i = 0; i < singles.size(); ++i) {
      if (singles[i] != 0) {
        puzzle[i] = singles[i];
      }
    }

    print_puzzle();
    ++iterations;
    // check for errors
    if (!is_valid()) {
      std::cout << "AAAAHHH" << '\n';
    }
  }
  for (const auto &puzzle : puzzles) {
    print_puzzle(puzzle);
  }
  std::cout << "Applied " << iterations
            << " iterations of deductive logic.\n\n";
}

bool check_validity(const std::array<std::array<int, 9>, 9> &scope) {
  for (const auto list : scope) {
    std::bitset<10> seen;
    for (const auto index : list) {
      if (puzzle[index] != 0) {
        if (seen[puzzle[index]]) {
          return false;
        }
        seen.set(puzzle[index]);
      }
    }
  }
  return true;
}

/**
 * @brief Used to create a list of candidate values for each empty square
 * @param value a candidate value
 * @param val_index the cell in which it might be placed
 * @return True - the candidate value is valid
 * False - the candidate value is invalid
 */
bool check_validity(const int value, const int val_index) {
  for (int i = 0; i < 3; ++i) {
    std::bitset<10> seen;
    seen.set(value);
    for (const auto index :
         i == 2   ? indices::boxes[indices::associations[val_index][i]]
         : i == 1 ? indices::columns[indices::associations[val_index][i]]
                  : indices::rows[indices::associations[val_index][i]]) {
      if (puzzle[index] != 0) {
        if (seen[puzzle[index]]) {
          return false;
        }
        seen.set(puzzle[index]);
      }
    }
  }
  return true;
}

bool is_valid() {
  return check_validity(indices::rows) && check_validity(indices::columns) &&
         check_validity(indices::boxes);
}

void print_puzzle() {
  for (int i = 0; i < puzzle.size(); ++i) {
    if (puzzle[i] != 0) {
      std::cout << ' ' << puzzle[i] << ' ';
    } else {
      std::cout << " . ";
    }
    if ((i + 1) % 9 == 0) {
      std::cout << '\n';
    }
  }
  std::cout << '\n';
}

void print_puzzle(const std::vector<int> &vec) {
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
