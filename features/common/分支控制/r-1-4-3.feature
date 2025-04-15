# language: zh-CN
功能: R-1-4-3 禁止空 switch 语句

  @acceptance
  场景: 报告空 switch 语句
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(void)
        {
            int x = 0;
            switch (x)
            {
            }
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-4-3" 时
    那么应该报告 "test.c:6:5: warning: 禁止空 switch 语句 [gjb8114-r-1-4-3]"
    并且总共报告 1 个 warning