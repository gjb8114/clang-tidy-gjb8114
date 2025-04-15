#pragma once
#include "clang-tidy/ClangTidyCheck.h"
#include <map>
#include <string>
#include <vector>

namespace clang::tidy::gjb8114 {
class VariableShadowingCheck : public ClangTidyCheck {
public:
  VariableShadowingCheck(llvm::StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};
} // namespace clang::tidy::gjb8114