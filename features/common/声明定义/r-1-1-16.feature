# language: zh-CN
@skip
# 注意：考虑到这个规则跨越了多个 Translation Unit，所以这个规则的检查需要在链接阶段进行。
# 而 clang-tidy 是在编译阶段进行检查的，分析的是单个 TU 的 AST。
功能: R-1-1-16 禁止外部声明变量的类型与定义不一致

  @acceptance
  场景: 报告外部声明的变量类型与定义不一致
    假如有一个源码文件 "data.c"，内容如下:
        """
        int the_foo = 0;
        """
    并且有一个源码文件 "test.c"，内容如下:
        """
        extern float the_foo;
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-1-16" 时
    那么应该报告 "test.c:1:14: warning: 禁止外部声明变量的类型与定义不一致 [gjb8114-r-1-1-16]"
    并且总共报告 1 个 warning

  @acceptance
  场景: 不报告外部声明的变量类型与定义一致
    假如有一个源码文件 "data.c"，内容如下:
        """
        int the_foo = 0;
        """
    并且有一个源码文件 "test.c"，内容如下:
        """
        extern int the_foo;
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-1-16" 时
    那么不应该报告任何警告