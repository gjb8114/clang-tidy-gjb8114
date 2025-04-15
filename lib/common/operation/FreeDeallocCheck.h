#pragma once
#include "clang-tidy/ClangTidyCheck.h"

namespace clang::tidy::gjb8114 {
class FreeDeallocCheck : public ClangTidyCheck {
public:
  FreeDeallocCheck(llvm::StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  std::set<const VarDecl *> ChangedPtrs;
};
} // namespace clang::tidy::gjb8114