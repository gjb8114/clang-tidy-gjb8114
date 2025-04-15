# language: zh-CN
功能: R-1-13-6 禁止变量与 typedef 类型同名

  @acceptance
  场景: 报告变量与 typedef 类型同名
    假如有一个源码文件 "test.c"，内容如下:
        """
        typedef int Int;

        void foo(void)
        {
            int Int;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-13-6" 时
    那么应该报告 "test.c:5:9: warning: 禁止变量与 typedef 类型同名 [gjb8114-r-1-13-6]"
    并且总共报告 1 个 warning