# language: zh-CN
功能: R-1-1-11 禁止位域未明确符号性

  @acceptance
  场景: 报告位域未明确符号性
    假如有一个源码文件 "test.c"，内容如下:
        """
        struct Foo
        {
            char a : 2;
        };
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-1-11" 时
    那么应该报告 "test.c:3:5: warning: 禁止位域未明确符号性 [gjb8114-r-1-1-11]"
    并且总共报告 1 个 warning

  @acceptance
  场景: 不报告明确有符号性的位域
    假如有一个源码文件 "test.c"，内容如下:
        """
        struct Foo
        {
            signed char a : 2;
        };
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-1-11" 时
    那么不应该报告任何 warning

  场景: 不报告使用 stdint 类型的位域
    假如有一个源码文件 "test.c"，内容如下:
        """
        #include <stdint.h>

        struct Foo
        {
            int8_t    a : 2;
            uint8_t   b : 2;
            uint16_t  c : 12;
        };
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-1-11" 时
    那么不应该报告任何 warning