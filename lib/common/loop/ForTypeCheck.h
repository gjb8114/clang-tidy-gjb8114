#pragma once
#include "clang-tidy/ClangTidyCheck.h"
namespace clang::tidy::gjb8114 {
/// Check for rule R-1-9-2: for loop control variables must use integer type variables.
///
/// This check ensures that control variables used in for-loops are always
/// of integer type rather than floating point type.
class ForTypeCheck : public ClangTidyCheck {
public:
  ForTypeCheck(llvm::StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;
};
} // namespace clang::tidy::gjb8114