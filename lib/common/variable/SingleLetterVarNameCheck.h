#pragma once
#include "clang-tidy/ClangTidyCheck.h"
#include <set>

namespace clang::tidy::gjb8114 {
class SingleLetterVarNameCheck : public ClangTidyCheck {
public:
  SingleLetterVarNameCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  std::set<const VarDecl *> ReportedVars;
};
} // namespace clang::tidy::gjb8114