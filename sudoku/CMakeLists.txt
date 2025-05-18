add_executable(sudoku_cli)
set_target_properties(sudoku_cli PROPERTIES OUTPUT_NAME sudoku)
add_executable(sudoku::cli ALIAS sudoku_cli)

target_sources(
        sudoku_cli
        PRIVATE
        main.c
)
target_link_libraries(
        sudoku_cli
        PRIVATE
        common-options
        sudoku::core
)

set_source_files_properties(sudoku.h PROPERTIES LANGUAGE C)