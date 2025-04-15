# language: zh-CN
功能: R-1-7-5 禁止函数声明和定义的参数类型不一致

  重合: #R-1-1-14

  @acceptance
  场景: 报告函数声明和定义的参数类型不一致
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(int x);

        void foo(unsigned x)
        {
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-7-5" 时
    那么应该报告 "test.c:1:10: warning: 禁止函数声明和定义的参数类型不一致 [gjb8114-r-1-7-5]"
    并且总共报告 1 个 warning

  @acceptance
  场景: 不报告函数声明和定义的参数类型一致
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(int x);

        void foo(int x)
        {
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-7-5" 时
    那么不应该报告任何 warning