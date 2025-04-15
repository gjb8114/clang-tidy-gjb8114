# language: zh-CN
功能: R-1-3-9 禁止使用整型数 0 表示空指针

  @acceptance
  场景: 报告使用整型数 0 表示空指针
    假如有一个源码文件 "test.c"，内容如下:
        """
        #include <stdlib.h>

        void foo(void)
        {
            int *p = (int*) malloc(sizeof(int));
            if (p != 0)
            {
                *p = 0;
            }
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-3-9" 时
    那么应该报告 "test.c:6:14: warning: 禁止使用整型数 0 表示空指针 [gjb8114-r-1-3-9]"
    并且总共报告 1 个 warning

  @acceptance
  场景: 不报告使用 NULL 表示空指针
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
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-3-9" 时
    那么不应该报告任何 warning

  场景: 报告整型数 0 初始化空指针
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(void)
        {
            int *p = 0;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-3-9" 时
    那么应该报告 "test.c:3:14: warning: 禁止使用整型数 0 表示空指针 [gjb8114-r-1-3-9]"
    并且总共报告 1 个 warning

  场景: 报告整型数 0 赋值为空指针
    假如有一个源码文件 "test.c"，内容如下:
        """
        #include <stdlib.h>

        void foo(void)
        {
            int *p = NULL;
            p = 0;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-3-9" 时
    那么应该报告 "test.c:6:9: warning: 禁止使用整型数 0 表示空指针 [gjb8114-r-1-3-9]"
    并且总共报告 1 个 warning

  场景: 报告整型数 0 作为函数参数
    假如有一个源码文件 "test.c"，内容如下:
        """
        void bar(int *x);

        void foo(void)
        {
            bar(0);
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-3-9" 时
    那么应该报告 "test.c:5:9: warning: 禁止使用整型数 0 表示空指针 [gjb8114-r-1-3-9]"
    并且总共报告 1 个 warning

  场景: 报告 C 风格强制类型转换
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(void)
        {
            int *p = (int*)0;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-3-9" 时
    那么应该报告 "test.c:3:20: warning: 禁止使用整型数 0 表示空指针 [gjb8114-r-1-3-9]"
    并且总共报告 1 个 warning

  场景: 报告 C++ 风格强制类型转换
    假如有一个源码文件 "test.cpp"，内容如下:
        """
        void bar(int *x);
        void foo(void)
        {
            bar(static_cast<int*>(0));
        }
        """
    当对 "test.cpp" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-3-9" 时
    那么应该报告 "test.cpp:4:27: warning: 禁止使用整型数 0 表示空指针 [gjb8114-r-1-3-9]"
    并且总共报告 1 个 warning

  场景: 不报告整型数 0 作为普通变量
    假如有一个源码文件 "test.c"，内容如下:
        """
        void bar(int *x);
        void foo(void)
        {
            int i = 0;
            int *p = (int*)i;
            bar(p);
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-3-9" 时
    那么不应该报告任何 warning
