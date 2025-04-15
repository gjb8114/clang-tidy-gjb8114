# language: zh-CN
功能: R-1-13-16 禁止字符串数组未以 '\0' 结束

  @acceptance
  场景: 报告字符串数组未以 '\0' 结束
    假如有一个源码文件 "test.c"，内容如下:
        """
        #include <stdio.h>

        void foo(void)
        {
            char str[5];
            str[0] = 'o';
            str[1] = 'k';
            printf("%s", str);
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-13-16" 时
    那么应该报告 "test.c:8:18: warning: 禁止字符串数组未以 '\0' 结束 [gjb8114-r-1-13-16]"
    并且总共报告 1 个 warning

  @acceptance
  场景: 不报告字符串数组以 '\0' 结束
    假如有一个源码文件 "test.c"，内容如下:
        """
        #include <stdio.h>

        void foo(void)
        {
            char str[5];
            str[0] = 'o';
            str[1] = 'k';
            str[2] = '\0';
            printf("%s", str);
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-13-16" 时
    那么不应该报告任何 warning

  # FIXME: 不支持 C，原因未明
  场景: 报告 wchar_t 类型的字符串未以 '\0' 结束
    假如有一个源码文件 "test.cpp"，内容如下:
        """
        #include <stdio.h>
        #include <wchar.h>

        void foo(void)
        {
            wchar_t str[5];
            str[0] = 'o';
            str[1] = 'k';
            printf("%ls", str);
        }
        """
    当对 "test.cpp" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-13-16" 时
    那么应该报告 "test.cpp:9:19: warning: 禁止字符串数组未以 '\0' 结束 [gjb8114-r-1-13-16]"
    并且总共报告 1 个 warning

  场景: 不报告 0 初始化的字符串数组
    假如有一个源码文件 "test.c"，内容如下:
        """
        #include <stdio.h>
        void foo(void)
        {
            char str[5] = {0};
            str[0] = 'o';
            str[1] = 'k';
            printf("%s", str);
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-13-16" 时
    那么不应该报告任何 warning

  场景: 不报告 memset 初始化的字符串数组
    假如有一个源码文件 "test.cpp"，内容如下:
        """
        #include <stdio.h>
        #include <string.h>

        void foo(void)
        {
            char str[5];
            memset(str, 0, sizeof(str));
            str[0] = 'o';
            str[1] = 'k';
            printf("%s", str);
        }
        """
    当对 "test.cpp" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-13-16" 时
    那么不应该报告任何 warning

  场景: 不报告非 '\0' 结束的函数调用
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(char *str)
        {
            str[0] = 'o';
            str[1] = 'k';
        }

        void bar(void)
        {
            char str[5];
            foo(str);
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-13-16" 时
    那么不应该报告任何 warning

  场景: 报告其它函数调用中字符串数组未以 '\0' 结束
    假如有一个源码文件 "test.c"，内容如下:
        """
        #include <string.h>
        #include <stdio.h>

        void foo(void)
        {
            char empty[5] = {0};
            char str[5];
            str[0] = 'o';
            str[1] = 'k';
            strlen(str);
            strcmp(str, empty);
            strncmp(empty, str, sizeof(str));
            FILE *f;
            fprintf(f, "%s", str);
            fputs(str, f);
            puts(str);
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-13-16" 时
    那么应该报告 "test.c:10:12: warning: 禁止字符串数组未以 '\0' 结束 [gjb8114-r-1-13-16]"
    并且应该报告 "test.c:11:12: warning: 禁止字符串数组未以 '\0' 结束 [gjb8114-r-1-13-16]"
    并且应该报告 "test.c:12:20: warning: 禁止字符串数组未以 '\0' 结束 [gjb8114-r-1-13-16]"
    并且应该报告 "test.c:14:22: warning: 禁止字符串数组未以 '\0' 结束 [gjb8114-r-1-13-16]"
    并且应该报告 "test.c:15:11: warning: 禁止字符串数组未以 '\0' 结束 [gjb8114-r-1-13-16]"
    并且应该报告 "test.c:16:10: warning: 禁止字符串数组未以 '\0' 结束 [gjb8114-r-1-13-16]"
    并且总共报告 6 个 warning
