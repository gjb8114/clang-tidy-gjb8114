# language: zh-CN
功能: R-1-13-7 禁止变量遮蔽

  变量遮蔽是指在一个作用域中定义了一个与外部作用域中同名的变量，导致外部变量被遮蔽。

  @acceptance
  场景: 报告变量遮蔽
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(void)
        {
            int i = 0;
            {
                int i = 0;
            }
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-13-7" 时
    那么应该报告 "test.c:5:13: warning: 禁止变量遮蔽 [gjb8114-r-1-13-7]"
    并且总共报告 1 个 warning

  场景: 报告 for 循环中的变量遮蔽
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(void)
        {
            int i;
            {
                for (int i=0; i<10; i++)
                    ;
            }
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-13-7" 时
    那么应该报告 "test.c:5:18: warning: 禁止变量遮蔽 [gjb8114-r-1-13-7]"
    并且总共报告 1 个 warning

  场景: 不报告退出作用域后的同名变量
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(void)
        {
            {
                int i;
            }
            int i;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-13-7" 时
    那么不应该报告任何 warning

  场景: 不报告全局变量遮蔽
    假如有一个源码文件 "test.cpp"，内容如下:
        """
        int i;
        void foo(void)
        {
            int i;
        }
        namespace bar { int i; }
        """
    当对 "test.cpp" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-13-7" 时
    那么不应该报告任何 warning