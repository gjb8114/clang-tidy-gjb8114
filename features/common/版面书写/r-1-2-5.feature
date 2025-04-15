# language: zh-CN
功能: R-1-2-5 禁止比较表达式中的运算项未使用括号

  在比较表达式中，运算项未使用括号可能导致二义性和错误的逻辑判断。
  例如，表达式 "x & y == z" 可能被误解为 "(x & y) == z" 或 "x & (y == z)"。
  为了避免这种情况，建议在比较表达式中使用括号来明确运算顺序。

  @acceptance
  场景: 报告运算项未使用括号
    假如有一个源码文件 "test.c"，内容如下:
        """
        int is_odd(unsigned x)
        {
            return x & 0x01 == 0x01;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-2-5" 时
    那么应该报告 "test.c:3:12: warning: 禁止比较表达式中的运算项未使用括号 [gjb8114-r-1-2-5]"
    并且总共报告 1 个 warning

  @acceptance
  场景: 不报告运算项使用括号
    假如有一个源码文件 "test.c"，内容如下:
        """
        int is_odd(unsigned x)
        {
            return (x & 0x01) == 0x01;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-2-5" 时
    那么不应该报告任何 warning

  场景大纲: 报告运算项未使用括号
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(int x, int y, int z)
        {
            if (<expr>)
            {
            }
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-2-5" 时
    那么应该报告 "test.c:3:9: warning: 禁止比较表达式中的运算项未使用括号 [gjb8114-r-1-2-5]"
    并且总共报告 1 个 warning

    例子:
      | expr        |
      | x & y == z  |
      | x \| y == z |
      | x ^ y == z  |
      | x & y != z  |
      | x << 2 > y  |
      | x >> 1 < z  |
      | x + y == z  |

  场景大纲: 不报告运算项使用括号
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(int x, int y, int z)
        {
            if (<expr>)
            {
            }
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-2-5" 时
    那么不应该报告任何 warning

    例子:
      | expr          |
      | (x & y) == z  |
      | (x \| y) == z |
      | (x ^ y) == z  |
      | (x & y) != z  |
      | (x << 2) > y  |
      | (x >> 1) < z  |
      | (x + y) == z  |