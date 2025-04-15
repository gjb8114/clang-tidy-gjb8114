# language: zh-CN
功能: R-1-13-9 禁止仅小写字母 “l” 与数字 “1” 区分的变量

  @acceptance
  场景: 报告仅小写字母 “l” 与数字 “1” 区分的变量
    假如有一个源码文件 "test.c"，内容如下:
            """
            void foo(void)
            {
                int variable;
                int variab1e;
            }
            """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-13-9" 时
    那么应该报告 "test.c:4:9: warning: 禁止仅小写字母 “l” 与数字 “1” 区分的变量 [gjb8114-r-1-13-9]"
    并且总共报告 1 个 warning