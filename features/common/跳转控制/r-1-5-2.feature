# language: zh-CN
功能: R-1-5-2 禁止 setjmp/longjmp

  @acceptance
  场景: 报告 setjmp/longjmp
    假如有一个源码文件 "test.c"，内容如下:
        """
        #include <setjmp.h>

        jmp_buf jump_buffer;

        void do_jump()
        {
            longjmp(jump_buffer, 1);
        }

        void foo(void)
        {
            if (setjmp(jump_buffer) == 0)
            {
                do_jump();
            }
            else
            {
            }
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-5-2" 时
    那么应该报告 "test.c:7:5: warning: 禁止 setjmp/longjmp [gjb8114-r-1-5-2]"
    并且应该报告 "test.c:12:9: warning: 禁止 setjmp/longjmp [gjb8114-r-1-5-2]"
    并且总共报告 2 个 warning