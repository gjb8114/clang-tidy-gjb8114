# language: zh-CN
功能: R-1-7-12 禁止使用 (void) 转换 void 函数调用的返回值

  @acceptance
  场景: 报告禁止使用 (void) 转换 void 函数调用的返回值
    假设有一个源码文件 "test.c"，内容如下:
        """
        void foo(int x);

        void bar(void)
        {
            (void)foo(1);
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-7-12" 时
    那么应该报告 "test.c:5:5: warning: 禁止使用 (void) 转换 void 函数调用的返回值 [gjb8114-r-1-7-12]"
    并且总共报告 1 个 warning

  @acceptance
  场景: 不报告不使用 (void) 转换 void 函数调用的返回值
    假设有一个源码文件 "test.c"，内容如下:
        """
        void foo(int x);

        void bar(void)
        {
            foo(1);
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-7-12" 时
    那么不应该报告任何 warning