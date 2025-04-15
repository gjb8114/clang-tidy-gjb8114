# language: zh-CN
@skip
功能: R-1-13-12 禁止程序外部可改写的变量未使用 volatile 修饰

  例如：与其他线程共享的变量

  @acceptance
  场景: 报告程序外部可改写的变量未使用 volatile 修饰
    假如有一个源码文件 "test.c"，内容如下:
        """
        int is_stop = 0;

        void thread_a(void)
        {
            while (!is_stop)
            {
                // do something
            }
        }

        int main(void)
        {
            is_stop = 1;
            return 0;
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-13-12" 时
    那么应该报告 "test.c:1:5: warning: 禁止程序外部可改写的变量未使用 volatile 修饰 [gjb8114-r-1-13-12]"
    并且总共报告 1 个 warning