add_library(sudoku_core)
set_target_properties(sudoku_core PROPERTIES OUTPUT_NAME libsudoku)
add_library(sudoku::core ALIAS sudoku_core)

target_sources(
        sudoku_core
        PRIVATE
        sudoku.c
)
target_link_libraries(
        sudoku_core
        PRIVATE
        common-options
)
target_include_directories(
        sudoku_core
        PUBLIC
        ../libsudoku
)
