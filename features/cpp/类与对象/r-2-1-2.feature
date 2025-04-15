# language: zh-CN
功能: R-2-1-2 禁止使用 reinterpret_cast 对虚基类向下转型

  @acceptance
  场景: 报告使用 reinterpret_cast 对虚基类向下转型
    假如有一个源码文件 "test.cpp"，内容如下:
        """
        class Shape
        {
        public:
            virtual ~Shape();
        };

        class Rect : public virtual Shape
        {
        public:
            ~Rect() override;
        };

        void foo()
        {
            Shape *s = new Rect;
            Rect *r1 = reinterpret_cast<Rect*>(s);
            Rect &r2 = reinterpret_cast<Rect&>(*s);
            Shape &rs = *s;
            Rect &r3 = reinterpret_cast<Rect&>(rs);
        }
        """
    当对 "test.cpp" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-2-1-2" 时
    那么应该报告 "test.cpp:16:16: warning: 禁止使用 reinterpret_cast 对虚基类向下转型 [gjb8114-r-2-1-2]"
    并且应该报告 "test.cpp:17:16: warning: 禁止使用 reinterpret_cast 对虚基类向下转型 [gjb8114-r-2-1-2]"
    并且应该报告 "test.cpp:19:16: warning: 禁止使用 reinterpret_cast 对虚基类向下转型 [gjb8114-r-2-1-2]"
    并且总共报告 3 个 warning

  @acceptance
  场景: 不报告使用 dynamic_cast 对虚基类向下转型
    假如有一个源码文件 "test.cpp"，内容如下:
        """
        class Shape
        {
        public:
            virtual ~Shape();
        };

        class Rect : public virtual Shape
        {
        public:
            ~Rect() override;
        };

        void foo()
        {
            Rect r;
            Shape *s = &r;
            Rect *pr = dynamic_cast<Rect*>(s);
            Rect &rr = dynamic_cast<Rect&>(*s);
        }
        """
    当对 "test.cpp" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-2-1-2" 时
    那么不应该报告任何 warning
