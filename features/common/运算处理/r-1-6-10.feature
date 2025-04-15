# language: zh-CN
功能: R-1-6-10 禁止对字面量值取逻辑非

  @acceptance
  场景: 报告对字面量值取逻辑非
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(int x)
        {
            if (x == !0)
            {
                // do something
            }
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-6-10" 时
    那么应该报告 "test.c:3:14: warning: 禁止对字面量值取逻辑非 [gjb8114-r-1-6-10]"
    并且总共报告 1 个 warning

  @acceptance
  场景: 不报告对变量取逻辑非
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(int x)
        {
            unsigned int y = 0;
            if (x == !y)
            {
                // do something
            }
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-6-10" 时
    那么不应该报告任何 warning