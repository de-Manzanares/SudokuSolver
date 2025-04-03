#include "Sudoku.hpp"

bool Sudoku::prune_locked_pointing_candidates() {
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
                return 0;
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
                return 0;
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