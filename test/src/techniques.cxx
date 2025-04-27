#include "Sudoku.hpp"
#include "test_helpers.hxx"
#include <iostream>

#include <catch2/catch_all.hpp>
#include <fstream>

TEST_CASE("naked subsets") { attempt_solve("subsets-naked.txt"); }

TEST_CASE("hidden subsets") { attempt_solve("subsets-hidden.txt"); }

TEST_CASE("X-Wing") { attempt_solve("x-wing.txt"); }

TEST_CASE("Swordfish") { attempt_solve("swordfish.txt"); }
