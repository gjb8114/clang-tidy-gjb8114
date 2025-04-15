# language: zh-CN
功能: R-1-8-4 禁止使用八进制数

  例外: 八进制数加以 `/* octal */` 注释说明

  @acceptance
  场景: 报告使用八进制数
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(void)
        {
            int i = 010;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-8-4" 时
    那么应该报告 "test.c:3:13: warning: 禁止使用八进制数 [gjb8114-r-1-8-4]"
    并且总共报告 1 个 warning

  @acceptance
  场景: 不报告使用八进制数的注释
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(void)
        {
            int i = 010; /* octal */
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-8-4" 时
    那么不应该报告任何 warning