# language: zh-CN
功能: R-1-13-13 禁止表达式多次引用同一 volatile 变量

  @acceptance
  场景: 报告多个同一 volatile 变量的运算
    假如有一个源码文件 "test.c"，内容如下:
        """
        int foo(int *x)
        {
            volatile int v = 1;
            *x = v + v / 2;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-13-13" 时
    那么应该报告 "test.c:4:12: warning: 禁止表达式多次引用同一 volatile 变量 [gjb8114-r-1-13-13]"
    并且总共报告 1 个 warning

  @acceptance
  场景: 不报告单一 volatile 变量的运算
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(int *x)
        {
            volatile int v = 1;
            *x = v;
            *x = *x + *x / 2;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-13-13" 时
    那么不应该报告任何 warning