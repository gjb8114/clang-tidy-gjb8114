# language: zh-CN
功能: R-1-10-2 禁止长整数变量赋给短整数变量

  例外: 长整型经过强制转换

  @acceptance
  场景: 报告长整数赋值
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(long l, int i, short s)
        {
            char c;
            c = s;
            c = i;
            c = l;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-10-2" 时
    那么应该报告 "test.c:4:9: warning: 禁止长整数变量赋给短整数变量 [gjb8114-r-1-10-2]"
    并且应该报告 "test.c:5:9: warning: 禁止长整数变量赋给短整数变量 [gjb8114-r-1-10-2]"
    并且应该报告 "test.c:6:9: warning: 禁止长整数变量赋给短整数变量 [gjb8114-r-1-10-2]"
    并且总共报告 3 个 warning

  @acceptance
  场景: 不报告强制转换赋值
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(long l, int i, short s)
        {
            char c;
            c = (char)s;
            c = (char)i;
            c = (char)l;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-10-2" 时
    那么不应该报告任何 warning

  场景: 不报告 C++ 转换
    假如有一个源码文件 "test.cpp"，内容如下:
        """
        void foo(long l, int i, short s)
        {
            char c;
            c = static_cast<char>(s);
            c = static_cast<char>(i);
            c = static_cast<char>(l);
        }
        """
    当对 "test.cpp" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-10-2" 时
    那么不应该报告任何 warning
