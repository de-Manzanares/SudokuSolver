#include "Sudoku.hpp"

bool Sudoku::solve() {
  // print_candidates("Candidates:\n", _candidates);

  // const auto start = std::chrono::high_resolution_clock::now();

  bool progress = true;
  while (progress) {
    progress = false;

    if (solve_naked_singles()) {
      progress = true;
      continue;
    }

    if (solve_hidden_singles()) {
      progress = true;
      continue;
    }

    if (prune_naked_subset(SetSize::pair)) {
      progress = true;
    }

    if (prune_naked_subset(SetSize::triple)) {
      progress = true;
    }

    if (prune_hidden_subsets(2)) {
      progress = true;
    }

    if (prune_hidden_subsets(3)) {
      progress = true;
      continue;
    }

    if (prune_naked_subset(SetSize::quad)) {
      progress = true;
    }

    if (prune_hidden_subsets(4)) {
      progress = true;
      continue;
    }

    if (prune_locked_pointing_candidates()) {
      progress = true;
      continue;
    }

    if (prune_locked_claiming_candidates()) {
      progress = true;
      continue;
    }

    if (prune_x_wings(2)) {
      progress = true;
      continue;
    }

    if (prune_x_wings(3)) {
      progress = true;
      continue;
    }

    if (prune_x_wings(4)) {
      progress = true;
      continue;
    }
  }
  // const auto end = std::chrono::high_resolution_clock::now();

  // const auto elapsed =
  //     std::chrono::duration_cast<std::chrono::microseconds>(end - start);

  // std::cout << "\nElapsed time: " << elapsed.count() << " microseconds";
  // print_readout();

  return _unknown.none();
}
