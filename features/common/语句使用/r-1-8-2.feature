# language: zh-CN
功能: R-1-8-2 禁止不可达分支

  @acceptance
  场景: 报告不可达分支
    假如有一个源码文件 "test.cpp"，内容如下:
        """
        void foo(unsigned x)
        {
            if (x >= 0)
            {
            }
            else
            {
            }
            if (0 <= x)
            {
            }
            else
            {
            }
        }
        """
    当对 "test.cpp" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-8-2" 时
    那么应该报告 "test.cpp:6:5: warning: 禁止不可达分支 [gjb8114-r-1-8-2]"
    并且应该报告 "test.cpp:12:5: warning: 禁止不可达分支 [gjb8114-r-1-8-2]"
    并且总共报告 2 个 warning

  @acceptance
  场景: 不报告不可达分支
    假如有一个源码文件 "test.cpp"，内容如下:
        """
        void foo(int x)
        {
            if (x >= 0)
            {
            }
            else
            {
            }
        }
        """
    当对 "test.cpp" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-8-2" 时
    那么不应该报告任何 warning

  场景: 报告多个 else-if, else 不可达分支
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(void)
        {
            int branch = 0;
            unsigned int value = 5;
            if (value < 10)
            {
                branch = 1;
            }
            else if (value >= 10)
            {
                branch = 2;
            }
            else if (1)
            {
                branch = 3;
            }
            else
            {
                branch = 4;
            }
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-8-2" 时
    那么应该报告 "test.c:9:10: warning: 禁止不可达分支 [gjb8114-r-1-8-2]"
    并且应该报告 "test.c:13:10: warning: 禁止不可达分支 [gjb8114-r-1-8-2]"
    并且应该报告 "test.c:17:5: warning: 禁止不可达分支 [gjb8114-r-1-8-2]"
    并且总共报告 3 个 warning

  场景: 报告带有字面量条件的不可达分支
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(void)
        {
            int branch = 0;
            if (1)
            {
                branch = 1;
            }
            else
            {
                branch = 2;
            }
            
            if (0)
            {
                branch = 3;
            }
            else
            {
                branch = 4;
            }
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-8-2" 时
    那么应该报告 "test.c:8:5: warning: 禁止不可达分支 [gjb8114-r-1-8-2]"
    并且应该报告 "test.c:13:5: warning: 禁止不可达分支 [gjb8114-r-1-8-2]"
    并且总共报告 2 个 warning

  场景: 报告不可达分支嵌套在可达分支中
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(void)
        {
            int value = 10;
            int branch = 0;
            
            if (value > 5)
            {
                if (1)
                {
                    branch = 1;
                }
                else
                {
                    branch = 2;
                }
            }
            else
            {
                branch = 3;
            }
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-8-2" 时
    那么应该报告 "test.c:12:9: warning: 禁止不可达分支 [gjb8114-r-1-8-2]"
    并且应该报告 "test.c:17:5: warning: 禁止不可达分支 [gjb8114-r-1-8-2]"
    并且总共报告 2 个 warning

  场景: 报告不可达分支嵌套在不可达分支中
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(void)
        {
            int value = 10;
            int branch = 0;

            if (value > 5)
            {
                branch = 1;
            }
            else
            {
                if (0)
                {
                    branch = 2;
                }
                else
                {
                    branch = 3;
                }
            }
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-8-2" 时
    那么应该报告 "test.c:10:5: warning: 禁止不可达分支 [gjb8114-r-1-8-2]"
    并且总共报告 1 个 warning

  场景: 不报告分支条件变量可能被引用修改
    假如有一个源码文件 "test.cpp"，内容如下:
        """
        void foo(int &value)
        {
            value = 15;
        }

        void bar(void)
        {
            int value = 10;
            int branch = 0;

            foo(value);
            if (value > 5)
            {
                branch = 1;
            }
            else
            {
                branch = 2;
            }
        }
        """
    当对 "test.cpp" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-8-2" 时
    那么不应该报告任何 warning

  场景: 不报告分支条件变量可能被指针修改
    假如有一个源码文件 "test.cpp"，内容如下:
        """
        void foo(int *value);

        void bar(void)
        {
            int value = 10;
            int branch = 0;

            foo(&value);
            if (value > 5)
            {
                branch = 1;
            }
            else
            {
                branch = 2;
            }
        }
        """
    当对 "test.cpp" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-8-2" 时
    那么不应该报告任何 warning

  场景: 报告分支条件变量可能被不可变引用
    假如有一个源码文件 "test.cpp"，内容如下:
        """
        void foo(void)
        {
            int value = 10;
            int branch = 0;

            const int *pv = &value;
            const int &rv = value;
            int prv = value;

            if (value > 5)
            {
                branch = 1;
            }
            else
            {
                branch = 2;
            }
        }
        """
    当对 "test.cpp" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-8-2" 时
    那么应该报告 "test.cpp:14:5: warning: 禁止不可达分支 [gjb8114-r-1-8-2]"
    并且总共报告 1 个 warning
