# language: zh-CN
功能: R-1-6-8 禁止数组越界使用

  @acceptance
  场景: 报告数组字面量下标越界
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(void)
        {
            int array[7];
            array[7] = 0;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-6-8" 时
    那么应该报告 "test.c:4:5: warning: 禁止数组越界使用 [gjb8114-r-1-6-8]"

  @acceptance
  场景: 不报告数组下标未越界
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(void)
        {
            int array[7];
            array[6] = 0;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-6-8" 时
    那么不应该报告任何 warning

  场景: 报告数组类型参数下标越界
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(int a[], int n)
        {
            for (int i = 0; i <= n; i++)
            {
                a[i] = 0;
            }
        }

        void bar(void)
        {
            int array[7];
            foo(array, 7);
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-6-8" 时
    那么应该报告 "test.c:3:21: warning: 禁止数组越界使用 [gjb8114-r-1-6-8]"
    并且总共报告 1 个 warning

  场景: 不报告数组类型参数下标未越界
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(int a[], int n)
        {
            for (int i = 0; i < n; i++)
            {
                a[i] = 0;
            }
        }

        void bar(void)
        {
            int array[7];
            foo(array, 7);
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-6-8" 时
    那么不应该报告任何 warning
