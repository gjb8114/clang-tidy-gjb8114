# language: zh-CN
功能: R-1-3-4 禁止返回局部变量地址

  @acceptance
  场景: 报告返回局部变量地址
    假如有一个源码文件 "test.c"，内容如下:
        """
        int* foo(void)
        {
            int tmp = 0;
            return &tmp;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-3-4" 时
    那么应该报告 "test.c:4:12: warning: 禁止返回局部变量地址 [gjb8114-r-1-3-4]"
    并且总共报告 1 个 warning

  @acceptance
  场景: 不报告返回全局变量地址
    假如有一个源码文件 "test.c"，内容如下:
        """
        int the_var = 0;
        int* foo(void)
        {
            return &the_var;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-3-4" 时
    那么不应该报告任何 warning