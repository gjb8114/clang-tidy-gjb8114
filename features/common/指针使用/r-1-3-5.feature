# language: zh-CN
功能: R-1-3-5 禁止使用或释放未分配或已释放的指针

  注意：与 #R-1-6-16 《禁止使用已释放的内存》有某种程度上重复

  因此将本规则理解为：

  - 禁止使用未分配空间的指针
  - 禁止释放未分配空间的指针
  - 禁止释放已被释放的指针

  @acceptance
  场景: 报告使用或释放未分配或已释放的指针
    假如有一个源码文件 "test.c"，内容如下:
        """
        #include <stdlib.h>

        void foo(void)
        {
            int *pa = NULL;
            *pa = 0;
            free(pa);
            int *pb = (int *)malloc(sizeof(int));
            free(pb);
            free(pb);
        }
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-3-5" 时
    那么应该报告 "test.c:6:6: warning: 禁止使用或释放未分配或已释放的指针 [gjb8114-r-1-3-5]"
    并且应该报告 "test.c:7:10: warning: 禁止使用或释放未分配或已释放的指针 [gjb8114-r-1-3-5]"
    并且应该报告 "test.c:10:10: warning: 禁止使用或释放未分配或已释放的指针 [gjb8114-r-1-3-5]"
