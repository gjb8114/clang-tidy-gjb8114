# language: zh-CN
功能: R-1-9-1 禁止 for 循环控制变量使用非局部变量

  @acceptance
  场景: 报告使用非局部变量
    假如有一个源码文件 "test.c"，内容如下:
        """
        int i = 0;

        void foo(void)
        {
            for (i = 0; i < 7; ++i)
                ;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-9-1" 时
    那么应该报告 "test.c:5:10: warning: 禁止 for 循环控制变量使用非局部变量 [gjb8114-r-1-9-1]"

  场景: 不报告使用局部变量
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(void)
        {
            for (int i = 0; i < 7; ++i)
                ;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-9-1" 时
    那么不应该报告任何 warning