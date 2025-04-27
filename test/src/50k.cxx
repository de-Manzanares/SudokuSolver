#include "Sudoku.hpp"
#include "test_helpers.hxx"
#include <catch2/catch_all.hpp>

TEST_CASE("50k") { attempt_solve("top50000.txt"); }
