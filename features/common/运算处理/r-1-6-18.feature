# language: zh-CN
功能: R-1-6-18 禁止使用 gets 函数

  推荐使用 fgets 或者其他安全函数替代 gets 函数

  @acceptance
  场景: 报告使用 gets 函数
    假如有一个源码文件 "test.c"，内容如下:
        """
        #include <stdio.h>

        void foo(void)
        {
            char buffer[1024];
            gets(buffer);
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-6-18" 时
    那么应该报告 "test.c:6:5: warning: 禁止使用 gets 函数 [gjb8114-r-1-6-18]"
    并且总共报告 1 个 warning

  @acceptance
  场景: 不报告使用 fgets 函数
    假如有一个源码文件 "test.c"，内容如下:
        """
        #include <stdio.h>

        void bar(void)
        {
            char buffer[1024];
            fgets(buffer, sizeof(buffer), stdin);
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-6-18" 时
    那么不应该报告任何 warning

  场景: 报告函数指针形式使用 gets 函数
    假如有一个源码文件 "test.c"，内容如下:
        """
        #include <stdio.h>

        void baz(void)
        {
            char buffer[1024];
            char* (*fp)(char*) = gets;
            fp(buffer);
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-6-18" 时
    那么应该报告 "test.c:6:26: warning: 禁止使用 gets 函数 [gjb8114-r-1-6-18]"
    并且总共报告 1 个 warning