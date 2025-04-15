# language: zh-CN
功能: R-2-1-1 禁止含动态分配成员的类省略拷贝构造函数和赋值操作符

  @acceptance
  场景: 报告未编写拷贝构造函数和赋值操作符
    假如有一个源码文件 "test.cpp"，内容如下:
        """
        class Foo
        {
           int *ptr;
        public:
            Foo();
            ~Foo();
        };

        Foo::Foo() : ptr(new int(0)) {}

        Foo::~Foo() { delete ptr; }
        """
    当对 "test.cpp" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-2-1-1" 时
    那么应该报告 "test.cpp:1:7: warning: 禁止含动态分配成员的类省略拷贝构造函数和赋值操作符 [gjb8114-r-2-1-1]"
    并且总共报告 1 个 warning

  @acceptance
  场景: 不报告已编写拷贝构造函数和赋值操作符
    假如有一个源码文件 "test.cpp"，内容如下:
        """
        class Foo
        {
           int *ptr;
        public:
            Foo();
            ~Foo();
            Foo(const Foo &other);
            Foo &operator=(const Foo &rhs);
        };

        Foo::Foo() : ptr(new int(0)) {}

        Foo::~Foo() { delete ptr; }

        Foo::Foo(const Foo &other) : ptr(new int(*other.ptr)) {}

        Foo &Foo::operator=(const Foo &rhs)
        {
            if (this != &rhs)
                *ptr = *rhs.ptr;
            return *this;
        }
        """
    当对 "test.cpp" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-2-1-1" 时
    那么不应该报告任何 warning

  场景: 报告使用默认拷贝构造函数和赋值操作符
    假如有一个源码文件 "test.cpp"，内容如下:
        """
        class Foo
        {
           int *ptr;
        public:
            Foo();
            ~Foo();
            Foo(const Foo &other) = default;
            Foo &operator=(const Foo &rhs) = default;
        };

        Foo::Foo() : ptr(new int(0)) {}

        Foo::~Foo() { delete ptr; }
        """
    当对 "test.cpp" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-2-1-1" 时
    那么应该报告 "test.cpp:1:7: warning: 禁止含动态分配成员的类省略拷贝构造函数和赋值操作符 [gjb8114-r-2-1-1]"
    并且总共报告 1 个 warning

  场景: 不报告不含动态分配成员的类
    假如有一个源码文件 "test.cpp"，内容如下:
        """
        class Foo
        {
           int *a;
        public:
            Foo(int *x);
            ~Foo();
        };

        Foo::Foo(int *x) : a(x) {}

        Foo::~Foo() {}
        """
    当对 "test.cpp" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-2-1-1" 时
    那么不应该报告任何 warning

  场景: 报告使用构造函数体初始化动态分配成员
    假如有一个源码文件 "test.cpp"，内容如下:
        """
        class Foo
        {
           int *ptr;
        public:
            Foo();
            ~Foo();
        };

        Foo::Foo() { ptr = new int(0); }

        Foo::~Foo() { delete ptr; }
        """
    当对 "test.cpp" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-2-1-1" 时
    那么应该报告 "test.cpp:1:7: warning: 禁止含动态分配成员的类省略拷贝构造函数和赋值操作符 [gjb8114-r-2-1-1]"
    并且总共报告 1 个 warning

  场景: 不报告智能指针管理的动态分配成员
    假如有一个源码文件 "test.cpp"，内容如下:
        """
        #include <memory>

        class Foo
        {
           std::unique_ptr<int> ptr;
        public:
            Foo();
            ~Foo();
        };

        Foo::Foo() : ptr(new int(0)) {}

        Foo::~Foo() {}
        """
    当对 "test.cpp" 运行带有 gjb8114 插件的 clang-tidy 检查 "gjb8114-r-2-1-1" 时
    那么不应该报告任何 warning