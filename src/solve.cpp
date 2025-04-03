#include "Sudoku.hpp"

void Sudoku::solve() {
  initialize_candidates();
  print_candidates("Candidates:\n", _candidates);

  bool progress = true;
  while (progress) {
    progress = false;

    if (solve_explicit_singles()) {
      std::cout << '\n';
      print_puzzle();
      progress = true;
      continue;
    }

    if (find_implicit_singles()) {
      solve_implicit_singles();
      std::cout << '\n';
      print_puzzle();
      progress = true;
      continue;
    }

    if (prune_pointing_locked_candidates()) {
      progress = true;
      continue;
    }

    if (prune_claiming_locked_candidates()) {
      progress = true;
      continue;
    }

    // if (prune_hidden_subsets()) {
    //   progress = true;
    //   continue;
    // }
  }
}
