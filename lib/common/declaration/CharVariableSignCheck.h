#pragma once
#include "clang-tidy/ClangTidyCheck.h"

#include <set>

namespace clang::tidy::gjb8114 {
class CharVariableSignCheck : public ClangTidyCheck {
public:
  CharVariableSignCheck(StringRef Name, ClangTidyContext *Context);
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  std::set<const VarDecl *> PotentialViolations;
};
} // namespace clang::tidy::gjb8114