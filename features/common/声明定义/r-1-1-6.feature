# language: zh-CN
功能: R-1-1-6 禁止函数内的 #define 和 #undef 不配对

  函数内的 #define 在函数退出前必须有对应 #undef

  @acceptance
  场景: 报告 #define 和 #undef 不配对
    假如有一个源码文件 "test.cpp"，内容如下:
        """
        int foo1(void)
        {
            #define FOO 1
            return FOO;
        }

        void foo2(void)
        {
            #undef FOO
        }
        """
    当对 "test.cpp" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-1-6" 时
    那么应该报告 "test.cpp:3:13: warning: 禁止函数内的 #define 和 #undef 不配对 [gjb8114-r-1-1-6]"
    并且应该报告 "test.cpp:9:12: warning: 禁止函数内的 #define 和 #undef 不配对 [gjb8114-r-1-1-6]"
    并且总共报告 2 个 warning

  @acceptance
  场景: 不报告 #define 和 #undef 配对
    假如有一个源码文件 "test.cpp"，内容如下:
        """
        int foo1(void)
        {
            #define FOO 1
            int x = FOO;
            #undef FOO
            return x;
        }
        """
    当对 "test.cpp" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-1-6" 时
    那么不应该报告任何 warning

  场景: 报告缺少 #undef
    假如有一个源码文件 "test.cpp"，内容如下:
        """
        int foo(void)
        {
            #define FOO
            int x = 0U;
            return x;
        }
        """
    当对 "test.cpp" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-1-6" 时
    那么应该报告 "test.cpp:3:13: warning: 禁止函数内的 #define 和 #undef 不配对 [gjb8114-r-1-1-6]"
    并且总共报告 1 个 warning

  场景: 报告逆序的 #define 和 #undef
    假如有一个源码文件 "test.cpp"，内容如下:
        """
        int foo(void)
        {
            #undef FOO
            int x = 0U;
            #define FOO
            return x;
        }
        """
    当对 "test.cpp" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-1-6" 时
    那么应该报告 "test.cpp:3:12: warning: 禁止函数内的 #define 和 #undef 不配对 [gjb8114-r-1-1-6]"
    并且应该报告 "test.cpp:5:13: warning: 禁止函数内的 #define 和 #undef 不配对 [gjb8114-r-1-1-6]"
    并且总共报告 2 个 warning

  场景: 报告对函数外的 #define 的 #undef
    假如有一个源码文件 "test.cpp"，内容如下:
        """
        #define FOO
        void foo(void)
        {
            #undef FOO
        }
        """
    当对 "test.cpp" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-1-6" 时
    那么应该报告 "test.cpp:4:12: warning: 禁止函数内的 #define 和 #undef 不配对 [gjb8114-r-1-1-6]"

  场景: 报告多余的 #undef
    假如有一个源码文件 "test.cpp"，内容如下:
        """
        void foo(void)
        {
            #define FOO
            #undef FOO
            #define FOO
            #undef FOO
            #undef FOO
        }
        """
    当对 "test.cpp" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-1-6" 时
    那么应该报告 "test.cpp:7:12: warning: 禁止函数内的 #define 和 #undef 不配对 [gjb8114-r-1-1-6]"
