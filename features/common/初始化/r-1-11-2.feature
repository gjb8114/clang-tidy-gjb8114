# language: zh-CN
功能: R-1-11-2 禁止变量缺省初始化

  @acceptance
  场景: 报告变量初始化依赖系统缺省值
    假如有一个源码文件 "test.c"，内容如下:
      """
      int the_flag;

      void foo(void)
      {
          static int the_state;
      }
      """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-11-2" 时
    那么应该报告 "test.c:1:5: warning: 禁止变量缺省初始化 [gjb8114-r-1-11-2]"
    并且应该报告 "test.c:5:16: warning: 禁止变量缺省初始化 [gjb8114-r-1-11-2]"
    并且总共报告 2 个 warning

  @acceptance
  场景: 不报告变量显式初始化
    假如有一个源码文件 "test.c"，内容如下:
      """
      int the_flag = 0;

      void foo(void)
      {
          static int the_state = 0;
      }
      """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-11-2" 时
    那么不应该报告任何 warning

  场景: 报告类静态成员变量缺省初始化
    假如有一个源码文件 "test.cpp"，内容如下:
      """
      class A
      {
      public:
          static int the_flag;
      };
      """
    当对 "test.cpp" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-11-2" 时
    那么应该报告 "test.cpp:4:16: warning: 禁止变量缺省初始化 [gjb8114-r-1-11-2]"
    并且总共报告 1 个 warning
