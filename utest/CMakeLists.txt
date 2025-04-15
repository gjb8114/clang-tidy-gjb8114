include(FetchContent)

find_package(GTest QUIET)
if (NOT GTest_FOUND)
    message(STATUS "GTest not found, fetching from GitHub (HEAD version)...")
    FetchContent_Declare(
            googletest
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG main
    )
    FetchContent_MakeAvailable(googletest)
endif ()

add_subdirectory(helper)
add_subdirectory(matcher)