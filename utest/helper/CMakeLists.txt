find_package(Clang REQUIRED CONFIG)

add_library(utest-helper INTERFACE)
target_include_directories(utest-helper INTERFACE ..)
target_link_libraries(utest-helper INTERFACE GTest::gmock_main clangFrontendTool clangTooling)