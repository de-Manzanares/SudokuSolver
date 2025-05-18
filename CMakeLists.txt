cmake_minimum_required(VERSION 3.10)

project(SudokuSolver VERSION 0.1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 99)

set(CompilerOptions -Wall -Wextra -Wpedantic
        # -Werror
)
set(DEBUGCompilerOptions -g ${CompilerOptions})
set(COVCompilerOptions --coverage ${CompilerOptions})
set(COVLinkerOptions --coverage)
set(SANCompilerOptions -fsanitize=address,undefined ${DEBUGCompilerOptions})
set(SANLinkerOptions -fsanitize=address,undefined)

add_library(
        common-options
        INTERFACE
)
target_compile_options(
        common-options
        INTERFACE
        $<$<CONFIG:Debug>:$<$<C_COMPILER_ID:GNU,Clang>:${DEBUGCompilerOptions}>>
        $<$<CONFIG:Coverage>:$<$<C_COMPILER_ID:GNU,Clang>:${COVCompilerOptions}>>
        $<$<CONFIG:Sanitizer>:$<$<C_COMPILER_ID:GNU,Clang>:${SANCompilerOptions}>>
        $<$<CONFIG:Release>:$<$<C_COMPILER_ID:GNU,Clang>:-Ofast>>
)
target_link_options(
        common-options
        INTERFACE
        $<$<CONFIG:Coverage>:$<$<C_COMPILER_ID:GNU,Clang>:${COVLinkerOptions}>>
        $<$<CONFIG:Sanitizer>:$<$<C_COMPILER_ID:GNU,Clang>:${SANLinkerOptions}>>
)

add_subdirectory(libsudoku)
add_subdirectory(sudoku)
