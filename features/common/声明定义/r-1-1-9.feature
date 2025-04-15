# language: zh-CN
功能: R-1-1-9 禁止结构体定义中含有匿名结构体

  @acceptance
  场景: 报告含有匿名结构体
    假如有一个源码文件 "test.c"，内容如下:
        """
        struct Outer
        {
            struct Inner
            {
                int a;
            };
        };
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-1-9" 时
    那么应该报告 "test.c:3:12: warning: 禁止结构体定义中含有匿名结构体 [gjb8114-r-1-1-9]"
    并且总共报告 1 个 warning

  @acceptance
  场景: 不报告含有命名结构体
    假如有一个源码文件 "test.c"，内容如下:
        """
        struct Outer
        {
            struct Inner
            {
                int a;
            } inner;
        };
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-1-9" 时
    那么不应该报告任何 warning