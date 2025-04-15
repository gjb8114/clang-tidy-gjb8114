#pragma once
#include "common/declaration/FunctionDeclConsistencyCheck.h"

namespace clang::tidy::gjb8114 {
class FunctionReturnTypeCheck : public FunctionDeclConsistencyCheck {
public:
  FunctionReturnTypeCheck(llvm::StringRef Name, ClangTidyContext *Context)
      : FunctionDeclConsistencyCheck(Name, Context) {}

protected:
  std::set<enum Report> getReportOpt() const override {
    return {Report::ReturnType};
  }
  const char *getDescription() const override {
    return "禁止函数声明和定义的返回值类型不一致";
  }
};
} // namespace clang::tidy::gjb8114
