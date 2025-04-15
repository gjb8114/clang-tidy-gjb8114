# language: zh-CN
功能: R-1-8-1 禁止不可达语句

  @acceptance
  场景: 报告不可达语句
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(int x)
        {
            switch (x)
            {
                --x;
                case 1:
                    break;
                case 2:
                    break;
                default:
                    break;
            }
            return;
            x++;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-8-1" 时
    那么应该报告 "test.c:5:9: warning: 禁止不可达语句 [gjb8114-r-1-8-1]"
    并且应该报告 "test.c:14:5: warning: 禁止不可达语句 [gjb8114-r-1-8-1]"

  场景: 不报告不可达分支
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(int x)
        {
            if (0)
            {
            }
            else
            {
            }
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-8-1" 时
    那么不应该报告任何 warning

  场景: 报告不可达 return
    假如有一个源码文件 "test.c"，内容如下:
        """
        int foo(int x)
        {
            int i = 0;
            if (x)
            {
                return i;
            }
            else
            {
                return x;
            }
            return 0;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-8-1" 时
    那么应该报告 "test.c:12:12: warning: 禁止不可达语句 [gjb8114-r-1-8-1]"
    并且总共报告 1 个 warning