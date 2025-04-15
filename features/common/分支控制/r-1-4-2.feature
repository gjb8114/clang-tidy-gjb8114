# language: zh-CN
功能: R-1-4-2 禁止条件分支为空

  例外: 使用空语句 `;` 并随后加以 `/* no deal with */` 注释说明。

  @acceptance
  场景: 报告条件分支为空
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(int x)
        {
            int y = 3;
            if (x > 0)
            {
                y = 1;
            }
            else;
            if (y > 1)
            {
                y *= 2;
            }
            else if (y == 1)
            {
                ;
            }
            else if (y == 0)
            {
            }
            else
            {
                /* no deal with */
            }
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-4-2" 时
    那么应该报告 "test.c:8:9: warning: 禁止条件分支为空 [gjb8114-r-1-4-2]"
    并且应该报告 "test.c:15:9: warning: 禁止条件分支为空 [gjb8114-r-1-4-2]"
    并且应该报告 "test.c:18:5: warning: 禁止条件分支为空 [gjb8114-r-1-4-2]"
    并且应该报告 "test.c:21:5: warning: 禁止条件分支为空 [gjb8114-r-1-4-2]"
    并且总共报告 4 个 warning

  @acceptance
  场景: 不报告条件分支有注释
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(int x)
        {
            int y = 3;
            if (x > 0)
            {
                y = 1;
            }
            else
                ; /* no deal with */
            if (y > 1)
            {
                y *= 2;
            }
            else if (y == 1)
            {
                ; /* no deal with */
            }
            else if (y == 0)
            {
                ; /* no deal with */
            }
            else
            {
                ; /* no deal with */
            }
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-4-2" 时
    那么不应该报告任何 warning

  场景: 报告多个空语句
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(void)
        {
            int i = 0;
            if (i == 0)
            {
                i = 1;
            }
            else
            {
                ;;
            }
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-4-2" 时
    那么应该报告 "test.c:10:9: warning: 禁止条件分支为空 [gjb8114-r-1-4-2]"
    并且总共报告 1 个 warning

  场景: 不报告非空分支
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(void)
        {
            int i = 0;
            if (i == 0)
            {
                i = 1;
            }
            else if (i == 1)
            {
                i = 2;
                ;
            }
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-4-2" 时
    那么不应该报告任何 warning

  场景: 不报告注释前有空白
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(void)
        {
            int i = 0;
            if (i == 0)
            {
                i = 1;
            }
            else
            {
                ;
                /* no deal with */
            }
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-4-2" 时
    那么不应该报告任何 warning