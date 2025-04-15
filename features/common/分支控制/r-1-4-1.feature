# language: zh-CN
功能: R-1-4-1 禁止省略 if-else if 语句的 else 分支

  @acceptance
  场景: 报告省略 else 分支
    假如有一个源码文件 "test.c"，内容如下:
        """
        int foo(int x)
        {
            if (x > 1)
            {
                return 1;
            }
            else if (x < -1)
            {
                return -1;
            }
            return x;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-4-1" 时
    那么应该报告 "test.c:10:5: warning: 禁止省略 if-else if 语句的 else 分支 [gjb8114-r-1-4-1]"
    并且总共报告 1 个 warning

  @acceptance
  场景: 不报告包含 else 分支
    假如有一个源码文件 "test.c"，内容如下:
        """
        int foo(int x)
        {
            if (x > 1)
            {
                return 1;
            }
            else if (x < -1)
            {
                return -1;
            }
            else
            {
                return x;
            }
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-4-1" 时
    那么不应该报告任何 warning