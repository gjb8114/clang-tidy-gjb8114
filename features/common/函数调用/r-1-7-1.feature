# language: zh-CN
功能: R-1-7-1 禁止覆盖标准库函数

  @acceptance
  场景: 报告覆盖标准库函数
    假如有一个源码文件 "test.c"，内容如下:
        """
        void exit(int code)
        {
            // do something
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-7-1" 时
    那么应该报告 "test.c:1:6: warning: 禁止覆盖标准库函数 [gjb8114-r-1-7-1]"
    并且总共报告 1 个 warning