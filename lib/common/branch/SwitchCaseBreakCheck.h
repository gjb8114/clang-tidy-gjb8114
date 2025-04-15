#pragma once
#include "clang-tidy/ClangTidyCheck.h"

#include <set>

namespace clang::tidy::gjb8114 {
class SwitchCaseBreakCheck : public ClangTidyCheck {
public:
  SwitchCaseBreakCheck(llvm::StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  void checkFallThrough(const ast_matchers::MatchFinder::MatchResult &Result,
                        const SwitchCase *Case);

  std::set<const Stmt *> Reported;
};
} // namespace clang::tidy::gjb8114