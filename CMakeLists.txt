cmake_minimum_required(VERSION 3.30)

project(sudoku_solver)

set(CMAKE_CXX_STANDARD 17)

set(CompilerOptions -Wall -Wextra -Wpedantic)
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

add_executable(sudoku_solver)
target_sources(
        sudoku_solver
        PRIVATE
        src/main.cpp
        src/check.cpp
        src/initialize_candidates.cpp
        src/print.cpp
        src/solve.cpp
        src/singles.cpp
)
target_include_directories(
        sudoku_solver
        PRIVATE
        include
)
target_link_libraries(
        sudoku_solver
        PRIVATE
        common-options
)
