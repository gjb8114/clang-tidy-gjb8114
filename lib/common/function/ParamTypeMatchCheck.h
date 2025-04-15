#pragma once
#include "common/declaration/FunctionDeclConsistencyCheck.h"

namespace clang::tidy::gjb8114 {
class ParamTypeMatchCheck : public FunctionDeclConsistencyCheck {
public:
  ParamTypeMatchCheck(llvm::StringRef Name, ClangTidyContext *Context)
      : FunctionDeclConsistencyCheck(Name, Context) {}

protected:
  std::set<enum Report> getReportOpt() const override {
    return {Report::ParameterType};
  }
  const char *getDescription() const override {
    return "禁止函数声明和定义的参数类型不一致";
  }
};
} // namespace clang::tidy::gjb8114