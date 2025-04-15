# language: zh-CN
功能: R-1-13-15 禁止无符号变量赋负值

  @acceptance
  场景: 报告赋负值
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(void)
        {
            unsigned x;
            x = -1;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-13-15" 时
    那么应该报告 "test.c:4:9: warning: 禁止无符号变量赋负值 [gjb8114-r-1-13-15]"
    并且总共报告 1 个 warning

  @acceptance
  场景: 不报告显式转换
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(void)
        {
            unsigned x;
            x = (unsigned)(-1);
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-13-15" 时
    那么总共报告 0 个 warning