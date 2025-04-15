cmake_minimum_required(VERSION 3.23)
project(clang-tidy-gjb8114 C CXX)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(lib)

enable_testing()
add_subdirectory(utest)
