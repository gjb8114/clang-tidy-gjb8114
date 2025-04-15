add_executable(matcher_test
        MutableDeclRefTest.cpp)

target_link_libraries(matcher_test PRIVATE utest-helper clang-tidy-gjb8114-matchers)