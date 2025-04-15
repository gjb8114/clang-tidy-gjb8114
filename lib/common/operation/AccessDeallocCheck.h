#pragma once
#include "clang-tidy/ClangTidyCheck.h"

namespace clang::tidy::gjb8114 {
class AccessDeallocCheck : public ClangTidyCheck {
public:
  AccessDeallocCheck(llvm::StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  void emitDiagnostics(const Stmt *Usage);
  std::set<const clang::Stmt *> Reported;
};
} // namespace clang::tidy::gjb8114