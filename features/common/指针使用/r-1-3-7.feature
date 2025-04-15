# language: zh-CN
功能: R-1-3-7 禁止动态分配的指针变量未初始化

  @acceptance
  场景: 报告动态分配的指针变量未初始化
    假如有一个源码文件 "test.c"，内容如下:
        """
        #include <stdlib.h>

        void foo(void)
        {
            int *p;
            p = (int*) malloc(sizeof(int));
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-3-7" 时
    那么应该报告 "test.c:5:10: warning: 禁止动态分配的指针变量未初始化 [gjb8114-r-1-3-7]"
    并且总共报告 1 个 warning

  @acceptance
  场景: 不报告已初始化动态分配的指针变量
    假如有一个源码文件 "test.c"，内容如下:
        """
        #include <stdlib.h>

        void foo(void)
        {
            int *p = NULL;
            p = (int*) malloc(sizeof(int));
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-3-7" 时
    那么不应该报告任何 warning

  场景: 不报告 static 变量
    假如有一个源码文件 "test.c"，内容如下:
        """
        #include <stdlib.h>

        void foo(void)
        {
            static int *p;
            p = (int*) malloc(sizeof(int));
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-3-7" 时
    那么不应该报告任何 warning

  场景: 不报告全局变量
    假如有一个源码文件 "test.c"，内容如下:
        """
        #include <stdlib.h>

        int *the_p;
        void foo(void)
        {
            the_p = (int*) malloc(sizeof(int));
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-3-7" 时
    那么不应该报告任何 warning