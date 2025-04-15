find_package(Clang REQUIRED CONFIG)

add_subdirectory(matchers)

add_library(clang-tidy-gjb8114 SHARED
        Registry.cpp
)

target_include_directories(clang-tidy-gjb8114 PRIVATE ${CLANG_INCLUDE_DIRS} .)

target_link_libraries(clang-tidy-gjb8114 PRIVATE
        clang-tidy-gjb8114-matchers
        clangASTMatchers
        clangAST
        clangTidyUtils
)

add_subdirectory(utils)
add_subdirectory(common)
add_subdirectory(cpp)