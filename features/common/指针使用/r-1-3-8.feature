# language: zh-CN
功能: R-1-3-8 禁止动态分配的指针变量未检查即使用

  @acceptance
  场景: 报告未检查即使用
    假如有一个源码文件 "test.c"，内容如下:
        """
        #include <stdlib.h>

        void foo(void)
        {
            int *p = (int*) malloc(sizeof(int));
            *p = 1;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-3-8" 时
    那么应该报告 "test.c:6:5: warning: 禁止动态分配的指针变量未检查即使用 [gjb8114-r-1-3-8]"
    并且总共报告 1 个 warning

  @acceptance
  场景: 不报告判别 NULL 后使用
    假如有一个源码文件 "test.c"，内容如下:
        """
        #include <stdlib.h>

        void foo(void)
        {
            int *p = (int*) malloc(sizeof(int));
            if (p != NULL)
            {
                *p = 1;
            }
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-3-8" 时
    那么不应该报告任何 warning

  场景: 不报告反复分配后未使用且未判别
    假如有一个源码文件 "test.c"，内容如下:
        """
        #include <stdlib.h>
        void foo(void)
        {
            int *p = NULL;
            p = (int*) malloc(sizeof(int));
            p = (int*) malloc(sizeof(int));
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-3-8" 时
    那么不应该报告任何 warning

  场景: 报告使用后再检查
    假如有一个源码文件 "test.cxx"，内容如下:
        """
        #include <stdlib.h>

        void foo(void)
        {
            int *p = (int*) malloc(sizeof(int));
            *p = 1;
            if (p == nullptr)
            {
                return;
            }
        }
        """
    当对 "test.cxx" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-3-8" 时
    那么应该报告 "test.cxx:6:5: warning: 禁止动态分配的指针变量未检查即使用 [gjb8114-r-1-3-8]"
    并且总共报告 1 个 warning

  场景: 不报告未使用的分配
    假如有一个源码文件 "test.c"，内容如下:
        """
        #include <stdlib.h>

        void foo(void)
        {
            int *p = (int*) malloc(sizeof(int));
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-3-8" 时
    那么不应该报告任何 warning

  场景: 报告全局变量未检查
    假如有一个源码文件 "test.c"，内容如下:
        """
        #include <stdlib.h>

        int *p = NULL;
        void foo(void)
        {
            p = (int*) malloc(sizeof(int));
            *p = 1;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-3-8" 时
    那么应该报告 "test.c:7:5: warning: 禁止动态分配的指针变量未检查即使用 [gjb8114-r-1-3-8]"
    并且总共报告 1 个 warning

  场景: 一个变量只报告一次
    假如有一个源码文件 "test.c"，内容如下:
        """
        #include <stdlib.h>
        void foo(void)
        {
            int *pa = NULL;
            pa = (int*) malloc(sizeof(int) * 2);
            int *pb = (int*) malloc(sizeof(int) * 2);
            pa[0] = 1;
            pa[1] = 2;
            pb[0] = 3;
            pb[1] = 4;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-3-8" 时
    那么应该报告 "test.c:7:5: warning: 禁止动态分配的指针变量未检查即使用 [gjb8114-r-1-3-8]"
    并且应该报告 "test.c:9:5: warning: 禁止动态分配的指针变量未检查即使用 [gjb8114-r-1-3-8]"
    并且总共报告 2 个 warning

  场景: 报告 calloc 分配未检查
    假如有一个源码文件 "test.c"，内容如下:
        """
        #include <stdlib.h>

        void foo(void)
        {
            int *p = NULL;
            p = (int*) calloc(1, sizeof(int));
            p[0] = 1;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-3-8" 时
    那么应该报告 "test.c:7:5: warning: 禁止动态分配的指针变量未检查即使用 [gjb8114-r-1-3-8]"
    并且总共报告 1 个 warning

  场景: 报告 realloc 分配未检查
    假如有一个源码文件 "test.c"，内容如下:
        """
        #include <stdlib.h>

        void foo(void)
        {
            int *p = NULL;
            p = (int*) calloc(1, sizeof(int));
            if (p == NULL)
                return;
            p[0] = 1;
            p = (int*) realloc(p, sizeof(int) * 2);
            p[1] = 2;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-3-8" 时
    那么应该报告 "test.c:11:5: warning: 禁止动态分配的指针变量未检查即使用 [gjb8114-r-1-3-8]"
    并且总共报告 1 个 warning

  场景: 不报告 C++ 简写检查
    假如有一个源码文件 "test.cxx"，内容如下:
        """
        #include <stdlib.h>

        void foo()
        {
            int *p = (int*) malloc(sizeof(int));
            if (!p)
                return;
            p[0] = 1;
        }

        void bar()
        {
            int *p = (int*) malloc(sizeof(int));
            if (p) p[0] = 1;
        }

        void func()
        {
            int *p = (int*) malloc(sizeof(int));
            bool failed = !p;
            if (!failed) p[0] = 1;
        }

        void explict_cast_func()
        {
            int *p = (int*) malloc(sizeof(int));
            if (static_cast<bool>(p)) p[0] = 1;
        }
        """
    当对 "test.cxx" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-3-8" 时
    那么不应该报告任何 warning

  场景: 不报告 C 简写检查
    假如有一个源码文件 "test.c"，内容如下:
        """
        #include <stdlib.h>
        #include <stdbool.h>

        void foo(void)
        {
            int *p = (int*) malloc(sizeof(int));
            if (!p)
                return;
            p[0] = 1;
        }

        void bar(void)
        {
            int *p = (int*) malloc(sizeof(int));
            if (p) {
                p[0] = 1;
            }
        }

        void func(void)
        {
            int *p = (int*) malloc(sizeof(int));
            bool good = p;
            if (good) p[0] = 1;
        }

        void explict_cast_func(void)
        {
            int *p = (int*) malloc(sizeof(int));
            if ((bool)p) p[0] = 1;
        }

        void double_negative_func(void)
        {
            int *p = (int*) malloc(sizeof(int));
            if (!!p) p[0] = 1;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-3-8" 时
    那么不应该报告任何 warning
