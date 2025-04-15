# language: zh-CN
功能: R-1-6-16 禁止使用已释放的内存

  重合: #R-1-3-5 《禁止使用或释放未分配或已释放的指针》有某种程度上重复，做了详细界定

  @acceptance
  场景: 报告已释放的内存使用
    假如有一个源码文件 "test.c"，内容如下:
        """
        #include <stdlib.h>

        void foo(void)
        {
            char c, *p = (char*) malloc(1);
            if (p)
            {
                free(p);
                c = *p;
            }
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-6-16" 时
    那么应该报告 "test.c:9:14: warning: 禁止使用已释放的内存 [gjb8114-r-1-6-16]"
    并且总共报告 1 个 warning

  @acceptance
  场景: 不报告释放前的使用
    假如有一个源码文件 "test.c"，内容如下:
        """
        #include <stdlib.h>

        void foo(void)
        {
            char *p = (char*) malloc(1);
            if (p)
            {
                *p = 1;
                free(p);
            }
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-6-16" 时
    那么不应该报告任何 warning

  场景: 报告已释放的指针下标引用
    假如有一个源码文件 "test.c"，内容如下:
        """
        #include <stdlib.h>

        void foo(void)
        {
            int *p = (int*) malloc(sizeof(int));
            if (p) {
                free(p);
                p[0] = 1;
            }
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-6-16" 时
    那么应该报告 "test.c:8:9: warning: 禁止使用已释放的内存 [gjb8114-r-1-6-16]"
    并且总共报告 1 个 warning

  场景: 不报告释放后重新分配的指针使用
    假如有一个源码文件 "test.c"，内容如下:
        """
        #include <stdlib.h>

        int foo(void)
        {
            int *p = (int*) malloc(sizeof(int));
            free(p);
            p = (int*) malloc(sizeof(int));
            p[0] = 1;
            return *p;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-6-16" 时
    那么不应该报告任何 warning

  场景: 报告 C++ 已释放的指针使用
    假如有一个源码文件 "test.cpp"，内容如下:
        """
        int foo()
        {
            int *p = new int;
            delete p;
            p[0] = 1;
            return *p;
        }
        """
    当对 "test.cpp" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-6-16" 时
    那么应该报告 "test.cpp:5:5: warning: 禁止使用已释放的内存 [gjb8114-r-1-6-16]"
    那么应该报告 "test.cpp:6:13: warning: 禁止使用已释放的内存 [gjb8114-r-1-6-16]"
    并且总共报告 2 个 warning

  场景: 报告 C++ 已释放的数组指针使用
    假如有一个源码文件 "test.cpp"，内容如下:
        """
        int foo()
        {
            int *p = new int[10];
            p[0] = 1;
            delete[] p;
            p[3] = 3;
            return *p;
        }
        """
    当对 "test.cpp" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-6-16" 时
    那么应该报告 "test.cpp:6:5: warning: 禁止使用已释放的内存 [gjb8114-r-1-6-16]"
    那么应该报告 "test.cpp:7:13: warning: 禁止使用已释放的内存 [gjb8114-r-1-6-16]"
    并且总共报告 2 个 warning

  场景: 不报告不同分支被释放的指针使用
    假如有一个源码文件 "test.c"，内容如下:
        """
        #include <stdlib.h>

        int* foo(int sw)
        {
            int *p = (int*) malloc(sizeof(int));
            if (sw == 3) {
                free(p);
                return NULL;
            } else {
                *p = 1;
            }
            return p;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-6-16" 时
    那么不应该报告任何 warning

