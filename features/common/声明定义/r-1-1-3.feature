# language: zh-CN
功能: R-1-1-3 禁止遮蔽 typedef 类型

  @acceptance
  场景: 报告重新定义 typedef 类型
    假如有一个源码文件 "test.c"，内容如下:
        """
        typedef int ValueT;

        void foo(void)
        {
            typedef unsigned ValueT;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-1-3" 时
    那么应该报告 "test.c:5:22: warning: 禁止遮蔽 typedef 类型 [gjb8114-r-1-1-3]"
    并且总共报告 1 个 warning