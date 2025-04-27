if (ENABLE_TESTS)
    include(FetchContent)
    FetchContent_Declare(
            Catch2
            GIT_REPOSITORY https://github.com/catchorg/Catch2.git
            GIT_TAG v3.8.0
            GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(Catch2)

    add_executable(test)
    target_sources(
            test
            PRIVATE
            test.cxx
    )
    target_link_libraries(
            test
            PRIVATE
            common-options
            SudokuSolver::SudokuSolver
            Catch2::Catch2WithMain
    )
endif ()
