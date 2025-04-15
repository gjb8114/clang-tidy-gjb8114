# language: zh-CN
功能: R-1-7-4 禁止 K&R 风格函数定义

  K&R 风格函数定义是指函数的参数在函数体外声明，函数体内不再声明参数类型。K&R 风格函数定义的语法如下:

  ```C
  rettype
  function (parmnames)
    parm_declarations
  {
    body
  }
  ```

  参考文献: [Old-Style Function Definitions](https://www.gnu.org/software/c-intro-and-ref/manual/html_node/Old_002dStyle-Function-Definitions.html#:~:text=An%20old%2Dstyle%20(or%20“,are%20listed%2C%20separated%20by%20commas.))

  @acceptance
  场景: 报告 K&R 风格函数定义
    假如有一个源码文件 "test.c"，内容如下:
        """
        int
        main(argc, argv)
          int argc;
          char *argv[];
        {
            return 0;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-7-4" 时
    那么应该报告 "test.c:2:1: warning: 禁止 K&R 风格函数定义 [gjb8114-r-1-7-4]"
    并且总共报告 1 个 warning

  @acceptance
  场景: 不报告 ANSI 风格函数定义
    假如有一个源码文件 "test.c"，内容如下:
        """
        int main(int argc, char* argv[])
        {
            return (0);
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-7-4" 时
    那么不应该报告任何 warning