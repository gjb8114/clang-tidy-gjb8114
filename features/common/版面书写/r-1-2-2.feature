# language: zh-CN
功能: R-1-2-2 禁止 if、else if、else 省略大括号

  @acceptance
  场景: 报告未使用大括号
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(int x)
        {
            if (x == 2)
                ;
            else if (x == 3)
                ;
            else
                x = 0;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-2-2" 时
    那么应该报告 "test.c:4:9: warning: 禁止 if 省略大括号 [gjb8114-r-1-2-2]"
    并且应该报告 "test.c:6:9: warning: 禁止 else if 省略大括号 [gjb8114-r-1-2-2]"
    并且应该报告 "test.c:8:9: warning: 禁止 else 省略大括号 [gjb8114-r-1-2-2]"
    并且总共报告 3 个 warning

  @acceptance
  场景: 不报告使用大括号
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(int x)
        {
            if (x == 2)
            {}
            else if (x == 3)
            {}
            else
            {
                x = 0;
            }
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-2-2" 时
    那么不应该报告任何 warning