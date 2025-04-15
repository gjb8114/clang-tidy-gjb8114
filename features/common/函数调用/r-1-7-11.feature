# language: zh-CN
功能: R-1-7-11 禁止忽略函数调用返回值而未加 (void)

  非 void 函数的返回值可能会影响程序的执行结果，忽略时应显式地加上 (void) 说明

  @acceptance
  场景: 报告未加 (void) 说明的函数调用
    假如有一个源码文件 "test.c"，内容如下:
        """
        int foo(int x);

        void bar(void)
        {
            foo(1);
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-7-11" 时
    那么应该报告 "test.c:5:5: warning: 禁止忽略函数调用返回值而未加 (void) [gjb8114-r-1-7-11]"
    并且总共报告 1 个 warning

  @acceptance
  场景: 不报告已加 (void) 说明的函数调用
    假如有一个源码文件 "test.c"，内容如下:
        """
        int foo(int x);

        void bar(void)
        {
            (void)foo(1);
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-7-11" 时
    那么不应该报告任何 warning