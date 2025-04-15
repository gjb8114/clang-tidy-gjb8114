# language: zh-CN
功能: R-1-6-15 禁止缓存区写入越界

  @acceptance
  场景: 报告缓存区写入越界
    假如有一个源码文件 "test.c"，内容如下:
        """
        #include <string.h>

        void foo(void)
        {
            char src[2] = {1};
            char dst[1];
            memcpy(dst, src, sizeof(src));
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-6-15" 时
    那么应该报告 "test.c:7:22: warning: 禁止缓存区写入越界 [gjb8114-r-1-6-15]"
    并且总共报告 1 个 warning

  @acceptance
  场景: 不报告缓存区写入未越界
    假如有一个源码文件 "test.c"，内容如下:
        """
        #include <string.h>

        void foo(void)
        {
            int src[4] = {1, 2, 3, 4};
            int des[4] = {0, 0};
            memcpy(des, src, sizeof(des));
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-6-15" 时
    那么不应该报告任何 warning