# language: zh-CN
功能: R-1-7-7 禁止非 void 函数省略 return 语句

  @acceptance
  场景: 报告非 void 函数省略返回语句
    假如有一个源码文件 "test.c"，内容如下:
        """
        int foo(int x)
        {
            if (x == 0)
            {
                return 0;
            }
            else
            {
            }
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-7-7" 时
    那么应该报告 "test.c:9:5: warning: 禁止非 void 函数省略 return 语句 [gjb8114-r-1-7-7]"
    并且总共报告 1 个 warning

  @acceptance
  场景: 不报告非 void 函数有完备的返回语句
    假如有一个源码文件 "test.c"，内容如下:
        """
        int fun(int x)
        {
            if (x == 0)
            {
                return 0;
            }
            else
            {
                return 1;
            }
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-7-7" 时
    那么不应该报告任何 warning