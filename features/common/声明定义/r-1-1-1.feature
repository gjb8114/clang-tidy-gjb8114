# language: zh-CN
功能: R-1-1-1 禁止宏定义覆盖关键字

  @acceptance
  场景: 报告宏定义改变关键字
    假如有一个源码文件 "test.c"，内容如下:
      """
      #define int 100
      """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-1-1" 时
    那么应该报告 "test.c:1:9: warning: 禁止宏定义覆盖关键字 [gjb8114-r-1-1-1]"
    并且总共报告 1 个 warning

  @acceptance
  场景: 不报告普通宏定义
    假如有一个源码文件 "test.c"，内容如下:
      """
      #define PI 3.1415926
      """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-1-1" 时
    并且不应该报告任何 warning

  场景大纲: 报告宏定义改变关键字
    假如有一个源码文件 "test.c"，内容如下:
      """
      #define <keyword> void
      """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-1-1" 时
    那么应该报告 "test.c:1:9: warning: 禁止宏定义覆盖关键字 [gjb8114-r-1-1-1]"
    并且总共报告 1 个 warning

    例子:
      | keyword  |
      | auto     |
      | register |
      | static   |
      | extern   |
      | switch   |
      | break    |
      | case     |
      | const    |
      | volatile |
      | continue |
      | default  |
      | for      |
      | do       |
      | while    |
      | if       |
      | else     |
      | goto     |
      | inline   |
      | restrict |
      | return   |
      | sizeof   |
      | struct   |
      | union    |
      | enum     |
      | typedef  |

  场景大纲: 报告宏定义改变基本类型
    假如有一个源码文件 "test.c"，内容如下:
      """
      #define <type> float
      """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-1-1" 时
    那么应该报告 "test.c:1:9: warning: 禁止宏定义覆盖关键字 [gjb8114-r-1-1-1]"
    并且总共报告 1 个 warning

    例子:
      | type     |
      | int      |
      | char     |
      | short    |
      | long     |
      | float    |
      | double   |
      | void     |
      | signed   |
      | unsigned |
