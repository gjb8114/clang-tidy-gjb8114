# language: zh-CN
功能: R-1-1-14 禁止函数声明与定义不一致

  包括: 函数返回值类型、参数类型、参数名。

  例外: C++ 允许重载，参数类型、参数名不一致是可能的，但函数返回值类型不一致是禁止的。

  @acceptance
  场景: 报告函数声明与定义不一致
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(void);
        int foo(void)
        {
            return 0;
        }

        void bar(int x);
        void bar(float x)
        {
        }

        void baz(int x);
        void baz(int y)
        {
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-1-14" 时
    那么应该报告 "test.c:1:1: warning: 禁止函数声明与定义不一致 [gjb8114-r-1-1-14]"
    并且应该报告 "test.c:7:10: warning: 禁止函数声明与定义不一致 [gjb8114-r-1-1-14]"
    并且应该报告 "test.c:12:14: warning: 禁止函数声明与定义不一致 [gjb8114-r-1-1-14]"
    并且总共报告 3 个 warning

  @acceptance
  场景: 不报告函数声明与定义一致
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(void);
        void foo(void)
        {
        }

        void bar(int x);
        void bar(int x)
        {
        }

        void baz(int x);
        void baz(int x)
        {
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-1-14" 时
    那么不应该报告任何 warning

  场景大纲: 报告函数声明中参数名不一致
    假如有一个源码文件 "test.<扩展名>"，内容如下:
        """
        int foo(int x);
        int foo(int y) {}
        """
    当对 "test.<扩展名>" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-1-14" 时
    那么应该报告 "test.<扩展名>:1:13: warning: 禁止函数声明与定义不一致 [gjb8114-r-1-1-14]"

    例子:
      | 扩展名 |
      | c   |
      | cpp |

  场景大纲: 报告函数声明中返回值类型不一致
    假如有一个源码文件 "test.<扩展名>"，内容如下:
        """
        int foo(int x);
        float foo(int x) {}
        """
    当对 "test.<扩展名>" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-1-14" 时
    那么应该报告 "test.<扩展名>:1:1: warning: 禁止函数声明与定义不一致 [gjb8114-r-1-1-14]"

    例子:
      | 扩展名 |
      | c   |
      | cpp |

  场景: 报告 C 函数声明中缺少参数
    假如有一个源码文件 "test.c"，内容如下:
        """
        int foo(int);
        int foo(int x, int y) {}
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-1-14" 时
    那么应该报告 "test.c:1:5: warning: 禁止函数声明与定义不一致 [gjb8114-r-1-1-14]"

  场景: 报告 C 函数声明中多余参数
    假如有一个源码文件 "test.c"，内容如下:
        """
        int foo(int x, int y);
        int foo(int x) {}
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-1-14" 时
    那么应该报告 "test.c:1:5: warning: 禁止函数声明与定义不一致 [gjb8114-r-1-1-14]"

  场景: 不报告 C++ 函数声明中多余参数
    假如有一个源码文件 "test.cpp"，内容如下:
        """
        int foo(int x, int y);
        int foo(int x) {}
        """
    当对 "test.cpp" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-1-14" 时
    那么不应该报告任何 warning

  场景: 不报告 C++ 函数声明中不同的参数类型
    假如有一个源码文件 "test.cpp"，内容如下:
        """
        int foo(int x);
        int foo(float x) {}
        """
    当对 "test.cpp" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-1-14" 时
    那么不应该报告任何 warning
