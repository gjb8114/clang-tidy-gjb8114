# language: zh-CN
功能: R-1-10-5 禁止指针与非指针变量之间赋值

  例外: 指针与非指针变量之间的经过强制转换

  @acceptance
  场景: 报告指针与非指针变量之间的直接赋值
    假如有一个源码文件 "test.c"，内容如下:
        """
        #include <stdint.h>

        void foo(int x)
        {
            uintptr_t addr;
            addr = &x;
            int *p;
            p = addr;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-10-5" 时
    那么应该报告 "test.c:6:12: warning: 禁止指针与非指针变量之间赋值 [gjb8114-r-1-10-5]"
    并且应该报告 "test.c:8:9: warning: 禁止指针与非指针变量之间赋值 [gjb8114-r-1-10-5]"
    并且总共报告 2 个 warning

  @acceptance
  场景: 不报告指针与非指针变量之间的强制转换赋值
    假如有一个源码文件 "test.c"，内容如下:
        """
        #include <stdint.h>

        void foo(int x)
        {
            uintptr_t addr;
            addr = (uintptr_t)&x;
            int *p;
            p = (int*)addr;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-10-5" 时
    那么不应该报告任何 warning

  场景大纲: 不报告空指针赋值
    假如有一个源码文件 "test.<lang>"，内容如下:
        """
        #include <stdlib.h>

        void foo(void)
        {
            int *ptr = <nil>;
        }
        """
    当对 "test.<lang>" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-10-5" 时
    那么不应该报告任何 warning

    例子:
      | lang | nil     |
      | c    | NULL    |
      | cxx  | nullptr |
      | cxx  | NULL    |