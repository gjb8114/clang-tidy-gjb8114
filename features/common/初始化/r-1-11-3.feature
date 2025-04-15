# language: zh-CN
功能: R-1-11-3 禁止结构体初始化列表的嵌套结构与定义不一致

  @acceptance
  场景: 报告结构体初始化列表的嵌套结构与定义不一致
    假如有一个源码文件 "test.c"，内容如下:
      """
      struct Outer
      {
          int a;
          int b;
          struct Inner
          {
              int y;
          } inner;
      };

      void foo(void)
      {
          struct Outer outer = {1, 2, 3};
      }
      """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-11-3" 时
    那么应该报告 "test.c:13:33: warning: 禁止结构体初始化列表的嵌套结构与定义不一致 [gjb8114-r-1-11-3]"
    并且总共报告 1 个 warning

  @acceptance
  场景: 不报告结构体初始化列表的嵌套结构与定义一致
    假如有一个源码文件 "test.c"，内容如下:
      """
      struct Outer
      {
          int a
          int b
          struct Inner
          {
              int c;
          } inner;
      };

      void foo(void)
      {
          struct Outer outer = {1, 2, {3}};
      }
      """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-11-3" 时
    那么不应该报告任何 warning

  场景: 报告结构体初始化的嵌套结构多个与定义不一致
    假如有一个源码文件 "test.c"，内容如下:
      """
      struct Outer
      {
          int a;
          int b;
          struct Inner
          {
              int c;
          } inner;
          struct Inner2
          {
              int d;
          } inner2;
      };

      void foo(void)
      {
          struct Outer outer = {1, 2, 3, 4};
      }
      """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-11-3" 时
    那么应该报告 "test.c:17:33: warning: 禁止结构体初始化列表的嵌套结构与定义不一致 [gjb8114-r-1-11-3]"
    那么应该报告 "test.c:17:36: warning: 禁止结构体初始化列表的嵌套结构与定义不一致 [gjb8114-r-1-11-3]"
    并且总共报告 2 个 warning

