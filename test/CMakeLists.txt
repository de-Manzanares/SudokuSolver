if (ENABLE_TESTS)

    add_executable(50k)
    add_executable(techniques)

    include(FetchContent)
    FetchContent_Declare(
            Catch2
            GIT_REPOSITORY https://github.com/catchorg/Catch2.git
            GIT_TAG v3.8.0
            GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(Catch2)

    add_library(TestHelpers INTERFACE)
    target_include_directories(
            TestHelpers
            INTERFACE
            include
    )

    add_library(common-test-options INTERFACE)
    target_link_libraries(
            common-test-options
            INTERFACE
            common-options
            TestHelpers
            SudokuSolver::SudokuSolver
            Catch2::Catch2WithMain
    )

    target_sources(
            techniques
            PRIVATE
            src/techniques.cxx
    )
    target_link_libraries(
            techniques
            PRIVATE
            common-test-options
    )

    target_sources(
            50k
            PRIVATE
            src/50k.cxx
    )
    target_link_libraries(
            50k
            PRIVATE
            common-test-options
    )
endif ()
