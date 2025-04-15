# language: zh-CN
功能: R-1-3-1 禁止多级指针

  多级指针是三级与三级以上的指针。多级指针的使用会增加代码的复杂性和可读性问题，因此不推荐使用。

  @acceptance
  场景: 报告多级指针
    假如有一个源码文件 "test.c"，内容如下:
        """
        #include <stdlib.h>

        void fun(void)
        {
            int ***ppptr = NULL;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-3-1" 时
    那么应该报告 "test.c:5:12: warning: 禁止多级指针 [gjb8114-r-1-3-1]"
    并且总共报告 1 个 warning

  场景: 报告参数类型多级指针
    假如有一个源码文件 "test.c"，内容如下:
        """
        void fun(int ***ppptr)
        {
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-3-1" 时
    那么应该报告 "test.c:1:17: warning: 禁止多级指针 [gjb8114-r-1-3-1]"
    并且总共报告 1 个 warning

  场景: 报告结构体成员多级指针
    假如有一个源码文件 "test.c"，内容如下:
        """
        struct Foo
        {
            unsigned int ***ppptr;
        };
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-3-1" 时
    那么应该报告 "test.c:3:21: warning: 禁止多级指针 [gjb8114-r-1-3-1]"
    并且总共报告 1 个 warning

  场景: 报告类成员多级指针
    假如有一个源码文件 "test.cpp"，内容如下:
        """
        class Foo
        {
            int ***ppptr;
        public:
            Foo() : ppptr(nullptr) {}
            static char ****pppptr;
        };
        """
    当对 "test.cpp" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-3-1" 时
    那么应该报告 "test.cpp:3:12: warning: 禁止多级指针 [gjb8114-r-1-3-1]"
    并且应该报告 "test.cpp:6:21: warning: 禁止多级指针 [gjb8114-r-1-3-1]"
    并且总共报告 2 个 warning
