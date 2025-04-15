# language: zh-CN
功能: R-1-6-7 禁止移位数为负数

  @acceptance
  场景: 报告移位数为负数
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(int x, int y)
        {
            x << -1;
            int bits = -2;
            y >> bits;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-6-7" 时
    那么应该报告 "test.c:3:10: warning: 禁止移位数为负数 [gjb8114-r-1-6-7]"
    并且应该报告 "test.c:5:10: warning: 禁止移位数为负数 [gjb8114-r-1-6-7]"
    并且总共报告 2 个 warning