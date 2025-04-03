#include "Sudoku.hpp"

void Sudoku::solve() {
  initialize_candidates();
  print_candidates("Candidates:\n", _candidates);

  bool progress = true;
  while (progress) {
    progress = false;

    if (solve_naked_singles()) {
      std::cout << '\n';
      print_puzzle();
      progress = true;
      continue;
    }

    if (solve_hidden_singles()) {
      std::cout << '\n';
      print_puzzle();
      progress = true;
      continue;
    }

    if (prune_locked_claiming_candidates()) {
      progress = true;
    }

    if (prune_locked_pointing_candidates()) {
      progress = true;
      continue;
    }

    // if (prune_hidden_subsets()) {
    //   progress = true;
    //   continue;
    // }
  }
}
