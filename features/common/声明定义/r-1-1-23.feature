# language: zh-CN
功能: R-1-1-23 禁止函数参数表为空时显式声明 void

  @acceptance
  场景: 报告函数参数表为空时未使用 void 明确说明
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo();
        void foo()
        {
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-1-23" 时
    那么应该报告 "test.c:1:6: warning: 禁止函数参数表为空时显式声明 void [gjb8114-r-1-1-23]"
    并且应该报告 "test.c:2:6: warning: 禁止函数参数表为空时显式声明 void [gjb8114-r-1-1-23]"
    并且总共报告 2 个 warning

  @acceptance
  场景: 不报告函数参数表为空时使用 void 明确说明
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(void);
        void foo(void)
        {
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-1-23" 时
    那么不应该报告任何 warning

  场景: 报告 C++ 函数声明中缺少参数
    假如有一个源码文件 "test.cpp"，内容如下:
        """
        void foo();
        void foo(void)
        {
        }
        """
    当对 "test.cpp" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-1-23" 时
    那么应该报告 "test.cpp:1:6: warning: 禁止函数参数表为空时显式声明 void [gjb8114-r-1-1-23]"
    并且总共报告 1 个 warning
