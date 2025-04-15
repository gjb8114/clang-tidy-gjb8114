# language: zh-CN
功能: R-1-6-1 禁止将浮点字面量赋值给整型变量

  @acceptance
  场景: 报告浮点字面量赋值给整型变量
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(void)
        {
            int a = 3.14;
            int b;
            b = 1.0f;
            int c = 1.0l;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-6-1" 时
    那么应该报告 "test.c:3:13: warning: 禁止将浮点字面量赋值给整型变量 [gjb8114-r-1-6-1]"
    并且应该报告 "test.c:5:9: warning: 禁止将浮点字面量赋值给整型变量 [gjb8114-r-1-6-1]"
    并且应该报告 "test.c:6:13: warning: 禁止将浮点字面量赋值给整型变量 [gjb8114-r-1-6-1]"
    并且总共报告 3 个 warning

  @acceptance
  场景: 不报告整型字面量赋值给整型变量
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(void)
        {
            int a = 3;
            int b;
            b = 1;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-6-1" 时
    那么不应该报告任何 warning

  场景: 不报告浮点字面量赋值给浮点变量
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(void)
        {
            float a = 3.14;
            double b = 1.0f;
            long double c = 1.0l;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-6-1" 时
    那么不应该报告任何 warning