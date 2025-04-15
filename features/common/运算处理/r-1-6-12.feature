# language: zh-CN
功能: R-1-6-12 禁止零除

  @acceptance
  场景: 报告除于零变量
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(int x)
        {
            int a, b = 0;
            a = x / b;
            a = x % b;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-6-12" 时
    那么应该报告 "test.c:4:11: warning: 禁止零除 [gjb8114-r-1-6-12]"
    并且应该报告 "test.c:5:11: warning: 禁止零除 [gjb8114-r-1-6-12]"
    并且总共报告 2 个 warning

  @acceptance
  场景: 不报告除于 0 参数变量
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(int x, int y)
        {
            int a;
            a = x / y;
            a = x % y;
        }

        void bar(void)
        {
            int a, b = 0;
            foo(a, b);
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-6-12" 时
    那么不应该报告任何 warning

  场景: 报告除于 0 字面量
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(int x)
        {
            int a;
            a = x / 0;
            a = x % 0;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-6-12" 时
    那么应该报告 "test.c:4:11: warning: 禁止零除 [gjb8114-r-1-6-12]"
    并且应该报告 "test.c:5:11: warning: 禁止零除 [gjb8114-r-1-6-12]"
    并且总共报告 2 个 warning