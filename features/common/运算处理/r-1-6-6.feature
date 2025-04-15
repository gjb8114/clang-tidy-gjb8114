# language: zh-CN
功能: R-1-6-6 禁止移位运算超出变量长度

  @acceptance
  场景: 报告移位运算超出变量长度
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(void)
        {
            unsigned short a = 1;
            a << 16;
            unsigned short b = 0xFFFF;
            b >> 16;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-6-6" 时
    那么应该报告 "test.c:4:7: warning: 禁止移位运算超出变量长度 [gjb8114-r-1-6-6]"
    并且应该报告 "test.c:6:7: warning: 禁止移位运算超出变量长度 [gjb8114-r-1-6-6]"
    并且总共报告 2 个 warning

  @acceptance
  场景: 不报告移位运算未超出变量长度
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(void)
        {
            unsigned int a = 1;
            a << 31;
            unsigned int b = 0xFFFF;
            b >> 31;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-6-6" 时
    那么不应该报告任何 warning