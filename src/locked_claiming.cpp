#include "Indices.hpp"
#include "Sudoku.hpp"
#include <functional>

bool Sudoku::prune_locked_claiming_candidates() {
  const bool row = prune_locked_claiming_candidates(House::row);
  const bool column = prune_locked_claiming_candidates(House::column);
  return row || column;
}

// claiming locked candidates
// https://hodoku.sourceforge.net/en/tech_intersections.php
// If in a row (or column) all candidates of a certain digit are confined to
// one block, that candidate that be eliminated from all other cells in that
// block.
bool Sudoku::prune_locked_claiming_candidates(const House tag) {
  bool got_one = false;
  const auto &houses = tag == House::row ? Indices::rows : Indices::columns;

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
          if (tag == House::row) {
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
              return 0;
            };
          }
          if (tag == House::column) {
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
              return 0;
            };
          }
          // for each cell in the box
          for (const auto cell : Indices::boxes[choose_box()]) {
            // if the cell is outside the intersection
            if (std::find(houses[i].begin(), houses[i].end(), cell) ==
                houses[i].end()) {
              // and contains a claimed candidate
              // remove the claimed candidate from that cell
              if (std::find(_candidates[cell].begin(), _candidates[cell].end(),
                            claimed) != _candidates[cell].end()) {
                got_one = true;
                // std::cout << "eliminate candidate by locked claiming : "
                //           << std::setw(2) << cell << " " << claimed << '\n';
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
