#pragma once
#include "clang-tidy/ClangTidyCheck.h"

namespace clang::tidy::gjb8114 {
using llvm::StringRef;

class IfElseBracesCheck : public ClangTidyCheck {
public:
  IfElseBracesCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  void
  emitDiagnosticWithFixIt(const Stmt *Body, StringRef StmtKind,
                          const ast_matchers::MatchFinder::MatchResult &Result);
};
} // namespace clang::tidy::gjb8114