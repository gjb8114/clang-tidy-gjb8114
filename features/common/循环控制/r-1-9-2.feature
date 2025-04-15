# language: zh-CN
功能: R-1-9-2 禁止 for 循环控制变量为非整数型

  @acceptance
  场景: 报告非整数型变量
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(void)
        {
            float i;
            for (i = 0.0; i < 7.0; i = i + 1.0)
                ;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-9-2" 时
    那么应该报告 "test.c:3:5: warning: 禁止 for 循环控制变量为非整数型 [gjb8114-r-1-9-2]"
    并且总共报告 1 个 warning

  场景: 报告 for 内部变量
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(void)
        {
            for (float i = 0.0; i < 7.0; i = i + 1.0)
                ;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-9-2" 时
    那么应该报告 "test.c:3:10: warning: 禁止 for 循环控制变量为非整数型 [gjb8114-r-1-9-2]"
    并且总共报告 1 个 warning

  场景: 不报告整数型变量
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(void)
        {
            int i = 0;
            for (i = 0; i < 7; i++)
                ;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-9-2" 时
    那么不应该报告任何 warning