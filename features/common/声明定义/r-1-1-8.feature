# language: zh-CN
功能: R-1-1-8 禁止匿名结构、联合、枚举

  @acceptance
  场景: 报告匿名结构、联合、枚举
    假如有一个源码文件 "test.c"，内容如下:
        """
        struct {
            int a;
        } foo;

        union {
            char buf[4];
            int a;
        } bar;

        enum {
            A = 0,
        } e;
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-1-8" 时
    那么应该报告 "test.c:1:1: warning: 禁止匿名结构、联合、枚举 [gjb8114-r-1-1-8]"
    并且应该报告 "test.c:5:1: warning: 禁止匿名结构、联合、枚举 [gjb8114-r-1-1-8]"
    并且应该报告 "test.c:10:1: warning: 禁止匿名结构、联合、枚举 [gjb8114-r-1-1-8]"
    并且总共报告 3 个 warning

  @acceptance
  场景: 不报告具名结构、联合、枚举
    假如有一个源码文件 "test.c"，内容如下:
        """
        struct Foo {
            int a;
        } foo;

        union Bar {
            char buf[4];
            int a;
        } bar;

        enum E {
            A = 0,
        } e;
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-1-8" 时
    那么不应该报告任何 warning

  场景: 报告匿名结构体
    假如有一个源码文件 "test.c"，内容如下:
        """
        struct { int a; };
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-1-8" 时
    那么应该报告 "test.c:1:1: warning: 禁止匿名结构、联合、枚举 [gjb8114-r-1-1-8]"
    并且总共报告 1 个 warning