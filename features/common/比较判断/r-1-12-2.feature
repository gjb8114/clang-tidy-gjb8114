# language: zh-CN
功能: R-1-12-2 禁止对指针进行无意义比较

  @acceptance
  场景: 报告对指针进行无意义比较
    假如有一个源码文件 "test.c"，内容如下:
      """
      int cmp(int *x, int *y)
      {
          if (x > y)
          {
              return 1;
          }
          else if (x < y)
          {
              return -1;
          }
          return 0;
      }
      """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-12-2" 时
    那么应该报告 "test.c:3:11: warning: 禁止对指针进行无意义比较 [gjb8114-r-1-12-2]"
    并且应该报告 "test.c:7:16: warning: 禁止对指针进行无意义比较 [gjb8114-r-1-12-2]"
    并且总共报告 2 个 warning

  @acceptance
  场景: 不报告对指针指向的值进行逻辑比较
    假如有一个源码文件 "test.c"，内容如下:
      """
      int cmp(int *x, int *y)
      {
          if (*x > *y)
          {
              return 1;
          }
          else if (*x < *y)
          {
              return -1;
          }
          return 0;
      }
      """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-12-2" 时
    那么不应该报告任何 warning

  场景: 报告对指针进行大于等于或小于等于的逻辑比较
    假如有一个源码文件 "test.c"，内容如下:
      """
      void foo(int *x, int *y)
      {
          int i = x >= y;
          i = x <= y;
      }
      """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-12-2" 时
    那么应该报告 "test.c:3:15: warning: 禁止对指针进行无意义比较 [gjb8114-r-1-12-2]"
    并且应该报告 "test.c:4:11: warning: 禁止对指针进行无意义比较 [gjb8114-r-1-12-2]"
    并且总共报告 2 个 warning
