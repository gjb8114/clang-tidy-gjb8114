# language: zh-CN
功能: R-1-6-19 禁止字符串函数操作导致目标存储空间越界

  @acceptance
  场景: 报告字符串函数操作导致目标存储空间越界
    假如有一个源码文件 "test.c"，内容如下:
        """
        #include <string.h>
        #include <stdio.h>

        void foo(void)
        {
            char str[2] = {0};
            strncpy(str, "foo", 3);
            strncat(str, "()", 2);
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-6-19" 时
    那么应该报告 "test.c:7:25: warning: 禁止字符串函数操作导致目标存储空间越界 [gjb8114-r-1-6-19]"
    # 暂不支持上下文跟踪 buffer 使用情况
    #并且应该报告 "test.c:10:31: warning: 禁止字符串函数操作导致目标存储空间越界 [gjb8114-r-1-6-19]"
    #并且总共报告 2 个 warning

  @acceptance
  场景: 不报告字符串函数操作导致目标存储空间越界
    假如有一个源码文件 "test.c"，内容如下:
        """
        #include <string.h>
        #include <stdio.h>

        void foo(void)
        {
            char str[6] = {0};
            strncpy(str, "foo", 3);
            strncat(str, "()", 2);
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-6-19" 时
    那么不应该报告任何 warning