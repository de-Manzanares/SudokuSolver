#include <array>
#include <bitset>
#include <iostream>
#include <vector>

std::vector<int> puzzle = {4, 8, 1, 3, 0, 0, 0, 7, 0, 5, 0, 7, 9, 8, 2, 0, 0,
                           0, 3, 0, 0, 1, 0, 0, 0, 6, 8, 0, 3, 0, 0, 0, 0, 1,
                           8, 5, 0, 9, 0, 0, 2, 5, 0, 0, 4, 0, 7, 0, 4, 0, 3,
                           0, 0, 9, 0, 0, 0, 2, 9, 0, 8, 0, 0, 6, 1, 2, 0, 0,
                           0, 4, 0, 0, 0, 0, 8, 0, 4, 7, 2, 1, 0};

// std::vector<int> puzzle = {0, 0, 0, 0, 0, 2, 8, 0, 6, 3, 0, 1, 0, 0, 0, 2, 0,
//                            0, 0, 2, 0, 5, 0, 6, 0, 0, 0, 0, 0, 0, 3, 8, 0, 0,
//                            0, 5, 0, 3, 0, 2, 0, 0, 0, 1, 0, 0, 0, 0, 0, 9, 0,
//                            3, 0, 0, 0, 0, 9, 0, 6, 0, 5, 0, 0, 0, 4, 0, 0, 0,
//                            0, 0, 0, 7, 1, 0, 0, 8, 0, 0, 0, 0, 0};

struct indices {
  using row = std::array<int, 9>;
  static constexpr row row0 = {0, 1, 2, 3, 4, 5, 6, 7, 8};
  static constexpr row row1 = {9, 10, 11, 12, 13, 14, 15, 16, 17};
  static constexpr row row2 = {18, 19, 20, 21, 22, 23, 24, 25, 26};
  static constexpr row row3 = {27, 28, 29, 30, 31, 32, 33, 34, 35};
  static constexpr row row4 = {36, 37, 38, 39, 40, 41, 42, 43, 44};
  static constexpr row row5 = {45, 46, 47, 48, 49, 50, 51, 52, 53};
  static constexpr row row6 = {54, 55, 56, 57, 58, 59, 60, 61, 62};
  static constexpr row row7 = {63, 64, 65, 66, 67, 68, 69, 70, 71};
  static constexpr row row8 = {72, 73, 74, 75, 76, 77, 78, 79, 80};
  static constexpr std::array<const row *, 9> rows = {
      &row0, &row1, &row2, &row3, &row4, &row5, &row6, &row7, &row8};

  using column = std::array<int, 9>;
  static constexpr column column0 = {0, 9, 18, 27, 36, 45, 54, 63, 72};
  static constexpr column column1 = {1, 10, 19, 28, 37, 46, 55, 64, 73};
  static constexpr column column2 = {2, 11, 20, 29, 38, 47, 56, 65, 74};
  static constexpr column column3 = {3, 12, 21, 30, 39, 48, 57, 66, 75};
  static constexpr column column4 = {4, 13, 22, 31, 40, 49, 58, 67, 76};
  static constexpr column column5 = {5, 14, 23, 32, 41, 50, 59, 68, 77};
  static constexpr column column6 = {6, 15, 24, 33, 42, 51, 60, 69, 78};
  static constexpr column column7 = {7, 16, 25, 34, 43, 52, 61, 70, 79};
  static constexpr column column8 = {8, 17, 26, 35, 44, 53, 62, 71, 80};
  static constexpr std::array<const column *, 9> columns = {
      &column0, &column1, &column2, &column3, &column4,
      &column5, &column6, &column7, &column8};

  using subgrid = std::array<int, 9>;
  static constexpr subgrid subgrid0 = {0, 1, 2, 9, 10, 11, 18, 19, 20};
  static constexpr subgrid subgrid1 = {3, 4, 5, 12, 13, 14, 21, 22, 23};
  static constexpr subgrid subgrid2 = {6, 7, 8, 15, 16, 17, 24, 25, 26};
  static constexpr subgrid subgrid3 = {27, 28, 29, 36, 37, 38, 45, 46, 47};
  static constexpr subgrid subgrid4 = {30, 31, 32, 39, 40, 41, 48, 49, 50};
  static constexpr subgrid subgrid5 = {33, 34, 35, 42, 43, 44, 51, 52, 53};
  static constexpr subgrid subgrid6 = {54, 55, 56, 63, 64, 65, 72, 73, 74};
  static constexpr subgrid subgrid7 = {57, 58, 59, 66, 67, 68, 75, 76, 77};
  static constexpr subgrid subgrid8 = {60, 61, 62, 69, 70, 71, 78, 79, 80};
  static constexpr std::array<const subgrid *, 9> subgrids = {
      &subgrid0, &subgrid1, &subgrid2, &subgrid3, &subgrid4,
      &subgrid5, &subgrid6, &subgrid7, &subgrid8};

  using group = const std::array<int, 9> *;
  using triplet = const std::array<group, 3>;
  static constexpr std::array<triplet, 81> associations = {
      {{rows[0], columns[0], subgrids[0]},
       {rows[0], columns[1], subgrids[0]},
       {rows[0], columns[2], subgrids[0]},
       {rows[0], columns[3], subgrids[1]},
       {rows[0], columns[4], subgrids[1]},
       {rows[0], columns[5], subgrids[1]},
       {rows[0], columns[6], subgrids[2]},
       {rows[0], columns[7], subgrids[2]},
       {rows[0], columns[8], subgrids[2]},
       //
       {rows[1], columns[0], subgrids[0]},
       {rows[1], columns[1], subgrids[0]},
       {rows[1], columns[2], subgrids[0]},
       {rows[1], columns[3], subgrids[1]},
       {rows[1], columns[4], subgrids[1]},
       {rows[1], columns[5], subgrids[1]},
       {rows[1], columns[6], subgrids[2]},
       {rows[1], columns[7], subgrids[2]},
       {rows[1], columns[8], subgrids[2]},
       //
       {rows[2], columns[0], subgrids[0]},
       {rows[2], columns[1], subgrids[0]},
       {rows[2], columns[2], subgrids[0]},
       {rows[2], columns[3], subgrids[1]},
       {rows[2], columns[4], subgrids[1]},
       {rows[2], columns[5], subgrids[1]},
       {rows[2], columns[6], subgrids[2]},
       {rows[2], columns[7], subgrids[2]},
       {rows[2], columns[8], subgrids[2]},
       //
       {rows[3], columns[0], subgrids[3]},
       {rows[3], columns[1], subgrids[3]},
       {rows[3], columns[2], subgrids[3]},
       {rows[3], columns[3], subgrids[4]},
       {rows[3], columns[4], subgrids[4]},
       {rows[3], columns[5], subgrids[4]},
       {rows[3], columns[6], subgrids[5]},
       {rows[3], columns[7], subgrids[5]},
       {rows[3], columns[8], subgrids[5]},
       //
       {rows[4], columns[0], subgrids[3]},
       {rows[4], columns[1], subgrids[3]},
       {rows[4], columns[2], subgrids[3]},
       {rows[4], columns[3], subgrids[4]},
       {rows[4], columns[4], subgrids[4]},
       {rows[4], columns[5], subgrids[4]},
       {rows[4], columns[6], subgrids[5]},
       {rows[4], columns[7], subgrids[5]},
       {rows[4], columns[8], subgrids[5]},
       //
       {rows[5], columns[0], subgrids[3]},
       {rows[5], columns[1], subgrids[3]},
       {rows[5], columns[2], subgrids[3]},
       {rows[5], columns[3], subgrids[4]},
       {rows[5], columns[4], subgrids[4]},
       {rows[5], columns[5], subgrids[4]},
       {rows[5], columns[6], subgrids[5]},
       {rows[5], columns[7], subgrids[5]},
       {rows[5], columns[8], subgrids[5]},
       //
       {rows[6], columns[0], subgrids[6]},
       {rows[6], columns[1], subgrids[6]},
       {rows[6], columns[2], subgrids[6]},
       {rows[6], columns[3], subgrids[7]},
       {rows[6], columns[4], subgrids[7]},
       {rows[6], columns[5], subgrids[7]},
       {rows[6], columns[6], subgrids[8]},
       {rows[6], columns[7], subgrids[8]},
       {rows[6], columns[8], subgrids[8]},
       //
       {rows[7], columns[0], subgrids[6]},
       {rows[7], columns[1], subgrids[6]},
       {rows[7], columns[2], subgrids[6]},
       {rows[7], columns[3], subgrids[7]},
       {rows[7], columns[4], subgrids[7]},
       {rows[7], columns[5], subgrids[7]},
       {rows[7], columns[6], subgrids[8]},
       {rows[7], columns[7], subgrids[8]},
       {rows[7], columns[8], subgrids[8]},
       //
       {rows[8], columns[0], subgrids[6]},
       {rows[8], columns[1], subgrids[6]},
       {rows[8], columns[2], subgrids[6]},
       {rows[8], columns[3], subgrids[7]},
       {rows[8], columns[4], subgrids[7]},
       {rows[8], columns[5], subgrids[7]},
       {rows[8], columns[6], subgrids[8]},
       {rows[8], columns[7], subgrids[8]},
       {rows[8], columns[8], subgrids[8]}}};
};

bool check_validity(int value, int val_index);
bool check_validity(const std::array<const std::array<int, 9> *, 9> &scope);
bool is_valid();
void print_puzzle();
int iterations{};

int main() {
  print_puzzle();

  bool exists_single_candidates = true;

  while (exists_single_candidates) {
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

    // check for single candidates
    for (const auto &list : candidates) {
      exists_single_candidates = false;
      if (list.size() == 1) {
        exists_single_candidates = true;
        break;
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

    // for each of the cells that has only one candidate, update the puzzle with
    // that value
    std::cout << "Applying explicit singles ...\n\n";
    for (int i = 0; i < candidates.size(); ++i) {
      if (candidates[i].size() == 1) {
        puzzle[i] = candidates[i][0];
      }
    }

    // check for implicit single candidates
    std::vector<int> singles(puzzle.size());

    // check rows
    for (int i = 0; i < indices::rows.size(); ++i) {
      std::vector<int> candidate_occurrences(10);
      for (const auto index : *indices::rows[i]) {
        for (const auto candidate : candidates[index]) {
          candidate_occurrences[candidate]++;
        }
      }
      for (int j = 1; j < 10; ++j) {
        if (candidate_occurrences[j] == 1) {
          exists_single_candidates = true;
          for (const auto index : *indices::rows[i]) {
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
      for (const auto index : *indices::columns[i]) {
        for (const auto candidate : candidates[index]) {
          candidate_occurrences[candidate]++;
        }
      }
      for (int j = 1; j < 10; ++j) {
        if (candidate_occurrences[j] == 1) {
          exists_single_candidates = true;
          for (const auto index : *indices::columns[i]) {
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

    // check subgrids
    for (int i = 0; i < indices::subgrids.size(); ++i) {
      std::vector<int> candidate_occurrences(10);
      for (const auto index : *indices::subgrids[i]) {
        for (const auto candidate : candidates[index]) {
          candidate_occurrences[candidate]++;
        }
      }
      for (int j = 1; j < 10; ++j) {
        if (candidate_occurrences[j] == 1) {
          exists_single_candidates = true;
          for (const auto index : *indices::subgrids[i]) {
            for (const auto candidate : candidates[index]) {
              if (candidate == j) {
                singles[index] = j;
              }
            }
          }
        }
      }
      std::cout << "subgrid " << i << " occurrences: ";
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
  std::cout << "Applied " << iterations << " iterations of deductive logic.\n";
}

bool check_validity(const std::array<const std::array<int, 9> *, 9> &scope) {
  for (const auto list : scope) {
    std::bitset<10> seen;
    for (const auto index : *list) {
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
  for (const auto list : indices::associations[val_index]) {
    std::bitset<10> seen;
    seen.set(value);
    for (const auto index : *list) {
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
         check_validity(indices::subgrids);
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
