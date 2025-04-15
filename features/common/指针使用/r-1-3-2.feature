# language: zh-CN
功能: R-1-3-2 禁止函数隐式转换为指针

  @acceptance
  场景: 报告函数隐式转换为指针
    假如有一个源码文件 "test.c"，内容如下:
        """
        #include <stdlib.h>

        void foo(int x);

        void bar(void)
        {
            if (foo == NULL)
                return;
            void (*p) (int) = foo;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-3-2" 时
    那么应该报告 "test.c:7:9: warning: 禁止函数隐式转换为指针 [gjb8114-r-1-3-2]"
    那么应该报告 "test.c:9:23: warning: 禁止函数隐式转换为指针 [gjb8114-r-1-3-2]"
    并且总共报告 2 个 warning

  @acceptance
  场景: 不报告函数取址为指针
    假如有一个源码文件 "test.c"，内容如下:
        """
        #include <stdlib.h>

        void foo(int x);

        void bar(void)
        {
            if (&foo == NULL)
                return;
            void (*p) (int) = &foo;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-3-2" 时
    那么不应该报告任何 warning