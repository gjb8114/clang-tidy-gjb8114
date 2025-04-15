# language: zh-CN
功能: R-1-6-4 禁止对逻辑表达式进行位运算

  @acceptance
  场景: 报告逻辑表达式的位运算
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(int x, int y)
        {
            if ((x < 0) & (y < 0))
            { }
            if ((x > 0) | (y > 0))
            { }
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-6-4" 时
    那么应该报告 "test.c:3:17: warning: 禁止对逻辑表达式进行位运算 [gjb8114-r-1-6-4]"
    并且应该报告 "test.c:5:17: warning: 禁止对逻辑表达式进行位运算 [gjb8114-r-1-6-4]"
    并且总共报告 2 个 warning

  @acceptance
  场景: 不报告逻辑表达式的逻辑运算
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(int x, int y)
        {
            if ((x < 0) && (y < 0))
            { }
            if ((x > 0) || (y > 0))
            { }
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-6-4" 时
    那么不应该报告任何 warning

  场景: 报告赋值表达式中的布尔位运算
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(int x, int y)
        {
            int b;
            b = (x >= 0) | (y < 0);
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-6-4" 时
    那么应该报告 "test.c:4:18: warning: 禁止对逻辑表达式进行位运算 [gjb8114-r-1-6-4]"
    并且总共报告 1 个 warning