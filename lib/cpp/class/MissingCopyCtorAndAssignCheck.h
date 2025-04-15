#pragma once
#include "clang-tidy/ClangTidyCheck.h"

namespace clang::tidy::gjb8114 {
class MissingCopyCtorAndAssignCheck : public ClangTidyCheck {
public:
  MissingCopyCtorAndAssignCheck(llvm::StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &C) override;
};
} // namespace clang::tidy::gjb8114