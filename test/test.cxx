#include "Sudoku.hpp"

#include <catch2/catch_all.hpp>
#include <fstream>

TEST_CASE("master") {
  int solved{};
  int count{};
  std::ifstream iFile("../../test/data/master.txt");
  std::string puzzle;
  std::getline(iFile, puzzle);
  if (Sudoku s(puzzle); s.solve()) {
    ++solved;
  }
  ++count;
  std::cout << "Solved: " << solved << '/' << count << '\t'
            << 100.0 * double(solved) / double(count) << "%" << std::endl;
}

TEST_CASE("X-Wing") {
  int solved{};
  int count{};
  std::ifstream iFile("../../test/data/x-wing.txt");
  std::string puzzle;
  while (std::getline(iFile, puzzle)) {
    if (Sudoku s(puzzle); s.solve()) {
      ++solved;
    } else {
      s.print_puzzle();
    }
    ++count;
  }
  std::cout << "Solved: " << solved << '/' << count << '\t'
            << 100.0 * double(solved) / double(count) << "%" << std::endl;
}

TEST_CASE("50k") {
  int solved{};
  int count{};
  std::ifstream iFile("../../test/data/top50000.txt");
  std::string puzzle;
  while (std::getline(iFile, puzzle)) {
    if (Sudoku s(puzzle); s.solve()) {
      ++solved;
    }
    ++count;
  }
  std::cout << "Solved: " << solved << '/' << count << '\t'
            << 100.0 * double(solved) / double(count) << "%" << std::endl;
}
