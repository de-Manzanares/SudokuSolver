#include "Sudoku.hpp"
#include "test_helpers.hxx"
#include <iostream>

#include <catch2/catch_all.hpp>
#include <fstream>

TEST_CASE("naked subsets") { attempt_solve("data/subsets-naked.txt"); }

TEST_CASE("hidden subsets") { attempt_solve("data/subsets-hidden.txt"); }

TEST_CASE("X-Wing") { attempt_solve("data/x-wing.txt"); }

TEST_CASE("Swordfish") { attempt_solve("data/swordfish.txt"); }
