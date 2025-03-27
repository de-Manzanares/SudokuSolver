#include <algorithm>
#include <array>
#include <bitset>
#include <iostream>
#include <list>
#include <map>
#include <unordered_set>
#include <vector>

// std::vector<int> puzzle = {4, 8, 1, 3, 0, 0, 0, 7, 0, 5, 0, 7, 9, 8, 2, 0, 0,
//                            0, 3, 0, 0, 1, 0, 0, 0, 6, 8, 0, 3, 0, 0, 0, 0, 1,
//                            8, 5, 0, 9, 0, 0, 2, 5, 0, 0, 4, 0, 7, 0, 4, 0, 3,
//                            0, 0, 9, 0, 0, 0, 2, 9, 0, 8, 0, 0, 6, 1, 2, 0, 0,
//                            0, 4, 0, 0, 0, 0, 8, 0, 4, 7, 2, 1, 0};

std::vector<int> puzzle = {0, 0, 0, 0, 0, 2, 8, 0, 6, 3, 0, 1, 0, 0, 0, 2, 0,
                           0, 0, 2, 0, 5, 0, 6, 0, 0, 0, 0, 0, 0, 3, 8, 0, 0,
                           0, 5, 0, 3, 0, 2, 0, 0, 0, 1, 0, 0, 0, 0, 0, 9, 0,
                           3, 0, 0, 0, 0, 9, 0, 6, 0, 5, 0, 0, 0, 4, 0, 0, 0,
                           0, 0, 0, 7, 1, 0, 0, 8, 0, 0, 0, 0, 0};

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

  using box = std::array<int, 9>;
  static constexpr box box0 = {0, 1, 2, 9, 10, 11, 18, 19, 20};
  static constexpr box box1 = {3, 4, 5, 12, 13, 14, 21, 22, 23};
  static constexpr box box2 = {6, 7, 8, 15, 16, 17, 24, 25, 26};
  static constexpr box box3 = {27, 28, 29, 36, 37, 38, 45, 46, 47};
  static constexpr box box4 = {30, 31, 32, 39, 40, 41, 48, 49, 50};
  static constexpr box box5 = {33, 34, 35, 42, 43, 44, 51, 52, 53};
  static constexpr box box6 = {54, 55, 56, 63, 64, 65, 72, 73, 74};
  static constexpr box box7 = {57, 58, 59, 66, 67, 68, 75, 76, 77};
  static constexpr box box8 = {60, 61, 62, 69, 70, 71, 78, 79, 80};
  static constexpr std::array<const box *, 9> boxes = {
      &box0, &box1, &box2, &box3, &box4, &box5, &box6, &box7, &box8};

  using group = const std::array<int, 9> *;
  using triplet = const std::array<group, 3>;
  static constexpr std::array<triplet, 81> associations = {
      {{rows[0], columns[0], boxes[0]},
       {rows[0], columns[1], boxes[0]},
       {rows[0], columns[2], boxes[0]},
       {rows[0], columns[3], boxes[1]},
       {rows[0], columns[4], boxes[1]},
       {rows[0], columns[5], boxes[1]},
       {rows[0], columns[6], boxes[2]},
       {rows[0], columns[7], boxes[2]},
       {rows[0], columns[8], boxes[2]},
       //
       {rows[1], columns[0], boxes[0]},
       {rows[1], columns[1], boxes[0]},
       {rows[1], columns[2], boxes[0]},
       {rows[1], columns[3], boxes[1]},
       {rows[1], columns[4], boxes[1]},
       {rows[1], columns[5], boxes[1]},
       {rows[1], columns[6], boxes[2]},
       {rows[1], columns[7], boxes[2]},
       {rows[1], columns[8], boxes[2]},
       //
       {rows[2], columns[0], boxes[0]},
       {rows[2], columns[1], boxes[0]},
       {rows[2], columns[2], boxes[0]},
       {rows[2], columns[3], boxes[1]},
       {rows[2], columns[4], boxes[1]},
       {rows[2], columns[5], boxes[1]},
       {rows[2], columns[6], boxes[2]},
       {rows[2], columns[7], boxes[2]},
       {rows[2], columns[8], boxes[2]},
       //
       {rows[3], columns[0], boxes[3]},
       {rows[3], columns[1], boxes[3]},
       {rows[3], columns[2], boxes[3]},
       {rows[3], columns[3], boxes[4]},
       {rows[3], columns[4], boxes[4]},
       {rows[3], columns[5], boxes[4]},
       {rows[3], columns[6], boxes[5]},
       {rows[3], columns[7], boxes[5]},
       {rows[3], columns[8], boxes[5]},
       //
       {rows[4], columns[0], boxes[3]},
       {rows[4], columns[1], boxes[3]},
       {rows[4], columns[2], boxes[3]},
       {rows[4], columns[3], boxes[4]},
       {rows[4], columns[4], boxes[4]},
       {rows[4], columns[5], boxes[4]},
       {rows[4], columns[6], boxes[5]},
       {rows[4], columns[7], boxes[5]},
       {rows[4], columns[8], boxes[5]},
       //
       {rows[5], columns[0], boxes[3]},
       {rows[5], columns[1], boxes[3]},
       {rows[5], columns[2], boxes[3]},
       {rows[5], columns[3], boxes[4]},
       {rows[5], columns[4], boxes[4]},
       {rows[5], columns[5], boxes[4]},
       {rows[5], columns[6], boxes[5]},
       {rows[5], columns[7], boxes[5]},
       {rows[5], columns[8], boxes[5]},
       //
       {rows[6], columns[0], boxes[6]},
       {rows[6], columns[1], boxes[6]},
       {rows[6], columns[2], boxes[6]},
       {rows[6], columns[3], boxes[7]},
       {rows[6], columns[4], boxes[7]},
       {rows[6], columns[5], boxes[7]},
       {rows[6], columns[6], boxes[8]},
       {rows[6], columns[7], boxes[8]},
       {rows[6], columns[8], boxes[8]},
       //
       {rows[7], columns[0], boxes[6]},
       {rows[7], columns[1], boxes[6]},
       {rows[7], columns[2], boxes[6]},
       {rows[7], columns[3], boxes[7]},
       {rows[7], columns[4], boxes[7]},
       {rows[7], columns[5], boxes[7]},
       {rows[7], columns[6], boxes[8]},
       {rows[7], columns[7], boxes[8]},
       {rows[7], columns[8], boxes[8]},
       //
       {rows[8], columns[0], boxes[6]},
       {rows[8], columns[1], boxes[6]},
       {rows[8], columns[2], boxes[6]},
       {rows[8], columns[3], boxes[7]},
       {rows[8], columns[4], boxes[7]},
       {rows[8], columns[5], boxes[7]},
       {rows[8], columns[6], boxes[8]},
       {rows[8], columns[7], boxes[8]},
       {rows[8], columns[8], boxes[8]}}};

  using subrange = std::array<int, 3>;
  static constexpr subrange box0row0 = intersection(box0, row0);
  static constexpr subrange box0row1 = intersection(box0, row1);
  static constexpr subrange box0row2 = intersection(box0, row2);
  static constexpr subrange box0column0 = intersection(box0, column0);
  static constexpr subrange box0column1 = intersection(box0, column1);
  static constexpr subrange box0column2 = intersection(box0, column2);
  //
  static constexpr subrange box1row0 = intersection(box1, row0);
  static constexpr subrange box1row1 = intersection(box1, row1);
  static constexpr subrange box1row2 = intersection(box1, row2);
  static constexpr subrange box1column3 = intersection(box1, column3);
  static constexpr subrange box1column4 = intersection(box1, column4);
  static constexpr subrange box1column5 = intersection(box1, column5);
  //
  static constexpr subrange box2row0 = intersection(box2, row0);
  static constexpr subrange box2row1 = intersection(box2, row1);
  static constexpr subrange box2row2 = intersection(box2, row2);
  static constexpr subrange box2column6 = intersection(box2, column6);
  static constexpr subrange box2column7 = intersection(box2, column7);
  static constexpr subrange box2column8 = intersection(box2, column8);
  //
  static constexpr subrange box3row3 = intersection(box3, row3);
  static constexpr subrange box3row4 = intersection(box3, row4);
  static constexpr subrange box3row5 = intersection(box3, row5);
  static constexpr subrange box3column0 = intersection(box3, column0);
  static constexpr subrange box3column1 = intersection(box3, column1);
  static constexpr subrange box3column2 = intersection(box3, column2);
  //
  static constexpr subrange box4row3 = intersection(box4, row3);
  static constexpr subrange box4row4 = intersection(box4, row4);
  static constexpr subrange box4row5 = intersection(box4, row5);
  static constexpr subrange box4column3 = intersection(box4, column3);
  static constexpr subrange box4column4 = intersection(box4, column4);
  static constexpr subrange box4column5 = intersection(box4, column5);
  //
  static constexpr subrange box5row3 = intersection(box5, row3);
  static constexpr subrange box5row4 = intersection(box5, row4);
  static constexpr subrange box5row5 = intersection(box5, row5);
  static constexpr subrange box5column6 = intersection(box5, column6);
  static constexpr subrange box5column7 = intersection(box5, column7);
  static constexpr subrange box5column8 = intersection(box5, column8);
  //
  static constexpr subrange box6row6 = intersection(box6, row6);
  static constexpr subrange box6row7 = intersection(box6, row7);
  static constexpr subrange box6row8 = intersection(box6, row8);
  static constexpr subrange box6column0 = intersection(box6, column0);
  static constexpr subrange box6column1 = intersection(box6, column1);
  static constexpr subrange box6column2 = intersection(box6, column2);
  //
  static constexpr subrange box7row6 = intersection(box7, row6);
  static constexpr subrange box7row7 = intersection(box7, row7);
  static constexpr subrange box7row8 = intersection(box7, row8);
  static constexpr subrange box7column3 = intersection(box7, column3);
  static constexpr subrange box7column4 = intersection(box7, column4);
  static constexpr subrange box7column5 = intersection(box7, column5);
  //
  static constexpr subrange box8row6 = intersection(box8, row6);
  static constexpr subrange box8row7 = intersection(box8, row7);
  static constexpr subrange box8row8 = intersection(box8, row8);
  static constexpr subrange box8column6 = intersection(box8, column6);
  static constexpr subrange box8column7 = intersection(box8, column7);
  static constexpr subrange box8column8 = intersection(box8, column8);

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

    // print candidates
    // std::cout << "\nCandidates:\n";
    // for (int i = 0; i < candidates.size(); ++i) {
    //   if (!candidates[i].empty()) {
    //     std::cout << "Cell " << i << " : ";
    //     for (const int j : candidates[i]) {
    //       std::cout << ' ' << j << ' ';
    //     }
    //     std::cout << '\n';
    //   }
    // }
    // std::cout << '\n';

    // prune list of candidates

    // https://hodoku.sourceforge.net/en/tech_intersections.php
    // If in a block all candidates of a certain digit are confined to a row or
    // column, that digit cannot appear outside of that block in that row or
    // column.

    // identify if a candidate only appears in one row of a box
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
          std::unordered_set<int> seen;
          for (auto val : set[k]) {
            if (frequency[val] ==
                std::count(set[k].begin(), set[k].end(), val)) {
              seen.insert(val);
            }
          }
          unique_candidates[k].assign(seen.begin(), seen.end());
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
                for (const auto index : *indices::rows[choose_row()]) {
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
                for (const auto index : *indices::columns[choose_column()]) {
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

    // identify if a candidate only appears in one row of a box

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

    // check boxes
    for (int i = 0; i < indices::boxes.size(); ++i) {
      std::vector<int> candidate_occurrences(10);
      for (const auto index : *indices::boxes[i]) {
        for (const auto candidate : candidates[index]) {
          candidate_occurrences[candidate]++;
        }
      }
      for (int j = 1; j < 10; ++j) {
        if (candidate_occurrences[j] == 1) {
          exists_single_candidates = true;
          for (const auto index : *indices::boxes[i]) {
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
