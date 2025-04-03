#include "Sudoku.hpp"

void Sudoku::print_puzzle() const {
  for (int i = 0; i < _puzzle.size(); ++i) {
    if (_puzzle[i] != 0) {
      std::cout << ' ' << _puzzle[i] << ' ';
    } else {
      std::cout << " . ";
    }
    if ((i + 1) % 9 == 0) {
      std::cout << '\n';
    }
  }
  std::cout << '\n';
}

void Sudoku::print_puzzle(const std::vector<int> &vec) {
  for (int i = 0; i < vec.size(); ++i) {
    if (vec[i] != 0) {
      std::cout << ' ' << vec[i] << ' ';
    } else {
      std::cout << " . ";
    }
    if ((i + 1) % 9 == 0) {
      std::cout << '\n';
    }
  }
  std::cout << '\n';
}

void Sudoku::print_unknown() const {
  std::cout << "Unknown Cells:\n";
  for (int i = 0; i < PUZZLE_SIZE; ++i) {
    if (_unknown[i]) {
      std::cout << i << " ";
    }
  }
  std::cout << "\n\n";
}

void Sudoku::print_candidates(const std::string &str,
                              const std::vector<std::vector<int>> &vec) {
  std::cout << str;
  for (int i = 0; i < vec.size(); ++i) {
    if (!vec[i].empty()) {
      std::cout << "Cell " << i << " : ";
      for (const int j : vec[i]) {
        std::cout << ' ' << j << ' ';
      }
      std::cout << '\n';
    }
  }
  std::cout << '\n';
}

void Sudoku::print_singles() const {
  bool printed_message = false;
  for (int i = 0; i < PUZZLE_SIZE; ++i) {
    if (_singles[i] != 0) {
      if (!printed_message) {
        std::cout << "Hidden singles:\n";
        printed_message = true;
      }
      std::cout << "Cell " << i << " : " << _singles[i] << '\n';
    }
  }
  std::cout << '\n';
}

void Sudoku::print_readout() const {
  std::cout << "\n\nApplied\n\t";
  std::cout << "Naked Singles   : " << std::setw(5) << _naked_singles << "\n\t";
  std::cout << "Hidden Singles  : " << std::setw(5) << _hidden_singles
            << "\n\t";
  std::cout << "\nCandidates eliminated by:\n\t";
  std::cout << "Locked claiming : " << std::setw(5)
            << _candidates_pruned_by._claiming_locked << "\n\t";
  std::cout << "Locked pointing : " << std::setw(5)
            << _candidates_pruned_by._pointing_locked << "\n\t";
}
