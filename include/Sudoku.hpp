#ifndef SUDOKU_SOLVER_INCLUDE_SUDOKU_HPP_
#define SUDOKU_SOLVER_INCLUDE_SUDOKU_HPP_

#include "indices.hpp"

#include <algorithm>
#include <array>
#include <bitset>
#include <iomanip>
#include <iostream>
#include <vector>

class Sudoku {
 public:
  enum class house { row, column, box };

  explicit Sudoku(const std::vector<int> &puzzle) : _puzzle{puzzle} {
    check_puzzle();
  };
  explicit Sudoku(const std::string &str) {
    for (const auto ch : str) {
      _puzzle.push_back(ch - '0');
    }
    while (_puzzle.size() > 81) {
      _puzzle.pop_back();
    }
    check_puzzle();
  }

  // check
  void check_puzzle() const;

  // initialize candidates
  void initialize_candidates();

  // singles
  bool solve_naked_singles();
  bool solve_hidden_singles();

  bool solve();

  bool prune_naked_subsets(std::size_t n);
  bool prune_hidden_subsets(std::size_t n);
  bool prune_locked_claiming_candidates();
  bool prune_locked_pointing_candidates();

  void print_puzzle() const;
  bool prune_x_wings(std::size_t n);
  static void print_puzzle(const std::vector<int> &vec);
  void print_unknown() const;
  static void print_candidates(const std::string &str,
                               const std::vector<std::vector<int>> &vec);
  void print_singles() const;

  void print_readout() const;

 private:
  static constexpr int UNKNOWN = 0;      ///< code for unknown cell value
  static constexpr int PUZZLE_SIZE = 81; ///< 81 cells in the puzzle

  std::vector<int> _puzzle; ///< the puzzle
  std::bitset<PUZZLE_SIZE> _unknown{};
  std::vector<std::vector<int>> _candidates{PUZZLE_SIZE};
  std::array<int, 81> _singles{};

  int _iterations{};
  std::vector<std::vector<int>> _puzzles;
  int _naked_singles{};
  int _hidden_singles{};

  struct _candidates_pruned_by {
    int _claiming_locked{};
    int _pointing_locked{};
    int _subset_hidden{};
    int _subset_naked{};
    int _xwing{};
  };

  _candidates_pruned_by _candidates_pruned_by{};

  // check
  [[nodiscard]] bool puzzle_is_valid() const;
  [[nodiscard]] bool check_houses(house tag) const;

  // initialize candidates
  void find_unknown_indices();
  void initialize_candidates(int cell);

  // singles
  bool find_hidden_singles();
  bool find_hidden_singles(house tag);
  void update_candidates(int cell);

  // locked claiming
  bool prune_locked_claiming_candidates(house tag);
};

#endif // SUDOKU_SOLVER_INCLUDE_SUDOKU_HPP_
