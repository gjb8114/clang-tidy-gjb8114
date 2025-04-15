# language: zh-CN
功能: R-1-6-13 禁止 sizeof 中使用赋值

  @acceptance
  场景: 报告 sizeof 中使用赋值
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(int x)
        {
            int a;
            int size = sizeof(a = x);
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-6-13" 时
    那么应该报告 "test.c:4:25: warning: 禁止 sizeof 中使用赋值 [gjb8114-r-1-6-13]"
    并且总共报告 1 个 warning

  @acceptance
  场景: 不报告 sizeof 中使用非赋值
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(int x)
        {
            int a;
            a = x;
            int size = sizeof(a);
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-6-13" 时
    那么不应该报告任何 warning
