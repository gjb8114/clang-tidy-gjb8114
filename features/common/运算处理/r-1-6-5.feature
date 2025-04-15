# language: zh-CN
功能: R-1-6-5 禁止在运算表达式或函数调用参数中使用自增自减

  @acceptance
  场景: 报告使用自增自减
    假如有一个源码文件 "test.c"，内容如下:
        """
        void bar(int x);

        void foo(int x, int y, int z)
        {
            y = y + (x--);
            z = z - (++y);
            bar(--z);
            bar(z++ * 2);
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-6-5" 时
    那么应该报告 "test.c:5:15: warning: 禁止在运算表达式或函数调用参数中使用自增自减 [gjb8114-r-1-6-5]"
    并且应该报告 "test.c:6:14: warning: 禁止在运算表达式或函数调用参数中使用自增自减 [gjb8114-r-1-6-5]"
    并且应该报告 "test.c:7:9: warning: 禁止在运算表达式或函数调用参数中使用自增自减 [gjb8114-r-1-6-5]"
    并且应该报告 "test.c:8:10: warning: 禁止在运算表达式或函数调用参数中使用自增自减 [gjb8114-r-1-6-5]"
    并且总共报告 4 个 warning

  @acceptance
  场景: 不报告未使用自增自减
    假如有一个源码文件 "test.c"，内容如下:
        """
        void bar(int x);

        void foo(int x, int y, int z)
        {
            y = y + x;
            --x;
            ++y;
            z = z - y;
            --z;
            bar(z);
            bar(z * 2);
            ++z;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-6-5" 时
    那么不应该报告任何 warning