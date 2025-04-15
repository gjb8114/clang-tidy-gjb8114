# language: zh-CN
功能: R-1-10-1 禁止浮点数变量赋给整型变量

  例外: 浮点数经过强制转换

  @acceptance
  场景: 报告浮点数赋值未强制转换
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(float x)
        {
            int i;
            i = x;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-10-1" 时
    那么应该报告 "test.c:4:9: warning: 禁止浮点数变量赋给整型变量 [gjb8114-r-1-10-1]"
    并且总共报告 1 个 warning

  @acceptance
  场景: 不报告浮点数赋值已强制转换
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(float x)
        {
            int i;
            i = (int)x;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-10-1" 时
    那么不应该报告任何 warning