# language: zh-CN
功能: R-1-1-15 禁止函数参数省略类型

  隐式声明的参数类型在 C 语言中是允许的，默认为“int”；但ISO C99 及更高版本以及 C++ 不支持隐式 int。

  @acceptance
  场景: 报告函数参数省略类型
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(x)
        {
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-1-15" 时
    那么应该报告 "test.c:1:10: warning: 禁止函数参数省略类型 [gjb8114-r-1-1-15]"
    并且总共报告 1 个 warning

  @acceptance
  场景: 不报告函数参数显式声明类型
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(int x)
        {
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-1-15" 时
    那么不应该报告任何 warning