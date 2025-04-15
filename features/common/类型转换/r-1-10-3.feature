# language: zh-CN
功能: R-1-10-3 禁止 double 型变量赋给 float 型变量

  例外: double 型经过强制转换

  @acceptance
  场景: 报告 double 型变量赋给 float 型变量
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(double x)
        {
            float f;
            f = x;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-10-3" 时
    那么应该报告 "test.c:4:9: warning: 禁止 double 型变量赋给 float 型变量 [gjb8114-r-1-10-3]"
    并且总共报告 1 个 warning

  @acceptance
  场景: 不报告 double 型变量赋给 float 型变量
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(double x)
        {
            float f;
            f = (float)x;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-10-3" 时
    那么不应该报告任何 warning