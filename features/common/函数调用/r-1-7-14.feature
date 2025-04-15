# language: zh-CN
功能: R-1-7-14 禁止同一表达式中调用多个相关函数

  相关函数是指在同一表达式中调用的函数，且它们之间存在数据依赖关系，将导致未定义行为。

  @acceptance
  场景: 报告多个相关函数调用
    假如有一个源码文件 "test.c"，内容如下:
        """
        int inc(int *x)
        {
            *x += 1;
            return *x;
        }

        int square(int *x)
        {
            *x *= *x;
            return *x;
        }

        void foo(void)
        {
            int x = 3;
            int y = inc(&x) + square(&x);
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-7-14" 时
    那么应该报告 "test.c:16:21: warning: 禁止同一表达式中调用多个相关函数 [gjb8114-r-1-7-14]"
    并且总共报告 1 个 warning

  @acceptance
  场景: 不报告多个相关函数调用不在同一表达式中
    假如有一个源码文件 "test.c"，内容如下:
        """
        int inc(int *x)
        {
            *x += 1;
            return *x;
        }

        int square(int *x)
        {
            *x *= *x;
            return *x;
        }

        void foo(void)
        {
            int x = 3;
            x = inc(&x);
            int y = x + square(&x);
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-7-14" 时
    那么不应该报告任何 warning