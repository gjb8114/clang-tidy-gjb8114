# language: zh-CN
功能: R-1-1-12 禁止位域基类型大小不一致或跨越基类型边界

  @acceptance
  场景: 报告位域基类型大小不一致或跨越基类型边界
    假如有一个源码文件 "test.c"，内容如下:
        """
        struct Foo
        {
            unsigned char a : 6;
            unsigned short b : 2;
        };

        struct Bar
        {
            unsigned short a : 14;
            unsigned short b : 4;
        };
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-1-12" 时
    那么应该报告 "test.c:4:5: warning: 禁止位域基类型大小不一致或跨越基类型边界 [gjb8114-r-1-1-12]"
    并且应该报告 "test.c:10:20: warning: 禁止位域基类型大小不一致或跨越基类型边界 [gjb8114-r-1-1-12]"
    并且总共报告 2 个 warning

  @acceptance
  场景: 不报告位定义的变量未跨越类型长度
    假如有一个源码文件 "test.c"，内容如下:
        """
        struct Foo
        {
            unsigned char a : 6;
            unsigned char b : 2;
        };

        struct Bar
        {
            unsigned short a : 14;
            unsigned short _reserved : 2;
            unsigned short b : 14;
        };
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-1-12" 时
    那么不应该报告任何 warning