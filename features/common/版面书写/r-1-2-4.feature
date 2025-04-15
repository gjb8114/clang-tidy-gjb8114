# language: zh-CN
功能: R-1-2-4 禁止二义性逻辑表达式

  二义性逻辑表达式是指在没有使用括号明确优先级的情况下，使用逻辑运算符连接的表达式。

  由于逻辑运算符的优先级和结合性可能导致不同的解释，因此在编写代码时应避免这种情况。

  @acceptance
  场景大纲: 报告 <expr> 形式的二义性逻辑表达式
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(int x, int y, int z)
        {
            if (<expr>) {}
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-2-4" 时
    那么应该报告 "test.c:3:9: warning: 禁止二义性逻辑表达式 [gjb8114-r-1-2-4]"
    并且总共报告 1 个 warning

    例子:
      | expr          |
      | x && y \|\| z |
      | x \|\| y && z |
      | !x && y       |
      | !x \|\| y     |

  @acceptance
  场景大纲: 不报告明确优先级的 <form> 形式逻辑表达式
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(int x, int y, int z)
        {
            if (<expr>) { }
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-2-4" 时
    那么不应该报告任何 warning

    例子:
      | form          | expr            |
      | x && y \|\| z | (x && y) \|\| z |
      | x \|\| y && z | x \|\| (y && z) |
      | !x && y       | (!x) && y       |
      | !x \|\| y     | (!x) \|\| y     |

  场景大纲: 报告 <expr> 形式的二义性赋值表达式
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(int x, int y, int z)
        {
            int boolean = <expr>;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-2-4" 时
    那么应该报告 "test.c:3:19: warning: 禁止二义性逻辑表达式 [gjb8114-r-1-2-4]"
    并且总共报告 1 个 warning

    例子:
      | expr          |
      | x && y \|\| z |
