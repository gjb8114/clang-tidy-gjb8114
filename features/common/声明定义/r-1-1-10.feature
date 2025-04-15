# language: zh-CN
功能: R-1-1-10 禁止有符号位域的位宽等于 1

  @acceptance
  场景: 报告有符号位域的位宽等于 1
    假如有一个源码文件 "test.c"，内容如下:
        """
        struct Foo
        {
            signed a : 1;
        };
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-1-10" 时
    那么应该报告 "test.c:3:16: warning: 禁止有符号位域的位宽等于 1 [gjb8114-r-1-1-10]"
    并且总共报告 1 个 warning

  @acceptance
  场景: 不报告有符号位域的位宽大于 1
    假如有一个源码文件 "test.c"，内容如下:
        """
        struct Foo
        {
            signed a : 2;
        };
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-1-10" 时
    那么不应该报告任何 warning

  场景: 不报告无符号位域的位宽等于 1
    假如有一个源码文件 "test.c"，内容如下:
        """
        struct Foo
        {
            unsigned a : 1;
        };
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-1-10" 时
    那么不应该报告任何 warning