# language: zh-CN
功能: R-1-8-3 禁止无效语句

  @acceptance
  场景: 报告无效语句
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(int x, int y)
        {
            x;
            y * 7;
            x != y;
            y < x;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-8-3" 时
    那么应该报告 "test.c:3:5: warning: 禁止无效语句 [gjb8114-r-1-8-3]"
    并且应该报告 "test.c:4:5: warning: 禁止无效语句 [gjb8114-r-1-8-3]"
    并且应该报告 "test.c:5:5: warning: 禁止无效语句 [gjb8114-r-1-8-3]"
    并且应该报告 "test.c:6:5: warning: 禁止无效语句 [gjb8114-r-1-8-3]"
    并且总共报告 4 个 warning

  @acceptance
  场景: 不报告赋值语句
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(int x, int y)
        {
            x = 5;
            y = x + y;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-8-3" 时
    那么不应该报告任何 warning

  场景: 不报告函数调用语句
    假如有一个源码文件 "test.c"，内容如下:
        """
        #include <stdio.h>

        void foo(void)
        {
            printf("Hello world");
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-8-3" 时
    那么不应该报告任何 warning

  场景: 报告复杂表达式中的无效语句
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(int x, int y, int z)
        {
            x + y;
            (z - x) * y;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-8-3" 时
    那么应该报告 "test.c:3:5: warning: 禁止无效语句 [gjb8114-r-1-8-3]"
    并且应该报告 "test.c:4:5: warning: 禁止无效语句 [gjb8114-r-1-8-3]"
    并且总共报告 2 个 warning

  场景: 不报告条件语句中的表达式
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(int x, int y)
        {
            if (x == y)
            {
            }
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-8-3" 时
    那么不应该报告任何 warning