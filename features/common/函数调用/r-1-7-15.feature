# language: zh-CN
功能: R-1-7-15 禁止函数参数列表中使用省略号

  省略号的使用会导致函数参数类型不明确，可能会引起不必要的错误

  @acceptance
  场景: 报告函数参数表中使用省略号
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(int x, ...);

        void foo(int x, ...)
        {
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-7-15" 时
    那么应该报告 "test.c:1:17: warning: 禁止函数参数列表中使用省略号 [gjb8114-r-1-7-15]"
    并且应该报告 "test.c:3:17: warning: 禁止函数参数列表中使用省略号 [gjb8114-r-1-7-15]"
    并且总共报告 2 个 warning

  @acceptance
  场景: 不报告函数参数表中未使用省略号
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(int x);

        void foo(int x)
        {
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-7-15" 时
    那么不应该报告任何 warning