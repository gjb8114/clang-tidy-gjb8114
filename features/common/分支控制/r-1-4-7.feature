# language: zh-CN
功能: R-1-4-7 禁止 switch 中 case 和 default 未以 break 或 return 终止

  例外: 共用 case 必须随后加以 `/* shared */` 注释说明

  @acceptance
  场景: 报告未终止的 case 和 default
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(int x, int y)
        {
            int z;
            switch (x)
            {
            case 1:
                if (y > 0)
                {
                    break;
                }
                z = 2;
            case 2:
            case 3:
                {
                    z = 3;
                }
            case 4:
                z = 4;
            case 5:
                z = 5;
                break;
            default:
                z = -1;
            }
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-4-7" 时
    那么应该报告 "test.c:6:5: warning: 禁止 switch 中 case 和 default 未以 break 或 return 终止 [gjb8114-r-1-4-7]"
    并且应该报告 "test.c:12:5: warning: 禁止 switch 中 case 和 default 未以 break 或 return 终止 [gjb8114-r-1-4-7]"
    并且应该报告 "test.c:13:5: warning: 禁止 switch 中 case 和 default 未以 break 或 return 终止 [gjb8114-r-1-4-7]"
    并且应该报告 "test.c:17:5: warning: 禁止 switch 中 case 和 default 未以 break 或 return 终止 [gjb8114-r-1-4-7]"
    并且应该报告 "test.c:22:5: warning: 禁止 switch 中 case 和 default 未以 break 或 return 终止 [gjb8114-r-1-4-7]"
    并且总共报告 5 个 warning

  @acceptance
  场景: 不报告已终止的 case 和 default
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(int x, int y)
        {
            int z = 0;
            switch (x)
            {
            case 1:
                if (y > 0)
                {
                    break;
                }
                z = 2;
                break;
            case 2: /* shared */
            case 3:
                {
                    y = 3;
                    break;
                }
            case 4: /* shared */
                y = 4;
            case 5:
                z = 5;
                break;
            default:
                z = -1;
                break;
            }
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-4-7" 时
    那么不应该报告任何 warning

  场景: 报告多个 Fall-through
    假如有一个源码文件 "test.c"，内容如下:
        """
        int foo(int x)
        {
            switch (x)
            {
            case 1:
            case 2:
              x = 2;
              break;
            case 3:
            case 4:
                return 1;
            }
            return 0;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-4-7" 时
    那么应该报告 "test.c:5:5: warning: 禁止 switch 中 case 和 default 未以 break 或 return 终止 [gjb8114-r-1-4-7]"
    并且应该报告 "test.c:9:5: warning: 禁止 switch 中 case 和 default 未以 break 或 return 终止 [gjb8114-r-1-4-7]"
    并且总共报告 2 个 warning

  场景: 不报告 return 语句终止
    假如有一个源码文件 "test.c"，内容如下:
        """
        int foo(int x)
        {
            switch (x)
            {
            case 1:
                return 1;
            }
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-4-7" 时
    那么不应该报告任何 warning

  场景: 不报告空 case 中 break 语句终止
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(int x)
        {
            switch (x)
            {
            case 1:
                break;
            }
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-4-7" 时
    那么不应该报告任何 warning

  场景: 不报告非空 case 中 break 语句终止
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(int x)
        {
            switch (x)
            {
            case 1:
                x = 1;
                break;
            }
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-4-7" 时
    那么不应该报告任何 warning

  场景: 报告 break 终止后出现语句
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(int x)
        {
            switch (x)
            {
            case 1:
                break;
                x = 1;
            case 2:
                break;
            }
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-4-7" 时
    那么应该报告 "test.c:5:5: warning: 禁止 switch 中 case 和 default 未以 break 或 return 终止 [gjb8114-r-1-4-7]"
    并且总共报告 1 个 warning

  @skip
  场景: 不报告复杂分支中的 break
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(int x)
        {
            switch (x)
            {
            case 1:
                if (x > 0)
                {
                    x = 1;
                    break;
                } else {
                    x = 2;
                    break;
                }
            case 2:
                break;
            }
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-4-7" 时
    那么不应该报告任何 warning
