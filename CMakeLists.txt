cmake_minimum_required(VERSION 3.10)

project(SudokuSolver LANGUAGES CXX)

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

add_library(_SudokuSolver STATIC)
add_library(SudokuSolver::SudokuSolver ALIAS _SudokuSolver)
target_sources(
        _SudokuSolver
        PRIVATE
        src/check.cpp
        src/initialize_candidates.cpp
        src/print.cpp
        src/solve.cpp
        src/singles.cpp
        src/locked_claiming.cpp
        src/locked_pointing.cpp
        src/subsets_hidden.cpp
        src/subsets_naked.cpp
        src/x-wing.cpp
)
target_include_directories(
        _SudokuSolver
        PRIVATE
        include/private
        PUBLIC
        include/public
)
target_link_libraries(
        _SudokuSolver
        PRIVATE
        common-options
)

add_subdirectory(test)
