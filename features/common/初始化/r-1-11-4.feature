# language: zh-CN
功能: R-1-11-4 禁止枚举元素定义不完整

  @acceptance
  场景: 报告枚举元素定义不完整
    假如有一个源码文件 "test.c"，内容如下:
      """
      enum Type
      {
          Int,
          Float = 1
      };
      """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-11-4" 时
    那么应该报告 "test.c:3:5: warning: 禁止枚举元素定义不完整 [gjb8114-r-1-11-4]"
    并且总共报告 1 个 warning

  @acceptance
  场景: 不报告枚举元素定义完整
    假如有一个源码文件 "test.c"，内容如下:
      """
      enum Type
      {
          Int = 0,
          Float
      };

      enum Kind
      {
          Constant = 0,
          Variable = 1
      };
      """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-11-4" 时
    那么不应该报告任何 warning