# language: zh-CN
功能: R-1-7-3 禁止实参与形参的个数不一致

  例外: C++ 函数重载

  @acceptance
  场景: 报告实参与形参的个数不一致
    假如有一个源码文件 "test.c"，内容如下:
        """
        void bar(int x, int y)
        {
            // do something
        }

        void foo(void)
        {
            bar(1, 0, 1);
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-7-3" 时
    那么应该报告 "test.c:8:15: warning: 禁止实参与形参的个数不一致 [gjb8114-r-1-7-3]"
    并且总共报告 1 个 warning

  场景: 报告实参个数少于形参
    假如有一个源码文件 "test.c"，内容如下:
        """
        void bar(int x, int y)
        {
            // do something
        }

        void foo(void)
        {
            bar(0);
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-7-3" 时
    那么应该报告 "test.c:8:10: warning: 禁止实参与形参的个数不一致 [gjb8114-r-1-7-3]"
    并且总共报告 1 个 warning