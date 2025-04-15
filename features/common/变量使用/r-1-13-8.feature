# language: zh-CN
功能: R-1-13-8 禁止仅大小写区分的变量

  @acceptance
  场景: 报告同一作用域内大小写区分的变量
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(void)
        {
            int var_A;
            int var_a;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-13-8" 时
    那么应该报告 "test.c:4:9: warning: 禁止仅大小写区分的变量 [gjb8114-r-1-13-8]"
    并且总共报告 1 个 warning

  场景: 报告与全局变量冲突的大小写区分变量
    假如有一个源码文件 "test.c"，内容如下:
        """
        int var_A;
        void foo(void)
        {
            int var_a;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-13-8" 时
    那么应该报告 "test.c:4:9: warning: 禁止仅大小写区分的变量 [gjb8114-r-1-13-8]"
    并且总共报告 1 个 warning

  场景: 报告多个字母大小写区分的变量
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(void)
        {
            int Var_A;
            int var_a;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-13-8" 时
    那么应该报告 "test.c:4:9: warning: 禁止仅大小写区分的变量 [gjb8114-r-1-13-8]"
    并且总共报告 1 个 warning

  场景: 不报告不同作用域内的大小写区分变量
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(void)
        {
            int var_a;
        }
        void bar(void)
        {
            int var_A;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-13-8" 时
    那么不应该报告任何 warning