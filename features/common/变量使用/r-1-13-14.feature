# language: zh-CN
功能: R-1-13-14 禁止将 NULL 用作整型数

  @acceptance
  场景: 报告将 NULL 作为整型数使用
    假如有一个源码文件 "test.c"，内容如下:
            """
            #include <stdlib.h>

            void bar(int x);

            void foo(void)
            {
                bar(NULL);
            }
            """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-13-14" 时
    那么应该报告 "test.c:7:9: warning: 禁止将 NULL 用作整型数 [gjb8114-r-1-13-14]"
    并且总共报告 1 个 warning

  场景: 不报告将 0 作为整型数使用
    假如有一个源码文件 "test.c"，内容如下:
            """
            int bar(int x);

            void foo(void)
            {
                bar(0);
            }
            """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-13-14" 时
    那么总共报告 0 个 warning

  场景: 报告 C++ 中将 NULL 作为整型数使用
    假如有一个源码文件 "test.cpp"，内容如下:
            """
            #include <stdlib.h>

            int bar(int x);

            void foo()
            {
                bar(NULL);
            }
            """
    当对 "test.cpp" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-13-14" 时
    那么应该报告 "test.cpp:7:9: warning: 禁止将 NULL 用作整型数 [gjb8114-r-1-13-14]"
    并且总共报告 1 个 warning