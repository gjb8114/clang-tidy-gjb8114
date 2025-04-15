# language: zh-CN
功能: R-1-1-21 禁止 #include 绝对路径

  @acceptance
  场景: 报告绝对路径
    假如有一个源码文件 "test.c"，内容如下:
        """
        #include "C:\\WindRiver\\vxworks-7\\target\\h\\vxWorks.h"
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-1-21" 时
    那么应该报告 "test.c:1:10: warning: 禁止 #include 绝对路径 [gjb8114-r-1-1-21]"
    并且总共报告 1 个 warning

  @acceptance
  场景: 不报告相对路径
    假如有一个源码文件 "test.c"，内容如下:
        """
        #include "..\\config.h"
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-1-21" 时
    那么不应该报告任何 warning

  场景: 报告 Unix 风格绝对路径
    假如有一个源码文件 "test.c"，内容如下:
        """
        #include "/usr/include/stdio.h"
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-1-21" 时
    那么应该报告 "test.c:1:10: warning: 禁止 #include 绝对路径 [gjb8114-r-1-1-21]"

  场景: 报告 Windows 风格绝对路径
    假如有一个源码文件 "test.c"，内容如下:
        """
        #include "D:/usr/include/stdio.h"
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-1-21" 时
    那么应该报告 "test.c:1:10: warning: 禁止 #include 绝对路径 [gjb8114-r-1-1-21]"

  场景: 不报告系统头文件
    假如有一个源码文件 "test.c"，内容如下:
        """
        #include </usr/include/stdio.h>
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-1-21" 时
    那么不应该报告任何 warning