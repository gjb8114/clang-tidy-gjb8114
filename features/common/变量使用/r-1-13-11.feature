# language: zh-CN
功能: R-1-13-11 禁止单独使用小写字母 “l” 或大写字母 “O” 作为变量名

  @acceptance
  场景: 违背示例
    假如有一个源码文件 "test.c"，内容如下:
            """
            int main(void)
            {
                int l = 0;     //违背1
                int O = 0;     //违背2
                l = O;
                O = l;
                return (0);
            }
            """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-13-11" 时
    那么应该报告 "test.c:3:9: warning: 禁止单独使用小写字母 “l” 或大写字母 “O” 作为变量名 [gjb8114-r-1-13-11]"
    并且应该报告 "test.c:4:9: warning: 禁止单独使用小写字母 “l” 或大写字母 “O” 作为变量名 [gjb8114-r-1-13-11]"
    并且总共报告 2 个 warning

  场景: 报告参数名违背
    假如有一个源码文件 "test.cpp"，内容如下:
            """
            int foo(int l, int O)
            {
                return (l + O);
            }

            void bar(void)
            {
                foo(1, [](int l) { return l + 1; }(3));
            }
            """
    当对 "test.cpp" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-13-11" 时
    那么应该报告 "test.cpp:1:13: warning: 禁止单独使用小写字母 “l” 或大写字母 “O” 作为变量名 [gjb8114-r-1-13-11]"
    并且应该报告 "test.cpp:1:20: warning: 禁止单独使用小写字母 “l” 或大写字母 “O” 作为变量名 [gjb8114-r-1-13-11]"
    并且应该报告 "test.cpp:8:19: warning: 禁止单独使用小写字母 “l” 或大写字母 “O” 作为变量名 [gjb8114-r-1-13-11]"
    并且总共报告 3 个 warning