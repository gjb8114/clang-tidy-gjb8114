# language: zh-CN
功能: R-1-6-11 禁止非枚举类型变量赋值枚举值

  @acceptance
  场景: 报告非枚举类型变量赋值枚举值
    假如有一个源码文件 "test.c"，内容如下:
        """
        enum State { Init = 0, Running, Finished };

        void foo(void)
        {
            int a, b = Init;
            a = Running;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-6-11" 时
    那么应该报告 "test.c:5:16: warning: 禁止非枚举类型变量赋值枚举值 [gjb8114-r-1-6-11]"
    并且应该报告 "test.c:6:9: warning: 禁止非枚举类型变量赋值枚举值 [gjb8114-r-1-6-11]"
    并且总共报告 2 个 warning

  @acceptance
  场景: 不报告枚举类型变量赋值枚举值
    假如有一个源码文件 "test.c"，内容如下:
        """
        enum State { Init = 0, Running, Finished };

        void foo(void)
        {
            enum State state;
            state = Init;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-6-11" 时
    那么不应该报告任何 warning
