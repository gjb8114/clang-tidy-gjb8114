# language: zh-CN
功能: R-1-6-14 禁止缓存区读取越界

  @acceptance
  场景: 报告缓存区读取越界
    假如有一个源码文件 "test.c"，内容如下:
        """
        #include <string.h>

        void foo(void)
        {
            char src[1] = {1};
            char dst[2];
            memcpy(dst, src, sizeof(dst));
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-6-14" 时
    那么应该报告 "test.c:7:22: warning: 禁止缓存区读取越界 [gjb8114-r-1-6-14]"
    并且总共报告 1 个 warning

  @acceptance
  场景: 不报告缓存区读取未越界
    假如有一个源码文件 "test.c"，内容如下:
        """
        #include <string.h>

        void foo(void)
        {
            char src[1] = {1};
            char dst[2];
            memcpy(dst, src, sizeof(src));
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-6-14" 时
    那么不应该报告任何 warning

  场景: 不报告丢失长度的数组
    假如有一个源码文件 "test.c"，内容如下:
        """
        #include <string.h>

        void foo(char src[])
        {
            char dst[] = {0, 0};
            memcpy(dst, src, sizeof(dst));
        }

        void bar(void)
        {
            char src[1] = {1};
            foo(src);
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-6-14" 时
    那么不应该报告任何 warning

  场景: 报告字面量的长度溢出
    假如有一个源码文件 "test.c"，内容如下:
        """
        #include <string.h>

        void foo(void)
        {
            char src[2] = {1, 2};
            char dst[7] = {0};
            memcpy(dst, src, 7);
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-6-14" 时
    那么应该报告 "test.c:7:22: warning: 禁止缓存区读取越界 [gjb8114-r-1-6-14]"
    并且总共报告 1 个 warning

  场景: 报告字面量运算表达式的长度溢出
    假如有一个源码文件 "test.c"，内容如下:
        """
        #include <string.h>

        void foo(void)
        {
            char src[2] = {1, 2};
            char dst[4] = {0};
            memcpy(dst, src, 3 * sizeof(char));
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-6-14" 时
    那么应该报告 "test.c:7:22: warning: 禁止缓存区读取越界 [gjb8114-r-1-6-14]"
    并且总共报告 1 个 warning
