# language: zh-CN
功能: R-1-1-18 禁止数组定义没有显式边界

  @acceptance
  场景: 报告数组定义没有显式边界
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(void)
        {
            int var[] = {0, 0, 0};
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-1-18" 时
    那么应该报告 "test.c:3:13: warning: 禁止数组定义没有显式边界 [gjb8114-r-1-1-18]"
    并且总共报告 1 个 warning

  @acceptance
  场景: 不报告数组定义有显式边界
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(void)
        {
            int var[3] = {0};
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-1-18" 时
    那么不应该报告任何 warning