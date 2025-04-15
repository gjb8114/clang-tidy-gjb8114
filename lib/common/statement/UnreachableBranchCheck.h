#pragma once
#include "clang-tidy/ClangTidyCheck.h"

namespace clang::tidy::gjb8114 {
class UnreachableBranchCheck : public ClangTidyCheck {
public:
  UnreachableBranchCheck(llvm::StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};
} // namespace clang::tidy::gjb8114