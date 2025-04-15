# language: zh-CN
功能: R-1-3-3 禁止对指针型参数重新赋值

  @acceptance
  场景: 报告对指针型参数重新赋值
    假如有一个源码文件 "test.c"，内容如下:
        """
        #include <stdlib.h>

        void foo(int *px)
        {
            px = NULL;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-3-3" 时
    那么应该报告 "test.c:5:8: warning: 禁止对指针型参数重新赋值 [gjb8114-r-1-3-3]"
    并且总共报告 1 个 warning

  @acceptance
  场景: 不报告对参数指针指向的内存赋值
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(int *px)
        {
            *px = 0;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-3-3" 时
    那么不应该报告任何 warning