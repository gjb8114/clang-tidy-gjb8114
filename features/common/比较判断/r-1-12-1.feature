# language: zh-CN
功能: R-1-12-1 禁止对 boolean 值进行无意义比较

  @acceptance
  场景: 报告对 bool 值进行无意义比较
    假如有一个源码文件 "test.c"，内容如下:
      """
      #include <stdbool.h>

      void foo(bool x, bool y)
      {
          bool b = x > y;
          b = x < y;
      }
      """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-12-1" 时
    那么应该报告 "test.c:5:16: warning: 禁止对 boolean 值进行无意义比较 [gjb8114-r-1-12-1]"
    并且应该报告 "test.c:6:11: warning: 禁止对 boolean 值进行无意义比较 [gjb8114-r-1-12-1]"
    并且总共报告 2 个 warning

  @acceptance
  场景: 不报告对 boolean 值进行合法运算
    假如有一个源码文件 "test.c"，内容如下:
      """
      #include <stdbool.h>

      void foo(bool x, bool y)
      {
          bool b = x && !y;
          b = !x && y;
          b = x == y;
          b = x != y;
      }
      """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-12-1" 时
    那么不应该报告任何 warning

  场景大纲: 报告对 typedef <builtin_type> boolean 值进行无意义比较
    假如有一个源码文件 "test.c"，内容如下:
      """
      typedef <builtin_type> bool;

      void foo(bool x, bool y)
      {
          bool b = x > y;
      }
      """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-12-1" 时
    那么应该报告 "test.c:5:16: warning: 禁止对 boolean 值进行无意义比较 [gjb8114-r-1-12-1]"
    并且总共报告 1 个 warning

    例子:
      | builtin_type       |
      | char               |
      | unsigned char      |
      | signed char        |
      | short              |
      | unsigned short     |
      | signed short       |
      | int                |
      | unsigned int       |
      | signed int         |
      | long               |
      | unsigned long      |
      | signed long        |
      | long long          |
      | unsigned long long |
      | signed long long   |
