# language: zh-CN
功能: R-1-4-6 禁止 switch 省略 default

  例外: 枚举类型并且完全列举

  @acceptance
  场景: 报告 switch 省略 default
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(int x)
        {
            switch (x)
            {
            case 1:
                break;
            case 2:
                break;
            }
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-4-6" 时
    那么应该报告 "test.c:3:5: warning: 禁止 switch 省略 default [gjb8114-r-1-4-6]"
    并且总共报告 1 个 warning

  @acceptance
  场景: 不报告switch包含default
    假如有一个源码文件 "test.c"，内容如下:
        """
        void foo(int x)
        {
            switch (x)
            {
            case 1:
                break;
            case 2:
                break;
            default:
                break;
            }
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-4-6" 时
    那么不应该报告任何 warning

  场景: 不报告完全列举枚举类型
    假如有一个源码文件 "test.cxx"，内容如下:
        """
        enum class Color {RED, GREEN, BLUE} color;

        int main(void)
        {
            int result = 0;
            switch (color)
            {
            case Color::RED:
                result = 1;
                break;
            case Color::GREEN:
                result = 2;
                break;
            case Color::BLUE:
                result = 3;
                break;
            }
            return result;
        }
        """
    当对 "test.cxx" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-4-6" 时
    那么不应该报告任何 warning

  场景: 不报告枚举类型包含 default
    假如有一个源码文件 "test.cxx"，内容如下:
        """
        enum class Color {RED, GREEN, BLUE} color;

        int main(void)
        {
            int result = 0;
            switch (color)
            {
            case Color::RED:
                result = 1;
                break;
            case Color::GREEN:
                result = 2;
                break;
            default:
                result = 0;
                break;
            }
            return result;
        }
        """
    当对 "test.cxx" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-4-6" 时
    那么不应该报告任何 warning