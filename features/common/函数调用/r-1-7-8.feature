# language: zh-CN
功能: R-1-7-8 禁止 void 函数 return 语句带值

  @acceptance
  场景: 报告 void 函数 return 语句带值
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(void)
        {
            return 0;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-7-8" 时
    那么应该报告 "test.c:3:5: warning: 禁止 void 函数 return 语句带值 [gjb8114-r-1-7-8]"
    并且总共报告 1 个 warning

  @acceptance
  场景: 不报告 void 函数 return 语句不带值
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(void)
        {
            return;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-7-8" 时
    那么不应该报告任何 warning