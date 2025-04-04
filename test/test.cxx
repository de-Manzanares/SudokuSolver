#include "Sudoku.hpp"

#include <catch2/catch_all.hpp>
#include <fstream>

TEST_CASE("simple") {
  int solved{};
  int count{};
  std::ifstream iFile("../../test/data/simple.txt");
  std::string puzzle;
  while (std::getline(iFile, puzzle)) {
    if (Sudoku s(puzzle); s.solve()) {
      ++solved;
    }
    ++count;
  }
  std::cout << "Solved: " << solved << '/' << count << '\t'
            << 100.0 * double(solved) / double(count) << "%\n";
}

TEST_CASE("50k") {
  int solved{};
  int count{};
  std::ifstream iFile("../../test/data/top50000.txt");
  std::string puzzle;
  for (int i = 0; i < 3000; ++i) {
    (std::getline(iFile, puzzle));
    if (Sudoku s(puzzle); s.solve()) {
      ++solved;
    }
    ++count;
  }
  std::cout << "Solved: " << solved << '/' << count << '\t'
            << 100.0 * double(solved) / double(count) << "%" << std::endl;
}
