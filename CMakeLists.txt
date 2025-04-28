cmake_minimum_required(VERSION 3.10)

project(SudokuSolver VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

set(CompilerOptions -Wall -Wextra -Wpedantic -Werror)
set(DEBUGCompilerOptions -g ${CompilerOptions})
set(COVCompilerOptions --coverage ${CompilerOptions})
set(COVLinkerOptions --coverage)
set(SANCompilerOptions -fsanitize=address,undefined ${CompilerOptions})
set(SANLinkerOptions -fsanitize=address,undefined)

add_library(
        common-options
        INTERFACE
)
target_compile_options(
        common-options
        INTERFACE
        $<$<CONFIG:Debug>:$<$<CXX_COMPILER_ID:GNU,Clang>:${DEBUGCompilerOptions}>>
        $<$<CONFIG:Coverage>:$<$<CXX_COMPILER_ID:GNU,Clang>:${COVCompilerOptions}>>
        $<$<CONFIG:Sanitizer>:$<$<CXX_COMPILER_ID:GNU,Clang>:${SANCompilerOptions}>>
        $<$<CXX_COMPILER_ID:MSVC>:/W4>
)
target_link_options(
        common-options
        INTERFACE
        $<$<CONFIG:Coverage>:$<$<CXX_COMPILER_ID:GNU,Clang>:${COVLinkerOptions}>>
        $<$<CONFIG:Sanitizer>:$<$<CXX_COMPILER_ID:GNU,Clang>:${SANLinkerOptions}>>
)

add_library(_SudokuSolver_old)

add_library(SudokuSolver::SudokuSolver_old ALIAS _SudokuSolver_old)

target_sources(
        _SudokuSolver_old
        PRIVATE
        old/src/check.cpp
        old/src/initialize_candidates.cpp
        old/src/print.cpp
        old/src/solve.cpp
        old/src/singles.cpp
        old/src/locked_claiming.cpp
        old/src/locked_pointing.cpp
        old/src/subsets_hidden.cpp
        old/src/subsets_naked.cpp
        old/src/x-wing.cpp
)
target_include_directories(
        _SudokuSolver_old
        PUBLIC
        old/include/public
        PRIVATE
        old/include/private
)
target_link_libraries(
        _SudokuSolver_old
        PRIVATE
        common-options
)

add_library(_SudokuPuzzle INTERFACE)
add_library(SudokuSolver::SudokuPuzzle ALIAS _SudokuPuzzle)
target_include_directories(_SudokuPuzzle INTERFACE include)

add_subdirectory(test)
add_subdirectory(old/test)
