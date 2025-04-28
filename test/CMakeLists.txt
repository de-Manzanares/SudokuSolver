add_executable(test-sudoku-puzzle)

target_sources(
        test-sudoku-puzzle
        PRIVATE test_sudoku_puzzle.cxx
)
target_link_libraries(
        test-sudoku-puzzle
        PRIVATE
        common-options
        SudokuSolver::SudokuPuzzle
        Catch2::Catch2WithMain
)
