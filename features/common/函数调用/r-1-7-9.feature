# language: zh-CN
功能: R-1-7-9 禁止非 void 函数的 return 语句不带值

  @acceptance
  场景: 报告有返回值函数的返回语句不带返回值
    假如有一个源码文件 "test.c"，内容如下:
        """
        int foo(void)
        {
            return;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-7-9" 时
    那么应该报告 "test.c:3:5: warning: 禁止非 void 函数的 return 语句不带值 [gjb8114-r-1-7-9]"
    并且总共报告 1 个 warning

  场景: 报告多个返回语句不带返回值
    假如有一个源码文件 "test.c"，内容如下:
        """
        int foo(int x)
        {
            if (x > 0)
            {
                return;
            }
            return;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-7-9" 时
    那么应该报告 "test.c:5:9: warning: 禁止非 void 函数的 return 语句不带值 [gjb8114-r-1-7-9]"
    并且应该报告 "test.c:7:5: warning: 禁止非 void 函数的 return 语句不带值 [gjb8114-r-1-7-9]"
    并且总共报告 2 个 warning

  场景: 不报告所有分支均带有返回值
    假如有一个源码文件 "test.c"，内容如下:
        """
        int foo(int x)
        {
            if (x > 0)
            {
                return 1;
            }
            return 0;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-7-9" 时
    那么不应该报告任何 warning