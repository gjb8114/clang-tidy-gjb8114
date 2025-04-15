# language: zh-CN
功能: R-1-7-16 禁止递归

  嵌入式系统中，递归函数可能会导致栈溢出

  @acceptance
  场景: 报告直接递归
    假如有一个源码文件 "test.c"，内容如下:
        """
        int factorial(int n) {
            if (n <= 1) {
                return 1;
            }
            return n * factorial(n - 1);
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-7-16" 时
    那么应该报告 "test.c:5:16: warning: 禁止递归 [gjb8114-r-1-7-16]"
    并且总共报告 1 个 warning

  场景: 报告间接递归
    假如有一个源码文件 "test.c"，内容如下:
        """
        int is_even(int n);
        int is_odd(int n);

        int is_even(int n) {
            if (n == 0) {
                return 1;
            }
            return is_odd(n - 1);
        }

        int is_odd(int n) {
            if (n == 0) {
                return 0;
            }
            return is_even(n - 1);
        }

        int foo() {
            return is_even(4);
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-7-16" 时
    那么应该报告 "test.c:8:12: warning: 禁止递归 [gjb8114-r-1-7-16]"
    并且应该报告 "test.c:15:12: warning: 禁止递归 [gjb8114-r-1-7-16]"
    并且总共报告 2 个 warning

  场景: 不报告非递归调用
    假如有一个源码文件 "test.c"，内容如下:
        """
        int foo(int n) {
            int result = 1;
            for (int i = 2; i <= n; i++) {
                result *= i;
            }
            return result;
        }

        int bar() {
            return foo(43);
        }

        int main() {
            return foo(bar());
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-7-16" 时
    那么不应该报告任何 warning