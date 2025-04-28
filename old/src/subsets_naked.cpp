#include "Indices.hpp"
#include "Sudoku.hpp"

#include <algorithm>
#include <functional>
#include <optional>
#include <unordered_set>

bool Sudoku::prune_naked_subsets(const SetSize set_size) {
  bool got_one = false;

  auto is_good_cell = [this, set_size](const auto x) {
    return 2 <= _candidates[x].size() &&
           _candidates[x].size() <= static_cast<std::size_t>(set_size);
  };

  const auto &house_type =
      std::vector{&Indices::rows, &Indices::columns, &Indices::boxes};

  for (const auto &houses : house_type) {
    for (const auto &house : *houses) {

      std::vector<int> good_cells;
      good_cells.reserve(9);
      std::copy_if(house.begin(), house.end(), std::back_inserter(good_cells),
                   is_good_cell);

      if (const auto naked_subset = find_naked_subset(good_cells, set_size)) {
        const auto &set_cells = naked_subset->set_cells;
        const auto &set_candidates = naked_subset->set_candidates;

        for (const auto cell : house) {
          if (set_cells.find(cell) == set_cells.end()) {
            auto it = std::remove_if(
                _candidates[cell].begin(), _candidates[cell].end(),
                [&set_candidates](auto x) {
                  return set_candidates.find(x) != set_candidates.end();
                });
            if (it != _candidates[cell].end()) {
              for (auto itt = it; itt != _candidates[cell].end(); ++itt) {
                ++_candidates_pruned_by._subset_naked;
              }
              got_one = true;
              _candidates[cell].erase(it, _candidates[cell].end());
            }
          }
        }
      }
    }
  }
  return got_one;
}

std::optional<Subset>
Sudoku::find_naked_subset(const std::vector<int> &good_cells,
                          const SetSize set_size) {
  if (good_cells.size() >= static_cast<std::size_t>(set_size)) {
    std::unordered_set<int> set_cells;
    std::unordered_set<int> set_candidates;

    std::vector cell_mask(good_cells.size(), false);
    std::fill(std::prev(cell_mask.end(), static_cast<std::ptrdiff_t>(set_size)),
              cell_mask.end(), true);

    do {
      set_cells.clear();
      set_candidates.clear();

      for (std::size_t i = 0; i < cell_mask.size(); ++i) {
        if (cell_mask[i]) {
          set_cells.insert(good_cells[i]);
        }
      }
      for (const auto cell : set_cells) {
        for (const auto candidate : _candidates[cell]) {
          set_candidates.insert(candidate);
        }
      }
      if (set_candidates.size() == static_cast<std::size_t>(set_size)) {
        return Subset{std::move(set_cells), std::move(set_candidates)};
      }
    } while (std::next_permutation(cell_mask.begin(), cell_mask.end()));
  }
  return std::nullopt;
}
