#ifndef SUDOKU_SOLVER_TEST_INCLUDE_TEST_HELPERS_CXX_
#define SUDOKU_SOLVER_TEST_INCLUDE_TEST_HELPERS_CXX_

#include <filesystem>
#include <fstream>
#include <iostream>

inline void attempt_solve(std::string_view filename) {
  if (!std::filesystem::exists(filename)) {
    std::cerr << "Cannot find file '" << filename << "' in search path:\n"
              << std::filesystem::current_path();
    std::exit(EXIT_FAILURE);
  }
  if (std::ifstream in_file(filename.data()); in_file.is_open()) {
    double count{};
    double solved{};
    std::string puzzle;
    while (std::getline(in_file, puzzle)) {
      if (Sudoku sudoku(puzzle); sudoku.solve()) {
        ++solved;
      }
      ++count;
    }
    std::cout << "Solved: " << solved << '/' << count << '\t'
              << 100.0 * solved / count << "%" << '\n';
  } else {
    std::cerr << "Failed to open file " << filename << '\n';
    std::exit(EXIT_FAILURE);
  }
}

#endif // SUDOKU_SOLVER_TEST_INCLUDE_TEST_HELPERS_CXX_
