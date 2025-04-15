# language: zh-CN
功能: R-1-1-7 禁止宏函数的参数和结果未加括号

  @acceptance
  场景: 报告参数和结果未用括号括起
    假如有一个源码文件 "test.c"，内容如下:
        """
        #define SET_BIT(x, bit) x |= (1 << bit)
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-1-7" 时
    那么应该报告 "test.c:1:25: warning: 禁止宏函数的参数和结果未加括号 [gjb8114-r-1-1-7]"
    并且总共报告 1 个 warning

  @acceptance
  场景: 不报告参数和结果已用括号括起
    假如有一个源码文件 "test.c"，内容如下:
        """
        #define SET_BIT(x, bit) ((x) |= (1 << (bit)))
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-1-7" 时
    那么不应该报告任何 warning

  场景: 报告参数缺少括号
    假如有一个源码文件 "test.c"，内容如下:
        """
        #define SET_BIT(x, bit) (x |= (1 << bit))
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-1-7" 时
    那么应该报告 "test.c:1:26: warning: 禁止宏函数的参数和结果未加括号 [gjb8114-r-1-1-7]"
    并且总共报告 1 个 warning

  场景: 不报告非参数标识符缺少括号
    假如有一个源码文件 "test.c"，内容如下:
        """
        int reg = 0;
        #define REG_SET_BIT(bit) (reg |= (1 << (bit)))
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-1-7" 时
    那么不应该报告任何 warning