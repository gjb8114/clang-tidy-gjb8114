# language: zh-CN
功能: R-1-7-2 禁止实参和形参类型不一致

  例外: C++ 函数重载

  @acceptance
  场景: 报告实参和形参类型不一致
    假如有一个源码文件 "test.c"，内容如下:
        """
        void bar(unsigned int num)
        {
            // do something
        }

        void foo(void)
        {
            int n = 2;
            bar(n);
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-7-2" 时
    那么应该报告 "test.c:9:9: warning: 禁止实参和形参类型不一致 [gjb8114-r-1-7-2]"
    并且总共报告 1 个 warning

  @acceptance
  场景: 不报告实参和形参类型一致
    假如有一个源码文件 "test.c"，内容如下:
        """
        void bar(unsigned int num) {
            // do something
        }

        void foo(void)
        {
            bar(2u);
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-7-2" 时
    那么不应该报告任何 warning