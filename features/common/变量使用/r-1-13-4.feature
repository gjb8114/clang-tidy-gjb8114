# language: zh-CN
功能: R-1-13-4 禁止变量与标识符同名

  @acceptance
  场景: 报告变量与标识符同名
    假如有一个源码文件 "test.c"，内容如下:
        """
        struct A
        {
        };

        void foo(void)
        {
            int A;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-13-4" 时
    那么应该报告 "test.c:7:9: warning: 禁止变量与标识符同名 [gjb8114-r-1-13-4]"
    并且总共报告 1 个 warning

  @acceptance
  场景: 不报告变量与标识符大小写不同
    假如有一个源码文件 "test.c"，内容如下:
        """
        struct A
        {
        };

        void foo(void)
        {
            struct A a;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-13-4" 时
    那么不应该报告任何 warning