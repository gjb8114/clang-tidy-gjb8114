# language: zh-CN
功能: R-1-10-6 禁止无实质作用的类型转换

  @acceptance
  场景: 报告相同类型的类型转换
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(int x, int y)
        {
            int z = x + (int) y;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-10-6" 时
    那么应该报告 "test.c:3:17: warning: 禁止无实质作用的类型转换 [gjb8114-r-1-10-6]"
    并且总共报告 1 个 warning

  # integer promotion
  场景: 报告整型数自动提升情况下不必要的类型转换
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(int x, short y)
        {
            int z = x + (int) y;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-10-6" 时
    那么应该报告 "test.c:3:13: warning: 禁止无实质作用的类型转换 [gjb8114-r-1-10-6]"
    并且总共报告 1 个 warning

  # usual arithmetic conversions
  场景: 报告常见算术转换情况下不必要的类型转换
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(int x, int y)
        {
            float average = (float) (x + y) / 2.0f;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-10-6" 时
    那么应该报告 "test.c:3:21: warning: 禁止无实质作用的类型转换 [gjb8114-r-1-10-6]"
    并且总共报告 1 个 warning

  @acceptance
  场景: 不报告截断类型转换
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(void)
        {
            float pi = 3.14f;
            double d;
            d = (int) pi + 2.0;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-10-6" 时
    那么不应该报告任何 warning

  场景大纲: 不报告必要的类型转换
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(<type1> x)
        {
            <type2> y = (<type2>) x;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-10-6" 时
    那么不应该报告任何 warning

    例子:
      | type1 | type2        |
      | int   | float        |
      | float | int          |
      | int   | unsigned int |