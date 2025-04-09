if (ENABLE_TESTS)
    include(FetchContent)
    FetchContent_Declare(
            Catch2
            GIT_REPOSITORY https://github.com/catchorg/Catch2.git
            GIT_TAG v3.8.0
    )
    FetchContent_MakeAvailable(Catch2)

    add_executable(test)
    target_sources(
            test
            PRIVATE
            test.cxx
    )
    target_include_directories(
            test
            PRIVATE
            ../include
    )
    target_link_libraries(
            test
            PRIVATE
            Catch2::Catch2WithMain
            sudoku-solver
            common-options
    )
endif ()