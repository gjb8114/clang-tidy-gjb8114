# language: zh-CN
功能: R-1-1-22 禁止重复包含头文件

  @acceptance
  场景: 报告头文件重复包含
    假如有一个源码文件 "foo.h"，内容如下:
        """
        int the_var;
        """
    并且有一个源码文件 "bar.h"，内容如下:
        """
        #include "foo.h"
        """
    并且有一个源码文件 "test.c"，内容如下:
        """
        #include "bar.h"
        #include "foo.h"
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-1-22" 时
    那么应该报告 "test.c:2:10: warning: 禁止重复包含头文件 [gjb8114-r-1-1-22]"
    并且总共报告 1 个 warning

  @acceptance
  场景: 不报告头文件重复包含
    假如有一个源码文件 "foo.h"，内容如下:
        """
        int the_var;
        """
    并且有一个源码文件 "bar.h"，内容如下:
        """
        #ifndef _FOO_H_
        #define _FOO_H_
        #include "foo.h"
        #endif
        """
    并且有一个源码文件 "test.c"，内容如下:
        """
        #include "bar.h"
        #ifndef _FOO_H_
        #define _FOO_H_
        #include "foo.h"
        #endif
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-1-22" 时
    那么不应该报告任何 warning

  场景: 报告头文件重复包含两次
    假如有一个源码文件 "foo.h"，内容如下:
        """
        int the_var;
        """
    并且有一个源码文件 "test.c"，内容如下:
        """
        #include "foo.h"
        #include "foo.h"
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-1-22" 时
    那么应该报告 "test.c:2:10: warning: 禁止重复包含头文件 [gjb8114-r-1-1-22]"
    并且总共报告 1 个 warning