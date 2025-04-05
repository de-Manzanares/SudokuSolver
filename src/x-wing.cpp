#include "Sudoku.hpp"

bool Sudoku::prune_x_wings(const std::size_t n) {
  bool got_one = false;

  const auto &house_type = {indices::rows, indices::columns};
  for (const auto &house : house_type) {
    const auto &base_sets = house;
    const auto &cover_sets =
        base_sets == indices::rows ? indices::columns : indices::rows;

    std::vector<std::vector<std::vector<int>>> candidate_locations(
        9, std::vector<std::vector<int>>(10));
    for (std::size_t i = 0; i < house.size(); ++i) {
      for (int j = 0; j < 9; ++j) {
        for (const auto candidate : _candidates[house[i][j]]) {
          candidate_locations[i][candidate].push_back(j);
        }
      }
    }

    for (std::size_t i = 0; i < 9; ++i) { // base set
      for (int j = 1; j < 10; ++j) {      // candidate value
        if (candidate_locations[i][j].size() == n) {
          for (std::size_t k = i + 1; k < 9; ++k) { // start at base set
            if (candidate_locations[i][j] == candidate_locations[k][j]) {
              for (const auto &cover_set : candidate_locations[i][j]) {
                for (std::size_t l = 0; l < 9; ++l) {
                  if (i != l && k != l) {
                    auto it = std::remove_if(
                        _candidates[cover_sets[cover_set][l]].begin(),
                        _candidates[cover_sets[cover_set][l]].end(),
                        [j](auto x) { return x == j; });
                    if (it != _candidates[cover_sets[cover_set][l]].end()) {
                      got_one = true;
                      for (auto itt = it;
                           itt != _candidates[cover_sets[cover_set][l]].end();
                           ++itt) {
                        ++_candidates_pruned_by._xwing;
                      }
                      _candidates[cover_sets[cover_set][l]].erase(
                          it, _candidates[cover_sets[cover_set][l]].end());
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