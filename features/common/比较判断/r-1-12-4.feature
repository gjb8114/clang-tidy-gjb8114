# language: zh-CN
功能: R-1-12-4 禁止对无符号数进行无意义比较

  @acceptance
  场景: 报告对无符号数进行无意义比较
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(unsigned int x, unsigned int y)
        {
            int always_true = x >= 0;
            always_true = 0 <= x;
            int always_false = y < 0;
            always_false = 0 > y;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-12-4" 时
    那么应该报告 "test.c:3:25: warning: 禁止对无符号数进行无意义比较 [gjb8114-r-1-12-4]"
    并且应该报告 "test.c:4:21: warning: 禁止对无符号数进行无意义比较 [gjb8114-r-1-12-4]"
    并且应该报告 "test.c:5:26: warning: 禁止对无符号数进行无意义比较 [gjb8114-r-1-12-4]"
    并且应该报告 "test.c:6:22: warning: 禁止对无符号数进行无意义比较 [gjb8114-r-1-12-4]"
    并且总共报告 4 个 warning
