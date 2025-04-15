# language: zh-CN
功能: R-1-9-3 禁止 for 循环内修改控制变量

  for 循环控制变量在循环体内被修改，可能导致循环条件判断错误，造成死循环或意外行为。

  @acceptance
  场景: 报告 for 循环内修改控制变量
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(void)
        {
            for (int i = 0; i < 100; i++)
            {
                i = 2 * i;
            }
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-9-3" 时
    那么应该报告 "test.c:5:9: warning: 禁止 for 循环内修改控制变量 [gjb8114-r-1-9-3]"
    并且总共报告 1 个 warning