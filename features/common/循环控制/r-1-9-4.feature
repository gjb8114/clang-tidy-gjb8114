# language: zh-CN
功能: R-1-9-4 禁止 while(1) 以外的无限循环

  @acceptance
  场景: 报告 for(;;) 无限循环
    假如有一个源码文件 "test.c"，内容如下:
        """
        int running = 1;

        void foo(void)
        {
            for (;;) {
                if (!running) {
                    break;
                }
            }
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-9-4" 时
    那么应该报告 "test.c:5:5: warning: 禁止 while(1) 以外的无限循环 [gjb8114-r-1-9-4]"
    并且总共报告 1 个 warning

  @acceptance
  场景: 不报告 while(1) 无限循环
    假如有一个源码文件 "test.c"，内容如下:
        """
        int running = 0;

        void foo(void)
        {
            while (1) {
                if (!running) {
                    break;
                }
            }
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-9-4" 时
    那么不应该报告任何 warning