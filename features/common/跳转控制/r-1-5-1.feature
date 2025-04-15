# language: zh-CN
功能: R-1-5-1 禁止向内或向后 goto

  向内 goto: 指的是在复合语句外跳转到复合语句内的标签

  向后 goto: 指的是在前（下）方跳转到后（上）方的标签

  @acceptance
  场景: 报告向复合语句内 goto
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(void)
        {
            goto Inner;
            for (int i = 0; i < 7; ++i)
            {
        Inner:
            }
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-5-1" 时
    并且应该报告 "test.c:3:10: warning: 禁止向内或向后 goto [gjb8114-r-1-5-1]"
    并且总共报告 1 个 warning

  @acceptance
  场景: 不报告向前 goto
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(void)
        {
            goto NEXT;
        NEXT:
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-5-1" 时
    那么不应该报告任何 warning

  场景: 报告向后 goto
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(void)
        {
        Back:
            for (int i = 0; i < 7; ++i)
            {
            }
            goto Back;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-5-1" 时
    并且应该报告 "test.c:7:10: warning: 禁止向内或向后 goto [gjb8114-r-1-5-1]"
    并且总共报告 1 个 warning

  场景: 不报告向复合语句外 goto
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(int x, int y)
        {
            if (x < 0)
            {
                if (y < 0)
                {
                    goto EXIT;
                }
            }

        EXIT:
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-5-1" 时
    那么不应该报告任何 warning