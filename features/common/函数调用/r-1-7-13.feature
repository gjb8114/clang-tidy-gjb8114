# language: zh-CN
功能: R-1-7-13 禁止未使用的静态函数

  @acceptance
  场景: 报告未使用的静态函数
    假如有一个源码文件 "test.c"，内容如下:
        """
        static void foo(int paData)
        {
        }

        int main(void)
        {
            return 0;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-7-13" 时
    那么应该报告 "test.c:1:13: warning: 禁止未使用的静态函数 [gjb8114-r-1-7-13]"
    并且总共报告 1 个 warning