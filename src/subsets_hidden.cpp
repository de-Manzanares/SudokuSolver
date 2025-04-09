#include "Indices.hpp"
#include "Sudoku.hpp"

#include <algorithm>
#include <functional>
#include <map>
#include <unordered_map>

bool Sudoku::prune_hidden_subsets(const SetSize set_size) {
  bool got_one = false;

  const auto house_type =
      std::vector{&Indices::rows, &Indices::columns, &Indices::boxes};

  for (const auto &houses : house_type) {
    for (const auto &house : *houses) {

      std::vector<std::size_t> candidate_frequency(10);
      for (const auto cell : house) {
        for (const auto candidate : _candidates[cell]) {
          ++candidate_frequency[candidate];
        }
      }

      auto is_good_candidate = [set_size, &candidate_frequency](const auto x) {
        return 2 <= candidate_frequency[x] &&
               candidate_frequency[x] <= static_cast<std::size_t>(set_size);
      };

      std::vector<int> good_candidates;
      good_candidates.reserve(9);
      for (int i = 1; i < 10; ++i) {
        if (is_good_candidate(i)) {
          good_candidates.emplace_back(i);
        }
      }

      std::unordered_map<int, std::vector<int>> candidate_to_cells;
      for (const auto val : good_candidates) {
        for (const auto cell : house) {
          for (const auto candidate : _candidates[cell]) {
            if (val == candidate) {
              candidate_to_cells[val].push_back(cell);
            }
          }
        }
      }

      if (const auto hidden_subset = find_hidden_subsets(
              good_candidates, candidate_to_cells, set_size)) {
        const auto &set_cells = hidden_subset->set_cells;
        const auto &set_candidates = hidden_subset->set_candidates;

        for (const auto cell : set_cells) {
          auto it = std::remove_if(
              _candidates[cell].begin(), _candidates[cell].end(),
              [&set_candidates](auto x) {
                return set_candidates.find(x) == set_candidates.end();
              });
          if (it != _candidates[cell].end()) {
            got_one = true;
            for (auto itt = it; itt != _candidates[cell].end(); ++itt) {
              ++_candidates_pruned_by._subset_hidden;
            }
            _candidates[cell].erase(it, _candidates[cell].end());
          }
        }
      }
    }
  }
  return got_one;
}

std::optional<Subset> Sudoku::find_hidden_subsets(
    const std::vector<int> &good_candidates,
    const std::unordered_map<int, std::vector<int>> &candidates_to_cells,
    const SetSize set_size) {
  if (good_candidates.size() >= static_cast<std::size_t>(set_size)) {
    std::unordered_set<int> set_candidates;
    std::unordered_set<int> set_cells;

    std::vector candidate_mask(good_candidates.size(), false);
    std::fill(
        std::prev(candidate_mask.end(), static_cast<std::ptrdiff_t>(set_size)),
        candidate_mask.end(), true);

    do {
      set_candidates.clear();
      set_cells.clear();

      for (std::size_t i = 0; i < candidate_mask.size(); ++i) {
        if (candidate_mask[i]) {
          set_candidates.insert(good_candidates[i]);
        }
      }
      for (const auto candidate : set_candidates) {
        for (const auto cell : candidates_to_cells.at(candidate)) {
          set_cells.insert(cell);
        }
      }
      if (set_cells.size() == static_cast<std::size_t>(set_size)) {
        return Subset{std::move(set_cells), std::move(set_candidates)};
      }
    } while (
        std::next_permutation(candidate_mask.begin(), candidate_mask.end()));
  }
  return std::nullopt;
}
