# language: zh-CN
功能: R-1-8-5 禁止小写字母数字后缀

  @acceptance
  场景: 报告小写字母数字后缀
    假如有一个源码文件 "test.c"，内容如下:
        """
        unsigned int u = 0u;
        long l = 0l;
        float f = 0.0f;
        double d = 0.0l;
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-8-5" 时
    那么应该报告 "test.c:1:19: warning: 禁止小写字母数字后缀 [gjb8114-r-1-8-5]"
    并且应该报告 "test.c:2:11: warning: 禁止小写字母数字后缀 [gjb8114-r-1-8-5]"
    并且应该报告 "test.c:3:14: warning: 禁止小写字母数字后缀 [gjb8114-r-1-8-5]"
    并且应该报告 "test.c:4:15: warning: 禁止小写字母数字后缀 [gjb8114-r-1-8-5]"
    并且总共报告 4 个 warning

  @acceptance
  场景: 不报告大写字母数字后缀
    假如有一个源码文件 "test.c"，内容如下:
        """
        unsigned int u = 0U;
        long l = 0L;
        float f = 0.0F;
        double d = 0.0L;
        """
    当对 "test.c" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-1-8-5" 时
    那么不应该报告任何 warning