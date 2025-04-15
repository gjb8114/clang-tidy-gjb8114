# language: zh-CN
功能: R-1-13-2 禁止形参与全局变量同名

  @acceptance
  场景: 报告形参与全局变量同名
    假如有一个源码文件 "test.c"，内容如下:
        """
        int the_global_var = 0;

        void foo(int the_global_var)
        {
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-13-2" 时
    那么应该报告 "test.c:3:14: warning: 禁止形参与全局变量同名 [gjb8114-r-1-13-2]"
    并且总共报告 1 个 warning