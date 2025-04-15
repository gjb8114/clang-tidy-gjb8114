# language: zh-CN
功能: R-1-1-20 禁止用于数值计算的字符型变量未定义符号性

  @acceptance
  场景: 报告未定义符号性的字符型变量
    假如有一个源码文件 "test.c"，内容如下:
        """
        int foo(void)
        {
            char i = 0;
            return i + 1;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-1-20" 时
    那么应该报告 "test.c:3:5: warning: 禁止用于数值计算的字符型变量未定义符号性 [gjb8114-r-1-1-20]"
    并且总共报告 1 个 warning

  @acceptance
  场景: 不报告已定义符号性的字符型变量
    假如有一个源码文件 "test.c"，内容如下:
        """
        int foo(void)
        {
            signed char i = 0;
            return i + 1;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-1-20" 时
    那么不应该报告任何 warning

  场景大纲: 报告全局变量未定义符号性
    假如有一个源码文件 "test.c"，内容如下:
        """
        char i = 0xFF;
        int main(void)
        {
            int j = <arithmetic expr>;
            return j;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-1-20" 时
    那么应该报告 "test.c:1:1: warning: 禁止用于数值计算的字符型变量未定义符号性 [gjb8114-r-1-1-20]"
    并且总共报告 1 个 warning

    例子:
      | arithmetic expr |
      | i + 1           |
      | i - 1           |
      | i * 2           |
      | i / 2           |
      | i % 2           |
      | 1 + i           |
      | 1 - i           |
      | 2 * i           |
      | 2 / i           |
      | 2 % i           |
      | i++             |
      | i--             |
      | ++i             |
      | --i             |
