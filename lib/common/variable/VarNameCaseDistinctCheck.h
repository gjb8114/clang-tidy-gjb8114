#pragma once
#include "VarNameDistinctCheck.h"
#include <cctype>

namespace clang::tidy::gjb8114 {
class VarNameCaseDistinctCheck : public VarNameDistinctCheck {
public:
  VarNameCaseDistinctCheck(llvm::StringRef Name, ClangTidyContext *Context)
      : VarNameDistinctCheck(Name, Context) {}

protected:
  std::string getKey(const std::string &Name) const override {
    std::string Result = Name;
    std::ranges::transform(Result, Result.begin(),
                           [](auto c) { return std::tolower(c); });
    return Result;
  }
  const char *getDescription() const override {
    return "禁止仅大小写区分的变量";
  }
};
} // namespace clang::tidy::gjb8114