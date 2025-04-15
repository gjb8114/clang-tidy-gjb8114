add_library(clang-tidy-gjb8114-matchers INTERFACE)

target_include_directories(clang-tidy-gjb8114-matchers INTERFACE .. ${CLANG_INCLUDE_DIRS})

target_link_libraries(clang-tidy-gjb8114-matchers INTERFACE clangASTMatchers)
