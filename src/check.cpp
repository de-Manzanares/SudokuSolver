#include "Sudoku.hpp"

void Sudoku::check_puzzle() const {
  if (_puzzle.size() != PUZZLE_SIZE || !puzzle_is_valid()) {
    std::cerr << "puzzle is malformed\n";
    std::exit(EXIT_FAILURE);
  }
}

bool Sudoku::puzzle_is_valid() const {
  return check_houses(house::row) && check_houses(house::column) &&
         check_houses(house::box);
}

bool Sudoku::check_houses(const house tag) const {
  auto &houses = tag == house::row      ? indices::rows
                 : tag == house::column ? indices::columns
                                        : indices::boxes;
  for (const auto &house : houses) {
    std::bitset<10> seen;
    for (const auto index : house) {
      if (_puzzle[index] != 0) {
        if (seen[_puzzle[index]]) {
          return false;
        }
        seen.set(_puzzle[index]);
      }
    }
  }
  return true;
}
