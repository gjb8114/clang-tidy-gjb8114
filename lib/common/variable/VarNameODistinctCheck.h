#pragma once
#include "VarNameDistinctCheck.h"
#include <ranges>

namespace clang::tidy::gjb8114 {
class VarNameODistinctCheck : public VarNameDistinctCheck {
public:
  VarNameODistinctCheck(llvm::StringRef Name, ClangTidyContext *Context)
      : VarNameDistinctCheck(Name, Context) {}

protected:
  std::string getKey(const std::string &Name) const override {
    std::string Result = Name;
    std::ranges::replace(Result, '0', 'O');
    return Result;
  }
  const char *getDescription() const override {
    return "禁止仅小写字母 “l” 与数字 “1” 区分的变量";
  }
};
} // namespace clang::tidy::gjb8114