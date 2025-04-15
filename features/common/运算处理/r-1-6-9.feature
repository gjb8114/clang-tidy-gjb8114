# language: zh-CN
功能: R-1-6-9 禁止数组下标为负数

  @acceptance
  场景: 报告数组下标为负数
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(int buffer[])
        {
            int i = -1;
            buffer[i] = 0;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-6-9" 时
    那么应该报告 "test.c:4:12: warning: 禁止数组下标为负数 [gjb8114-r-1-6-9]"
    并且总共报告 1 个 warning

  @acceptance
  场景: 不报告数组下标为非负数
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(int buffer[])
        {
            int i = 0;
            buffer[i] = 0;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-6-9" 时
    那么不应该报告任何 warning

  场景: 报告负数字面量下标
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(int buffer[])
        {
            buffer[-1] = 0;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-6-9" 时
    那么应该报告 "test.c:3:12: warning: 禁止数组下标为负数 [gjb8114-r-1-6-9]"
    并且总共报告 1 个 warning