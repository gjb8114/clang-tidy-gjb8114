# language: zh-CN
功能: R-1-6-2 禁止整数赋值越界

  @acceptance
  场景: 报告整数赋值越界
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(void)
        {
            unsigned short us = 0;
            us = 65536;
            signed short ss = -32769;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-6-2" 时
    那么应该报告 "test.c:4:10: warning: 禁止整数赋值越界 [gjb8114-r-1-6-2]"
    并且应该报告 "test.c:5:24: warning: 禁止整数赋值越界 [gjb8114-r-1-6-2]"
    并且总共报告 2 个 warning

  @acceptance
  场景: 不报告合法整数赋值
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(void)
        {
            unsigned short us = 65535;
            signed short ss;
            ss = -32768;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-6-2" 时
    那么不应该报告任何 warning