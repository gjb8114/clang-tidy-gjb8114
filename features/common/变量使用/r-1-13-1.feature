# language: zh-CN
功能: R-1-13-1 禁止局部变量与全局变量同名

  @acceptance
  场景: 报告局部变量与全局变量同名
    假如有一个源码文件 "test.c"，内容如下:
      """
      int the_global_var = 0;

      void foo(void)
      {
          int the_global_var = 0;
      }
      """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-13-1" 时
    那么应该报告 "test.c:5:9: warning: 禁止局部变量与全局变量同名 [gjb8114-r-1-13-1]"
    并且总共报告 1 个 warning

  场景: 不报告局部变量与全局变量不同名
    假如有一个源码文件 "test.c"，内容如下:
      """
      int the_global_var = 0;

      void foo(void)
      {
          int local_var = 0;
      }
      """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-13-1" 时
    那么不应该报告任何 warning

  场景: 报告多个函数中局部变量与全局变量同名
    假如有一个源码文件 "test.c"，内容如下:
      """
      int counter = 0;
      float value = 1.0;

      void foo(void)
      {
          int counter = 1;
          float value = 2.0;
      }
      """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-13-1" 时
    那么应该报告 "test.c:6:9: warning: 禁止局部变量与全局变量同名 [gjb8114-r-1-13-1]"
    并且应该报告 "test.c:7:11: warning: 禁止局部变量与全局变量同名 [gjb8114-r-1-13-1]"
    并且总共报告 2 个 warning

  场景: 报告嵌套块中局部变量与全局变量同名
    假如有一个源码文件 "test.c"，内容如下:
      """
      int data = 0;
      int result = 0;

      void foo(void)
      {
          int i = 0;
          for (i = 0; i < 10; i++)
          {
              int data = i;
          }

          if (i > 0)
          {
              int result = 1;
          }
      }
      """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-13-1" 时
    那么应该报告 "test.c:9:13: warning: 禁止局部变量与全局变量同名 [gjb8114-r-1-13-1]"
    并且应该报告 "test.c:14:13: warning: 禁止局部变量与全局变量同名 [gjb8114-r-1-13-1]"
    并且总共报告 2 个 warning