#pragma once

#include <clang-tidy/ClangTidyCheck.h>

namespace clang::tidy::gjb8114 {
class RecursiveFunctionCheck : public ClangTidyCheck {
public:
  RecursiveFunctionCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};
} // namespace clang::tidy::gjb8114