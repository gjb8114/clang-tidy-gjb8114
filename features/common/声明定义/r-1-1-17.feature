# language: zh-CN
功能: R-1-1-17 禁止函数体内 extern 声明

  @acceptance
  场景: 报告函数体内 extern 声明
    假如有一个源码文件 "data.c"，内容如下:
        """
        int the_var = 0;
        void foo(int x)
        {
        }
        """
    并且有一个源码文件 "test.c"，内容如下:
        """
        void bar(void)
        {
            extern int the_var;
            extern void foo(int x);
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-1-17" 时
    那么应该报告 "test.c:3:5: warning: 禁止函数体内 extern 声明 [gjb8114-r-1-1-17]"
    并且应该报告 "test.c:4:5: warning: 禁止函数体内 extern 声明 [gjb8114-r-1-1-17]"
