# clang-tidy-gjb8114

## 使用说明

### 安装

您可以通过以下几种方式安装此工具：

###### 使用 Docker（推荐）

最简单的使用方式是通过 Docker。从 GitHub Container Registry 拉取预构建镜像：

```bash
docker pull ghcr.io/gjb8114/clang-tidy-gjb8114:latest
```

###### 使用预构建包

从发布页面下载适用于您平台的预构建包。

###### 从源代码构建

按照本文档的[构建](#build)部分中的构建说明进行操作。

### 命令行使用
此工具根据 GJB-8114 编码标准为 C/C++ 代码提供静态分析检查。使用 clang-tidy 运行它：

###### 使用 Docker

挂载您的源代码目录并使用 Docker 运行检查：

```bash
docker run --rm -v "$(pwd)":/workspace ghcr.io/gjb8114/clang-tidy-gjb8114:latest \
    your_file.cpp -checks='-*,gjb8114-*' -load='/usr/local/lib/libclang-tidy-gjb8114.so'
```

列出所有可用检查：

```bash
docker run --rm ghcr.io/gjb8114/clang-tidy-gjb8114:latest \
    -list-checks -checks='-*,gjb8114-*' -load='/usr/local/lib/libclang-tidy-gjb8114.so'
```

###### 不使用 Docker

```bash
clang-tidy your_file.cpp -checks='-*,gjb8114-*' -load='/path/to/libclang-tidy-gjb8114.so'
```

还可以通过创建 `.clang-tidy` 配置文件将其与的构建系统集成。

规则的命名约定为 `gjb8114-[准则编号]`，例如：

- `gjb8114-r-1-1-1`：禁止宏定义覆盖关键字
- `gjb8114-r-1-1-10`：禁止有符号位域的位宽等于 1

`list-checks` 命令可以列出所有可用的检查：

```bash
# 使用 Docker
docker run --rm ghcr.io/gjb8114/clang-tidy-gjb8114:latest \
    -list-checks -checks='-*,gjb8114-*' -load='/usr/local/lib/libclang-tidy-gjb8114.so'

# 不使用 Docker
clang-tidy -list-checks -checks='-*,gjb8114-*' -load='/path/to/libclang-tidy-gjb8114.so'
```

或者查看[文档](https://gjb8114.github.io)以获取更多信息。

## 开发说明

### 搭建开发环境

###### 前置要求

- cmake，版本3.23或更高
- llvm
- ruby gems

###### 使用 Docker 构建

您无需在本机安装任何依赖，可以直接使用 Docker 构建项目：

```bash
docker build -t clang-tidy-gjb8114 .
```

构建特定版本（调试模式）：

```bash
docker build --build-arg BUILD_TYPE=Debug -t clang-tidy-gjb8114:debug .
```

###### macOS

安装前置要求：

```bash
brew install llvm
```

### <a name="build"></a>构建

构建项目最简单的方法是使用CMake预设。

```bash
cmake --preset=debug
cmake --build --preset=debug
```

> 将`--preset=debug`更改为`--preset=release`以进行发布构建。

### 运行测试

##### Cucumber 测试
项目包含 Cucumber 测试以验证插件行为。

###### 安装项目依赖

```bash
bundle install  # 安装Gemfile中所需的Ruby gems
```

###### 运行测试

使用默认配置文件运行（自动检测路径）

```bash
bundle exec rake cucumber
```

使用默认配置文件运行（使用环境变量覆盖路径）

```bash
CLANG_TIDY_PATH=/path/to/clang-tidy BIN_DIR=/build/release bundle exec rake cucumber
```
