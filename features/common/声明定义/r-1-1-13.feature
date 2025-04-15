# language: zh-CN
功能: R-1-1-13 禁止函数声明省略参数类型或名称

  例外: C++ 允许重载，空参数列表的函数声明是可能指向另一个函数

  @acceptance
  场景: 报告函数声明省略参数类型或名称
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo();

        void foo(int x, int y)
        {
        }

        void bar(int, int);

        void bar(int x, int y)
        {
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-1-13" 时
    那么应该报告 "test.c:1:6: warning: 禁止函数声明省略参数类型或名称 [gjb8114-r-1-1-13]"
    并且应该报告 "test.c:7:6: warning: 禁止函数声明省略参数类型或名称 [gjb8114-r-1-1-13]"

  @acceptance
  场景: 函数声明中参数变量名完整
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(int x, int y);

        void foo(int x, int y)
        {
            return x + y;
        }

        void bar(int x, int y);

        void bar(int x, int y)
        {
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-1-13" 时
    那么不应该报告任何 warning

  场景: 报告 C++ 函数声明省略参数类型或名称
    假如有一个源码文件 "test.cpp"，内容如下:
        """
        void foo(int);

        void foo(int x)
        {
        }
        """
    当对 "test.cpp" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-1-13" 时
    那么应该报告 "test.cpp:1:6: warning: 禁止函数声明省略参数类型或名称 [gjb8114-r-1-1-13]"
    并且总共报告 1 个 warning

  场景: 报告 C 函数声明省略参数类型或名称
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(int);

        void foo(int x)
        {
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-1-13" 时
    那么应该报告 "test.c:1:6: warning: 禁止函数声明省略参数类型或名称 [gjb8114-r-1-1-13]"
    并且总共报告 1 个 warning

  场景: 不报告 C++ 函数声明中省略参数列表
    假如有一个源码文件 "test.cpp"，内容如下:
        """
        void foo();

        void foo(int x, int y)
        {
        }
        """
    当对 "test.cpp" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-1-13" 时
    那么不应该报告任何 warning

