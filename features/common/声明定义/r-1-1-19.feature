# language: zh-CN
功能: R-1-1-19 禁止 extern 声明初始化变量

  @acceptance
  场景: 报告 extern 声明变量初始化
    假如有一个源码文件 "var.c"，内容如下:
        """
        int the_var;
        """
    并且有一个源码文件 "test.c"，内容如下:
        """
        extern int the_var = 2;
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-1-19" 时
    那么应该报告 "test.c:1:22: warning: 禁止 extern 声明初始化变量 [gjb8114-r-1-1-19]"
    并且总共报告 1 个 warning

  @acceptance
  场景: 不报告 extern 声明变量初始化
    假如有一个源码文件 "var.c"，内容如下:
        """
        int the_var = 2;
        """
    并且有一个源码文件 "test.c"，内容如下:
        """
        extern int the_var;
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-1-19" 时
    那么不应该报告任何 warning