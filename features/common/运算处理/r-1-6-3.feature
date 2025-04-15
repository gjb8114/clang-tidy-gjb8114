# language: zh-CN
功能: R-1-6-3 禁止将赋值语句作为逻辑表达式

  @acceptance
  场景: 报告 if 语句中的赋值语句
    假如有一个源码文件 "test.c"，内容如下:
        """
        #include <stdio.h>

        void foo(void)
        {
            FILE *fp;
            if (fp = fopen("test.txt", "r"))
            {
            }
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-6-3" 时
    那么应该报告 "test.c:6:12: warning: 禁止将赋值语句作为逻辑表达式 [gjb8114-r-1-6-3]"
    并且总共报告 1 个 warning

  @acceptance
  场景: 不报告 if 语句中隐式转换的逻辑表达式
    假如有一个源码文件 "test.c"，内容如下:
        """
        #include <stdio.h>

        void foo(void)
        {
            FILE *fp = fopen("test.txt", "r");
            if (fp)
            {
            }
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-6-3" 时
    那么不应该报告任何 warning

  场景: 报告 while 循环中的赋值语句
    假如有一个源码文件 "test.c"，内容如下:
        """
        #include <stdio.h>

        void foo(FILE* fp)
        {
            int r;
            char c;
            while (r = fread(&c, 1, 1, fp))
            {
            }
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-6-3" 时
    那么应该报告 "test.c:7:14: warning: 禁止将赋值语句作为逻辑表达式 [gjb8114-r-1-6-3]"
    并且总共报告 1 个 warning

  场景: 报告 for 循环中的赋值语句
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(void)
        {
            int i;
            for (i = 0; i = 1; i++)
            {
            }
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-6-3" 时
    那么应该报告 "test.c:4:19: warning: 禁止将赋值语句作为逻辑表达式 [gjb8114-r-1-6-3]"
    并且总共报告 1 个 warning