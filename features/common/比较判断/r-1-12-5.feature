# language: zh-CN
功能: R-1-12-5 禁止无符号数与有符号数比较

  @acceptance
  场景: 报告无符号数与有符号数比较
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(int x, unsigned y)
        {
            int b = y < x;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-12-5" 时
    那么应该报告 "test.c:3:15: warning: 禁止无符号数与有符号数比较 [gjb8114-r-1-12-5]"
    并且总共报告 1 个 warning

  @acceptance
  场景: 不报告无符号数与有符号数类型转换后比较
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(int x, unsigned y)
        {
            int b = (int)y < x;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-12-5" 时
    并且不应该报告任何 warning

  @acceptance
  场景: 报告无符号数与有符号数的其它比较运算符
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(int x, unsigned y)
        {
            if (y == x) { }
            if (y != x) { }
            if (y > x) { }
            if (y >= x) { }
            if (y < x) { }
            if (y <= x) { }
            if (x == y) { }
            if (x != y) { }
            if (x > y) { }
            if (x >= y) { }
            if (x < y) { }
            if (x <= y) { }
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-12-5" 时
    那么应该报告 "test.c:3:11: warning: 禁止无符号数与有符号数比较 [gjb8114-r-1-12-5]"
    并且应该报告 "test.c:4:11: warning: 禁止无符号数与有符号数比较 [gjb8114-r-1-12-5]"
    并且应该报告 "test.c:5:11: warning: 禁止无符号数与有符号数比较 [gjb8114-r-1-12-5]"
    并且应该报告 "test.c:6:11: warning: 禁止无符号数与有符号数比较 [gjb8114-r-1-12-5]"
    并且应该报告 "test.c:7:11: warning: 禁止无符号数与有符号数比较 [gjb8114-r-1-12-5]"
    并且应该报告 "test.c:8:11: warning: 禁止无符号数与有符号数比较 [gjb8114-r-1-12-5]"
    并且应该报告 "test.c:9:11: warning: 禁止无符号数与有符号数比较 [gjb8114-r-1-12-5]"
    并且应该报告 "test.c:10:11: warning: 禁止无符号数与有符号数比较 [gjb8114-r-1-12-5]"
    并且应该报告 "test.c:11:11: warning: 禁止无符号数与有符号数比较 [gjb8114-r-1-12-5]"
    并且应该报告 "test.c:12:11: warning: 禁止无符号数与有符号数比较 [gjb8114-r-1-12-5]"
    并且应该报告 "test.c:13:11: warning: 禁止无符号数与有符号数比较 [gjb8114-r-1-12-5]"
    并且应该报告 "test.c:14:11: warning: 禁止无符号数与有符号数比较 [gjb8114-r-1-12-5]"
    并且总共报告 12 个 warning
