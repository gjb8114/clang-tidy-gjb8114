# language: zh-CN
功能: R-1-1-2 禁止宏定义到关键字

  @acceptance
  场景: 报告宏定义为关键字
    假如有一个源码文件 "test.c"，内容如下:
        """
        #define CHAR char
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-1-2" 时
    那么应该报告 "test.c:1:9: warning: 禁止宏定义到关键字 [gjb8114-r-1-1-2]"
    并且总共报告 1 个 warning

  @acceptance
  场景: 不报告宏定义包含关键字
    假如有一个源码文件 "test.c"，内容如下:
        """
        #define CString const char*
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-1-2" 时
    并且不应该报告任何 warning