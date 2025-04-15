# language: zh-CN
功能: R-1-12-3 禁止浮点数相等比较

  应该判断精度误差范围。

  @acceptance
  场景: 报告浮点数相等比较
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(float x, float y)
        {
            int is_equal = x == y;
            int is_not_equal = x != y;
            int is_zero = x == 0.0;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-12-3" 时
    那么应该报告 "test.c:3:22: warning: 禁止浮点数相等比较 [gjb8114-r-1-12-3]"
    并且应该报告 "test.c:4:26: warning: 禁止浮点数相等比较 [gjb8114-r-1-12-3]"
    并且应该报告 "test.c:5:21: warning: 禁止浮点数相等比较 [gjb8114-r-1-12-3]"
    并且总共报告 3 个 warning

  @acceptance
  场景: 不报告浮点数范围比较
    假如有一个源码文件 "test.c"，内容如下:
        """
        #include <math.h>

        void foo(float x, float y)
        {
            float error = 1e-6;
            int is_equal = fabs(x - y) < error;
            int is_not_equal = fabs(x - y) >= error;
            int is_zero = fabs(x) < 1e-6;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-12-3" 时
    那么不应该报告任何 warning