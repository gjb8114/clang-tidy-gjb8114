# language: zh-CN
功能: R-1-1-5 禁止重复定义宏

  例外:

  - 不同编译条件中的定义
  - 删除后的再定义

  @acceptance
  场景: 报告重复定义宏
    假如有一个源码文件 "test.cpp"，内容如下:
        """
        #define FOO 1
        #define FOO 2
        """
    当对 "test.cpp" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-1-5" 时
    那么应该报告 "test.cpp:2:9: warning: 禁止重复定义宏 [gjb8114-r-1-1-5]"
    并且总共报告 1 个 warning

  @acceptance
  场景: 不报告删除后重新定义的宏
    假如有一个源码文件 "test.cpp"，内容如下:
        """
        #define FOO 1
        #undef FOO
        #define FOO 2
        """
    当对 "test.cpp" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-1-5" 时
    那么不应该报告任何 warning

  @acceptance
  场景: 报告 #endif 之后的重复定义宏
    假如有一个源码文件 "test.cpp"，内容如下:
        """
        #define FOO 1
        #if !defined(FOO)
        #define FOO
        #endif

        #define FOO 2
        """
    当对 "test.cpp" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-1-5" 时
    那么应该报告 "test.cpp:6:9: warning: 禁止重复定义宏 [gjb8114-r-1-1-5]"
    并且总共报告 1 个 warning

  @acceptance
  场景: 不报告不同条件分支中的重复定义宏
    假如有一个源码文件 "test.cpp"，内容如下:
        """
        #if 1
        #define FOO 1
        #else
        #define FOO 2
        #endif
        """
    当对 "test.cpp" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-1-5" 时
    那么不应该报告任何 warning

  @acceptance
  场景: 报告重复执行的条件分支中的重复定义宏
    假如有一个源码文件 "test.cpp"，内容如下:
        """
        #if defined(PRESET_FOO)
        #define FOO 1
        #else
        #define FOO 0
        #endif

        #if defined(BAR)
        #define FOO 3
        #else
        #define FOO 4
        #endif
        """
    当对 "test.cpp" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-1-5" 时
    那么应该报告 "test.cpp:10:9: warning: 禁止重复定义宏 [gjb8114-r-1-1-5]"
    并且总共报告 1 个 warning
