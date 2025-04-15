# language: zh-CN
功能: R-1-3-6 禁止释放指针变量后未置空

  @acceptance
  场景: 报告释放后未置空
    假如有一个源码文件 "test.c"，内容如下:
        """
        #include <stdlib.h>

        void foo(void)
        {
            int *p = (int*) malloc(sizeof(int));
            if (p) {
                free(p);
            }
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-3-6" 时
    那么应该报告 "test.c:7:9: warning: 禁止释放指针变量后未置空 [gjb8114-r-1-3-6]"
    并且总共报告 1 个 warning

  @acceptance
  场景: 不报告释放后置空
    假如有一个源码文件 "test.c"，内容如下:
        """
        #include <stdlib.h>

        void foo(void)
        {
            int *p = (int*) malloc(sizeof(int));
            if (p) {
                free(p);
                p = NULL;
            }
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-3-6" 时
    那么不应该报告任何 warning

  场景: 报告 delete 后未置空
    假如有一个源码文件 "test.cpp"，内容如下:
        """
        #include <iostream>

        void foo()
        {
            int *p = new int(1);
            if (p) {
                delete p;
            }
        }
        """
    当对 "test.cpp" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-3-6" 时
    那么应该报告 "test.cpp:7:9: warning: 禁止释放指针变量后未置空 [gjb8114-r-1-3-6]"
    并且总共报告 1 个 warning

  场景: 报告多次 delete 后未置空
    假如有一个源码文件 "test.cpp"，内容如下:
        """
        #include <iostream>

        void foo()
        {
            int *p = new int(1);
            if (NULL != p) {
                delete p;
                p = nullptr;
                p = new int(2);
                delete p;
            }
        }
        """
    当对 "test.cpp" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-3-6" 时
    那么应该报告 "test.cpp:10:9: warning: 禁止释放指针变量后未置空 [gjb8114-r-1-3-6]"
    并且总共报告 1 个 warning

  场景: 报告 delete 后在互斥分支置空
    假如有一个源码文件 "test.cpp"，内容如下:
        """
        #include <iostream>

        void foo()
        {
            int *p = new int(1);
            if (NULL != p) {
                delete p;
            } else {
                p = nullptr;
            }
        }
        """
    当对 "test.cpp" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-3-6" 时
    那么应该报告 "test.cpp:7:9: warning: 禁止释放指针变量后未置空 [gjb8114-r-1-3-6]"
    并且总共报告 1 个 warning
