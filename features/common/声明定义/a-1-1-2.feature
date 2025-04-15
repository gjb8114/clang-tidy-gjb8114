# language: zh-CN
@skip
功能: A-1-1-2 建议宏内括号匹配

  @acceptance
  场景: 报告宏内括号不匹配
    假如有一个源码文件 "test.c"，内容如下:
        """
        #define FOR_I(x) for (int i = 0; i < (x); ++i) {

        void foo()
        {
            FOR_I(10)
              // do something
            }
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-a-1-1-2" 时
    那么应该报告 "test.c:1:28: note: 建议宏内括号匹配 [gjb8114-a-1-1-2]"
    并且总共报告 1 个 note

  @acceptance
  场景: 不报告宏内括号匹配
    假如有一个源码文件 "test.c"，内容如下:
        """
        #define FOR_I(x) for (int i = 0; i < (x); ++i)

        void foo()
        {
            FOR_I(10) {
              // do something
            }
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-a-1-1-2" 时
    那么不应该报告任何 note