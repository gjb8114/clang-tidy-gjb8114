# language: zh-CN
功能: R-1-2-1 禁止循环体省略大括号

  @acceptance
  场景: 报告未用大括号的 for 循环
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(void)
        {
            for (int i = 0; i < 7; ++i)
                ;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-2-1" 时
    那么应该报告 "test.c:4:9: warning: 禁止循环体省略大括号 [gjb8114-r-1-2-1]"
    并且总共报告 1 个 warning

  @acceptance
  场景: 不报告用大括号的 for 循环
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(void)
        {
            for (int i = 0; i < 7; ++i)
            {
            }
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-2-1" 时
    那么不应该报告任何 warning

  场景: 报告未用大括号的 while 循环
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(void)
        {
            int i = 0;
            while (i < 7)
                ;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-2-1" 时
    那么应该报告 "test.c:5:9: warning: 禁止循环体省略大括号 [gjb8114-r-1-2-1]"
    并且总共报告 1 个 warning

  场景: 报告未用大括号的 do-while 循环
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(void)
        {
            int i = 0;
            do
                ;
            while (i < 7);
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-2-1" 时
    那么应该报告 "test.c:5:9: warning: 禁止循环体省略大括号 [gjb8114-r-1-2-1]"
    并且总共报告 1 个 warning

  场景: 不报告用大括号的 while 循环
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(void)
        {
            int i = 0;
            while (i < 7)
            {
            }
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-2-1" 时
    那么不应该报告任何 warning

  场景: 不报告用大括号的 do-while 循环
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(void)
        {
            int i = 0;
            do
            {
                ;
            } while (i < 7);
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-2-1" 时
    那么不应该报告任何 warning