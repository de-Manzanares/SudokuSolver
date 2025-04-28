#include "Indices.hpp"
#include "Sudoku.hpp"
#include <algorithm>

int choose_box(House tag, int house, int intersection);

bool Sudoku::prune_locked_claiming_candidates() {
  const bool row = prune_locked_claiming_candidates(House::row);
  const bool column = prune_locked_claiming_candidates(House::column);
  return row || column;
}

bool Sudoku::prune_locked_claiming_candidates(const House tag) {
  bool got_one = false;
  const auto &houses = tag == House::row ? Indices::rows : Indices::columns;

  // house
  for (int i = 0; i < 9; ++i) {
    if (const auto optional_claimed_candidates =
            find_claimed_candidates(houses, i)) {
      const auto &claimed_candidates = *optional_claimed_candidates;

      // intersection
      for (int k = 0; k < 3; ++k) {
        if (!claimed_candidates[k].empty()) {
          const auto &box = Indices::boxes[choose_box(tag, i, k)];
          std::vector<int> target_cells;
          target_cells.reserve(6);
          std::copy_if(box.cbegin(), box.cend(),
                       std::back_inserter(target_cells), [&](const auto x) {
                         return std::find(houses[i].cbegin(), houses[i].cend(),
                                          x) == houses[i].cend();
                       });
          for (const auto cell : target_cells) {
            auto it = std::remove_if(
                _candidates[cell].begin(), _candidates[cell].end(),
                [&](const auto x) {
                  return std::find(claimed_candidates[k].cbegin(),
                                   claimed_candidates[k].cend(),
                                   x) != claimed_candidates[k].cend();
                });
            if (it != _candidates[cell].end()) {
              got_one = true;
              for (auto itt = it; itt != _candidates[cell].end(); ++itt) {
                ++_candidates_pruned_by._claiming_locked;
              }
              _candidates[cell].erase(it, _candidates[cell].end());
            }
          }
        }
      }
    }
  }
  return got_one;
}

std::optional<std::vector<std::vector<int>>>
Sudoku::find_claimed_candidates(const std::array<std::array<int, 9>, 9> &houses,
                                const int house) {
  std::vector<std::vector<int>> intersections(3);
  for (int cell = 0; cell < 9; ++cell) {
    const int intersection = cell / 3;
    for (const auto candidate : _candidates[houses[house][cell]]) {
      intersections[intersection].push_back(candidate);
    }
  }
  std::vector<int> frequency(10);
  for (const auto &intersection : intersections) {
    for (const auto candidate : intersection) {
      ++frequency[candidate];
    }
  }

  auto is_claimed = [&](const auto x, const auto k) {
    return frequency[x] ==
           std::count(intersections[k].begin(), intersections[k].end(), x);
  };

  std::vector<std::vector<int>> claimed_candidates(3);
  for (int intersection = 0; intersection < 3; ++intersection) {
    for (const auto candidate : intersections[intersection]) {
      if (is_claimed(candidate, intersection)) {
        claimed_candidates[intersection].push_back(candidate);
      }
    }
  }
  const bool no_claimed_candidates =
      std::all_of(claimed_candidates.cbegin(), claimed_candidates.cend(),
                  [](const auto &v) { return v.empty(); });

  if (no_claimed_candidates) {
    return std::nullopt;
  }
  return claimed_candidates;
}

int choose_box(const House tag, const int house, const int intersection) {
  if (tag == House::row) {
    if (house < 3) {
      return intersection;
    }
    if (house < 6) {
      return 3 + intersection;
    }
    if (house < 9) {
      return 6 + intersection;
    }
  }
  if (tag == House::column) {
    if (house < 3) {
      return intersection * 3;
    }
    if (house < 6) {
      return intersection * 3 + 1;
    }
    if (house < 9) {
      return intersection * 3 + 2;
    }
  }
  return 0;
}
