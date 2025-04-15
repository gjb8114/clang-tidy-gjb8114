# language: zh-CN
功能: R-1-1-4 禁止遮蔽标准库标识符

  C/C++ 标准库标识符包含类型与变量标识符，禁止用户重新定义这些标识符。

  @acceptance
  场景: 违背示例
    假如有一个源码文件 "test.cpp"，内容如下:
        """
        void foo(void)
        {
            unsigned int EOF = 0;
            unsigned int clog = 0;
            unsigned int cerr = 0;
            unsigned int endl = 0;
        }
        """
    当对 "test.cpp" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-1-4" 时
    那么应该报告 "test.cpp:3:18: warning: 禁止遮蔽标准库标识符 [gjb8114-r-1-1-4]"
    并且应该报告 "test.cpp:4:18: warning: 禁止遮蔽标准库标识符 [gjb8114-r-1-1-4]"
    并且应该报告 "test.cpp:5:18: warning: 禁止遮蔽标准库标识符 [gjb8114-r-1-1-4]"
    并且应该报告 "test.cpp:6:18: warning: 禁止遮蔽标准库标识符 [gjb8114-r-1-1-4]"
    并且总共报告 4 个 warning