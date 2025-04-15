# language: zh-CN
功能: R-1-4-4 禁止对逻辑表达式使用 switch 语句

  @acceptance
  场景: 报告对逻辑表达式使用 switch
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(int x)
        {
            int y;
            switch (x == 0)
            {
            case 1:
                y = 1;
                break;
            default:
                y = 0;
                break;
            }
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-4-4" 时
    那么应该报告 "test.c:4:13: warning: 禁止对逻辑表达式使用 switch 语句 [gjb8114-r-1-4-4]"
    并且总共报告 1 个 warning

  场景: 不报告对整型变量使用 switch
    假如有一个源码文件 "test.<lang>"，内容如下:
        """
        void foo(int x)
        {
            int y = 0;
            switch (x)
            {
            case 1:
                y = 1;
                break;
            default:
                y = 0;
                break;
            }
        }
        """
    当对 "test.<lang>" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-4-4" 时
    那么不应该报告任何 warning

  场景大纲: 报告对逻辑表达式使用 switch
    假如有一个源码文件 "test.<lang>"，内容如下:
        """
        void foo(int x, int y)
        {
            int z = 0;
            switch (x > y && y < 3)
            {
            case 1:
                z = 1;
                break;
            default:
                z = 0;
                break;
            }
        }
        """
    当对 "test.<lang>" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-4-4" 时
    那么应该报告 "test.<lang>:4:13: warning: 禁止对逻辑表达式使用 switch 语句 [gjb8114-r-1-4-4]"
    并且总共报告 1 个 warning

    例子:
      | lang |
      | c    |
      | cpp  |

  场景大纲: 报告对布尔变量使用 switch
    假如有一个源码文件 "test.<lang>"，内容如下:
        """
        #include <stdbool.h>
        void foo(bool x)
        {
            int y = 0;
            switch (x)
            {
            case true:
                y = 1;
                break;
            case false:
                y = 0;
                break;
            }
        }
        """
    当对 "test.<lang>" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-4-4" 时
    那么应该报告 "test.<lang>:5:13: warning: 禁止对逻辑表达式使用 switch 语句 [gjb8114-r-1-4-4]"
    并且总共报告 1 个 warning

    例子:
      | lang |
      | c    |
      | cpp  |

  场景大纲: 报告对逻辑表达式使用 switch
    假如有一个源码文件 "test.<lang>"，内容如下:
        """
        #include <stdbool.h>
        void foo(bool flag)
        {
            int y = 0;
            switch (!flag)
            {
            case true:
                y = 1;
                break;
            case false:
                y = 0;
                break;
            }
        }
        """
    当对 "test.<lang>" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-4-4" 时
    那么应该报告 "test.<lang>:5:13: warning: 禁止对逻辑表达式使用 switch 语句 [gjb8114-r-1-4-4]"
    并且总共报告 1 个 warning

    例子:
      | lang |
      | c    |
      | cpp  |
