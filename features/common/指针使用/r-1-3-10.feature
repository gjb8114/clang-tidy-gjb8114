# language: zh-CN
功能: R-1-3-10 禁止文件指针未关闭

  @acceptance
  场景: 报告未关闭文件指针
    假如有一个源码文件 "test.c"，内容如下:
        """
        #include <stdio.h>
        
        void foo(void)
        {
            FILE *file = fopen("file.txt", "r");
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-3-10" 时
    那么应该报告 "test.c:5:11: warning: 禁止文件指针未关闭 [gjb8114-r-1-3-10]"
    并且总共报告 1 个 warning

  @acceptance
  场景: 不报告已关闭文件指针
    假如有一个源码文件 "test.c"，内容如下:
        """
        #include <stdio.h>

        void foo(void)
        {
            FILE *file = fopen("file.txt", "r");
            fclose(file);
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-3-10" 时
    那么不应该报告任何 warning

  场景: 报告赋值时打开文件
    假如有一个源码文件 "test.c"，内容如下:
        """
        #include <stdio.h>

        void foo(void)
        {
            FILE *file;
            file = fopen("file.txt", "r");
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-3-10" 时
    那么应该报告 "test.c:6:5: warning: 禁止文件指针未关闭 [gjb8114-r-1-3-10]"
    并且总共报告 1 个 warning

  场景: 报告在互斥分支关闭文件
    假如有一个源码文件 "test.c"，内容如下:
        """
        #include <stdio.h>

        void foo(void)
        {
            FILE *file = NULL;
            if (!file)
            {
                file = fopen("file.txt", "r");
            }
            else
            {
                fclose(file);
            }
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-3-10" 时
    那么应该报告 "test.c:8:9: warning: 禁止文件指针未关闭 [gjb8114-r-1-3-10]"
    并且总共报告 1 个 warning