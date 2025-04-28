#include "Indices.hpp"
#include "Sudoku.hpp"
#include <iostream>

void Sudoku::check_puzzle() const {
  if (_puzzle.size() != PUZZLE_SIZE || !puzzle_is_valid()) {
    std::cerr << "puzzle is malformed\n";
    std::exit(EXIT_FAILURE);
  }
}

bool Sudoku::puzzle_is_valid() const {
  return check_houses(House::row) && check_houses(House::column) &&
         check_houses(House::box);
}

bool Sudoku::check_houses(const House tag) const {
  auto &houses = tag == House::row      ? Indices::rows
                 : tag == House::column ? Indices::columns
                                        : Indices::boxes;
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
