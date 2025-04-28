#include <SudokuPuzzle.hpp>
#include <catch2/catch_all.hpp>

TEST_CASE("constructor checking") {
  SECTION("valid") {
    // clang-format off
  SudokuPuzzle sp0("108000067000050000000000030006100040450000900000093000200040010003002700807001005");
  SudokuPuzzle sp1("1*8****67****5***********3***61***4*45****9******93***2***4**1***3**27**8*7**1**5");
  SudokuPuzzle sp2("1.8....67....5...........3...61...4.45....9......93...2...4..1...3..27..8.7..1..5");
  SudokuPuzzle sp3("1 8    67    5           3   61   4 45    9      93   2   4  1   3  27  8 7  1  5");
  SudokuPuzzle sp4("1\n8\n\n\n\n67\n\n\n\n5\n\n\n\n\n\n\n\n\n\n\n3\n\n\n61\n\n\n4\n45\n\n\n\n9\n\n\n\n\n\n93\n\n\n2\n\n\n4\n\n1\n\n\n3\n\n27\n\n8\n7\n\n1\n\n5");
  SudokuPuzzle sp5("1^8^^^^67^^^^5^^^^^^^^^^^3^^^61^^^4^45^^^^9^^^^^^93^^^2^^^4^^1^^^3^^27^^8^7^^1^^5");
    // clang-format on
  }
  SECTION("invalid") {
    try {
      // clang-format off
      SudokuPuzzle sp6("11101.67111151111111111131116111141451111911111193111211141111113112711817111115");
      // clang-format on
    } catch (const std::exception &e) {
      std::cerr << e.what() << '\n';
      std::cerr << "this error message is part of a test\n";
    }
    try {
      // clang-format off
      SudokuPuzzle sp6("118101.67111151111111111131116111141451111911111193111211141111113112711817111115");
      // clang-format on
    } catch (const std::exception &e) {
      std::cerr << e.what() << '\n';
      std::cerr << "this error message is part of a test\n";
    }
  }
}
