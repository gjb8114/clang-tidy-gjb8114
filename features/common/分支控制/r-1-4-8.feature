# language: zh-CN
功能: R-1-4-8 禁止 switch 语句的分支层次不一致

  @acceptance
  场景: 报告分支层次不一致
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(int x, int y)
        {
            switch (x)
            {
            case 1:
                {
                case 2:
                    break;
                }
                break;
            default:
                break;
            }
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-4-8" 时
    那么应该报告 "test.c:7:9: warning: 禁止 switch 语句的分支层次不一致 [gjb8114-r-1-4-8]"
    并且总共报告 1 个 warning

  @acceptance
  场景: 不报告分支层次一致
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(int x, int y)
        {
            switch (x)
            {
            case 1:
                break;
            case 2:
                break;
            default:
                break;
            }
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-4-8" 时
    那么不应该报告任何 warning

  场景: 报告 case 出现在循环内
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(int x, int y)
        {
            int z = 0;
            switch (x)
            {
            case 1:
                for (; y < 5; ++y)
                {
                case 2:
                    z++;
                    break;
                }
                break;
            default:
                break;
            }
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-4-8" 时
    那么应该报告 "test.c:9:9: warning: 禁止 switch 语句的分支层次不一致 [gjb8114-r-1-4-8]"
    并且总共报告 1 个 warning

  场景: 报告多个分支层次不一致
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(int x, int y)
        {
            int z = 0;
            switch (x)
            {
            case 1:
                for (;;)
                {
                    case 2:
                        z = 1;
                        break;
                }
                if (y > 0) 
                {
                    case 3:
                        z = 2;
                        break;
                }
                break;
            default:
                break;
            }
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-4-8" 时
    那么应该报告 "test.c:9:13: warning: 禁止 switch 语句的分支层次不一致 [gjb8114-r-1-4-8]"
    并且应该报告 "test.c:15:13: warning: 禁止 switch 语句的分支层次不一致 [gjb8114-r-1-4-8]"
    并且总共报告 2 个 warning

  场景: 不报告嵌套的 switch
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(int x, int y)
        {
            int z = 0;
            switch (x)
            {
            case 1:
                switch (y)
                {
                case 1:
                    z = 1;
                    break;
                default:
                    z = 2;
                    break;
                }
                break;
            default:
                break;
            }
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-4-8" 时
    那么不应该报告任何 warning

  场景: 不报告连续 fall-through
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(int x, int y)
        {
            int z = 0;
            switch (x)
            {
            case 1:
            case 2:
            case 3:
            case 4:
                z = 2;
                break;
            default:
                break;
            }
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-4-8" 时
    那么不应该报告任何 warning