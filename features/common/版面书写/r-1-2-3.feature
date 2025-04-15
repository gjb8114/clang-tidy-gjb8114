# language: zh-CN
功能: R-1-2-3 禁止头文件包含出现在可执行代码之后

  背景:
    假如有一个源码文件 "foo.h"，内容如下:
        """
        """

  @acceptance
  场景: 报告头文件包含出现在可执行代码之后
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(void)
        {
            #include "foo.h"
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-2-3" 时
    那么应该报告 "test.c:3:5: warning: 禁止头文件包含出现在可执行代码之后 [gjb8114-r-1-2-3]"
    并且总共报告 1 个 warning

  @acceptance
  场景: 不报告头文件包含出现在可执行代码之前
    假如有一个源码文件 "test.c"，内容如下:
        """
        #include "foo.h"

        void foo(void)
        {
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-2-3" 时
    那么不应该报告任何 warning

  场景: 不报告字符串中的 include 关键字
    假如有一个源码文件 "test.c"，内容如下:
        """
        #include "foo.h"

        void foo(void)
        {
            const char *c = "#include directive";
            char buffer[100] = "#include <stdio.h>";
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-2-3" 时
    那么不应该报告任何 warning