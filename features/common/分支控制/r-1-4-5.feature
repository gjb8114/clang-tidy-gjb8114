# language: zh-CN
功能: R-1-4-5 禁止 switch 语句仅包含 default

  @acceptance
  场景: 报告仅包含 default 的 switch 语句
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(int x)
        {
            switch (x)
            {
            default:
                break;
            }
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-4-5" 时
    那么应该报告 "test.c:5:5: warning: 禁止 switch 语句仅包含 default [gjb8114-r-1-4-5]"
    并且总共报告 1 个 warning

  场景: 不报告同时包含 case 和 default 的 switch 语句
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(int x)
        {
            switch (x)
            {
            case 0:
                break;
            case 1:
                break;
            default:
                break;
            }
            return 0;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-4-5" 时
    那么不应该报告任何 warning