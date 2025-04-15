# language: zh-CN
功能: R-1-6-17 禁止释放非最初分配地址的指针

  @acceptance
  场景: 报告释放非最初分配地址的指针
    假如有一个源码文件 "test.c"，内容如下:
        """
        #include <stdlib.h>

        void foo(void)
        {
            char *p = (char *)malloc(1);
            if (p)
            {
                p++;
                free(p);
            }
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-6-17" 时
    那么应该报告 "test.c:9:14: warning: 禁止释放非最初分配地址的指针 [gjb8114-r-1-6-17]"
    并且总共报告 1 个 warning

  @acceptance
  场景: 不报告释放最初分配地址的指针
    假如有一个源码文件 "test.c"，内容如下:
        """
        #include <stdlib.h>

        void foo(void)
        {
            char *p = (char *) malloc(1);
            if (p)
            {
                char* q = p;
                p++;
                free(q);
            }
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-6-17" 时
    那么不应该报告任何 warning

  场景: 报告释放非最初 calloc 分配地址的指针
    假如有一个源码文件 "test.c"，内容如下:
        """
        #include <stdlib.h>

        void foo(void)
        {
            char *p = (char *) calloc(1, 1);
            if (p)
            {
                p++;
                free(p);
            }
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-6-17" 时
    那么应该报告 "test.c:9:14: warning: 禁止释放非最初分配地址的指针 [gjb8114-r-1-6-17]"
    并且总共报告 1 个 warning

  场景: 不报告互斥分支修改指针释放
    假如有一个源码文件 "test.c"，内容如下:
        """
        #include <stdlib.h>

        char* foo(int x)
        {
            char *p = (char *) malloc(1);
            if (x == 1)
            {
                p += 1;
                return p;
            }
            else
            {
                free(p);
                return NULL;
            }
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-6-17" 时
    那么不应该报告任何 warning