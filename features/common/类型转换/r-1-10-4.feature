# language: zh-CN
功能: R-1-10-4 禁止指针变量的赋值类型与指针变量类型不一致

  @acceptance
  场景: 报告指针变量赋值类型不一致
    假如有一个源码文件 "test.c"，内容如下:
        """
        #include <stdlib.h>

        void foo(char *buf)
        {
            unsigned char *bytes = NULL;
            bytes = buf;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-10-4" 时
    那么应该报告 "test.c:6:13: warning: 禁止指针变量的赋值类型与指针变量类型不一致 [gjb8114-r-1-10-4]"
    并且总共报告 1 个 warning

  @acceptance
  场景: 不报告指针变量赋值类型一致
    假如有一个源码文件 "test.c"，内容如下:
        """
        #include <stdlib.h>

        void foo(unsigned char *buf)
        {
            unsigned char *bytes = NULL;
            bytes = buf;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-10-4" 时
    那么不应该报告任何 warning

  场景大纲: 不报告空指针赋值
    假如有一个源码文件 "test.<lang>"，内容如下:
        """
        #include <stdlib.h>

        void foo(void)
        {
            unsigned int *ptr = NULL;
            ptr = <nil>;
        }
        """
    当对 "test.<lang>" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-10-4" 时
    那么不应该报告任何 warning

    例子:
      | lang | nil     |
      | c    | NULL    |
      | cxx  | nullptr |
      | cxx  | NULL    |