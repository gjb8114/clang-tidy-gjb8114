# language: zh-CN
功能: R-1-11-1 禁止使用未初始化变量

  @acceptance
  场景: 报告使用未初始化变量
    假如有一个源码文件 "test.c"，内容如下:
      """
      void bar(int);

      void foo(void)
      {
          int x, y, z;
          x = z;
          bar(x + y);
      }
      """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-11-1" 时
    那么应该报告 "test.c:6:9: warning: 禁止使用未初始化变量 [gjb8114-r-1-11-1]"
    并且应该报告 "test.c:7:13: warning: 禁止使用未初始化变量 [gjb8114-r-1-11-1]"
    并且总共报告 2 个 warning

  @acceptance
  场景: 不报告已初始化变量的使用
    假如有一个源码文件 "test.c"，内容如下:
      """
      void foo(void)
      {
          int x, y, z = 0;
          x = z;
          y = x;

      }
      """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-11-1" 时
    并且不应该报告任何 warning

  场景: 不报告使用未初始化变量初始化的变量的使用
    假如有一个源码文件 "test.c"，内容如下:
      """
      int main(void)
      {
          int x, y;
          x = y;
          if (x >= 0)
          {
              //...
          }
          return 0;
      }
      """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-11-1" 时
    那么不应该报告 "test.c:5:9: warning: 禁止使用未初始化变量 [gjb8114-r-1-11-1]"
    并且应该报告 "test.c:4:9: warning: 禁止使用未初始化变量 [gjb8114-r-1-11-1]"

  场景: 不报告标准库的使用未初始化变量
    假如有一个源码文件 "test.cpp"，内容如下:
      """
      #include <iostream>

      int main() {
        int uninitializedVariable;
        std::cout << uninitializedVariable << std::endl;
        return 0;
      }
      """
    当对 "test.cpp" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-11-1" 时
    那么不应该报告 "test.cpp:5:8: warning: 禁止使用未初始化变量 [gjb8114-r-1-11-1]"
    并且应该报告 "test.cpp:5:16: warning: 禁止使用未初始化变量 [gjb8114-r-1-11-1]"

  场景: 不报告全局变量的未初始化
    假如有一个源码文件 "test.cpp"，内容如下:
      """
      #include <iostream>

      int globalVariable;
      int main() {
        if (globalVariable == 0) {
          std::cout << globalVariable << std::endl;
        }
        return 0;
      }
      """
    当对 "test.cpp" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-11-1" 时
    那么不应该报告任何 warning

  场景: 不报告全局变量的未初始化（C）
    假如有一个源码文件 "test.c"，内容如下:
      """
      int globalVariable;
      int main() {
        if (globalVariable == 0) {
          return 0;
        }
        return 1;
      }
      """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-11-1" 时
    那么不应该报告任何 warning