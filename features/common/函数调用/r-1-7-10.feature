# language: zh-CN
功能: R-1-7-10 禁止函数返回值类型与定义不一致

  @acceptance
  场景: 报告函数返回值类型与定义不一致
    假设有一个源码文件 "test.c"，内容如下:
        """
        unsigned int foo(int x)
        {
            if (x < 0)
            {
                return -1;
            }
            return x;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-7-10" 时
    那么应该报告 "test.c:5:16: warning: 禁止函数返回值类型与定义不一致 [gjb8114-r-1-7-10]"
    并且应该报告 "test.c:7:12: warning: 禁止函数返回值类型与定义不一致 [gjb8114-r-1-7-10]"
    并且总共报告 2 个 warning

  场景: 不报告函数返回值类型与定义一致
    假设有一个源码文件 "test.c"，内容如下:
    """
    unsigned int foo(int x)
    {
        if (x == 1)
        {
            return 1U;
        }
        else if (x < 0)
        {
            return (unsigned int)-1;
        }
        return (unsigned int)x;
    }
    """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-7-10" 时
    那么不应该报告任何 warning