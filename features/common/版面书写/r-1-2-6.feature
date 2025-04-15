# language: zh-CN
功能: R-1-2-6 禁止嵌套注释

  @acceptance
  场景: 报告嵌套注释
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(void)
        {
            /*
                comment ...
                /*
                    nested comment ...
                */
            */
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-2-6" 时
    那么应该报告 "test.c:5:9: warning: 禁止嵌套注释 [gjb8114-r-1-2-6]"
    并且总共报告 1 个 warning

  @acceptance
  场景: 不报告非嵌套注释
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(void)
        {
            /*
                comment ...
            */
            /*
                comment ...
            */
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-2-6" 时
    那么不应该报告任何 warning

  场景: 不报告单行注释中包含注释符号
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(void)
        {
            // 这不是嵌套注释 /* 注释的一部分 */
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-2-6" 时
    那么不应该报告任何 warning

  场景: 报告单行的嵌套注释
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(void)
        {
            /* This is a comment /* 这是嵌套注释 */ .... */
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-2-6" 时
    那么应该报告 "test.c:3:26: warning: 禁止嵌套注释 [gjb8114-r-1-2-6]"
    并且总共报告 1 个 warning

  场景: 不报告不完整的嵌套注释
    假如有一个源码文件 "test.c"，内容如下:
       """
        void foo(void)
        {
            /* This is a comment /* not nested. */
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-2-6" 时
    那么不应该报告任何 warning

  场景: 报告多个嵌套注释
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(void)
        {
            /* This is a comment /* 一个是嵌套注释 */ .... */
        }
        /* This is a comment /* 另一个也是嵌套注释 */ .... */
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-2-6" 时
    那么应该报告 "test.c:3:26: warning: 禁止嵌套注释 [gjb8114-r-1-2-6]"
    那么应该报告 "test.c:5:22: warning: 禁止嵌套注释 [gjb8114-r-1-2-6]"
    并且总共报告 2 个 warning

  场景: 报告多个层级的嵌套注释
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(void)
        {
            const char* local = "a";
            /*
                level 1
                /*
                    level 2
                    /*
                        level 3
                    */
                */
            */
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-2-6" 时
    那么应该报告 "test.c:6:9: warning: 禁止嵌套注释 [gjb8114-r-1-2-6]"
    那么应该报告 "test.c:8:13: warning: 禁止嵌套注释 [gjb8114-r-1-2-6]"
    并且总共报告 2 个 warning

  场景: 不报告字符串字面量中的类似嵌套注释
    假如有一个源码文件 "test.cpp"，内容如下:
        """
        void foo(void)
        {
            int local = 0;
            const char* msg1 = "This is a message: /* ... /* ... */ .... */ ...";
            const char* msg2 = R"(This is a message:
            /* ... /* ... */ .... */ ...
            )";
        }
        """
    当对 "test.cpp" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-2-6" 时
    那么不应该报告任何 warning
